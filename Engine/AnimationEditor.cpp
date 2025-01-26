#include "pch.h"
#include "AnimationEditor.h"

AnimationEditor::AnimationEditor()
{
	SetName("Animation");
	state = State::Active;
}

void AnimationEditor::Initialize()
{

}

void AnimationEditor::Update()
{

}

void AnimationEditor::Draw()
{
	if (state == State::Active && nullptr != selectedObject)
	{

	}
}

void AnimationEditor::SetSelectedObject(Object* _obj)
{
	selectedObject = _obj;
}
