#pragma once
#include "BoneInfo.h"
// class BoneInfo;
class SkeletonInfo // 읽기용 정보
{
public:
	SkeletonInfo() = default;
	~SkeletonInfo();

	BoneInfo* GetBoneInfoByIndex(int _index);
	BoneInfo* GetBoneInfoByName(const std::string& _boneName);
	int GetBoneIndexByName(std::string_view _boneName);

	void AddBone(BoneInfo* _boneInfo);
private:

public:

private:
	std::vector<BoneInfo*> bones;
	std::unordered_map<std::string, int> boneMappingTable;
	std::unordered_map<std::string, int> meshMappingTalbe;
};

