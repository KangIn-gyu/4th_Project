#include "pch.h"
#include "D2DRenderComponent.h"

#include "Bitmap.h"
#include "ResourceSystem.h"
#include "D2DFont.h"
#include "D2DClass.h"
#include "FontManager.h"
#include "ImageComponent.h"
#include "Helper.h"
#include "Object.h"
#include "Renderer.h"
#include "ResourceSystem.h"
#include "CSVLoader.h"

D2DRenderComponent::D2DRenderComponent()
{
	RENDERER->D2DAddRenderComponent(this);
}

D2DRenderComponent::~D2DRenderComponent()
{
	SafeExtinction::SAFE_DELETE(font);
	SafeExtinction::SAFE_CLEAR_CONTAINER(imageDatas);
}

void D2DRenderComponent::SceneCSVDataLoad(std::string_view _filePath)
{
	CSVdatas = CSVLOADER->FindData(_filePath);
}

void D2DRenderComponent::Load2DImage(std::string_view _filePath)
{
	Bitmap* newBitmap = new Bitmap;
	newBitmap->Load(_filePath);
	if (imageDatas.size() == 0)
	{ // 사이즈가 0일때는 자동으로 그릴 비트맵 설정하게 처리한거임
		drawBitmap = newBitmap;
	}
	imageDatas.push_back(newBitmap);
}

void D2DRenderComponent::Set2DImageSize(float _width, float _height)
{
	drawBitmap->SetSize(_width, _height);
}
D2D_VECTOR_2F D2DRenderComponent::Get2DImageSize()
{
	return { drawBitmap->GetRect().right, drawBitmap->GetRect().bottom };
}
void D2DRenderComponent::Set2DImagePos(float _x, float _y)
{
	drawBitmap->SetPos(_x, _y);
}
D2D_VECTOR_2F D2DRenderComponent::Get2DImagePos()
{
	return { drawBitmap->GetRect().left, drawBitmap->GetRect().top };
}

void D2DRenderComponent::ChangeBitmap(int _index)
{
	if (_index > 0 && _index < imageDatas.size())
	{
		drawBitmap = imageDatas[_index];
	}
}

Bitmap* D2DRenderComponent::GetBitmap(int _index)
{
	if (_index > 0 && _index < imageDatas.size())
	{
		return imageDatas[_index];
	}
}

void D2DRenderComponent::LoadFont(const std::string& _filePath)
{	// TODO : 다시 만들어야 됨
	font = FONTMANAGER->LoadFont(_filePath);
}

void D2DRenderComponent::SetDialog(std::wstring_view _dialog)
{
	font->SetDialog(_dialog);
}

void D2DRenderComponent::SetFontColor(D2D1_COLOR_F color)
{
	font->SetColor(color);
}

void D2DRenderComponent::SetFontPos(float _X, float _Y)
{
	font->SetPos(_X, _Y);
}

void D2DRenderComponent::SetFontBoxSize(float _width, float _height)
{
	font->SetBoxSize(_width, _height);
}

void D2DRenderComponent::SetTextSize(float _FontSize, DWRITE_TEXT_RANGE _textRange)
{
	font->SetTextSize(_FontSize, _textRange);
}

void D2DRenderComponent::SetAlignment(D2DFont::Setting _SortX, D2DFont::Setting _SortY)
{
	font->Alignment(_SortX, _SortY);
}
void D2DRenderComponent::Draw()
{
#if _DEBUG
	if (font != nullptr)
	{
		font->DrawTextBox();
	}
#endif
	if (drawBitmap != nullptr)
	{
		D2DClass::GetD2DDeviceContext()->DrawBitmap(drawBitmap->GetImageData(), drawBitmap->GetRect(), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
	}
	if (font != nullptr)
	{
		D2DClass::GetD2DDeviceContext()->DrawTextLayout( font->GetPos(), font->GetTextLayout(), font->GetBrush());
	}
}


