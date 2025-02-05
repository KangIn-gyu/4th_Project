#include "pch.h"
#include "D2DTestObj.h"
#include "../Engine/D2DRenderComponent.h"
#include <dwrite.h>

void D2DTestObj::Initialize()
{
	auto D2D = CreateComponent<D2DRenderComponent>();
	D2D->LoadFont("Font/DNFBitBitv2.ttf",50);
	D2D->SetDialog(L"¾È³çÇÏ¼¼¿ä");
	D2D->SetFontPos(100, 300);
	D2D->SetFontBoxSize(300, 300);
	D2D->SetAlignment(D2DFont::LEFT,D2DFont::TOP);

	D2D->Load2DImage("STAGE1/UI/Tutorial_01.png");
	D2D->Set2DImageSize(1920, 1080);
	D2D->Set2DImagePos(0, 0);
}
