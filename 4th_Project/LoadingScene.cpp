#include "pch.h"
#include "LoadingScene.h"
#include "D2DBaseObj.h"
#include "../Engine/D2DRenderComponent.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Helper.h"

void LoadingScene::Enter()
{
	auto* loadingImage = CreatorObject<D2DBaseObj>("LoadingImage", Object::ObjectType::UI);
	loadingImage->GetComponent<D2DRenderComponent>()->Load2DImage("LoadingScene/UI/98.png");		
}

void LoadingScene::Update(const float _deltaTime)
{
	Scene::Update(_deltaTime);

	LoadingTime += _deltaTime;
	if (LoadingTime >= maxLoadingTime)
	{
		LoadingTime = 0;
		SCENEMANAGER->ChangeScene("TutorialScene");
	}
}

void LoadingScene::ResetInformation()
{
	Scene::ResetInformation();
	maxLoadingTime = RandomUtil::GetRandomFloat(2.0f, 5.0f);
}
