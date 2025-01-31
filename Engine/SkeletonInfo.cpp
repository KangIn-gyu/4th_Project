#include "pch.h"
#include "SkeletonInfo.h"
#include "BoneInfo.h"
#include "Helper.h"

SkeletonInfo::~SkeletonInfo()
{
	SafeExtinction::SAFE_CLEAR_CONTAINER(bones);
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

int SkeletonInfo::GetBoneIndexByName(std::string_view _boneName)
{
	auto iter = boneMappingTable.find(_boneName.data());
	if (iter != boneMappingTable.end())
	{
		return iter->second;
	}
	else
	{
		throw std::runtime_error("index over bones array");
	}
}

void SkeletonInfo::AddBone(BoneInfo* _boneInfo)
{
	if (nullptr == _boneInfo) return;

	int index = static_cast<int>(bones.size());
	bones.push_back(_boneInfo);
	boneMappingTable[_boneInfo->GetName()] = index;
}
