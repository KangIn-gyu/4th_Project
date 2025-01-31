#include "pch.h"
#include "BoneWeightVertex.h"

BoneWeightVertex::BoneWeightVertex(const BoneWeightVertex& other)
{
	vertex = other.vertex;
	std::copy(std::begin(other.blendIndeces), std::end(other.blendIndeces), std::begin(blendIndeces));
	std::copy(std::begin(other.blendWeights), std::end(other.blendWeights), std::begin(blendWeights));
}

BoneWeightVertex::BoneWeightVertex(BoneWeightVertex&& other) noexcept
{
	vertex = std::move(other.vertex);
	std::move(std::begin(other.blendIndeces), std::end(other.blendIndeces), std::begin(blendIndeces));
	std::move(std::begin(other.blendWeights), std::end(other.blendWeights), std::begin(blendWeights));
}

BoneWeightVertex& BoneWeightVertex::operator=(const BoneWeightVertex& _other)
{
	if (this != &_other)  // 자기 자신에게 대입하는 경우를 방지
	{
		vertex = _other.vertex;
		std::copy(std::begin(_other.blendIndeces), std::end(_other.blendIndeces), std::begin(blendIndeces));
		std::copy(std::begin(_other.blendWeights), std::end(_other.blendWeights), std::begin(blendWeights));
	}
	return *this;
}

BoneWeightVertex& BoneWeightVertex::operator=(BoneWeightVertex&& _other) noexcept
{
	if (this != &_other)  // 자기 자신에게 대입하는 경우를 방지
	{
		vertex = std::move(_other.vertex);
		std::move(std::begin(_other.blendIndeces), std::end(_other.blendIndeces), std::begin(blendIndeces));
		std::move(std::begin(_other.blendWeights), std::end(_other.blendWeights), std::begin(blendWeights));

		// 원본 객체의 상태를 비워두거나 초기화
		std::fill(std::begin(_other.blendIndeces), std::end(_other.blendIndeces), 0);
		std::fill(std::begin(_other.blendWeights), std::end(_other.blendWeights), 0.0f);
	}
	return *this;
}

void BoneWeightVertex::AddBoneData(unsigned int _id, float _weight)
{
	assert(blendWeights[0] == 0.f || blendWeights[1] == 0.f || blendWeights[2] == 0.f || blendWeights[3] == 0.f);
	for (int i = 0; i < 4; i++)
	{
		if (blendWeights[i] == 0.f)
		{
			blendIndeces[i] = _id;
			blendWeights[i] = _weight;
			return;
		}
	}
}

void BoneWeightVertex::LoadAiMeshToVertex(aiMesh* _aiMesh, int _index)
{
	vertex.LoadAiMeshToVertex(_aiMesh, _index);
}

