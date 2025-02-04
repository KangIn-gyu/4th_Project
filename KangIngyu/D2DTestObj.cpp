#include "pch.h"
#include "D2DTestObj.h"
#include "../Engine/D2DRenderComponent.h"
#include <dwrite.h>

void D2DTestObj::Initialize()
{
	auto D2D = CreateComponent<D2DRenderComponent>();
	D2D->LoadFont("Font/DNFBitBitv2.ttf");
	D2D->SetDialog(L"¾È³çÇÏ¼¼¿ä");
	D2D->SetFontPos(100, 100);
	D2D->SetFontBoxSize(300, 300);

	D2D->Load2DImage("STAGE1/UI/mybutton.png");
	D2D->Set2DImageSize(100, 100);
//	D2D->Set2DImagePos(100, 100);
//	D2D->SetTextSize(50);
}
