#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class AiNode;
class IndexBuffer;
class VertexBuffer;
class Material;
class Model;
class FBXLoader
{
public:
	FBXLoader() = default;
	~FBXLoader();

	std::shared_ptr<Model> FBXLoad(std::wstring_view _filePath); // 로드하고 무엇을 리턴해야 될가..?
private:
	bool HasBones(const aiScene* _scene);
	AiNode* ProcessNode(aiNode* _node, const aiScene* _scene, AiNode* _parent);
	void ProessIndexs(aiMesh* _mesh, unsigned int _indexSize);
	void ProcessVertexs(aiMesh* _mesh, unsigned int _vertexSize);

	void ProcessMesh(aiMesh* _mesh, const aiScene* _scene);
	void SaveMeshData(std::string_view _name, Mesh _mesh);
	void ProcessMaterial(const aiScene* _scene, const std::wstring_view _modelFilePath);

public:

private:
	Assimp::Importer importer;
	unsigned int importFlags {};
	bool isStaticMesh = true;

	const std::wstring texturesFolder = L"Textures/"; // 폴더 가르키기 용

	std::unordered_map<std::string, VertexBuffer*> vertexBufferMap {};
	std::unordered_map<std::string, IndexBuffer*>  indexBufferMap  {};
	std::unordered_map<std::string, std::vector<Mesh>> meshs {};
	std::unordered_map<std::string, std::vector<Material*>> materials;
};

