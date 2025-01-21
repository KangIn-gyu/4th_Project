#pragma once
#include "Mesh.h"

class SkeletalMesh : public Mesh
{
public:
	SkeletalMesh() = default;
	virtual ~SkeletalMesh() = default;

	// TODO : 나중에 구현
	virtual void Update(const float _deltaTime) override {};
private:

public:

private:
};
// TODO: 보류 : 스켈레탈 매쉬 구현해야됨 FBX로드도 안함