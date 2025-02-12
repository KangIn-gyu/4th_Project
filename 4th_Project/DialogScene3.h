#pragma once
#include "../Engine/Scene.h"
class UIButton;
class D2DBaseObj;
class D2DBitMapFontScript;
class DialogScene3 : public Scene
{

public:
	DialogScene3(std::string_view _Name);

	virtual void Enter() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
	virtual void ResetInformation() override;
private:
	D2DBaseObj* fading;
	D2DBaseObj* dialog;
	UIButton* skipbutton{};
public:

private:
	D2DBitMapFontScript* script{};
	bool eventFlag1 = true;
	bool eventFlag2 = true;
};
