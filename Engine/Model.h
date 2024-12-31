#pragma once
#include "IResources.h"

class Mesh;
class Material;
class AiNode;
class Model : public IResources
{
public:
	Model();
	virtual ~Model();
	virtual void Load(std::wstring_view _filePath) override;

	void SetAiNode(AiNode* _aiNode);
private:

public:

private:
	std::vector<std::shared_ptr<Material>> materials{};  // 고민이 됨 이거 하나만 있어도 될거 같은데?

	std::vector<Mesh*> meshs {};
	AiNode* rootNode {};
};

