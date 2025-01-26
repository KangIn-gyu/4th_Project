#pragma once
#include "IResources.h"
#include "AiNode.h"

class Mesh;
class Material;
class ModelComponent;
class Animation;
class Model : public IResources
{
	struct ModelData
	{
		std::vector<Mesh*>* meshs{};
		std::vector<Material*>* materials {};
		std::vector<AiNode*>* treeNodes {};
		std::vector<Animation*>* animations{};
		AiNode* rootNode {};
		
		void Show();
	};

public:
	Model();
	virtual ~Model();

	void Initialize();
	void SetOwner(ModelComponent* _owner); // 용도 모델컴포넌트에서 가져올거 필요할때
	void SetMesh(std::vector<Mesh*>* _meshs);
	void SetMateria(std::vector<Material*>* _materials);
	void SetAnimation(std::vector<Animation*>* _animations);
	void SetNodes(std::vector<AiNode*>* _nodes);

	ModelData* GetModelData() { return data; }
	DXMath::Vector3 extent;
private:
	virtual void Load(std::string_view _filePath) override {}; // TODO: 보류 사항 모델의 로드가 필요할까??

public:

private:
	ModelComponent* owner;
	ModelData* data;
};

