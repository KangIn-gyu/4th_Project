#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class AiNode;
class FBXLoader
{
public:
	FBXLoader() = default;
	~FBXLoader() = default;

	void FBXLoad(std::wstring_view _filePath);
	void ProcessNode(aiNode* _node, const aiScene* _scene, AiNode* _parent);

private:
	bool HasBones(const aiScene* _scene);
	std::vector<DWORD> ProessIndexs(aiMesh* _mesh, unsigned int _indexSize);
	std::vector<Vertex> ProcessVertexs(aiMesh* _mesh, unsigned int _vertexSize);
	void ProcessMesh(aiMesh* _mesh, const aiScene* _scene);
	void ProcessMaterial(aiMesh* _mesh, const aiScene* _scene);
public:

private:
	Assimp::Importer importer;
	unsigned int importFlags {};
	bool isStaticMesh = true;
};

