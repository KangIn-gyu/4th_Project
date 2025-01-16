#pragma once
#include <directxtk/SimpleMath.h> 
namespace DX = DirectX;
namespace DXMath = DirectX::SimpleMath;


// 용도 : 각 버퍼의 데이터를 저장하는 곳
// alignas(16) 용도 16바이트로 정렬 예로 float 하나를 만들면 16으로 됨
// 매크로를 통해서 편하게 확인할 수 있다
// 상수 버퍼는 렌더러 클래스가 가지고 있게 할 예정 따로 컨테이너에 안넣음.

// 상수 버퍼 크기 검증을 위한 유틸리티 함수 여기서만 사용해서 유틸함수쪽으로 안뺌
template <typename T>
void ValidateConstantBufferSize()
{
	static_assert((sizeof(T) % 16) == 0, "Constant Buffer size must be 16-byte aligned");
} // 귀찮지만 생성자에 넣을것.

struct alignas(16) MatrixBuffer
{
	MatrixBuffer()
	{
		ValidateConstantBufferSize<MatrixBuffer>();
	}

	DXMath::Matrix worldMatrix {};
	DXMath::Matrix viewMatrix  {};
	DXMath::Matrix projectionMatrix {};
};

struct alignas(16) ObjectBuffer
{
	ObjectBuffer()
	{
		ValidateConstantBufferSize<ObjectBuffer>();
	}

	float metalness {};
	float roughness {};
	float padding0	{};
	float padding1	{};
};