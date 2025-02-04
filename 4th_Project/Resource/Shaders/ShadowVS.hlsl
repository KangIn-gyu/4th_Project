 #include "Header.hlsli"

struct VS_SHADOW_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 DepthPos : TEXCOORD0;
};

VS_SHADOW_OUTPUT main(VertexInputType input)
{
    VS_SHADOW_OUTPUT output = (VS_SHADOW_OUTPUT) 0;
    
    // 스키닝 계산 - shared.fxh의 본 구조체 사용
    float4 skinnedPosition = float4(0, 0, 0, 0);
    float weightSum = input.BlendWeight.x + input.BlendWeight.y +
                     input.BlendWeight.z + input.BlendWeight.w;
    
    if (weightSum > 0.0f)
    {
        float4 pos = float4(input.Position.xyz, 1.0f);
        skinnedPosition = mul(pos, MatrixPalleteArray[input.BlendIndices.x]) * input.BlendWeight.x +
                         mul(pos, MatrixPalleteArray[input.BlendIndices.y]) * input.BlendWeight.y +
                         mul(pos, MatrixPalleteArray[input.BlendIndices.z]) * input.BlendWeight.z +
                         mul(pos, MatrixPalleteArray[input.BlendIndices.w]) * input.BlendWeight.w;
    }
    else
    {
        skinnedPosition = float4(input.Position.xyz, 1.0f);
    }
    
    // 월드 변환 후 라이트 뷰-프로젝션 적용
    float4 worldPosition = mul(skinnedPosition, worldMatrix);
    output.Pos = mul(worldPosition, lightViewProj);
    
    output.DepthPos = output.Pos;
    output.DepthPos.xyz /= output.DepthPos.w;
    
    return output;

}