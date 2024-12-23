#include "pch.h"
#include "Mesh.h"

void Mesh::CreateVertexBuffer(const std::vector<Vertex>& _vertees)
{
	vertexBuffer.Create(_vertees);
}

void Mesh::CreateIndexBuffer(const std::vector<DWORD>& _indices)
{
	indexBuffer.Create(_indices);
}

void Mesh::CreateInputLayout(const std::initializer_list<D3D11_INPUT_ELEMENT_DESC>& _elements, const ComPtr<ID3DBlob>& _shaderBuffer)
{
	inputLayout.IASetInputLayout(_elements, _shaderBuffer);
}
