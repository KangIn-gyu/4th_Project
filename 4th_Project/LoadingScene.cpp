#include "pch.h"
#include "LoadingScene.h"
#include "D2DBaseObj.h"
#include "../Engine/D2DRenderComponent.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Helper.h"
#include "../Engine/SoundSystem.h"

void LoadingScene::Enter()
{
	loadingImage = CreatorObject<D2DBaseObj>("LoadingImage", Object::ObjectType::UI);
	auto* d2dRender = loadingImage->GetComponent<D2DRenderComponent>();
	d2dRender->Load2DImage("LoadingScene/UI/Loading_0.png");
	d2dRender->Load2DImage("LoadingScene/UI/Loading_1.png");
	d2dRender->Load2DImage("LoadingScene/UI/Loading_2.png");
	d2dRender->Load2DImage("LoadingScene/UI/Loading_3.png");
	d2dRender->Load2DImage("LoadingScene/UI/Loading_4.png");
	d2dRender->Load2DImage("LoadingScene/UI/Loading_5.png");
	d2dRender->Load2DImage("LoadingScene/UI/Loading_6.png");
	d2dRender->Load2DImage("LoadingScene/UI/Loading_7.png");
	loadingImage->SetActive(false);
}

void LoadingScene::Update(const float _deltaTime)
{
	Scene::Update(_deltaTime);

	LoadingTime += _deltaTime;
	if (LoadingTime >= maxLoadingTime)
	{
		LoadingTime = 0;
		SCENEMANAGER->ChangeScene(nextSceneName);
	}
}

void LoadingScene::ResetInformation()
{
	Scene::ResetInformation();
	SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
	maxLoadingTime = RandomUtil::GetRandomFloat(2.0f, 5.0f);
	loadingImage->SetActive(true);
	auto* d2dRender = loadingImage->GetComponent<D2DRenderComponent>();
	d2dRender->ChangeBitmap(RandomUtil::GetRandomInt(0, 7));
}
