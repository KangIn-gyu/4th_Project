#include "Header.hlsli"  

VS_SKYBOX_OUTPUT main(VS_SKYBOX_INPUT input)
{
    VS_SKYBOX_OUTPUT output;
    
    // Remove translation from view matrix for skybox
    matrix viewRotationOnly = viewMatrix;
    viewRotationOnly[3] = float4(0, 0, 0, 1);
    
    // World 변환 제거하고 직접 View, Projection 적용
    float4 pos = float4(input.Pos, 1.0);
    pos = mul(pos, viewRotationOnly);
    pos = mul(pos, projectionMatrix);
    
    output.Pos = pos.xyww; // z를 w로 설정하여 항상 가장 뒤에 그려지도록
    output.TexCoord = input.Pos;
    
    return output;
}