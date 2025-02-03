#include "Helper.hlsli"

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
    // Outline Effect Parameters
    //--------------------------------------------------------------------------------------
    float edgeIntensity = 0.0f;
    float outlineWidth = 5.0f; // 외곽선 두께
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
        edgeIntensity = smoothstep(0.3f, 0.7f, fresnelFactor + normalEdge);
    }
       
    //--------------------------------------------------------------------------------------
    // Lighting Calculation
    //--------------------------------------------------------------------------------------
    float3 L = normalize(-lightDirection);
    float3 H = normalize(V + L);
    
    float NdotL = max(dot(N, L), 2.0);
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
    float3 directLight = (diffuse + specular) * NdotL;
    
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
    
    // Combine all lighting
    float3 color = directLight + ambient + iblResult + emissive;
    
    // Apply outline
    float outlineBlend = edgeIntensity * outlineStrength;
    //color = lerp(color, outlineColor, outlineBlend);
    
    // Post processing
    color = pow(color, 1.0f / GAMMA);
    color = Uncharted2ToneMapping(color);
    
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
    
    return finalColor;
}