#include "Header.hlsli"

PixelInputType main(VertexInputType input) 
{
    PixelInputType output;
   
	// 올바르게 행렬 연산을 하기 위하여 position 벡터를 w까지 있는 4성분이 있는 것으로 사용합니다.
    input.Position.w = 1.0f;

	// 정점의 위치를 월드, 뷰, 사영의 순으로 계산합니다.
    output.Position = mul(input.Position, worldMatrix);
    output.Position = mul(output.Position, viewMatrix);
    output.Position = mul(output.Position, projectionMatrix);
    
	// 픽셀 셰이더에서 사용하기 위해 입력 색상을 저장합니다.
    output.Color = input.Color;
    
    return output;
}