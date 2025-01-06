#include "pch.h"
#include "Renderer.h"

#include "RenderComponent.h"
#include "Model.h"
#include "Mesh.h"
#include "Vertex.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "ConstantBufferData.h"

void Renderer::Initialize(WindowInfo* _windowInfo)
{
	//D3D 초기화
	D3DGraphics = std::make_unique<D3DClass>();
	D3DGraphics->Initialize(_windowInfo);

	// 상수 버퍼 생성
	matrixConstantBuffer.Create(sizeof(MatrixBuffer));
	objectBuffer.Create(sizeof(ObjectBuffer));

	// 샘플러 생성
	D3DGraphics->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, linearWrapSampler);
	D3DGraphics->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, pointClampSampler);

	// 디바이스 컨테스트 받기
	d3dDeviceContext = D3DGraphics->GetD3DDeviceContext();
}


void Renderer::Render()
{
	D3DGraphics->BeginDraw({ 0.0f, 0.0f, 0.0f, 0.0f});
	Draw();
	D3DGraphics->EndDraw();
}

void Renderer::Draw()
{
	for (auto& renderComponent : work)
	{
		auto* modelData = renderComponent->GetModelData()->GetModelData();
		for (auto& data : modelData->meshs)
		{
			auto* MeshData= data.GetMeshInfo();
			//IA 입력 어셈블러 스테이지 설정
			auto* vertexBuffer = MeshData->vertexBuffer;
			d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			d3dDeviceContext->IASetVertexBuffers(0, 1, vertexBuffer->GetBuffer().GetAddressOf(), &vertexBuffer->vertextBufferStride, &vertexBuffer->vertextBufferOffset); // 여기 맨앞 슬롯 번호는 임풋 레이아웃 슬롯 번호임
			auto* indexBuffer = MeshData->indexBuffer;
			d3dDeviceContext->IASetIndexBuffer(indexBuffer->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
			d3dDeviceContext->IASetInputLayout(MeshData->inputLayout.GetInputLayout().Get());

			// VS 정점 셰이더 스테이지 설정
			d3dDeviceContext->VSSetShader(renderComponent->GetShder(ShaderType::VS)->GetVertexShader().Get(), nullptr, 0);
			d3dDeviceContext->VSSetConstantBuffers(0, 1, matrixConstantBuffer.GetBuffer().GetAddressOf());
			d3dDeviceContext->VSSetConstantBuffers(1, 1, objectBuffer.GetBuffer().GetAddressOf());
			// PS 정점 셰이더 스테이지 설정
			d3dDeviceContext->PSSetShader(renderComponent->GetShder(ShaderType::PS)->GetPixelShader().Get(), nullptr, 0);
			d3dDeviceContext->PSSetConstantBuffers(0, 1, matrixConstantBuffer.GetBuffer().GetAddressOf());
			d3dDeviceContext->PSSetConstantBuffers(1, 1, objectBuffer.GetBuffer().GetAddressOf());

			MatrixBuffer matrixData;
			matrixData.worldMatrix = DX::XMMatrixTranspose(MeshData->transform->GetWorldMatrix());
		//	matrixData.viewMatrix = DX::XMMatrixTranspose();
		//	matrixData.projectionMatrix = DX::XMMatrixTranspose();
		
			ObjectBuffer objectData;
		//	objectData.metalness = ;
		//	objectData.roughness = ;

			// 텍스처 처리하는 거 만들어야 함
			Material* material = modelData->materials[MeshData->GetMeshIndex()]; // 매쉬 인덱스랑 메터리얼 인덱스가 같다
			for (auto& textur : material->GetTextures())
			{ // 예전 코드에서 문제점인 스위치문으로 해서 더러웠지만 텍스처가 해당하는 레지스터 인덱스를 가지고 있어서 텍스처수만큼만 반복하면 됨.
				d3dDeviceContext->PSSetShaderResources(textur->GetTextureTypeIndex(), 1 , textur->GetTexture().GetAddressOf());
			}

			// 상수 버퍼 업데이트
			d3dDeviceContext->UpdateSubresource(matrixConstantBuffer.GetBuffer().Get(), 0, nullptr, &matrixData, 0, 0); // CPU -> GPU로 데이터 전송 처리
			d3dDeviceContext->UpdateSubresource(objectBuffer.GetBuffer().Get(), 0, nullptr, &objectData, 0, 0); // CPU -> GPU로 데이터 전송 처리
			d3dDeviceContext->DrawIndexed(indexBuffer->GetIndexCount(), 0, 0);
		}
	}
}

void Renderer::AddRenderComponent(RenderComponent* _renderComponent)
{
	work.emplace_back(_renderComponent);
}
