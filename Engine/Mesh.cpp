#include "pch.h"
#include "Mesh.h"
#include "Transform.h"

void Mesh::CreateInputLayout(const std::initializer_list<D3D11_INPUT_ELEMENT_DESC>& _elements, const ComPtr<ID3DBlob>& _shaderBuffer)
{
	inputLayout.IASetInputLayout(_elements, _shaderBuffer);
}

void Mesh::SetTransform(Transform* _Transform)
{
	transform = _Transform;
}

void Mesh::SetTransformParent(Transform* _Transform)
{
	transform->SetParent(_Transform);
}
