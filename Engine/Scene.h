#pragma once
#include "ObjectManager.h"

class Scene // 기반 클래스 이걸 상속해서 본인들이 원하는 씬을 만들면 됨
{
public:
	Scene(std::string_view _Name);
	virtual ~Scene();

	void Initialize();
	virtual void Enter() = 0;   // 해당 씬에서 내가 만들 오브젝트 초기화 하는 곳
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime);
	virtual void RateUpdate(const float _deltaTime);

	std::string_view GetName();
private:

public:

protected:
	std::string sceneName;
	ObjectManager* objectManager;

private:

};

// 이걸 상속해서 씬을 만든 다음 그걸 로드 하면 된다.
// 기본 제공 오브젝트는 메인 카메라, 라이트를 제공할 것이다.