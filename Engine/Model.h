#pragma once
#include "IResources.h"
#include "AiNode.h"

class Mesh;
class Material;
class ModelComponent;
class Model : public IResources
{
	struct ModelData
	{
		std::vector<Mesh> meshs{};
		std::vector<Material*> materials{};
		std::vector<AiNode*> treeNode{};
		AiNode* rootNode {};
	};

public:
	Model();
	virtual ~Model();

	void Initialize();
	void Update(const float _deltaTime);
	void SetOwner(ModelComponent* _owner); // 용도 모델컴포넌트에서 가져올거 필요할때
	void SetMesh(std::vector<Mesh> _meshs);
	void SetMateria(std::vector<Material*> _materials);
	void SetTreeNode(std::vector<AiNode*> _treeNode);

	ModelData* GetModelData() { return data; }

private:
	virtual void Load(std::string_view _filePath) override {}; // 일단 보류 사항

public:

private:
	ModelComponent* owner;
	ModelData* data;
};

