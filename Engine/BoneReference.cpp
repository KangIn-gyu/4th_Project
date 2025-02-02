#include "pch.h"
#include "BoneReference.h"

void BoneReference::SetNodeWolrdTransform(DXMath::Matrix* _nodeWorldTransform)
{
	nodeWorldTransform = _nodeWorldTransform;
}

const std::string& BoneReference::GetName()
{
    if (name.empty()) 
    {
        throw std::runtime_error("Bone name is empty! Possible uninitialized BoneReference.");
    }
    return name;
}
