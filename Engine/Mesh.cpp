#include "pch.h"
#include "Mesh.h"
#include "Transform.h"
#include "Helper.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	SafeExtinction::SAFE_DELETE(meshInfo);
}

Mesh::Mesh(const Mesh& _other) // 복사
{
	SafeExtinction::SAFE_DELETE(meshInfo);
	meshInfo = new MeshInfo(_other.meshInfo->GetMeshIndex());
	meshInfo->indexBuffer = _other.meshInfo->indexBuffer;
	meshInfo->inputLayout = _other.meshInfo->inputLayout;
	meshInfo->vertexBuffer = _other.meshInfo->vertexBuffer;
	meshInfo->meshName = _other.meshInfo->meshName;
	meshInfo->transform = _other.meshInfo->transform;
}

Mesh& Mesh::operator=(const Mesh& _other) // 복사
{
	if (this != &_other)
	{
		SafeExtinction::SAFE_DELETE(meshInfo);
		meshInfo = new MeshInfo(_other.meshInfo->GetMeshIndex());
		meshInfo->indexBuffer = _other.meshInfo->indexBuffer;
		meshInfo->inputLayout = _other.meshInfo->inputLayout;
		meshInfo->vertexBuffer = _other.meshInfo->vertexBuffer;
		meshInfo->meshName = _other.meshInfo->meshName;
		meshInfo->transform = _other.meshInfo->transform;
	}
	return *this;
}

Mesh::Mesh(Mesh&& other) noexcept // 이동
{
	SafeExtinction::SAFE_DELETE(meshInfo);
	meshInfo = std::move(other.meshInfo);
	other.meshInfo = nullptr;
}

Mesh& Mesh::operator=(Mesh&& _mesh) noexcept // 이동
{
	if (this != &_mesh)
	{
		SafeExtinction::SAFE_DELETE(meshInfo);
		meshInfo = std::move(_mesh.meshInfo);
		_mesh.meshInfo = nullptr;
	}
	return *this;
}

void Mesh::SetVertexBuffer(VertexBuffer* _vertexBuffer)
{
	meshInfo->vertexBuffer = _vertexBuffer;
}

void Mesh::SetIndexBuffer(IndexBuffer* _indexBuffer)
{
	meshInfo->indexBuffer = _indexBuffer;
}

void Mesh::CreateInputLayout(const std::initializer_list<D3D11_INPUT_ELEMENT_DESC>& _elements, std::string_view _shaderfilePath)
{
	meshInfo->inputLayout.IASetInputLayout(_elements, _shaderfilePath);
}

void Mesh::SetName(std::string_view _meshName)
{
	meshInfo->meshName = _meshName;
}

void Mesh::SetFBXMeshIndex(UINT _index)
{
	meshInfo = new MeshInfo(_index);
}

void Mesh::SetTransform(Transform* _Transform)
{
	meshInfo->transform = _Transform;
}

void Mesh::SetTransformParent(Transform* _Transform)
{
	meshInfo->transform->SetParent(_Transform);
}

std::string Mesh::GetName()
{
	return meshInfo->meshName;
}

UINT Mesh::GetFbxIndex()
{
	return meshInfo->GetMeshIndex();
}

