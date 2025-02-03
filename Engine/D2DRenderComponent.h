#pragma once
#include "Component.h"
#include "d2d1.h"
#include "D2DFont.h"

class D2DFont;
class D2DRenderComponent : public Component
{
public:
	D2DRenderComponent();
	~D2DRenderComponent();

	virtual void ComponentInitialize()override;
	virtual void ComponentUpdate(const float _deltaTime)override;

	void LoadBitMap(std::string_view _filePath);

	// 폰트 관련 함수들
	void LoadFont(const std::string& _filePath);
	void SetDialog(std::wstring_view _dialog);
	void SetFontColor(D2D1_COLOR_F color); // 컬러 변경 주의사항 : 렌더 하기전에 색을 변경해야 됨
	void SetFontPos(float _X, float _Y); // 위치 조정 로컬좌표일때 사용
	void SetFontBoxSize(float _width, float _height);
	void SetTextSize(float _FontSize, DWRITE_TEXT_RANGE _textRange);

	void Draw();
private:

public:

private:
	D2DFont* font;
//	D2D1_RECT_F DstRect{};		// RenderTarget의 Destination 영역		
//	D2D1_RECT_F srcRect{};		// D2D1Bitmap의 Source 영역

};

