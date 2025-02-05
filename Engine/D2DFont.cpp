#include "pch.h"
#include "D2DFont.h"
#include "FontManager.h"
#include "Helper.h"
#include "D2DClass.h"


D2DFont::D2DFont()
{
	D2DClass::GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &fontBrush);

#if _DEBUG
	D2DClass::GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &boxBrush);
	D2DClass::GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightPink), &boundBrush);
#endif
}

void D2DFont::SetDialog(std::wstring_view _dialog)
{
	dialog = _dialog;
	CreateLayoutText(_dialog.data());
}

void D2DFont::SetBoxSize(float _width, float _height)
{
	boxSize.width = _width;
	boxSize.height = _height;

	CreateLayoutText(dialog);
}

void D2DFont::SetTextFormat(IDWriteTextFormat* _Format)
{
	DWriteTextFormat = _Format;
	CreateLayoutText(dialog);
}

void D2DFont::SetTextSize(float _fontSize, DWRITE_TEXT_RANGE _textRange)
{
	fontSize = _fontSize;
	DWriteTextLayout->SetFontSize(fontSize, _textRange);
}

void D2DFont::Alignment(Setting _SortX, Setting _SortY)
{
	if (DWriteTextLayout == nullptr) { return; }

	if (_SortX == Setting::LEFT)
	{
		DWriteTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		FontX = LEFT;
	}
	else if (_SortX == Setting::MIDDLE)
	{
		DWriteTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		FontX = MIDDLE;
	}
	else if (_SortX == Setting::RIGHT)
	{
		DWriteTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		FontX = RIGHT;
	}
	else
	{
		DWriteTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		FontX = MIDDLE;
	}

	if (_SortY == Setting::TOP)
	{
		DWriteTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		FontY = TOP;
	}
	else if (_SortY == Setting::MIDDLE)
	{
		DWriteTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		FontY = MIDDLE;
	}
	else if (_SortY == Setting::BOTTOM)
	{
		DWriteTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		FontY = BOTTOM;
	}
	else
	{
		DWriteTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
}

void D2DFont::DrawTextBox()
{
	D2D1_RECT_F rect = { pos.x, pos.y,  pos.x + boxSize.width, pos.y + boxSize.height};
	D2DClass::GetD2DDeviceContext()->DrawRectangle(&rect, boxBrush);
}

void D2DFont::CreateLayoutText(std::wstring_view _detail)
{
	if (nullptr != DWriteTextLayout)
	{
		DWriteTextLayout->Release();
	}

	HRESULT hr = D2DClass::GetDWriteFactory()->CreateTextLayout(
		_detail.data(),
		static_cast<UINT32>(_detail.length()),
		DWriteTextFormat,
		static_cast<float>(boxSize.width),
		static_cast<float>(boxSize.height),
		&DWriteTextLayout
	);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create text Layout.");
	}

	Alignment(FontX, FontY);
}
