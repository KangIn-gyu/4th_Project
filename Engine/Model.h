#pragma once
#include "IResources.h"
class Mesh;
class Material;
class Model : public IResources
{
public:
	Model();
	virtual ~Model();
	virtual void Load(std::wstring_view _filePath) override;

	
private:

public:

private:
	std::vector<std::shared_ptr<Material>> materials;
	std::vector<Mesh*> meshs;
};

