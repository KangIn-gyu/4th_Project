#pragma once
#include "../Engine/Script.h"

class Object;
class D2DRenderComponent;

class JustFont : public Script
{
public:
	JustFont(Object* _own);
	virtual ~JustFont() = default;

	void SetMessage(int* _value) { value = _value; } //출력할 메시지담기 베팅칩갯수 스킬포인트 등등
	void SetMessage(int** _value) { value = *_value; } //출력할 메시지담기 베팅칩갯수 스킬포인트 등등
	void SetMessage(float* _value) { fvalue = _value; }
	virtual void ComponentSetting() override;              // 컴포넌트가 다 생성된 이후 수치 조정하는것
	virtual void Update(const float _deltaTime) override;  // 업데이트
	virtual void ResetInformation();
	void SetString(std::wstring str) {  message = str; }
	int* value = nullptr;
	float* fvalue = nullptr;
	void SetTextSize(float _FontSize, DWRITE_TEXT_RANGE _textRange = { 0, UINT32_MAX });
	void SetFontBoxSize(float _width, float _height);
	void SetFontPos(float _X, float _Y); // 위치 조정 로컬좌표일때 사용
private:
	D2DRenderComponent* ownerD2D;
	int bitmapSize = 0;
	int index = 0;
	std::wstring message;

};

