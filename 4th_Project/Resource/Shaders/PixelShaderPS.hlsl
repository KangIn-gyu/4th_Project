#include "Helper.hlsli"

//--------------------------------------------------------------------------------------
// Shadow Helper Functions
//--------------------------------------------------------------------------------------
float CalculateShadow(PixelInputType input)
{
    float3 projCoords = input.LightSpacePos.xyz / input.LightSpacePos.w;
    
    // 범위 체크
    if (projCoords.x < -1.0f || projCoords.x > 1.0f ||
        projCoords.y < -1.0f || projCoords.y > 1.0f ||
        projCoords.z < 0.0f || projCoords.z > 1.0f)
    {
        return 1.0f;
    }
    
    // NDC 좌표를 UV 좌표로 변환
    projCoords.x = projCoords.x * 0.5 + 0.5;
    projCoords.y = -projCoords.y * 0.5 + 0.5;
    
    // depth test 개선
    float currentDepth = projCoords.z;
    float bias = 0.0025; // 고정 bias 사용

    // 거리에 따른 PCF 커널 크기 조정
    float viewDistance = length(input.worldPos.xyz - eyePosition);
    int sampleRange = PCF_SAMPLES;
    float shadow = 0.0;
    float2 texelSize = 1.0f / float2(4096.0f, 4096.0f);
    
    // 개선된 PCF 필터링
    [unroll]
    for (int x = -sampleRange; x <= sampleRange; ++x)
    {
        [unroll]
        for (int y = -sampleRange; y <= sampleRange; ++y)
        {
            float2 offset = float2(x, y) * texelSize * 0.5; // PCF 범위 축소
            shadow += shadowMap.SampleCmpLevelZero(
                shadowSampler,
                projCoords.xy + offset,
                currentDepth - bias
            );
        }
    }
    
    int samples = (2 * sampleRange + 1) * (2 * sampleRange + 1);
    shadow /= samples;
    
    // 그림자 강도 조절
    shadow = shadow * 0.95 + 0.05; // 완전히 검은 그림자 방지
    
    return shadow;
}

