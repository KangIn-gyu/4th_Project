#include "Header.hlsli"

PixelInputType main(VertexInputType input) 
{
    //PixelInputType output = (PixelInputType) 0;
    //input.Position.w = 1.0f;
    
    //matrix boneMatrix = mul(MatrixPalleteArray[input.BlendIndices.x], input.BlendWeight.x);
    //boneMatrix += mul(MatrixPalleteArray[input.BlendIndices.y], input.BlendWeight.y);
    //boneMatrix += mul(MatrixPalleteArray[input.BlendIndices.z], input.BlendWeight.z);
    //boneMatrix += mul(MatrixPalleteArray[input.BlendIndices.w], input.BlendWeight.w);
    
    //matrix world = mul(boneMatrix, worldMatrix);
    
    //output.Position = mul(input.Position, world);
    //output.worldPos = output.Position;
    //output.Position = mul(output.Position, viewMatrix);
    //output.Position = mul(output.Position, projectionMatrix);
    
    //output.Normal = normalize(mul(input.Normal, (float3x3) world));
    //output.Tangent = normalize(mul(input.Tangent, (float3x3) world));
    //output.Binormal = normalize(mul(input.Binormal, (float3x3) world));
    
    //output.Color = input.Color;
    //output.TexCoord = input.TexCoord;
    
    //return output;
    
    PixelInputType output;
	// 올바르게 행렬 연산을 하기 위하여 position 벡터를 w까지 있는 4성분이 있는 것으로 사용합니다.
    input.Position.w = 1.0f;

    // === 1. 본 애니메이션 적용 여부 판별 ===
    float weightSum = input.BlendWeight[0] + input.BlendWeight[1] + input.BlendWeight[2] + input.BlendWeight[3];

    float4 skinnedPosition = input.Position; // 기본값은 원래 정점 위치
    float3 skinnedNormal = input.Normal;
    float3 skinnedTangent = input.Tangent;
    float3 skinnedBinormal = input.Binormal;
    
    if (weightSum > 0.0f)  // 본이 적용된 정점인지 확인
    {
        skinnedPosition = float4(0, 0, 0, 0);
        skinnedNormal = float3(0, 0, 0);
        skinnedTangent = float3(0, 0, 0);
        skinnedBinormal = float3(0, 0, 0);

        // 본 애니메이션 적용 (최대 4개 본 가중치)
        for (int i = 0; i < 4; ++i)
        {
            uint index = input.BlendIndices[i]; // 본 인덱스
            float weight = input.BlendWeight[i]; // 본 가중치

            // 본 행렬을 이용한 정점 변환
            skinnedPosition += weight * mul(input.Position, MatrixPalleteArray[index]);
            skinnedNormal += weight * mul(input.Normal, (float3x3) MatrixPalleteArray[index]);
            skinnedTangent += weight * mul(input.Tangent, (float3x3) MatrixPalleteArray[index]);
            skinnedBinormal += weight * mul(input.Binormal, (float3x3) MatrixPalleteArray[index]);
        }

        // 정규화 (노멀, 탄젠트, 바이노멀)
        skinnedNormal = normalize(skinnedNormal);
        skinnedTangent = normalize(skinnedTangent);
        skinnedBinormal = normalize(skinnedBinormal);
        output.Position = skinnedPosition;

    }
	else
	{
        
        output.Position = mul(input.Position, worldMatrix);
    }
	// 정점의 위치를 월드, 뷰, 사영의 순으로 계산합니다.

    output.worldPos = output.Position; // 로컬 좌표값이 들어가게 하기 위해서
    output.Position = mul(output.Position, viewMatrix);
    output.Position = mul(output.Position, projectionMatrix);
    
    output.LightSpacePos = mul(float4(output.worldPos.xyz, 1.0f), lightViewProj);
    
	// 픽셀 셰이더에서 사용하기 위해 입력 색상을 저장합니다.
    output.Color = input.Color;
    output.TexCoord = input.TexCoord;
    
    // World Matrix에서 이동성분을 제외하고 적용하며,  scale 있을수 있으므로 normalize 사용한다.
    output.Normal = normalize(mul(skinnedNormal, (float3x3) worldMatrix));
    output.Tangent = normalize(mul(skinnedTangent, (float3x3) worldMatrix));
    output.Binormal = normalize(mul(skinnedBinormal, (float3x3) worldMatrix));
    
    return output;
}