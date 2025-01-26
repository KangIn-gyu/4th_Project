#include "pch.h"
#include "AnimationNode.h"

void AnimationNode::Create(aiNodeAnim* _assimpAiNodeAnim, float _tickPerSec, float _totalAnimationTime)
{
	double timeFactor = _tickPerSec > 0.0 ? (1.0 / _tickPerSec) : 1.0;
	animationDuration = static_cast<float>(_totalAnimationTime);

	positionKeys.resize(_assimpAiNodeAnim->mNumPositionKeys);
	rotationKeys.resize(_assimpAiNodeAnim->mNumRotationKeys);
	scaleKeys.resize(_assimpAiNodeAnim->mNumScalingKeys);

    for (UINT i = 0; i < _assimpAiNodeAnim->mNumPositionKeys; ++i)
    { // position
        PositionKey key{};
        key.time = static_cast<float>(_assimpAiNodeAnim->mPositionKeys[i].mTime * timeFactor);
        key.position = DirectX::SimpleMath::Vector3(
            _assimpAiNodeAnim->mPositionKeys[i].mValue.x,
            _assimpAiNodeAnim->mPositionKeys[i].mValue.y,
            _assimpAiNodeAnim->mPositionKeys[i].mValue.z
        );
        positionKeys[i] = key;
    }

    for (UINT i = 0; i < _assimpAiNodeAnim->mNumRotationKeys; ++i)
    { // Rotation
        RotationKey key;
        key.time = static_cast<float>(_assimpAiNodeAnim->mRotationKeys[i].mTime * timeFactor);
        key.rotation = DirectX::SimpleMath::Quaternion(
            _assimpAiNodeAnim->mRotationKeys[i].mValue.x,
            _assimpAiNodeAnim->mRotationKeys[i].mValue.y,
            _assimpAiNodeAnim->mRotationKeys[i].mValue.z,
            _assimpAiNodeAnim->mRotationKeys[i].mValue.w
        );
        rotationKeys[i] = key;
    }

    for (UINT i = 0; i < _assimpAiNodeAnim->mNumScalingKeys; ++i)
    { // Scale
        ScaleKey key;
        key.time = static_cast<float>(_assimpAiNodeAnim->mScalingKeys[i].mTime * timeFactor);
        key.scale = DirectX::SimpleMath::Vector3(
            _assimpAiNodeAnim->mScalingKeys[i].mValue.x,
            _assimpAiNodeAnim->mScalingKeys[i].mValue.y,
            _assimpAiNodeAnim->mScalingKeys[i].mValue.z
        );
        scaleKeys[i] = key;
    }
}

void AnimationNode::Evaluate(const float _progressTime, DXMath::Vector3& _position, DXMath::Quaternion& _rotation, DXMath::Vector3& _scale)
{
    if (_progressTime < 0.0f || _progressTime > 1.0f)
    {
        std::cout << "Evaluate : 오류" << '\n';
        return;
    }

    float currentTime = _progressTime * animationDuration;
    _position = InterpolatePosition(currentTime);
    _rotation = InterpolateRotation(currentTime);
    _scale = InterpolateScale(currentTime);
}

void AnimationNode::SetName(std::string_view _nodeName)
{
    nodeName.assign(_nodeName);
}

DXMath::Vector3 AnimationNode::InterpolatePosition(float _currTime)
{
    if (positionKeys.empty())
    {
        return DirectX::SimpleMath::Vector3::Zero;
    }

    if (positionKeys.size() == 1)
    {
        return positionKeys[0].position;
    }

    for (size_t i = 1; i < positionKeys.size(); ++i)
    {   // 현재 시간에 가장 가까운 Position Key 찾기
        if (positionKeys[i].time >= _currTime)
        {
            float t = (_currTime - positionKeys[i - 1].time) / (positionKeys[i].time - positionKeys[i - 1].time);
            return DirectX::SimpleMath::Vector3::Lerp(positionKeys[i - 1].position, positionKeys[i].position, t);
        }
    }
    return positionKeys.back().position;
}

DXMath::Quaternion AnimationNode::InterpolateRotation(float _currTime)
{
    if (rotationKeys.empty())
    {
        return DirectX::SimpleMath::Quaternion::Identity;
    }

    if (rotationKeys.size() == 1) 
    {
        return rotationKeys[0].rotation;
    }

    for (size_t i = 1; i < rotationKeys.size(); ++i)
    {
        if (rotationKeys[i].time >= _currTime)
        {
            float t = (_currTime - rotationKeys[i - 1].time) / (rotationKeys[i].time - rotationKeys[i - 1].time);
            return DirectX::SimpleMath::Quaternion::Slerp(rotationKeys[i - 1].rotation, rotationKeys[i].rotation, t);
        }
    }
    return rotationKeys.back().rotation;
}

DXMath::Vector3 AnimationNode::InterpolateScale(float _currTime)
{
    if (scaleKeys.empty()) 
    { 
        return DirectX::SimpleMath::Vector3::One; 
    }

    if (scaleKeys.size() == 1) 
    { 
        return scaleKeys[0].scale;
    }

    for (size_t i = 1; i < scaleKeys.size(); ++i)
    {
        if (scaleKeys[i].time >= _currTime)
        {
            float t = (_currTime - scaleKeys[i - 1].time) / (scaleKeys[i].time - scaleKeys[i - 1].time);
            return DirectX::SimpleMath::Vector3::Lerp(scaleKeys[i - 1].scale, scaleKeys[i].scale, t);
        }
    }
    return scaleKeys.back().scale;
}
