#pragma once

struct PositionKey
{
	float time = 0.f;
	DXMath::Vector3 position;
};

struct RotationKey
{
	float time = 0.f;
	DXMath::Quaternion rotation;
};

struct ScaleKey
{
	float time = 0.f;
	DXMath::Vector3 scale;
};

class AnimationNode
{
public:
	AnimationNode() = default;
	~AnimationNode() = default;

	void Create(aiNodeAnim* _assimpAiNodeAnim, float _tickPerSec, float _totalAnimationTime);
	void Evaluate(const float& _progressTime, DXMath::Vector3& _position, DXMath::Quaternion& _rotation, DXMath::Vector3& _scale);
	void SetName(std::string_view _nodeName);
private:
	DXMath::Vector3 InterpolatePosition(float& currTime);
	DXMath::Quaternion InterpolateRotation(float& currTime);
	DXMath::Vector3 InterpolateScale(float& currTime);

public:

private:
	// AnimationKey
	std::vector<PositionKey> positionKeys;
	std::vector<RotationKey> rotationKeys;
	std::vector<ScaleKey>    scaleKeys;

	std::string nodeName;
	float animationDuration;
};

