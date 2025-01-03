#pragma once
#include "Vertex.h"
#include "IndexBuffer.h"
#include "InputLayout.h"

class Transform;
class Mesh // 기반 클래스
{
public:
	Mesh() { meshName.reserve(20); }
	virtual ~Mesh();

	void SetVertexBuffer(VertexBuffer* _vertexBuffer) { vertexBuffer = _vertexBuffer; }
	void SetIndexBuffer(IndexBuffer* _indexBuffer) { indexBuffer = _indexBuffer; }
	void CreateInputLayout(const std::initializer_list<D3D11_INPUT_ELEMENT_DESC>& _elements, std::wstring_view _shaderfilePath);
	std::string GetMeshName() { return meshName; }

	void SetName(std::string_view _meshName) { meshName = _meshName.data(); }
	void SetFBXMeshIndex(UINT _index) { fbxMeshIndex = _index; }

	void SetTransform(Transform* _Transform);
	void SetTransformParent(Transform* _Transform);

	std::string GetName() { return meshName; }
	UINT GetFbxIndex() { return fbxMeshIndex; }

private:

public:

protected:
	UINT fbxMeshIndex = -1;   // FBX로더를 통해서 좋은 지식 : 메테리얼 인덱스와 매쉬 인덱스가 같다


	VertexBuffer* vertexBuffer{};
	IndexBuffer*  indexBuffer{};
	InputLayout   inputLayout{};

	std::string meshName {};
	Transform* transform {};
private:

};

// 고민 사항 : 해당 매쉬에 이 텍스쳐를 사용해야 되요라고 알려주는게 필요 할수도 있다. 