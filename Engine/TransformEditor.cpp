#include "pch.h"
#include "TransformEditor.h"
#include "TransformComponent.h"
#include "Object.h"

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
        if(ImGui::CollapsingHeader("TransformComponent"))
        {
            auto transform = selectedObject->GetComponent<TransformComponent>();
            ImGui::Text(selectedObject->GetName().c_str());

            DXMath::Vector3 position = transform->GetPosition();
            DXMath::Quaternion rotation = transform->GetQuaternion();
            DXMath::Vector3 scale = transform->GetScale();

            DXMath::Vector3 eulerRotation = rotation.ToEuler();

            if (ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position), 0.1f))
            {
                transform->SetPosition(position);
            }

            // 회전 값 편집 (오일러 각도)
            if (ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&eulerRotation), 0.1f))
            {
                DXMath::Quaternion newRotation = DXMath::Quaternion::CreateFromYawPitchRoll(
                    DX::XMConvertToRadians(eulerRotation.y),
                    DX::XMConvertToRadians(eulerRotation.x),
                    DX::XMConvertToRadians(eulerRotation.z)
                );
                transform->SetQuaternion(newRotation);
            }

            // 스케일 값 편집
            if (ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&scale), 0.1f))
            {
                transform->SetScale(scale);
            }
        }
	}
}

void TransformEditor::SetSelectedObject(Object* obj)
{
    selectedObject = obj;
}
