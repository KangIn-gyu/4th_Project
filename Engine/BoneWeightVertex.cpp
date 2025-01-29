#include "pch.h"
#include "BoneWeightVertex.h"

void BoneWeightVertex::AddBoneData(int _id, float _weight)
{
	assert(blendweights[0] == 0.f || blendweights[1] == 0.f || blendweights[2] == 0.f || blendweights[3] == 0.f);
	for (int i = 0; i < 4; i++)
	{
		if (blendweights[i] == 0.f)
		{
			blendIndeces[i] = _id;
			blendweights[i] = _weight;
			return;
		}
	}
}