float4 main(PixelInputType input) : SV_TARGET
{
    //--------------------------------------------------------------------------------------
    // Material Parameters
    //--------------------------------------------------------------------------------------
    float4 baseColorWithAlpha = SampleBaseColorWithGamma(TextureAlbedo, input.TexCoord, float4(1, 1, 1, 1));
    float3 baseColor = baseColorWithAlpha.rgb;
    
    // Normal calculation
    float3 N = normalize(input.Normal);
    float3 V = normalize(eyePosition - input.worldPos.xyz);
    
    // Normal mapping
    float4 normalSample = NormalMap.Sample(samLinear, input.TexCoord);
    if (normalSample.a > 0.0f)
    {
        float3 normalMap = normalSample.rgb * 2.0f - 1.0f;
        float3x3 TBN = GetTangentSpace(input.Normal, input.Tangent);
        N = normalize(mul(normalMap, TBN));
    }
    
    // Metallic & Roughness
    float metallic = metalness;
    float rough = roughness;
    
    // Sample metalness map
    float metallicSample = MetalnessMap.Sample(samLinear, input.TexCoord).r;
    if (metallicSample > 0.0f)
    {
        metallic *= metallicSample;
    }
    
    float roughnessSample = ShininessColor.Sample(samLinear, input.TexCoord).r;
    if (roughnessSample > 0.0f)
    {
        rough *= roughnessSample;
    }

    //--------------------------------------------------------------------------------------
    // Shadow Calculation
    //--------------------------------------------------------------------------------------
    float shadowFactor = CalculateShadow(input);
    
    //--------------------------------------------------------------------------------------
    // Outline Effect Parameters
    //--------------------------------------------------------------------------------------
    float edgeIntensity = 0.0f;
    float outlineWidth = 1.0f; // 외곽선 두께
    float outlineStrength = 3.5f; // 외곽선 강도
    float3 outlineColor = float3(0.0f, 1.0f, 0.0f); // 외곽선 색
    
    bool useOutline = true;
    if (useOutline)
    {
    // 시야 방향과의 각도를 더 부드럽게 계산
        float rim = 1.0f - max(0.0f, dot(N, V));
        float rimPower = 3.0f; // 더 낮은 값으로 조정
        float fresnelFactor = pow(rim, rimPower);
    
    // 노말맵의 급격한 변화 감지
        float normalEdge = length(fwidth(N)) * 2.0f;
    
    // 최종 외곽선 강도 계산
        edgeIntensity = smoothstep(0.4f, 0.6f, fresnelFactor + normalEdge);
    }
       
    //--------------------------------------------------------------------------------------
    // Lighting Calculation
    //--------------------------------------------------------------------------------------
    float3 L = normalize(-lightDirection);
    float3 H = normalize(V + L);
    
    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0001);
    float NdotH = max(dot(N, H), 0.0);
    float HdotV = max(dot(H, V), 0.0);
    
    // PBR Parameters
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), baseColor, metallic);
    float D = D_GGX(NdotH, max(0.1f, rough));
    float3 F = F_Schlick(HdotV, F0);
    float G = G_Smith(NdotV, NdotL, rough);
    
    float3 specular = (D * F * G) / (4.0 * NdotV * NdotL + 0.0001);
    float3 kD = (1.0 - F) * (1.0 - metallic);
    
    //--------------------------------------------------------------------------------------
    // Final Color Composition
    //--------------------------------------------------------------------------------------
    // Direct lighting
    float3 diffuse = kD * baseColor / PI;
    float3 directLight = (diffuse + specular) * NdotL * shadowFactor;
    
    // Ambient lighting
    float3 ambient = baseColor * 0.3f;
    
    // IBL contribution
    float3 iblDiffuse = GetIBLIrradiance(N);
    float3 iblSpecular = GetIBLRadiance(N, V, rough);
    float2 brdf = IntegrateBRDF(NdotV, rough);
    float3 iblResult = lerp(kD * iblDiffuse * baseColor, iblSpecular * (F * brdf.x + brdf.y), metallic);
    
    // Emissive
    float3 emissive = any(EmissiveColor.Sample(samLinear, input.TexCoord).rgb > 0) ?
                     EmissiveColor.Sample(samLinear, input.TexCoord).rgb : 0;
    
    float3 rimColor = float3(0.0, 2.0, 0.0);
    
    float3 rimLight = CardSelectionRimLight(N, V, rimColor); // 초록색 계열의 림라이트
    
    float fresnelFactor = pow(1.0 - saturate(dot(N, V)), 2.0);
    rimLight += fresnelFactor * rimLight * 5.0;
    rimLight = (0, 0, 0);
    // Combine all lighting
    float3 color = directLight + ambient + iblResult + emissive + rimLight;
    
    //float3 finalRimColor = lerp(rimLight, outlineColor, edgeIntensity); 
    //color = color + finalRimColor;
    
    color = pow(color, 1.0f / GAMMA);
    color = ACESFilmicToneMapping(color);
    
    //--------------------------------------------------------------------------------------
    // Alpha Handling
    //--------------------------------------------------------------------------------------
    float4 finalColor = float4(color, 1.0f);
    float4 opacityColor = OpacityMap.Sample(samLinear, input.TexCoord);
    
    if (opacityColor.a < 0.01f)
    {
        finalColor.a = baseColorWithAlpha.a * input.Color.a;
    }
    else
    {
        float alpha = saturate(pow(opacityColor.a, 0.5f) * 1.5f);
        finalColor.a = alpha * input.Color.a;
        finalColor.rgb *= finalColor.a;
    }
    
    if (finalColor.a < 1.0f)
    {
        finalColor.rgb /= max(finalColor.a, 0.001f);
    }
    
    if (finalColor.a < 0.01f)
    {
        discard;
    }
    
    //return float4(shadowFactor.xxx, 1.0f);
    return finalColor;
}