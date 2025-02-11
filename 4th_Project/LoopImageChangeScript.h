#pragma once
#include "../Engine/Script.h"

// TOOD : 현재 사용하는 곳 없음
class D2DRenderComponent;
class LoopImageChangeScript : public Script
{
public:
	LoopImageChangeScript(Object* _own) : Script(_own) {};
	virtual ~LoopImageChangeScript() = default;

	virtual void ComponentSetting() override;               // 컴포넌트가 다 생성된 이후 수치 조정하는것
	virtual void Update(const float _deltaTime) override;  // 업데이트
	virtual void ResetInformation();
	void Set2DImagePos(float _x, float _y);
private:


private:
	D2DRenderComponent* ownerD2D;
	std::vector<std::string> bitmapFilePath;
	float posX{}, posY{};
	int index = {};
};

