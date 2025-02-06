#include "Helper.hlsli"

//PixelInputType main(VertexInputType input)
//{
//    PixelInputType output = (PixelInputType) 0;
    
//    // 월드 변환
//    float4 worldPos = mul(float4(input.Position.xyz, 1.0f), worldMatrix);
//    float3 worldNormal = normalize(mul(float4(input.Normal, 0.0f), worldMatrix).xyz);
    
//    // 뷰 공간으로 변환
//    float4 viewPos = mul(worldPos, viewMatrix);
//    float3 viewNormal = normalize(mul(float4(worldNormal, 0.0f), viewMatrix).xyz);
    
//    // 뷰 공간에서 노말 방향으로 확장
//    // NDC 공간에서 일정한 두께를 유지하도록 조정
//    float4 projPos = mul(viewPos, projectionMatrix);
//    float2 screenNormal = normalize(mul(float4(viewNormal, 0.0f), projectionMatrix).xy);
//    float2 pixelSize = 1.0f / 1920;
    
//    // 화면 공간에서 일정한 두께 유지
//    float OutlineWidth = 10.0f;
//    projPos.xy += screenNormal * OutlineWidth * pixelSize * projPos.w;
    
//    output.Position = projPos;
//    float3 OutlineColor = (1, 0, 0);
//    output.Color = float4(OutlineColor, 1.0f);
    
//    return output;
//}

//PixelInputType main(VertexInputType input)
//{
//    PixelInputType output = (PixelInputType) 0;
//    
//    // 기본 변환
//    float4 worldPos = mul(float4(input.Position.xyz, 1.0f), worldMatrix);
//    float3 worldNormal = normalize(mul(float4(input.Normal, 0.0f), worldMatrix).xyz);
//    
//    // NDC 공간으로 변환
//    float4 projPos = mul(mul(worldPos, viewMatrix), projectionMatrix);
//    
//    if (onOutline)
//    {
//        // NDC 공간에서 노말 확장
//        float3 normalVS = normalize(mul(mul(float4(input.Normal, 0.0f), worldMatrix), viewMatrix).xyz);
//        float2 normalNDC = normalize(mul(float4(normalVS, 0.0f), projectionMatrix).xy);
//        
//        float OutlineWidth = 0.005f; // NDC 공간에서의 크기이므로 작은 값 사용
//        projPos.xy += normalNDC * OutlineWidth * projPos.w;
//    }
//    
//    output.Position = projPos;
//    output.Color = float4(1.0f, 0.0f, 0.0f, 1.0f);
//    
//    return output;
//}

//PixelInputType main(VertexInputType input)
//{
//    PixelInputType output = (PixelInputType) 0;
//    
//    float OutlineWidth = 1.0f;
//    
//    // 로컬 공간에서 확장
//    float3 position = input.Position.xyz + (input.Normal * OutlineWidth);
//    
//    // 이후 변환
//    float4 worldPos = mul(float4(position, 1.0f), worldMatrix);
//    float4 viewPos = mul(worldPos, viewMatrix);
//    output.Position = mul(viewPos, projectionMatrix);
//    output.Color = float4(1.0f, 0.0f, 0.0f, 1.0f);
//    
//    return output;
//}

//PixelInputType main(VertexInputType input)
//{
//    PixelInputType output = (PixelInputType) 0;
//    
//    float OutlineWidth = 1.0f; // 적절한 값으로 조정 필요
//    
//    // 월드 공간에서 vertex를 법선 방향으로 확장
//    float3 worldNormal = normalize(mul(float4(input.Normal, 0.0f), worldMatrix).xyz);
//    float3 worldPos = input.Position.xyz + (worldNormal * OutlineWidth);
//    float4 finalWorldPos = mul(float4(worldPos, 1.0f), worldMatrix);
//    
//    // 이후 일반적인 변환
//    float4 viewPos = mul(finalWorldPos, viewMatrix);
//    output.Position = mul(viewPos, projectionMatrix);
//    output.Color = float4(1.0f, 0.0f, 0.0f, 1.0f);
//    
//    return output;
//}

PixelInputType main(VertexInputType input)
{
    PixelInputType output = (PixelInputType) 0;
    
    // 기본 변환
    float4 worldPos = mul(float4(input.Position.xyz, 1.0f), worldMatrix);
    float4 viewPos = mul(worldPos, viewMatrix);
    float4 projPos = mul(viewPos, projectionMatrix);
    
    if (onOutline)
    {
        // 뷰 공간의 노말을 NDC 공간으로 변환
        float3 viewNormal = normalize(mul(mul(float4(input.Normal, 0.0f), worldMatrix), viewMatrix).xyz);
        float2 ndcNormal = normalize(mul(float4(viewNormal, 0.0f), projectionMatrix).xy);
        
        // NDC 공간에서 일정한 두께로 확장
        float OutlineWidth = 0.005f;
        projPos.xy += ndcNormal * OutlineWidth * projPos.w;
    }
    
    output.Position = projPos;
    output.Color = float4(0.0f, 0.0f, 1.0f, 1.0f);
    return output;
}