#include "pch.h"
#include "BoneWeightVertex.h"

void BoneWeightVertex::AddBoneData(int _id, float _weight)
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
