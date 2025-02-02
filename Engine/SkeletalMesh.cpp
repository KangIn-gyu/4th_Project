#include "pch.h"
#include "SkeletalMesh.h"
#include "BoneInfo.h"
#include "ConstantBufferData.h"
#include "SkeletonInfo.h"

void SkeletalMesh::UpdateMatrixPallete(MatrixPallete* _pallete, SkeletonInfo* _skeletonInfo)
{
	if (boneReferences.size() > BoneBufferMaxSize)
	{
		char buffer[128] = "";
		sprintf_s(buffer, sizeof(buffer), "Bone reference size exceeds limit (%d)", BoneBufferMaxSize);
		throw std::runtime_error(buffer);
	}

	for (UINT i = 0; i < boneReferences.size(); ++i)
	{
		DXMath::Matrix BoneNodeWorldMatrix = *boneReferences[i].GetNodeWorldTransform(); // 본의 
		int index = boneReferences[i].GetIndex(); // 전체 본의 이름 순서로 정한 인덱스
		std::string name = boneReferences[i].GetName();
		BoneInfo* boneInfo = _skeletonInfo->GetBoneInfoByName(name);
	
		if (boneInfo == nullptr)
		{
			throw std::runtime_error("BoneInfo is NULL! Bone name: " + name);
		}
		_pallete->array[index] = (boneInfo->GetOffsetMatrix() * BoneNodeWorldMatrix).Transpose(); //  여기 인덱스 문제 있음 문제 해결해야됨
	}
}

void SkeletalMesh::SetBoneReference(std::vector<BoneReference> _boneReferences)
{
	boneReferences = _boneReferences;
}
