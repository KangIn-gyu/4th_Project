#include "Header.hlsli"  // 파일 기준으로 경로를 들어가야 한다

float4 main(PixelInputType input) : SV_TARGET
{ // 일단 보류
 //   int ID = input.VertexID;    
    float4 finalColor = input.Color;
    
    float3 normal =   normalize(input.Normal);
    float3 biTanget = normalize(input.Binormal);
    float3 tangent =  normalize(input.Tangent);
    
    float3 nomalMap = NormalMap.Sample(samLinear, input.TexCoord);
    float3 speclarMap = SpecularColor.Sample(samLinear, input.TexCoord).xyz;
    float4 emissiveMap = EmissiveColor.Sample(samLinear, input.TexCoord);
    
    float4 albedoMap = TextureAlbedo.Sample(samLinear, input.TexCoord);
    if(all(albedoMap) == 0)
    {
        albedoMap = float4(1, 1, 1, 1);
    }
    
    float metallicMap = MetalnessMap.Sample(samLinear, input.TexCoord).r;
    float roughnessMap = ShininessColor.Sample(samLinear, input.TexCoord).r;
     
    return albedoMap;
}