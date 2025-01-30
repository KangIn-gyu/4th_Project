#pragma once
#include "Vertex.h"

class BoneWeightVertex : public Vertex
{
public:
	void AddBoneData(int _id, float _weight);

private:

public:

private:
	int blendIndeces[4] = {};
	float blendWeights[4] = {};
};

