#include "pch.h"
#include "MeshInformationEditor.h"
#include "Mesh.h"
#include "Vertex.h"
#include "IndexBuffer.h"

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
			ImGui::Value("VertexBufferSize : ", (int)meshInfo->vertexBuffer->vertices.size());
			ImGui::Value("IndexBufferSize : ", meshInfo->indexBuffer->GetIndexCount());			
		}
	}
}

void MeshInformationEditor::SetSelectedMesh(Mesh* _mesh)
{
	selectedMesh = _mesh;
}
