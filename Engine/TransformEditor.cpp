#include "pch.h"
#include "TransformEditor.h"
#include "TransformComponent.h"
#include "Object.h"

#include "AiNode.h"
#include <imgui.h>

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
{ // 시간 낭비 하기 싫어서 걍 하드 코딩으로 처리함
	if (state == State::Active && nullptr != selectedObject)
	{ //TODO : 트랜스폼 정보 얻어와서 처리하는걸 만들자
        if(ImGui::CollapsingHeader("TransformComponent", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto* transform = selectedObject->GetComponent<TransformComponent>();
            ImGui::Text(selectedObject->GetName().c_str());

            DXMath::Vector3 position = transform->GetPosition();
            DXMath::Quaternion rotation = transform->GetQuaternion();
            DXMath::Vector3 scale = transform->GetScale();

            DXMath::Vector3 eulerRotation = rotation.ToEuler();
            if(ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position), 0.1f));
            {
                transform->SetPosition(position);
            }
        
            // 회전 값 편집 (오일러 각도)
            if (ImGui::SliderFloat3("Rotation", reinterpret_cast<float*>(&eulerRotation), -360.0f, 360.0f))
            {
                DXMath::Quaternion newRotation = DXMath::Quaternion::CreateFromYawPitchRoll(
                    DX::XMConvertToRadians(eulerRotation.y),
                    DX::XMConvertToRadians(eulerRotation.x),
                    DX::XMConvertToRadians(eulerRotation.z));
                transform->SetQuaternion(newRotation);
            }

            // 스케일 값 편집
            if (ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&scale), 0.1f))
            {
                transform->SetScale(scale);
            }

            DXMath::Matrix localMatrix = transform->GetLocalMatrix();
            if (ImGui::TreeNode("Local Matrix"))
            {
                for (int row = 0; row < 4; ++row)
                {
                    ImGui::Text("%.3f, %.3f, %.3f, %.3f",
                        localMatrix.m[row][0],
                        localMatrix.m[row][1],
                        localMatrix.m[row][2],
                        localMatrix.m[row][3]);
                }
                ImGui::TreePop();
            }

            // 월드 매트릭스 표시
            DXMath::Matrix worldMatrix = transform->GetWorldMatrix(); // 월드 매트릭스 가져오기
            if (ImGui::TreeNode("World Matrix"))
            {
                for (int row = 0; row < 4; ++row)
                {
                    ImGui::Text("%.3f, %.3f, %.3f, %.3f",
                        worldMatrix.m[row][0],
                        worldMatrix.m[row][1],
                        worldMatrix.m[row][2],
                        worldMatrix.m[row][3]);
                }
                ImGui::TreePop();
            }
        }
    }
    else if (state == State::Active && nullptr != selectedNode)
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto* transform = selectedNode->GetPointTransform();
            ImGui::Text(selectedNode->GetName().c_str());

            DXMath::Vector3 position = transform->GetPosition();
            DXMath::Quaternion rotation = transform->GetQuaternion();
            DXMath::Vector3 scale = transform->GetScale();

            DXMath::Vector3 eulerRotation = rotation.ToEuler();
            if (ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position), 0.1f));
            {
                transform->SetPosition(position);
            }

            // 회전 값 편집 (오일러 각도)
            if (ImGui::SliderFloat3("Rotation", reinterpret_cast<float*>(&eulerRotation), -360.0f, 360.0f))
            {
                DXMath::Quaternion newRotation = DXMath::Quaternion::CreateFromYawPitchRoll(
                    DX::XMConvertToRadians(eulerRotation.y),
                    DX::XMConvertToRadians(eulerRotation.x),
                    DX::XMConvertToRadians(eulerRotation.z));
                transform->SetQuaternion(newRotation);
            }

            // 스케일 값 편집
            if (ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&scale), 0.1f))
            {
                transform->SetScale(scale);
            }

            DXMath::Matrix localMatrix = transform->GetLocalMatrix();
            if (ImGui::TreeNode("Local Matrix"))
            {
                for (int row = 0; row < 4; ++row)
                {
                    ImGui::Text("%.3f, %.3f, %.3f, %.3f",
                        localMatrix.m[row][0],
                        localMatrix.m[row][1],
                        localMatrix.m[row][2],
                        localMatrix.m[row][3]);
                }
                ImGui::TreePop();
            }

            // 월드 매트릭스 표시
            DXMath::Matrix worldMatrix = transform->GetWorldMatrix(); // 월드 매트릭스 가져오기
            if (ImGui::TreeNode("World Matrix"))
            {
                for (int row = 0; row < 4; ++row)
                {
                    ImGui::Text("%.3f, %.3f, %.3f, %.3f",
                        worldMatrix.m[row][0],
                        worldMatrix.m[row][1],
                        worldMatrix.m[row][2],
                        worldMatrix.m[row][3]);
                }
                ImGui::TreePop();
            }

            if (nullptr != transform->GetParent())
            {
                ImGui::Text(reinterpret_cast <const char*>(u8"부모 트랜스폼이 있음"));
            }

        }
    }
}

void TransformEditor::SetSelectedObject(Object* _obj)
{
    selectedObject = _obj;
}

void TransformEditor::SetSelectedNode(AiNode* _node)
{
    selectedNode = _node;
}
