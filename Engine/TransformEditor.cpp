#include "pch.h"
#include "TransformEditor.h"
#include "TransformComponent.h"

TransformEditor::TransformEditor()
{
	SetName("Transform");
	state = State::Active;
}

void TransformEditor::Initialize()
{
}

void TransformEditor::Update()
{
}

void TransformEditor::Draw()
{
	if (state == State::Active && nullptr != selectedObject)
	{ //TODO : 트랜스폼 정보 얻어와서 처리하는걸 만들자

	}
}

void TransformEditor::SetSelectedObject(Object* obj)
{

}
