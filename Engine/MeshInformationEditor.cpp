#include "pch.h"
#include "MeshInformationEditor.h"
#include "Mesh.h"
#include "Vertex.h"
#include "IndexBuffer.h"
#include <imgui.h>
MeshInformationEditor::MeshInformationEditor()
{
	SetName("MeshInformation");
	state = State::Active;
}

void MeshInformationEditor::Initialize()
{
}

void MeshInformationEditor::Update()
{
}

void MeshInformationEditor::Draw()
{
	if (state == State::Active && nullptr != selectedMesh)
	{
		if (ImGui::CollapsingHeader(GetName().c_str() , ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto* meshInfo = selectedMesh->GetMeshInfo();
			ImGui::Text(meshInfo->meshName.c_str());
			ImGui::Value("MeshIndex : ", selectedMesh->GetFbxIndex());
			ImGui::Value("MaterialIndex : ", meshInfo->GetMaterialIndex());
			ImGui::Value("VertexBufferSize : ", (int)meshInfo->vertexBuffer->GetVertexSize());
			ImGui::Value("IndexBufferSize : ", meshInfo->indexBuffer->GetIndexCount());		
		}
	}
}

void MeshInformationEditor::SetSelectedMesh(Mesh* _mesh)
{
	selectedMesh = _mesh;
}
