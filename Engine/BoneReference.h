#pragma once

class BoneReference 
{
public:
	BoneReference() = default;
	~BoneReference() = default;

	void SetName(std::string_view _name) { name.assign(_name); }
	void SetIndex(int _index) { boneIndex = _index; }
	void SetNodeWolrdTransform(DXMath::Matrix* _nodeWorldTransform); // 이거 고민이 필요

	DXMath::Matrix* GetNodeWorldTransform() { return nodeWorldTransform; }
	int GetIndex() { return boneIndex; }
	const std::string& GetName();
private:

public:

private:
	std::string name;
	int boneIndex = -1;
	DXMath::Matrix* nodeWorldTransform{};
};

