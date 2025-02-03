#include "pch.h"
//#include "ShadowRenderer.h"
//#include "ConstantBufferData.h"
//#include "Shader.h"
//#include "ResourceSystem.h"
//
//
//void ShadowRenderer::InitShadowResources(ID3D11Device* device)
//{
//	// 로드 하는 다른 방법이 있는지 내일 물어볼것
//	D3D11_BUFFER_DESC shadowBufferDesc = {};
//	shadowBufferDesc.ByteWidth = sizeof(ShadowBuffer);
//	shadowBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	shadowBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	shadowBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	device->CreateBuffer(&shadowBufferDesc, nullptr, shadowCB.GetAddressOf());
//
//	InputLayout::Layout offset;
//	// float4 -> R32G32B32A32 / float3 -> R32G32B32 / float2 -> R32G32
//	std::initializer_list<D3D11_INPUT_ELEMENT_DESC> elements =
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,		 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,		 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{ "BLENDWEIGHT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0}
//	};
//	
//	// 설정하는걸
//	shadowIA.IASetInputLayout(elements, "Shader/ShadowVS.hlsl");
//	auto temp = RESOURCESYSTEM->Load<Shader>("Shader/ShadowVS.hlsl");
//	shadowVS = temp->GetVertexShader();
//
//	/*
//	update
//	그림자맵을 먼저그려요 (연산)
//	그림자맵 초기화
//	gbuffer
//	그림자맵 리소스 붙여넣고
//	디퍼드 오브젝트를 여기서 그리기
//	gbuffer끝내서 정상적인 상태로 돌리기 (포워드 상태)
//	라이팅 패스를 여기서 그려요(빛 맵)	
//	포워드 오브젝트 그리기
//	*/
//
//
//	D3D11_SAMPLER_DESC sampDesc = {};
//	sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
//	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
//	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
//	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
//	sampDesc.BorderColor[0] = 1.0f;
//	sampDesc.BorderColor[1] = 1.0f;
//	sampDesc.BorderColor[2] = 1.0f;
//	sampDesc.BorderColor[3] = 1.0f;
//	sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
//	sampDesc.MinLOD = 0;
//	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
//	device->CreateSamplerState(&sampDesc, shadowSampler.GetAddressOf());
//}
//
////이거 어캐함?
//void ShadowRenderer::RenderShadow(ID3D11DeviceContext* context, const DXMath::Matrix& lightViewProj)
//{
//	// srv 초기화
//	ID3D11ShaderResourceView* nullSRV = nullptr;
//	context->PSSetShaderResources(11, 1, &nullSRV);
//
//	context->VSSetShader(shadowVS.Get(), nullptr, 0);
//	context->PSSetShader(nullptr, nullptr, 0);
//	context->IASetInputLayout(shadowIA.GetInputLayout().Get());
//	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	// 재귀적으로 노드 처리하는 람다 함수
//	std::function<void(AiNode*, const DXMath::Matrix&)> renderNodeShadow =
//		[&](AiNode* node, const DXMath::Matrix& parentTransform) {
//		if (!node) return;
//
//		// 현재 노드의 로컬 변환 계산
//		DXMath::Matrix nodeTransform = node->GetTransform().GetLocalMatrix();
//
//		// 루트 노드인 경우 추가 변환 적용
//		if (node->GetChildren().empty()) {
//			if (m_useCustomWorldMatrix) {
//				nodeTransform = nodeTransform * m_worldMatrix;
//			}
//			else {
//				DXMath::Matrix scaleMatrix = DXMath::Matrix::CreateScale(m_modelInfo.scale);
//				DXMath::Matrix rotationMatrix = DXMath::Matrix::CreateFromYawPitchRoll(
//					DirectX::XMConvertToRadians(m_modelInfo.rotation.y),
//					DirectX::XMConvertToRadians(m_modelInfo.rotation.x),
//					DirectX::XMConvertToRadians(m_modelInfo.rotation.z));
//				DXMath::Matrix translationMatrix = DXMath::Matrix::CreateTranslation(m_modelInfo.position);
//				nodeTransform = nodeTransform * scaleMatrix * rotationMatrix * translationMatrix;
//			}
//		}
//
//		// 글로벌 변환 계산
//		DXMath::Matrix globalTransform = nodeTransform * parentTransform;
//
//		// 현재 노드의 메시 렌더링
//		Mesh* nodeMesh = node->GetMesh();
//		if (nodeMesh) {
//			// 쉐도우 상수 버퍼 업데이트
//			D3D11_MAPPED_SUBRESOURCE mappedResource;
//			context->Map(shadowCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//			ShadowBuffer* shadowData = (ShadowBuffer*)mappedResource.pData;
//			shadowData->lightviewproj = XMMatrixTranspose(lightViewProj);
//			context->Unmap(shadowCB.Get(), 0);
//
//			// 기본 변환 매트릭스 버퍼 업데이트
//			D3D11_MAPPED_SUBRESOURCE basicMappedResource;
//			context->Map(MatrixBuffer.Get, 0, D3D11_MAP_WRITE_DISCARD, 0, &basicMappedResource);
//			MatrixBuffer* basicData = (BasicBuffer*)basicMappedResource.pData;
//			basicData->mWorld = XMMatrixTranspose(globalTransform);
//			context->Unmap(m_basicBuffer.Get(), 0);
//
//			// 버퍼 바인딩
//			ID3D11Buffer* basicBuffer = m_basicBuffer.Get();
//			context->VSSetConstantBuffers(0, 1, &basicBuffer);
//			ID3D11Buffer* shadowBuffer = shadowCB.Get();
//			context->VSSetConstantBuffers(4, 1, &shadowBuffer);
//
//			nodeMesh->Render(context);
//		}
//
//		// 자식 노드 재귀 처리
//		for (AiNode* child : node->GetChildren()) {
//			renderNodeShadow(child, globalTransform);
//		}
//		};
//
//	// 루트 노드부터 시작하여 재귀적으로 렌더링
//	renderNodeShadow(m_rootNode, Matrix::Identity);
//
//	// 본 매트릭스 업데이트 (애니메이션이 있는 경우)
//	if (!m_finalBoneMatrices.empty()) {
//		UpdateBoneMatrices(context);
//	}
//}
