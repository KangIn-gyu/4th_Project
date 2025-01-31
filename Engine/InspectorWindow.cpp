#include "pch.h"
#include "InspectorWindow.h"
#include "Editor.h"
#include "Object.h"
#include "Helper.h"

#include "TransformEditor.h"
#include "MeshInformationEditor.h"
#include "TextureInformationEditor.h"
#include "CameraInformationEditor.h"
#include "AnimationEditor.h"
#include <imgui.h>
InspectorWindow::InspectorWindow()
{
	SetName("Inspector");
	SetSize(ImVec2(300, 600));
	SetFlags(ImGuiWindowFlags_None);
}

InspectorWindow::~InspectorWindow()
{
	SafeExtinction::SAFE_CLEAR_CONTAINER(editors);
}

void InspectorWindow::Initialize()
{
	editors.push_back(new TransformEditor());			 // 0
	editors.push_back(new MeshInformationEditor());		 // 1
	editors.push_back(new TextureInformationEditor());	 // 2
	editors.push_back(new CameraInformationEditor());	 // 3
	editors.push_back(new AnimationEditor());			 // 4
}

void InspectorWindow::Update()
{
	for (auto& editor : editors)
	{
		editor->Update();
	}
}

void InspectorWindow::Draw()
{
	ImGui::BeginChild("InspectorWindow", ImVec2(0, 0), true, ImGuiWindowFlags_None);
	for (auto& editor : editors)
	{
		editor->Draw();
	}
	ImGui::EndChild();
}

void InspectorWindow::Run()
{
	bool Active = (bool)GetState();
	ImGui::Begin(GetName().c_str(), &Active, GetFlag());
	Update();
	Draw();
	ImGui::End();
}

void InspectorWindow::OnEnable()
{
}

void InspectorWindow::OnDisable()
{
}

void InspectorWindow::OnDestroy()
{
}

void InspectorWindow::SetSelectedObject(Object* _obj)
{
	selectedObject = _obj;
	static_cast<TransformEditor*>(editors[0])->SetSelectedObject(selectedObject);
	static_cast<AnimationEditor*>(editors[4])->SetSelectedObject(selectedObject);

	if (nullptr != _obj && _obj->GetObjectType() == Object::ObjectType::Camera)
	{
		static_cast<CameraInformationEditor*>(editors[3])->OnEnable();
		static_cast<CameraInformationEditor*>(editors[3])->SetCameraObject(selectedObject);
	}
	else
	{
		static_cast<CameraInformationEditor*>(editors[3])->SetCameraObject(nullptr);
	}
}

void InspectorWindow::SetSelectedMesh(Mesh* _mesh)
{
	selectedMesh = _mesh;
	static_cast<MeshInformationEditor*>(editors[1])->SetSelectedMesh(selectedMesh);

	{
		static_cast<AnimationEditor*>(editors[4])->OnDisable();
	}
}

void InspectorWindow::SetSelectedTexture(Texture* _texture)
{
	static_cast<TextureInformationEditor*>(editors[2])->SetSelectedTexture(_texture);
}

void InspectorWindow::SetSelectedAiNode(AiNode* _aiNode)
{
	selectedAiNode = _aiNode;
	static_cast<TransformEditor*>(editors[0])->SetSelectedNode(_aiNode);
}
