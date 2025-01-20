#include "pch.h"
#include "InspectorWindow.h"
#include "Editor.h"
#include "Helper.h"

#include "TransformEditor.h"


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
	TransformEditor* transformeditor = new TransformEditor;
	editors.push_back(transformeditor);
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
	for (auto& editor : editors)
	{
		ImGui::BeginChild(editor->GetName().c_str(), ImVec2(0, 0), true, ImGuiWindowFlags_None);
		editor->Draw();
		ImGui::EndChild();
	}
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
}
