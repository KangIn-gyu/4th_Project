#include "pch.h"
#include "KangScene.h"
#include "AnimationObj.h"
#include "BaseObj.h"

KangScene::KangScene(std::string_view _Name) : Scene(_Name)
{

}

void KangScene::Enter()
{
	ObjectCreator<AnimationObj>("AnimationObj", Object::ObjectType::Basic);
	ObjectCreator<BaseObj>("BaseObj", Object::ObjectType::Basic);
}
