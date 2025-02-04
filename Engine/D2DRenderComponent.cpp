#include "pch.h"
#include "D2DRenderComponent.h"
#include "ResourceSystem.h"
#include "D2DFont.h"
#include "D2DClass.h"
#include "FontManager.h"
#include "Helper.h"
#include "Renderer.h"
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

void D2DRenderComponent::LoadFont(const std::string& _filePath)
{
	// TODO : 다시 만들어야 됨
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

void D2DRenderComponent::Draw()
{

#if _DEBUG
		font->DrawTextBox();
#endif
		D2DClass::GetD2DDeviceContext()->DrawTextLayout(font->GetPos(), font->GetTextLayout(), font->GetBrush());
}

void D2DRenderComponent::LoadBitMap(std::string_view _filePath)
{
	// 비트맵 처리 필요

}
