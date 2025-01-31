#pragma once
#include "Mesh.h"
#include "BoneReference.h"

class MatrixPallete;
class SkeletonInfo;
class SkeletalMesh : public Mesh
{
public:
	SkeletalMesh() = default;
	virtual ~SkeletalMesh() = default;

	void UpdateMatrixPallete(MatrixPallete* _pallete, SkeletonInfo* _skeletonInfo);
	void SetBoneReference(std::vector<BoneReference> _boneReferences);
	int GetBoneReferencesSize() { return boneReferences.size(); }
	std::vector<BoneReference>& GetBoneReferences() { return boneReferences; }
private:

public:

private:
	std::vector<BoneReference> boneReferences;
};
