#pragma once
#include "Mesh.h"

class StaticMesh : public Mesh
{
public:
	StaticMesh() = default;
	virtual ~StaticMesh() = default;

	virtual void Update(const float _deltaTime) override;
private:

public:

private:

};
// TODO: 보류 사항 : 스켈레탈 매쉬 업데이트 만들어야 함.
