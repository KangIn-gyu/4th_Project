#include "pch.h"
#include "DialogScene1.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" 
#include "../Engine/SoundSystem.h"
#include "../Engine/D2DRenderComponent.h"

#include "UIButton.h"
#include "SelectionDialogScript.h"

void DialogScene1::Enter()
{
    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
	auto* dialog = CreatorObject<D2DBaseObj>("DialogScene1", Object::ObjectType::UI,
        7, 23,"Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene1.csv", "abcd");
//    dialog->SetD2DLayerOrder(0);
//    dialog->CreateScript<D2DBitMapFontScript>();  // TODO : 해당 씬에서 이 스크립트가 맞음 테스트를 위해서 잠시 주석해놓음

//   auto testbutton1 = CreatorObject<UIButton>("Skip1", Object::ObjectType::UI, "STAGE1/Textures/Card_0010_CloverJack.png", DXMath::Vector2(900, 450), []() {});
//   auto testbutton2 = CreatorObject<UIButton>("Skip2", Object::ObjectType::UI, "STAGE1/Textures/Card_0011_CloverQueen.png", DXMath::Vector2(1200, 450), []() {});
//   dialog->CreateScript<SelectionDialogScript>()->SetButton(testbutton1, testbutton2);
   SOUNDSYSTEM->PlayMusic(eSoundList::Main_Theme, eSoundChannel::BGM);
}

void DialogScene1::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}

void DialogScene1::ResetInformation()
{
    Scene::ResetInformation();
    SOUNDSYSTEM->PlayMusic(eSoundList::Main_Theme, eSoundChannel::BGM);
}
