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
}

void D2DRenderComponent::SceneCSVDataLoad(std::string_view _filePath)
{
	CSVdatas = CSVLOADER->FindData(_filePath);
}

void D2DRenderComponent::Load2DImage(std::string_view _filePath)
{
	imageData = RESOURCESYSTEM->Load<Bitmap>(_filePath);
}

void D2DRenderComponent::Set2DImageSize(float _width, float _height)
{
	imageData->SetSize(_width, _height);
}
D2D_VECTOR_2F D2DRenderComponent::Get2DImageSize()
{
	return { imageData->GetRect().right, imageData->GetRect().bottom };
}

DXMath::Vector2 D2DRenderComponent::Get2DImageXY()
{
	return imageData->GetXY();
}
void D2DRenderComponent::Set2DImagePos(float _x, float _y)
{
	imageData->SetPos(_x, _y);
}
D2D_VECTOR_2F D2DRenderComponent::Get2DImagePos()
{
	return { imageData->GetRect().left, imageData->GetRect().top };
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
	//font->DrawTextBox();
#endif
	if (imageData != nullptr)
	{
		D2DClass::GetD2DDeviceContext()->DrawBitmap(imageData->GetImageData().Get(), imageData->GetRect());
	}
	if (font != nullptr)
	{
		//D2DClass::GetD2DDeviceContext()->DrawTextLayout( font->GetPos(), font->GetTextLayout(), font->GetBrush());
	}
}


