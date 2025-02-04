#pragma once
#include "Component.h"
#include "d2d1.h"
#include <dwrite.h>

class Bitmap;
class D2DFont;
class D2DRenderComponent : public Component
{
public:
	D2DRenderComponent();
	~D2DRenderComponent();

	virtual void ComponentInitialize()override {};
	virtual void ComponentUpdate(const float _deltaTime)override {};

	// 비트맵 함수
	void Load2DImage(std::string_view _filePath);
	void Set2DImageSize(float _width, float _height);
	void Set2DImagePos(float _x, float _y);

	// 폰트 관련 함수들 데이터 조정
	void SceneCSVDataLoad(std::string_view _filePath);
	void LoadFont(const std::string& _filePath);
	void SetDialog(std::wstring_view _dialog);
	void SetFontColor(D2D1_COLOR_F color); // 컬러 변경 주의사항 : 렌더 하기전에 색을 변경해야 됨
	void SetFontPos(float _X, float _Y); // 위치 조정 로컬좌표일때 사용
	void SetFontBoxSize(float _width, float _height);
	void SetTextSize(float _FontSize, DWRITE_TEXT_RANGE _textRange);

	// 폰트맵과 비트맵 같이 처리함
	void Draw();
private:

public:

private:
	D2DFont* font;
	std::shared_ptr<Bitmap> imageData;

	std::vector<std::pair<std::string, std::string>> CSVdatas;
};

