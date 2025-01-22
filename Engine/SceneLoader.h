#pragma once
#include "pch.h"

class objectData
{
	std::string        name;
	DXMath::Vector3    position;
	DXMath::Quaternion rotation;
	DXMath::Vector3    scale;

public:
	objectData(std::string _name, DXMath::Vector3 _position, DXMath::Vector3 _rotation, DXMath::Vector3 _scale)
	{
		name = _name;
		position = _position;
		auto quaternion = DXMath::Quaternion::CreateFromYawPitchRoll(_rotation.y, _rotation.x, _rotation.z);
		rotation = quaternion;
		scale = _scale;
	}
};

class SceneData
{
	int objNum;
	std::vector<objectData> objDatas;

public:

};

class SceneLoader
{

public:

};

