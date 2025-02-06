#pragma once
#include "../Engine/Scene.h"

class IntroCutScene : public Scene
{
public:
	IntroCutScene(std::string_view _Name) :Scene(_Name) {}
	virtual void Enter(); // 해당 씬에서 내가 만들 오브젝트 초기화 하는 곳 Initialize 보다 먼저 시작됨으로 오브젝트를 생성하고 이후 오브젝트의 초기화를 한다
	virtual void FixedUpdate(const float _deltaTime) {};
	virtual void RateUpdate(const float _deltaTime) {};

private:

public:

private:

};

