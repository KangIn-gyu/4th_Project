#include "pch.h"
#include "DialogScene1.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
void DialogScene1::Enter()
{
	std::vector<std::string> dialogScenebitmap = {
        "DialogScene1/Textures/0_Black,Normal.png",
        "DialogScene1/Textures/1_Main,Normal.png",
        "DialogScene1/Textures/2_Main,CustomerA.png",
        "DialogScene1/Textures/3_Main,CustomerB.png",
        "DialogScene1/Textures/4_Main,Player.png",
        "DialogScene1/Textures/5_Main,DealerA.png",
        "DialogScene1/Textures/6_Black,Normal,cocktail.png"
    };

    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
	auto* dialog = CreatorObject<D2DBaseObj>("Dialog", Object::ObjectType::UI, dialogScenebitmap,"Font/Bold.ttf", "DialogScene1/CSV/Scene0.csv", "TutorialScene");
    dialog->CreateScript<D2DBitMapFontScript>();
   
}

void DialogScene1::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}
