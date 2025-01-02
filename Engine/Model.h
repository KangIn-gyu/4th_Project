#pragma once
#include "IResources.h"

class Mesh;
class Material;
class AiNode;
class ModelComponent;
class Model : public IResources
{
public:
	Model();
	virtual ~Model();
	virtual void Load(std::wstring_view _filePath) override {}; // 일단 보류 사항

	void Initialize();
	void SetAiNode(AiNode* _aiNode);
	void SetOwner(ModelComponent* _owner);
	void SetMesh(std::vector<Mesh> _meshs);
private:

public:

private:
	std::vector<std::shared_ptr<Material>> materials{};  // 고민이 됨 이거 하나만 있어도 될거 같은데?

	std::vector<Mesh> meshs {};
	AiNode* rootNode {};
	ModelComponent* owner;
};

