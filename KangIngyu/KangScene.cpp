#include "pch.h"
#include "KangScene.h"
#include "AnimationObj.h"
#include "BaseObj.h"

#include "../Engine/SoundSystem.h"
#include "../Engine/D2DRenderComponent.h"
#include "D2DTestObj.h"

#include "../Engine/TransformComponent.h"


KangScene::KangScene(std::string_view _Name) : Scene(_Name)
{

}

void KangScene::Enter()
{
//	SOUNDSYSTEM->PlayMusic(eSoundList::Main_Theme, eSoundChannel::BGM);
	auto testani = ObjectCreator<AnimationObj>("AnimationObj", Object::ObjectType::Basic);
	//ObjectCreator<BaseObj>("BaseObj", Object::ObjectType::Basic);
	//ObjectCreator<D2DTestObj>("TEXT", Object::ObjectType::UI);
}
