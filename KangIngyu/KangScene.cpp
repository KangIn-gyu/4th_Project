#include "pch.h"
#include "KangScene.h"
#include "AnimationObj.h"
#include "BaseObj.h"
#include "../Engine/SoundSystem.h"

KangScene::KangScene(std::string_view _Name) : Scene(_Name)
{

}

void KangScene::Enter()
{
	//SOUNDSYSTEM->PlayMusic(eSoundList::Main_Theme, eSoundChannel::BGM);
	CreatorObject<AnimationObj>("AnimationObj", Object::ObjectType::Basic);
//	CreatorObject<BaseObj>("BaseObj", Object::ObjectType::Basic);
}
