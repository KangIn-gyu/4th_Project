#include "pch.h"
#include "TitleScene.h"
#include "D3DBaseObj.h"
#include "D3DAniObj.h"

void TitleScene::Enter()
{
	auto* map = CreatorObject<D3DBaseObj>("TitleScene/FBX/Map_Lowpoly.fbx", Object::ObjectType::Basic, "TitleScene/FBX/Map_Lowpoly.fbx");
	auto* Evelyn = CreatorObject<D3DAniObj>("TitleScene/FBX/Evelyn.fbx", Object::ObjectType::Basic, "TitleScene/FBX/Evelyn.fbx");
	
	// 맵
	// 캐릭터이다. 바에 있는 캐릭터 
	// 카메라 이동을 좌우로 보간해야됨 계속 다투윈 사용해야됨
	// 일단 카메라 이동 제외 2가지 추가 
}

void TitleScene::Update(const float _deltaTime)
{
	Scene::Update(_deltaTime);
}
