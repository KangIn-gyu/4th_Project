#include "pch.h"
#include "BoneInfo.h"

void BoneInfo::Set(const aiNode* _pNode)
{
	name = std::string(_pNode->mName.C_Str());
	relativeTransform = DXMath::Matrix(&_pNode->mTransformation.a1).Transpose();
	numChildren = _pNode->mNumChildren;
}

void BoneInfo::SetParentBoneName(std::string_view _name)
{
	parentBoneName.assign(_name);
}

void BoneInfo::SetOffsetMatrix(DXMath::Matrix _offsetMatrix)
{
	offsetMatrix = _offsetMatrix;
}
