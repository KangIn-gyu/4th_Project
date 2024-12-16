#pragma once

struct Vertex
{
	Vertex() : position(DXMath::Vector3::Zero) {}
	~Vertex() = default;

	DXMath::Vector3 position{};
};

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	void Create(const std::vector<Vertex>& vertees);

private:

public:

private:
	std::vector<Vertex> vertices{};       // 정점 모음
	ComPtr<ID3D11Buffer> vertexBuffer{};
	UINT vertextBufferStride{};		      // 버텍스 하나의 크기.
	UINT vertextBufferOffset{};		      // 버텍스 버퍼의 오프셋.
};