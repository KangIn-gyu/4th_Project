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
	editors.emplace(EditorType::Transform , new TransformEditor());			 
	editors.emplace(EditorType::MeshInformation ,new MeshInformationEditor());		
	editors.emplace(EditorType::CameraInformation ,new CameraInformationEditor());	 
	editors.emplace(EditorType::Animation, new AnimationEditor());			 
//	editors.emplace(new TextureInformationEditor());	 
}

void InspectorWindow::Update()
{
	for (auto& editor : editors)
	{
		editor.second->Update();
	}
}

void InspectorWindow::Draw()
{
	ImGui::BeginChild("InspectorWindow", ImVec2(0, 0), true, ImGuiWindowFlags_None);
	for (auto& editor : editors)
	{
		editor.second->Draw();
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
	auto transformEditor = dynamic_cast<TransformEditor*>(editors.find(EditorType::Transform)->second);
	transformEditor->SetSelectedObject(selectedObject);
	transformEditor->NodeActivation(false);

	dynamic_cast<AnimationEditor*>(editors.find(EditorType::Animation)->second)->SetSelectedObject(selectedObject);

	if (nullptr != _obj && _obj->GetObjectType() == Object::ObjectType::Camera)
	{
		auto cameraEditor = dynamic_cast<CameraInformationEditor*>(editors.find(EditorType::CameraInformation)->second);
		cameraEditor->OnEnable();
		cameraEditor->SetCameraObject(selectedObject);
	}
	else
	{
		dynamic_cast<CameraInformationEditor*>(editors.find(EditorType::CameraInformation)->second)->OnDisable();
	}
}

void InspectorWindow::SetSelectedMesh(Mesh* _mesh)
{
	selectedMesh = _mesh;
	dynamic_cast<MeshInformationEditor*>(editors.find(EditorType::MeshInformation)->second)->SetSelectedMesh(selectedMesh);
	editors.find(EditorType::Animation)->second->OnDisable();
}

void InspectorWindow::SetSelectedTexture(Texture* _texture)
{
//	static_cast<TextureInformationEditor*>(editors[2])->SetSelectedTexture(_texture);
}

void InspectorWindow::SetSelectedAiNode(AiNode* _aiNode)
{
	selectedAiNode = _aiNode;
	dynamic_cast<TransformEditor*>(editors.find(EditorType::Transform)->second)->NodeActivation(true);
	dynamic_cast<TransformEditor*>(editors.find(EditorType::Transform)->second)->SetSelectedNode(selectedAiNode);
}
