#pragma once
#include "Component.h"
#include "d2d1.h"
#include <dwrite.h>

#include "Bitmap.h"
#include "D2DFont.h"

class Bitmap;
class BoxCollider;
class D2DRenderComponent : public Component
{
public:
	D2DRenderComponent();
	~D2DRenderComponent();

	virtual void ComponentInitialize()override {};
	virtual void ComponentUpdate(const float _deltaTime)override {};

	// 비트맵 함수
	void			Load2DImage(std::string_view _filePath);
	void			Set2DImageSize(float _width, float _height);
	D2D_VECTOR_2F	Get2DImageSize();

	void			SetAlpha(float _alpha) { drawBitmap->SetAlpha(_alpha); }
	float			GetAlpha() { return drawBitmap->GetAlpha(); };

	DXMath::Vector2 Get2DImageXY();
	D2D_VECTOR_2F	Get2DImagePos();
	void			Set2DImagePos(float _x, float _y);

	void  			ChangeBitmap(int _index);
	Bitmap*			GetBitmap(int _index);

	// 폰트 관련 함수들 데이터 조정
	void LoadFont(const std::string& _filePath);
	void SetDialog(std::wstring_view _dialog);
	void SetFontColor(D2D1_COLOR_F color); // 컬러 변경 주의사항 : 렌더 하기전에 색을 변경해야 됨
	void SetFontPos(float _X, float _Y); // 위치 조정 로컬좌표일때 사용
	void SetFontBoxSize(float _width, float _height);
	void SetAlignment(D2DFont::Setting _SortX, D2DFont::Setting _SortY);
	void SetTextSize(float _FontSize, DWRITE_TEXT_RANGE _textRange = {0, UINT32_MAX});
	void SetLineSpacing(float _lineSpacing);

	// 폰트맵과 비트맵 같이 처리함
	void Draw();

	// CSV 로드
	void SceneCSVDataLoad(std::string_view _filePath);
	std::vector<std::pair<int, std::wstring>>& GetCSVDData() { return CSVdatas; }
private:

public:
	int bitmapLayerOrder = 0; // 0부터 순서대로 그려진다 그렇기 때문에 가장 높은 수가 가장 나중에 그림을 그림.

private:
	D2DFont* font{};
	Bitmap* drawBitmap{}; // 메인 비트맵 처리

	std::vector<Bitmap*> imageDatas; // 여러개의 비트맵을 들고 있는 비트맵
	std::vector<std::pair<int, std::wstring>> CSVdatas;
};

