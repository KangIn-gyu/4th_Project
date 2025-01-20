#include "pch.h"
#include "HierarchyWindow.h"
#include "ObjectManager.h"
#include "Object.h"
#include "ModelComponent.h"
#include "Model.h"
#include "Mesh.h"
#include "AiNode.h"

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
	const auto& objects = objectManager->GetObjects();
	for (const auto& [type, objList] : objects) 
	{
		for (size_t i = 0; i < objList.size(); ++i)
		{ // 객체의 이름으로 TreeNode를 생성합니다.
			if (ImGui::TreeNode(objList[i]->GetName().c_str()))
			{
				if (ImGui::IsItemClicked())
				{ // TODO : 추후 인스펙터창에 정보 넘기는 거 만들어야 함

				}

				// 객체의 ModelData의 rootNode를 가져옵니다.
				if (objList[i]->GetObjectType() == Object::ObjectType::Basic)
				{
					auto rootNode = objList[i]->GetComponent<ModelComponent>()->GetModel()->GetModelData()->rootNode;
					if (rootNode)
					{// AiNode 트리 구조를 재귀적으로 그립니다.
						DrawNodeRecursive(rootNode);
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

void HierarchyWindow::SetObjectManager(ObjectManager* _objectManager)
{
	objectManager = _objectManager;
}

void HierarchyWindow::DrawNodeRecursive(AiNode* node)
{
	if (!node) return;

	if (ImGui::TreeNode(node->GetName().c_str()))
	{
		if (ImGui::IsItemClicked())
		{
			// 노드 선택 처리 로직
		}

		// 자식 노드를 재귀적으로 그립니다.
		const auto& children = node->GetChildren();
		for (auto* child : children)
		{
			DrawNodeRecursive(child);
		}

		ImGui::TreePop(); // 현재 노드를 닫습니다.
	}
}
