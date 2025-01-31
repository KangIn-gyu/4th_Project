#pragma once
#include "Vertex.h"


class BoneWeightVertex
{
public:
	BoneWeightVertex(){}
    BoneWeightVertex(const BoneWeightVertex& other);
    BoneWeightVertex(BoneWeightVertex&& other) noexcept;				// 이동 생성자
	BoneWeightVertex& operator = (const BoneWeightVertex& _other);
	BoneWeightVertex& operator = (BoneWeightVertex&& _other) noexcept; // 이동 대입 연산자

	void AddBoneData(unsigned int _id, float _weight);
	void LoadAiMeshToVertex(aiMesh* _aiMesh, int _index);

private:

public:

private:
	Vertex vertex;
	int blendIndeces[4] = {};
	float blendWeights[4] = {};
};

