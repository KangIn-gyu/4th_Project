#include "pch.h"
#include "Vertex.h"
#include "Helper.h"

#include "D3DClass.h"
VertexBuffer::VertexBuffer()
{
	
}

VertexBuffer::~VertexBuffer()
{

}

void VertexBuffer::Create(const std::vector<Vertex>& vertees)
{
	vertices = std::move(vertees); // 인덱스 버퍼와 같이 고려 사항

	HRESULT hr = 0; // 결과값.
	// 버텍스 정보 초기화	
	D3D11_BUFFER_DESC vbDesc = {};
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.ByteWidth = sizeof(Vertex) * vertices.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;

	// 정점 버퍼 생성.
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices.data(); // 배열 데이터 할당.

	HR_T(hr = D3DClass::GetD3DDevice()->CreateBuffer(&vbDesc, &vbData, &vertexBuffer)); // 버퍼 만들기
	
	vertextBufferStride = sizeof(Vertex);
	vertextBufferOffset = 0;
}
