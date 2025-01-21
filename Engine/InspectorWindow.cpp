#include "pch.h"
#include "InspectorWindow.h"
#include "Editor.h"
#include "Helper.h"

#include "TransformEditor.h"
#include "MeshInformationEditor.h"
#include "TextureInformationEditor.h"

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
	editors.push_back(new TransformEditor());
	editors.push_back(new MeshInformationEditor());
	editors.push_back(new TextureInformationEditor());
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
}

void InspectorWindow::SetSelectedMesh(Mesh* _mesh)
{
	selectedMesh = _mesh;
	static_cast<TransformEditor*>(editors[0])->SetSelectedMesh(selectedMesh);
	static_cast<MeshInformationEditor*>(editors[1])->SetSelectedMesh(selectedMesh);
}

void InspectorWindow::SetSelectedTexture(Texture* _texture)
{
	static_cast<TextureInformationEditor*>(editors[2])->SetSelectedTexture(_texture);
}
