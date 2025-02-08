#include "pch.h"
#include "DialogScene1.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // ½ºÅ©¸³Æ®
#include "../Engine/SoundSystem.h"
void DialogScene1::Enter()
{
    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
	auto* dialog = CreatorObject<D2DBaseObj>("DialogScene1", Object::ObjectType::UI,
        7,23,"Font/Bold.ttf", "DialogScenes/CSV/Scene1.csv", "¿©±â¼öÁ¤ÇØ¾ßÇÔ");
  
    dialog->CreateScript<D2DBitMapFontScript>();
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
