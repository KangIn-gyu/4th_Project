#include "pch.h"
#include "DialogScene0.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h"

void DialogScene0::Enter()
{
	Scene::Enter();
    auto* dialog = CreatorObject<D2DBaseObj>("DialogScene0", Object::ObjectType::UI,
		1,6,"Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene0.csv", "TutorialScene");

    dialog->CreateScript<D2DBitMapFontScript>();
}

void DialogScene0::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}
