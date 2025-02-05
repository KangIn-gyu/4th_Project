#include "pch.h"
#include "KangScene.h"
#include "AnimationObj.h"
#include "BaseObj.h"

#include "../Engine/SoundSystem.h"
#include "../Engine/D2DRenderComponent.h"
#include "D2DTestObj.h"
KangScene::KangScene(std::string_view _Name) : Scene(_Name)
{

}

void KangScene::Enter()
{
//	SOUNDSYSTEM->PlayMusic(eSoundList::Main_Theme, eSoundChannel::BGM);
	//ObjectCreator<AnimationObj>("AnimationObj", Object::ObjectType::Basic);
	ObjectCreator<BaseObj>("BaseObj", Object::ObjectType::Basic);

	ObjectCreator<D2DTestObj>("XX", Object::ObjectType::UI);
}

void KangScene::Initialize()
{
	__super::Initialize();


}
