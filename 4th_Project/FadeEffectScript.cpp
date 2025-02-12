#include "pch.h"
#include "FadeEffectScript.h"
#include "D2DBaseObj.h"
#include "../Engine/D2DRenderComponent.h"
#include "../Engine/SceneManager.h"

void FadeEffectScript::ComponentSetting()
{
	ownerD2D = ownerObject->GetComponent<D2DRenderComponent>();

}

void FadeEffectScript::SetFadeSpeed(float speed)
{
	fadeSpeed = speed;
}
// 어두워지기
void FadeEffectScript::StartFadeIn(std::string _sceneName)
{
	alpha = 0.0f; fadingIn = true; active = true;
	sceneName = _sceneName;
}

// 밝아지기
void FadeEffectScript::StartFadeOut() { alpha = 1.0f; fadingIn = false; active = true; }

void FadeEffectScript::Update(const float _deltaTime)
{
	if (!active) return;
	// 어두워지기
	if (fadingIn)
	{
		alpha += _deltaTime * fadeSpeed;
		ownerD2D->SetAlpha(alpha);
		if (alpha >= 1.0f)
		{
			alpha = 0.0f;
			SCENEMANAGER->ChangeScene(sceneName);
		}

		// Todo: 여기에다가 씬체인지 부분을 넣던가 씬이 시작되고 마무리될때 함수가 실행되도록 하기 //
	}
	// 밝아지기
	if (!fadingIn)
	{
		alpha -= _deltaTime * fadeSpeed;
		ownerD2D->SetAlpha(alpha);
		if (alpha <= 0.0f)
		{
			alpha = 0.0f;
		}
	}
}