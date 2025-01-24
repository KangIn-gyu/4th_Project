#pragma once
#include "Vertex.h"
#include "IndexBuffer.h"
#include "InputLayout.h"

class Transform;
class Mesh // 기반 클래스
{
	struct MeshInfo
	{	       
		MeshInfo(UINT _index) : fbxMeshIndex(_index) { meshName.reserve(20); }
		std::string meshName;
		VertexBuffer* vertexBuffer {};
		IndexBuffer* indexBuffer {};
		InputLayout inputLayout;
		Transform* transform {};

		UINT GetMeshIndex() { return  fbxMeshIndex; }
		void SetMeshIndex(UINT _index) { fbxMeshIndex = _index; }

		int GetMaterialIndex() { return materialIndex; }
		void SetMaterialIndex(int _index) { materialIndex = _index; }

		void Show();

	private:
		UINT fbxMeshIndex = -1; 
		int  materialIndex = 1;
	};

public:
	Mesh();
	~Mesh();

	Mesh(const Mesh& other);
	Mesh& operator=(const Mesh& _mesh);

	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& _mesh) noexcept;

	virtual void Update(const float _deltaTime) {};

	void SetVertexBuffer(VertexBuffer* _vertexBuffer);
	void SetIndexBuffer(IndexBuffer* _indexBuffer);
	void CreateInputLayout(const std::initializer_list<D3D11_INPUT_ELEMENT_DESC>& _elements, std::string_view _shaderfilePath);

	int GetMaterialIndex();
	void SetMaterialIndex(int _index);

	void SetFBXMeshIndex(UINT _index);
	void SetName(std::string_view _meshName);

	void SetTransform(Transform* _Transform);
	void SetTransformParent(Transform* _Transform);

	std::string GetName();
	UINT GetFbxIndex();
	MeshInfo* GetMeshInfo() { return meshInfo; }

private:

public:

protected:
	MeshInfo* meshInfo {};

private:

};

// 고민 사항 : 해당 매쉬에 이 텍스쳐를 사용해야 되요라고 알려주는게 필요 할수도 있다. 