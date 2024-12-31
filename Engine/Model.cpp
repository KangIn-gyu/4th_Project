#include "pch.h"
#include "Model.h"

Model::Model()
{

}

Model::~Model()
{

}

void Model::Load(std::wstring_view _filePath)
{

}

void Model::SetAiNode(AiNode* _aiNode)
{
	rootNode = _aiNode;
}
