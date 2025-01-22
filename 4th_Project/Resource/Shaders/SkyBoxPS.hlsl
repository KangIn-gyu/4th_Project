#include "Header.hlsli" 

float4 main(VS_SKYBOX_OUTPUT input) : SV_Target
{
    float3 envColor = EnvironmentMap.Sample(samLinear, input.TexCoord).rgb;
    
    // HDR Åæ¸ÅÇÎ
    //envColor = ACESFilmicToneMapping(envColor);
    envColor = pow(envColor, 1.0f / GAMMA);
    
    return float4(envColor, 1.0);
}