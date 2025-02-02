#include "pch.h"
#include "HierarchyWindow.h"

#include "Object.h"
#include "ModelComponent.h"
#include "Model.h"
#include "Mesh.h"
#include "AiNode.h"
#include "Material.h"
#include "Texture.h"

#include "InspectorWindow.h"
#include "Scene.h"
#include "Layer.h"
#include <imgui.h>

HierarchyWindow::HierarchyWindow()
{
	SetName("Hierarchy");
	SetSize(ImVec2(300, 600));
	SetFlags(ImGuiWindowFlags_None);
}

HierarchyWindow::~HierarchyWindow()
{

}

void HierarchyWindow::Initialize()
{

}

void HierarchyWindow::Update()
{

}

void HierarchyWindow::Draw()
{
	const auto& objects = currentScene->GetGameObecjts();
	std::string sceneText = "Scene : " + currentScene->GetName();
	ImGui::Text(sceneText.c_str());
	for (const auto& objLayer : objects) 
	{
		for (size_t index = 0; index < objLayer->GetSize(); ++index)
		{ // 객체의 이름으로 TreeNode를 생성합니다.
			if (ImGui::TreeNode(objLayer->GetGameObject(index)->GetName().c_str()))
			{
				if (ImGui::IsItemClicked())
				{ 
					INSPECTOR->SetSelectedObject(objLayer->GetGameObject(index));
				}

				// 객체의 ModelData의 rootNode를 가져옵니다.
				if (objLayer->GetGameObject(index)->GetObjectType() == Object::ObjectType::Basic)
				{
					// auto rootNode = *objList[i]->GetComponent<ModelComponent>()->GetNodeData();
					auto modelComponent = objLayer->GetGameObject(index)->GetComponent<ModelComponent>();
					AiNode* rootNode {};
					if (nullptr != modelComponent)
					{
						rootNode = modelComponent->GetRootNode();
					}
					if (nullptr != rootNode)
					{ // AiNode 트리 구조를 재귀적으로 그립니다.
						DrawNodeRecursive(objLayer->GetGameObject(index)->GetComponent<ModelComponent>()->GetModel(), rootNode);
					}
				}				
				ImGui::TreePop(); // TreeNode를 닫습니다.
			}
		}
	}
}

void HierarchyWindow::Run()
{
	bool Active = (bool)GetState();
	ImGui::Begin(GetName().c_str(), &Active, GetFlag());
	Update();
	Draw();
	ImGui::End();
}

void HierarchyWindow::OnEnable()
{

}

void HierarchyWindow::OnDisable()
{

}

void HierarchyWindow::OnDestroy()
{

}

void HierarchyWindow::SetCurrentScene(Scene* _currentScene)
{
	currentScene = _currentScene;
}

void HierarchyWindow::DrawNodeRecursive(std::shared_ptr<Model> _model, AiNode* _node)
{
	if (!_node) return;

	if (ImGui::TreeNode(_node->GetName().c_str()))
	{
		if (ImGui::IsItemClicked())
		{ // 노드 선택 처리 로직 
			INSPECTOR->SetSelectedAiNode(_node);
			INSPECTOR->SetSelectedMesh(_node->GetMesh()); // 여기 매쉬 넣어야 됨
			if (_node->GetMesh() != nullptr)
			{ // TODO : 시간 없어서 텍스쳐 보는것은 처리 안함
				int index = _node->GetMesh()->GetFbxIndex();
				auto materials = _model->GetModelData()->materials;
				//	INSPECTOR->SetSelectedTexture();
			}
		}

		// 자식 노드를 재귀적으로 그립니다.
		const auto& children = _node->GetChildren();
		for (auto* child : children)
		{
			DrawNodeRecursive(_model, child);
		}

		ImGui::TreePop(); // 현재 노드를 닫습니다.
	}
}
