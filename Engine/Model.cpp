#include "pch.h"
#include "Object.h"
#include "Model.h"
#include "ModelComponent.h"
#include "Mesh.h"
#include "InputLayout.h"
#include "Helper.h"
#include "StaticMesh.h"
#include "SkeletalMesh.h"
Model::Model()
{
	data = new ModelData;
}

Model::~Model()
{
	SafeExtinction::SAFE_DELETE(data);
}

void Model::Initialize()
{
	for (auto& it : *data->meshs) // 음... 이 방식으로 하기 싫었지만 시간때문에 그냥 타협함.
	{
		if (owner->GetOwner()->GetObjectType() == Object::ObjectType::Basic)
		{
			InputLayout::Layout offset;
			// float4 -> R32G32B32A32 / float3 -> R32G32B32 / float2 -> R32G32
			std::initializer_list<D3D11_INPUT_ELEMENT_DESC> elements = 
			{			
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offset.Byte(InputLayout::Layout::Value::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offset.Byte(InputLayout::Layout::Value::Color),   D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT,	     0, offset.Byte(InputLayout::Layout::Value::Vector2), D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offset.Byte(InputLayout::Layout::Value::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offset.Byte(InputLayout::Layout::Value::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offset.Byte(InputLayout::Layout::Value::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0}
			};
		
			it->CreateInputLayout(elements, "Shaders/VertexShaderVS.hlsl");
		}
	}

	data->rootNode->GetPointTransform()->SetParent(owner->GetTransform());
}

void Model::Update(const float _deltaTime)
{ 
	data->rootNode->Update(_deltaTime);
}

void Model::SetOwner(ModelComponent* _owner)
{
	 owner = _owner; 
}

void Model::SetMesh(std::vector<Mesh*>* _meshs)
{
	data->meshs = _meshs;
}

void Model::SetMateria(std::vector<Material*>* _materials)
{
	data->materials = _materials;
}

void Model::SetTreeNode(std::vector<AiNode*>* _treeNode)
{
	data->treeNode = _treeNode;
	data->rootNode = (*data->treeNode)[0];
}
