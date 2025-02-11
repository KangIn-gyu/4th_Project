#include "Helper.hlsli"
//#define LIGHT_NUM 1 

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
    else
    {
        metallic = 0.4f;
    }
    
    float roughnessSample = ShininessColor.Sample(samLinear, input.TexCoord).r;
    if (roughnessSample > 0.0f)
    {
        rough *= roughnessSample;
    }
    else
    {
        rough = 0.7f;
    }

    //--------------------------------------------------------------------------------------
    // Shadow Calculation
    //--------------------------------------------------------------------------------------
    float shadowFactor = CalculateShadow(input);
    
    //--------------------------------------------------------------------------------------
    // Outline Effect Parameters
    //--------------------------------------------------------------------------------------
    //float edgeIntensity = 0.0f;
    //float innerEdgeIntensity = 0.0f;    // 안쪽 외곽선 변수
    //float outlineWidth = 5.0f;    // 외곽선 두께
    //float outlineStrength = 3.5f; // 외곽선 강도
    //float3 outlineColor = float3(1.0f, 0.0f, 0.0f); // 바깥 쪽 외곽선 색
    //float3 innerOutlineColor = float3(0.0f, 0.5f, 0.0f); // 안쪽 외곽선 색상
    //bool useOutline = true;
    //if (useOutline)
    //{
    //    // 시야 방향과의 각도를 더 부드럽게 계산
    //    float rim = 1.0f - max(0.0f, dot(N, V));
    //    float rimPower = 3.0f; // 더 낮은 값으로 조정
    //    float fresnelFactor = pow(rim, rimPower);
    //
    //    // 노말맵의 급격한 변화 감지
    //    float normalEdge = length(fwidth(N)) * 2.0f;
    //
    //    // 최종 외곽선 강도 계산 ( 임계 구간을 좁힐수록 더욱 선명해짐)
    //    // 현재는 85% ~ 100% 구간에서 변화 
    //    edgeIntensity = smoothstep(0.5f, 1.0f, fresnelFactor + normalEdge);
    //    
    //}
       
    //--------------------------------------------------------------------------------------
    // Lighting Calculation
    //--------------------------------------------------------------------------------------
    float3 L_dir = normalize(-lightDirection);
    float3 H_dir = normalize(V + L_dir);
    
    float NdotL_dir = max(dot(N, L_dir), 0.0);
    float NdotV = max(dot(N, V), 0.0001);
    float NdotH_dir = max(dot(N, H_dir), 0.0);
    float HdotV_dir = max(dot(H_dir, V), 0.0);
    
    // PBR Parameters
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), baseColor, metallic);
    float D_dir = D_GGX(NdotH_dir, max(0.1f, rough));
    float3 F_dir = F_Schlick(HdotV_dir, F0);
    float G_dir = G_Smith(NdotV, NdotL_dir, rough);
    
    float3 specular_dir = (D_dir * F_dir * G_dir) / (4.0 * NdotV * NdotL_dir + 0.0001);
    float3 kD_dir = (1.0 - F_dir) * (1.0 - metallic);
    
    //--------------------------------------------------------------------------------------
    // Final Color Composition
    //--------------------------------------------------------------------------------------
    // Direct lighting
    float3 diffuse_dir = kD_dir * baseColor / PI;
    float3 directionalLight = ((diffuse_dir + specular_dir) * 2.0f) * NdotL_dir * shadowFactor;
    
    // Initialize total lighting
    float3 totalSpotLight = float3(0, 0, 0);
    float3 F_accumulated = F_dir;           // 디렉셔널 라이트의 F로 초기화
    float3 kD_accumulated = kD_dir;         // 디렉셔널 라이트의 kD로 초기화

     // Add spot lights contribution
    for (int i = 0; i < LIGHT_NUM; i++)
    {
        // 스팟라이트 방향과 하프 벡터
        float3 L_spot = normalize(spotLights[i].position - input.worldPos.xyz);
        float3 H_spot = normalize(V + L_spot);
        float HdotV_spot = max(dot(H_spot, V), 0.0);
        
        // 이 스팟라이트의 프레넬 계산
        float3 F_spot = F_Schlick(HdotV_spot, F0);
        float3 kD_spot = (1.0 - F_spot) * (1.0 - metallic);
        
        // 프레넬과 kD 누적 (가중 평균 0.5)
        F_accumulated = lerp(F_accumulated, F_spot, 0.5);
        kD_accumulated = lerp(kD_accumulated, kD_spot, 0.5);
        
        // 기존 스팟라이트 계산
        totalSpotLight += CalculateSpotLight(
            spotLights[i],
            input.worldPos.xyz,
            N,
            V,
            baseColor,
            metallic,
            rough,
            F0
        );
    }
    
    // Ambient lighting
    float3 ambient = baseColor * 0.3f;
    
    // IBL contribution
    float3 iblDiffuse = GetIBLIrradiance(N);
    float3 iblSpecular = GetIBLRadiance(N, V, rough);
    float2 brdf = IntegrateBRDF(NdotV, rough);
    //float3 iblResult = lerp(kD_dir * iblDiffuse * baseColor, iblSpecular * (F_dir * brdf.x + brdf.y), metallic);
    
    // spotlight 누적된 값 선형보간
    float3 iblResult = lerp(
        kD_accumulated * iblDiffuse * baseColor,
        iblSpecular * (F_accumulated * brdf.x + brdf.y),
        metallic
    );
    // Emissive
    float3 emissive = any(EmissiveColor.Sample(samLinear, input.TexCoord).rgb > 0) ?
                     EmissiveColor.Sample(samLinear, input.TexCoord).rgb : 0;
    
    float3 rimColor = float3(0.0, 2.0, 0.0);
    
    // Combine all lighting
    //float3 finalRimColor = (0, 0, 0);
    //float3 rimLight = (0, 0, 0);
    //if(onOutline)
    //{
    //    float3 outerOutline = lerp(float3(0, 0, 0), outlineColor, edgeIntensity);
    //    float3 innerOutline = CardSelectionRimLight(N, V, innerOutlineColor);
    //   
    //
    //    finalRimColor = outerOutline + innerOutline;
    //    //finalRimColor = innerOutline;
    //} 
    
    // 디버그용 
    float3 debugVisualization = float3(0, 0, 0);
    bool showSpotLightDebug = true;
    if (showSpotLightDebug) // 디버그 플래그 추가 필요
    {
        for (int i = 0; i < LIGHT_NUM; i++)
        {
            debugVisualization += VisualizeSpotLightCone(input.worldPos.xyz, spotLights[i], baseColor);
        }
    }
    
    float3 color = directionalLight + totalSpotLight + ambient + iblResult + emissive;
    
    if(UpColor)
    {
        color *= 1.7f;
    }
    else
    {
        color *= 1.0f;
    }
    
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

    if(onBanned)
    {
        finalColor *= 0.2f;
    }
    
    return finalColor;
    //return float4(totalSpotLight, 1.0f);
    
}