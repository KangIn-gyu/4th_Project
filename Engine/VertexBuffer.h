#pragma once
#include "Vertex.h"
#include "BoneWeightVertex.h"
#include "D3DClass.h" 
#include "Helper.h"

template<class T> // T가 Vertex 상속한 존재인지와 포인터 타입이 아닐경우만 처리
concept VertexType = (std::is_same_v<T, Vertex> || std::is_same_v<T, BoneWeightVertex>) && !std::is_pointer_v<T>;

// Vertex 88 바이트, BoneWeightVertex 120 바이트
class VertexBuffer
{
public:
	VertexBuffer() = default;
	~VertexBuffer() = default;

	template<VertexType T>
	void Create(const std::vector<T>& _vertees);

	ComPtr<ID3D11Buffer> GetBuffer() { return vertexBuffer; }
	int GetVertexSize();
private:

public:
	UINT vertextBufferOffset{};							// 버텍스 버퍼의 오프셋.
	UINT vertextBufferStride{};							// 버텍스 하나의 크기.

private:
	std::vector<Vertex>  vertices{};					// 정점 모음
	std::vector<BoneWeightVertex> boneWeightVertices{};
	ComPtr<ID3D11Buffer> vertexBuffer;
};

template<VertexType T>
void VertexBuffer::Create(const std::vector<T>& _vertees)
{
	HRESULT hr = 0; // 결과값.
	// 버텍스 정보 초기화	
	D3D11_BUFFER_DESC vbDesc = {};
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;

	// 정점 버퍼 생성.
	D3D11_SUBRESOURCE_DATA vbData = {};
	if constexpr (std::is_same_v<T, Vertex>)
	{
		vertices = _vertees; // 인덱스 버퍼와 같이 고려 사항
		vbDesc.ByteWidth = sizeof(T) * vertices.size();
		vbData.pSysMem = vertices.data(); // 배열 데이터 할당.
	}
	else if constexpr (std::is_same_v<T, BoneWeightVertex>)
	{
		boneWeightVertices = _vertees;
		vbDesc.ByteWidth = sizeof(T) * boneWeightVertices.size();
		vbData.pSysMem = boneWeightVertices.data(); // 배열 데이터 할당.
	}

	HR_T(hr = D3DClass::GetD3DDevice()->CreateBuffer(&vbDesc, &vbData, &vertexBuffer)); // 버퍼 만들기

	vertextBufferStride = sizeof(T);
	vertextBufferOffset = 0;
}
