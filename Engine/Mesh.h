#pragma once
#include "Vertex.h"
#include "IndexBuffer.h"
#include "InputLayout.h"

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

// 고민 사항 : 해당 매쉬에 이 텍스쳐를 사용해야 되요라고 알려주는게 필요 할수도 있다. 