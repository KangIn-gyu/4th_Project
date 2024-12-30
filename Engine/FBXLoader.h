#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class AiNode;
class FBXLoader
{
public:
	FBXLoader() = default;
	~FBXLoader() = default;

	void FBXLoad(std::wstring_view _filePath);
	void ProcessNode(aiNode* _node, const aiScene* _scene, AiNode* _parent);

private:

public:

private:
	Assimp::Importer importer;
	unsigned int importFlags {};
};

