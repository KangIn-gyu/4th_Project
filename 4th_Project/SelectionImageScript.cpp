#include "pch.h"
#include "SelectionImageScript.h"
#include "../Engine/Object.h"
#include "DialogButton.h"

void SelectionImageScript::ComponentSetting()
{
	
}

void SelectionImageScript::Update(const float _deltaTime)
{
	if (selectButton1->isClick == true || selectButton2->isClick == true)
	{
		selectButton1->isClick = false;
		selectButton2->isClick = false;
		selectButton1->SetActive(false);
		selectButton2->SetActive(false);
		ownerObject->SetActive(false);
	}

	if (ownerObject->IsActive())
	{
		selectButton1->SetActive(true);
		selectButton2->SetActive(true);
	}
}

void SelectionImageScript::ResetInformation()
{
	ownerObject->SetActive(false);
}

void SelectionImageScript::SetButton(DialogButton* _selectButton1, DialogButton* _selectButton2)
{
	selectButton1 = _selectButton1;
	selectButton1->SetActive(false);
	selectButton2 = _selectButton2;
	selectButton2->SetActive(false);

}
