#pragma once
#include <assimp/scene.h>

class BoneInfo // 읽기용 정보
{
public:
	BoneInfo() = default;
	~BoneInfo() = default;

	void Set(const aiNode* _pNode);
	void SetParentBoneName(std::string_view _name);
	const std::string& GetName() { return name; }
	void SetOffsetMatrix(DXMath::Matrix _offsetMatrix);

	DXMath::Matrix GetOffsetMatrix() { return offsetMatrix; }
private:
	std::string name{};
	std::string parentBoneName{};

	DXMath::Matrix offsetMatrix {};			// 본에서의 메쉬의 상대적인 위치
	DXMath::Matrix relativeTransform {};	// 부모로부터의 상대적인 변환 
	int numChildren {};

	std::vector<std::string> meshNames;     // 본에 연결된 메쉬들의 이름
};

