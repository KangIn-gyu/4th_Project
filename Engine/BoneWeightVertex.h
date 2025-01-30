#pragma once
#include "Vertex.h"


class BoneWeightVertex : public Vertex
{
public:
	void AddBoneData(int _id, float _weight);
	BoneWeightVertex() : Vertex() {}
    BoneWeightVertex(const BoneWeightVertex& other);
    BoneWeightVertex(BoneWeightVertex&& other) noexcept;      // 이동 생성자
	BoneWeightVertex& operator = (const BoneWeightVertex& _other);
	BoneWeightVertex& operator = (BoneWeightVertex&& _other) noexcept; // 이동 대입 연산자

	virtual void LoadAiMeshToVertex(aiMesh* _aiMesh, int _index) override;
private:

public:

private:
	int blendIndeces[4] = {};
	float blendWeights[4] = {};
};

