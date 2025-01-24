#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "AiNode.h"

class IndexBuffer;
class VertexBuffer;
class Material;
class Model;
class FBXLoader
{
public:
	FBXLoader() = default;
	~FBXLoader();

	DXMath::Vector3 CalculateBoundingBox(const aiScene* scene);
	std::shared_ptr<Model> FBXLoad(std::string_view _filePath); // 로드하고 무엇을 리턴해야 될가..?
	void AllShow();
	void FindShow(std::string_view _filePath);
	std::vector<AiNode*> DeepCopyAiNodes(std::string_view key);
private:
	bool HasBones(const aiScene* _scene);
	AiNode* ProcessNode(aiNode* _node, const aiScene* _scene, AiNode* _parent, const std::string_view _filePath); // 여기 _filePath 추가한 이유 키값 공통으로 사용하기 위해
	
	void CollectNodes(AiNode* _rootNode, std::vector<AiNode*>* _nodes);

	void ProessIndexs(aiMesh* _mesh, unsigned int _indexSize, const std::string_view _filePath);
	void ProcessVertexs(aiMesh* _mesh, unsigned int _vertexSize, const std::string_view _filePath);

	void ProcessMesh(aiMesh* _mesh, const aiScene* _scene, const std::string_view _filePath);
	void SaveMeshData(std::string_view _filePath, Mesh* _mesh);
	void ProcessMaterial(const aiScene* _scene, const std::string_view _modelFilePath);

	void ProcessAnimation(const aiScene* scene);

	// 테스트용 리소스 정보
	void ShowMaterials();
	void ShowMesh();
	void ShowAiNode();
	void ShowVertexBuffer();
	void ShowIndexBuffer();
	void NodeAndMeshIndex(const std::string_view _filePath);

public:
	DXMath::Vector3 Min;
	DXMath::Vector3 Max;

private:
	Assimp::Importer importer;
	unsigned int importFlags {};
	bool isStaticMesh = true;

	const std::string texturesFolder = "Textures/"; // 폴더 가르키기 용

	std::unordered_map<std::string, int> nameCountMap; // 노드끼리 이름 같은 체크용

	std::unordered_map<std::string, std::vector<AiNode*>> aiNodeMap {};
	std::unordered_map<std::string, std::vector<VertexBuffer*>> vertexBufferMap {}; 
	std::unordered_map<std::string, std::vector<IndexBuffer*>>  indexBufferMap  {}; 
	std::unordered_map<std::string, std::vector<Mesh*>> meshMap {};           // 매쉬 모음
	std::unordered_map<std::string, std::vector<Material*>> materials {};  // 메테리얼 모음
};

// 텍스처는 리소스시스템에서 처리함
// meshs, materials, index/vertexBuffer 키값은 처음 로드할때 사용한 FBXLoad(std::wstring_view _filePath); 여기 파일 경로 값이다