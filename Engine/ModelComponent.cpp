#include "pch.h"
#include "Object.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

#include "ResourceSystem.h"
#include "Mesh.h"
#include "Model.h"

void ModelComponent::ComponentInitialize()
{
	objectTransform = owner->GetComponent<TransformComponent>(0); // 오너의 트랜스폼을 넣는다.
	
	// 테스트 코드
	//	mesh = new Mesh;
	//	
	//	Vertex v1;
	//	v1.position = { -0.5,-0.5,0.5 };
	//	Vertex v2;
	//	v2.position = { 0,0.5,0.5 };
	//	Vertex v3;
	//	v3.position = { 0.5,-0.5,0.5 };
	//	
	//	std::vector<Vertex> vec = { v1,v2,v3 };
	//	mesh->CreateVertexBuffer(vec);
	//	std::vector<DWORD> ind = { 0, 1, 2 };
	//	mesh->CreateIndexBuffer(ind);
	//	
	//	InputLayout::Layuout offset;
	//	std::initializer_list<D3D11_INPUT_ELEMENT_DESC> elements = 
	//	{
	//		{ "SV_VertexID", 0, DXGI_FORMAT_R32_UINT,			0, offset.Byte(InputLayout::Layuout::Value::Int),     D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{ "POSITION",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offset.Byte(InputLayout::Layuout::Value::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offset.Byte(InputLayout::Layuout::Value::Color),   D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{ "TEXCOORD0",   0, DXGI_FORMAT_R32G32_FLOAT,		0, offset.Byte(InputLayout::Layuout::Value::Vector2), D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offset.Byte(InputLayout::Layuout::Value::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{ "TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offset.Byte(InputLayout::Layuout::Value::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{ "BINORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offset.Byte(InputLayout::Layuout::Value::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0}
	//	};
	// mesh->CreateInputLayout(elements, );  // 일단 보류... 세이더 어떻게 넣지..? 일단 위 작업이 모델 클래스 하나 더 파서 다시 정리하자
}

void ModelComponent::Load(std::wstring_view _filePath)
{
	model = RESOURCESYSTEM->Load<Model>(_filePath);
}
