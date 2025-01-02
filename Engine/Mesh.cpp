#include "pch.h"
#include "Mesh.h"
#include "Transform.h"

Mesh::~Mesh()
{
//	VertexBuffer, IndexBuffer 이거 가른키고만 있는데 여기서 삭제할 필요가 있을가?? 고민좀 더 해보자
}

void Mesh::CreateInputLayout(const std::initializer_list<D3D11_INPUT_ELEMENT_DESC>& _elements, std::wstring_view _shaderfilePath)
{
	inputLayout.IASetInputLayout(_elements, _shaderfilePath);
}

void Mesh::SetTransform(Transform* _Transform)
{
	transform = _Transform;
}

void Mesh::SetTransformParent(Transform* _Transform)
{
	transform->SetParent(_Transform);
}
