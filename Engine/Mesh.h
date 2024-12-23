#pragma once
#include "InputLayout.h"
#include "Vertex.h"
#include "IndexBuffer.h"
#include "Shader.h"

class TransformComponent;
class Mesh // 기반 클래스
{
public:
	Mesh() {};
	~Mesh() {};

	void CreateVertexBuffer(const std::vector<Vertex>& _vertees);
	void CreateIndexBuffer(const std::vector<DWORD>& _indices);
	void CreateInputLayout(const std::initializer_list<D3D11_INPUT_ELEMENT_DESC>& _elements, const ComPtr<ID3DBlob>& _shaderBuffer);
private:

public:

private:
	VertexBuffer vertexBuffer {};
	IndexBuffer  indexBuffer  {};
	InputLayout  inputLayout  {};
};

