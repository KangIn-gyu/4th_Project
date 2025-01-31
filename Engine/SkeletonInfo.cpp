#include "pch.h"
#include "SkeletonInfo.h"

SkeletonInfo::~SkeletonInfo()
{
}

BoneInfo* SkeletonInfo::GetBoneInfoByIndex(int _index)
{
	if (_index >= 0 && _index < bones.size())
	{
		return bones[_index];
	}
	return nullptr;
}

BoneInfo* SkeletonInfo::GetBoneInfoByName(std::string_view _boneName)
{
	auto boneData = boneMappingTable.find(_boneName.data());
	if (boneData != boneMappingTable.end())
	{
		return bones[boneData->second];
	}
	return nullptr;
}
