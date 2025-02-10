#include "pch.h"
#include "LobbyScene.h"
#include "Card.h"
#include "../Engine/TransformComponent.h"
#include "UIButton.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h"
#include "../Engine/SceneManager.h"
#include "../Engine/SoundSystem.h"
void LobbyScene::Enter()
{
	//auto test = CreatorObject<TestObj2>("Map", Object::ObjectType::Background);
	//CreatorObject<UIButton>("bu1", Object::ObjectType::UI, "STAGE1/UI/mybutton2.png", DXMath::Vector2(500, 0), []() {});

    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular

// TODO: 등장시킨 딜러를 클릭하면 선택지 버튼이 나오게 한다. 1.대화하기 -> 게임시작 2.게임시작  ///
//
//	auto* dialog = CreatorObject<D2DBaseObj>("DialogScene1", Object::ObjectType::UI,
//        7, 23, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene1.csv", "TutorialScene");
//
//    dialog->CreateScript<D2DBitMapFontScript>();
//    CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
//        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(50, 50),
//        []() {SCENEMANAGER->ChangeScene("TutorialScene");});
    //SOUNDSYSTEM->PlayMusic(eSoundList::Main_Theme, eSoundChannel::BGM);
}

void LobbyScene::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}