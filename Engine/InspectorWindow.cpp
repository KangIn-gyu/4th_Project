#include "pch.h"
#include "InspectorWindow.h"
#include "Editor.h"
#include "Helper.h"

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
		editor->Draw();
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
