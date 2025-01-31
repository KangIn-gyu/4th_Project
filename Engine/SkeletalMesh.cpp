#include "pch.h"
#include "SkeletalMesh.h"
#include "BoneInfo.h"
#include "ConstantBufferData.h"
#include "SkeletonInfo.h"

void SkeletalMesh::UpdateMatrixPallete(MatrixPallete* _pallete, SkeletonInfo* _skeletonInfo)
{
	for (UINT i = 0; i < boneReferences.size(); ++i)
	{
		DXMath::Matrix BoneNodeWorldMatrix = boneReferences[i].GetNodeWorldTransform();
		int index = boneReferences[i].GetIndex();
		std::string name = boneReferences[i].GetName();
		BoneInfo* boneInfo = _skeletonInfo->GetBoneInfoByName(name);

		assert(boneInfo != nullptr);
		_pallete->array[index] = (boneInfo->GetOffsetMatrix() * BoneNodeWorldMatrix).Transpose();
	}
}

void SkeletalMesh::SetBoneReference(std::vector<BoneReference> _boneReferences)
{
	boneReferences = _boneReferences;
}
