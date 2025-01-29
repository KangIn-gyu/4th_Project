#include "Helper.hlsli"

//--------------------------------------------------------------------------------------
// Pixel Shader Main Function
//--------------------------------------------------------------------------------------
float4 main(PixelInputType input) : SV_TARGET
{ 
    //   int ID = input.VertexID;
    //--------------------------------------------------------------------------------------
    // Material Parameters
    //--------------------------------------------------------------------------------------
    float4 baseColorWithAlpha = SampleBaseColorWithGamma(TextureAlbedo, input.TexCoord, float4(1, 1, 1, 1));
    float3 baseColor = baseColorWithAlpha.rgb;
    
    float3 N = normalize(input.Normal);
    
    float4 normalSample = NormalMap.Sample(samLinear, input.TexCoord);
        // normal map 값이 있을 때만 (a > 0) 적용
    if (normalSample.a > 0.0f)
    {
        float3 normalMap = normalSample.rgb * 2.0f - 1.0f;
        float3x3 TBN = GetTangentSpace(input.Normal, input.Tangent);
        N = normalize(mul(normalMap, TBN));
    }
    
    float metallic = metalness;
    
    float metallicMap = MetalnessMap.Sample(samLinear, input.TexCoord).r;
    // metallic map 값이 0이면 기본값 유지
    if (metallicMap > 0.0f)
    {
        metallic *= metallicMap;
    }
    
    float rough = roughness;
    float roughnessMap = ShininessColor.Sample(samLinear, input.TexCoord).r;
    // roughness map 값이 0이면 기본값 유지
    if (roughnessMap > 0.0f)
    {
        rough *= roughnessMap;
    }
    
    //--------------------------------------------------------------------------------------
    // Lighting Vectors
    //--------------------------------------------------------------------------------------
    float3 V = normalize(eyePosition - input.worldPos.xyz);
    float3 L = normalize(-lightDirection);
    float3 H = normalize(V + L);
    
    float NdotL = dot(N, L);
    NdotL = max(NdotL, 0.0);

    float NdotV = max(dot(N, V), 0.0001);
    float NdotH = max(dot(N, H), 0.0);
    float HdotV = max(dot(H, V), 0.0);
    
    //--------------------------------------------------------------------------------------
    // PBR Calculations
    //--------------------------------------------------------------------------------------
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), baseColor, metallic);
    
    // Cook-Torrance BRDF
    float D = D_GGX(NdotH, max(0.1f, roughness));
    float3 F = F_Schlick(HdotV, F0);
    float G = G_Smith(NdotV, NdotL, roughness);
    
    float3 numerator = D * F * G;
    float denominator = 4.0 * NdotV * NdotL + 0.0001;
    float3 specular = numerator / denominator;

    // Energy conservation
    float3 kS = F;
    float3 kD = (1.0 - kS) * (1.0 - metallic);
    
    //--------------------------------------------------------------------------------------
    // Emissive Calculation
    //--------------------------------------------------------------------------------------
    float3 emissive = float3(0, 0, 0);
    
    float4 emissiveMap = EmissiveColor.Sample(samLinear, input.TexCoord);
    if (any(emissiveMap.rgb > 0))
    {
        emissive = emissiveMap.rgb;
    }
    
    //--------------------------------------------------------------------------------------
    // IBL Contribution 계산
    //--------------------------------------------------------------------------------------
    
    float3 iblDiffuse;
    float3 iblSpecular;
    
    iblDiffuse = GetIBLIrradiance(N);
    iblSpecular = GetIBLRadiance(N, V, roughness);
    
    // BRDF Integration
    float2 brdf = IntegrateBRDF(NdotV, roughness);
    float3 iblSpecularResult = iblSpecular * (F * brdf.x + brdf.y);
    
    // Diffuse와 Specular IBL 결합
    float3 iblResult = lerp(kD * iblDiffuse * baseColor, iblSpecular, metallic);
    
    //--------------------------------------------------------------------------------------
    // Final Color Composition
    //--------------------------------------------------------------------------------------
    // Ambient
    float3 ambient = baseColor;
    
    // Diffuse
    float3 diffuse = kD * baseColor / PI;
    
    // Direct lighting with shadow
    float3 directLight = (diffuse + specular) *  NdotL;
    
    // Ambient light는 그림자의 영향을 덜 받도록 조정
    float3 ambientLight = ambient;
    
    // Final Color with IBL and all components
    float3 color = directLight + ambientLight + iblResult + emissive;
    
    // Gamma Encode
    color = pow(color, 1.0f / GAMMA);
    
    //--------------------------------------------------------------------------------------
    // Opacity Calculation
    //--------------------------------------------------------------------------------------

    float4 finalColor = float4(color, 1.0f);
    
    float4 opacityColor = OpacityMap.Sample(samLinear, input.TexCoord);
    
    if (opacityColor.a < 0.01f)
    {
        // opacity map이 없는 부분은 baseColor의 알파값 사용
        finalColor.a = baseColorWithAlpha.a * input.Color.a;
    }
    else
    {
        // opacity map이 있는 부분
        float alpha = opacityColor.a;
        alpha = pow(alpha, 0.5f); // 알파값 커브 조정
        alpha = saturate(alpha * 1.5f); // 알파값 전체적으로 증가
            
        finalColor.a = alpha * input.Color.a;
        finalColor.rgb *= finalColor.a; // premultiplied alpha
    }
    
    // 알파 블렌딩 보정
    if (finalColor.a < 1.0f)
    {
        finalColor.rgb /= max(finalColor.a, 0.001f);
    }
    
    // 완전 투명한 픽셀 제거
    if (finalColor.a < 0.01f)
    {
        discard;
    }
    
    return finalColor; 
}