#include "pch.h"
#include "HierarchyWindow.h"

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
