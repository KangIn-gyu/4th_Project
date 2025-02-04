#include "pch.h"
#include "SkeletalMesh.h"
#include "BoneInfo.h"
#include "ConstantBufferData.h"
#include "SkeletonInfo.h"
#include "ConstantBufferData.h"

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
		const std::string& name = boneReferences[i].GetName();
		BoneInfo* boneInfo = _skeletonInfo->GetBoneInfoByName(boneReferences[i].GetName());
	
		if (boneInfo == nullptr)
		{
			throw std::runtime_error("BoneInfo is NULL! Bone name: " + name);
		} 
		_pallete->array[index] = (boneInfo->GetOffsetMatrix() * BoneNodeWorldMatrix).Transpose(); 
	}
}

void SkeletalMesh::SetBoneReference(std::vector<BoneReference> _boneReferences)
{
	boneReferences = _boneReferences;
}
