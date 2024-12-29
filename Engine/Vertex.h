#pragma once

struct Vertex
{
	Vertex() : position(DXMath::Vector4::Zero) {}
	~Vertex() = default;

	UINT vertexID {};            // 버텍스 아이디     4
	DXMath::Vector4 position{};  // 정점 위치 정보.  16
	DXMath::Color   color{};     // 기본 컬러        16
	DXMath::Vector2 uv{};        // uv 좌표          8
	DXMath::Vector3 normal{};    // 노말 벡터        12
	DXMath::Vector3 tangent{};	 // 탄젠트 벡터      12
	DXMath::Vector3 binormal{};	 // 바이노멀 벡터	    12
};

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	void Create(const std::vector<Vertex>& _vertees);

private:

public:
	std::vector<Vertex>  vertices{};       // 정점 모음

private:
	ComPtr<ID3D11Buffer> vertexBuffer{};
	UINT vertextBufferStride{};		       // 버텍스 하나의 크기.
	UINT vertextBufferOffset{};		       // 버텍스 버퍼의 오프셋.
};