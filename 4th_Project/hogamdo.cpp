#include "pch.h"
#include "hogamdo.h"
#include "../Engine/D2DRenderComponent.h"
#include "BlackJack.h"
hogamdo::hogamdo(std::string_view _name, Object::ObjectType _type, DXMath::Vector2 _pos) : Object(_name, _type)
{
	pos = _pos;
	imagedata = CreateComponent<D2DRenderComponent>();
}

hogamdo::~hogamdo()
{
}

void hogamdo::Initialize()
{
	Object::Initialize();
	imagedata->Load2DImage("UI/Favorite_20.png"); //1
	//imagedata->Load2DImage("UI/Button/" + GetName() + "_On.png");     //1
	imagedata->Load2DImage("UI/Favorite_40.png"); //1
	imagedata->Load2DImage("UI/Favorite_60.png"); //1
	imagedata->Load2DImage("UI/Favorite_80.png"); //1
	imagedata->Load2DImage("UI/Favorite_100.png"); //1
	imagedata->Set2DImagePos(pos.x,pos.y);
	imagedata->ChangeBitmap(0);
}

void hogamdo::Update(const float _deltaTime)
{
	if (BLACKJACK->player->love == 20)
	{
		imagedata->ChangeBitmap(0);
	}
	else if (BLACKJACK->player->love == 40)
	{
		imagedata->ChangeBitmap(1);
	}
	else if (BLACKJACK->player->love == 60)
	{
		imagedata->ChangeBitmap(2);
	}
	else if(BLACKJACK->player->love == 80)
	{
		imagedata->ChangeBitmap(3);
	}
	else if (BLACKJACK->player->love == 100)
	{
		imagedata->ChangeBitmap(4);
	}
}
