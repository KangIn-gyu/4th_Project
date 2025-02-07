#pragma once
#include "IResources.h"
#include <dwrite.h>
class D2DFont
{
public:
	enum Setting // 용도 정렬 선택 하기 위해서
	{
		LEFT,
		MIDDLE,
		RIGHT,
		TOP,
		BOTTOM,
		EXIT
	};

	D2DFont();
	~D2DFont();

	void SetDialog(std::wstring_view _dialog);
	void SetColor(D2D1_COLOR_F color) { fontBrush->SetColor(color); } // 컬러 변경 주의사항 : 렌더 하기전에 색을 변경해야 됨
	void SetPos(float _X, float _Y) { pos = { _X , _Y }; } // 위치 조정 로컬좌표일때 사용
	void SetBoxSize(float _width, float _height);
	void SetTextFormat(IDWriteTextFormat* _Format);
	void SetTextSize(float _fontSize, DWRITE_TEXT_RANGE _textRange = { 0, UINT32_MAX });  // DWRITE_TEXT_RANGE 사용법 {0 , 9 } 이렇게 했을때 0부터 9번째 까지의 글자 크기를 변경
	void SetLineSpacing(float _lineSpacing);
	
	void Alignment(Setting _SortX, Setting _SortY); // 왼쪽, 가운데, 오른쪽 정렬

	D2D1_POINT_2F GetPos() { return pos; }
	ID2D1SolidColorBrush* GetBrush() { return fontBrush; }
	ID2D1SolidColorBrush* GetBoundBrush() { return boundBrush; }
	IDWriteTextLayout* GetTextLayout() { return DWriteTextLayout; }

	void DrawTextBox();

private:
	void CreateLayoutText(const std::wstring& _detail);

public:

private:
	float fontSize = 10;
	D2D1_POINT_2F pos = { 0.f , 0.f };  // 로컬 좌표
	D2D_SIZE_F boxSize = { 100.f, 50.f }; // 텍스트 박스 사이즈
	std::wstring dialog {};  // 내용

	ID2D1SolidColorBrush* fontBrush {};
	ID2D1SolidColorBrush* boxBrush{};
	ID2D1SolidColorBrush* boundBrush{};
	IDWriteTextFormat* DWriteTextFormat {};
	IDWriteTextLayout* DWriteTextLayout {};

	Setting FontX = MIDDLE;
	Setting FontY = MIDDLE;
};

