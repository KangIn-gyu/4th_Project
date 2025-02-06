#include "Header.hlsli"

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
    input.Position.w = 1.0f;
    
    // 월드 변환
    output.Position = mul(input.Position, worldMatrix);
    output.worldPos = output.Position;
    
    // 뷰 변환
    output.Position = mul(output.Position, viewMatrix);
    
    // 투영 변환
    output.Position = mul(output.Position, projectionMatrix);
    
    // 그림자 맵핑을 위한 광원 시점 위치 계산
    output.LightSpacePos = mul(float4(output.worldPos.xyz, 1.0f), lightViewProj);
    
    // 색상과 텍스처 좌표 전달
    output.Color = input.Color;
    output.TexCoord = input.TexCoord;
    
    // 노말, 탄젠트, 바이노말 변환 (월드 행렬의 회전/스케일 성분만 적용)
    output.Normal = normalize(mul(input.Normal, (float3x3) worldMatrix));
    output.Tangent = normalize(mul(input.Tangent, (float3x3) worldMatrix));
    output.Binormal = normalize(mul(input.Binormal, (float3x3) worldMatrix));
    
    return output;
}