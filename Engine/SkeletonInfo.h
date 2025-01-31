#pragma once

class BoneInfo;
class SkeletonInfo // 읽기용 정보
{
public:
	SkeletonInfo() = default;
	~SkeletonInfo();

	BoneInfo* GetBoneInfoByIndex(int _index);
	BoneInfo* GetBoneInfoByName(std::string_view _boneName);

private:

public:

private:
	std::vector<BoneInfo*> bones;
	std::unordered_map<std::string, int> boneMappingTable;
	std::unordered_map<std::string, int> meshMappingTalbe;
};

