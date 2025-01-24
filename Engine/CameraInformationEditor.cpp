#include "pch.h"
#include "CameraInformationEditor.h"
#include "CameraObject.h"
#include "CameraCompoent.h"
#include "Declare.h"
#include <imgui.h>
CameraInformationEditor::CameraInformationEditor()
{
	SetName("CameraInformation");
	state = State::Disabled;
}

void CameraInformationEditor::Initialize()
{

}

void CameraInformationEditor::Update()
{

}

void CameraInformationEditor::Draw()
{
	if (state == State::Active && nullptr != selectedcamera)
	{
		auto* cameraInfo = selectedcamera->GetComponent<CameraCompoent>()->GetCameraInfo();
		if (ImGui::CollapsingHeader(GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat("Speed", &cameraInfo->Speed, 1.0f, 10.f, 3000.f);
			
			if (ImGui::DragFloat("FOV", &cameraInfo->FovAngleY, 0.01f, 0.1f, 3.14f)) 
			{
				cameraInfo->projectionUpdate = true;
			}

			if (ImGui::DragFloat("Near", &cameraInfo->Near, 0.1f, 0.1f, 15.f)) 
			{
				cameraInfo->projectionUpdate = true;
			}

			if (ImGui::DragFloat("Far", &cameraInfo->Far, 1.0f, 10.f))
			{
				cameraInfo->projectionUpdate = true;
			}
			
		}
	}
}

void CameraInformationEditor::OnEnable()
{
	state = State::Active;
}

void CameraInformationEditor::OnDisable()
{
	state = State::Disabled;
}

void CameraInformationEditor::SetCameraObject(Object* _camera)
{
	selectedcamera = static_cast<CameraObject*>(_camera);
}
