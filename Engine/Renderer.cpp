#include "pch.h"
#include "Renderer.h"
#include "Declare.h"
#include "RenderComponent.h"
#include "Object.h"
#include "ModelComponent.h"
#include "Model.h"
#include "Mesh.h"
#include "Vertex.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "ConstantBufferData.h"
#include "CameraObject.h"
#include "UserImGui.h"

#include "FontManager.h"


#include "AiNode.h"
#include "TransformComponent.h"


void Renderer::Initialize(WindowInfo* _windowInfo)
{
	D3DGraphics = std::make_unique<D3DClass>();
	D3DGraphics->Initialize(_windowInfo);

	IMGUI->Initialize(_windowInfo->hWnd, D3DGraphics->GetD3DDevice(), D3DGraphics->GetD3DDeviceContext());

	m_skybox.Init();

	matrixConstantBuffer.Create(sizeof(MatrixBuffer));
	objectBuffer.Create(sizeof(ObjectBuffer));
	cameraBuffer.Create(sizeof(CameraBuffer));

	D3DGraphics->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, linearWrapSampler);
	D3DGraphics->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, pointClampSampler);

	D2DGraphics = std::make_unique<D2DClass>();
	D2DGraphics->Initialize(_windowInfo);

	FontManager::GetInstance()->InitializeDWrite();


}

void Renderer::Update(float _deltaTime)
{
	IMGUI->Update(_deltaTime);
}

void Renderer::Render()
{

	D3DGraphics->BeginDraw(IMGUI->GetBankGroundColor());

	m_skybox.Render(D3DClass::GetD3DDeviceContext().Get());

	D2DGraphics->BeginDraw();

	D3DDraw();


	D3DGraphics->ExtractFinalImage();
	IMGUI->Render();

	D2DGraphics->EndDraw();
	D3DGraphics->EndDraw();
}

void Renderer::D3DDraw()
{
	ComPtr<ID3D11DeviceContext> d3dDeviceContext = D3DGraphics->GetD3DDeviceContext();
	d3dDeviceContext->PSSetSamplers(0, 1, &linearWrapSampler); 
	d3dDeviceContext->PSSetSamplers(1, 1, &pointClampSampler);

	d3dDeviceContext->VSSetConstantBuffers(2, 1, cameraBuffer.GetBuffer().GetAddressOf());
	d3dDeviceContext->PSSetConstantBuffers(2, 1, cameraBuffer.GetBuffer().GetAddressOf());
	CameraBuffer cameraData;
	cameraData.eyePosition = CameraObject::g_MainCameraObject->GetComponent<TransformComponent>()->GetPosition();
	cameraData.lightDirection = DXMath::Vector3(0, -1, 0);
	d3dDeviceContext->UpdateSubresource(cameraBuffer.GetBuffer().Get(), 0, nullptr, &cameraData, 0, 0);
	for (auto& renderComponent : work)
	{
		auto* modelData = renderComponent->GetModelData()->GetModelData();
		auto nodeData = *renderComponent->GetNodeData();
		for (auto& data : *modelData->meshs)
		{
			auto* meshData = data->GetMeshInfo();
			//IA 
			auto* vertexBuffer = meshData->vertexBuffer;
			d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			d3dDeviceContext->IASetVertexBuffers(0, 1, vertexBuffer->GetBuffer().GetAddressOf(), &vertexBuffer->vertextBufferStride, &vertexBuffer->vertextBufferOffset); // ?¬ê¸° ë§¨ì•ž ?¬ë¡¯ ë²ˆí˜¸???„í’‹ ?ˆì´?„ì›ƒ ?¬ë¡¯ ë²ˆí˜¸??
			auto* indexBuffer = meshData->indexBuffer;
			d3dDeviceContext->IASetIndexBuffer(indexBuffer->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
			d3dDeviceContext->IASetInputLayout(meshData->inputLayout.GetInputLayout().Get());

			// VS 
			d3dDeviceContext->VSSetShader(renderComponent->GetShder(ShaderType::VS)->GetVertexShader().Get(), nullptr, 0);
			d3dDeviceContext->VSSetConstantBuffers(0, 1, matrixConstantBuffer.GetBuffer().GetAddressOf());
			d3dDeviceContext->VSSetConstantBuffers(1, 1, objectBuffer.GetBuffer().GetAddressOf());
			// PS 
			d3dDeviceContext->PSSetShader(renderComponent->GetShder(ShaderType::PS)->GetPixelShader().Get(), nullptr, 0);
			d3dDeviceContext->PSSetConstantBuffers(0, 1, matrixConstantBuffer.GetBuffer().GetAddressOf());
			d3dDeviceContext->PSSetConstantBuffers(1, 1, objectBuffer.GetBuffer().GetAddressOf());

			MatrixBuffer matrixData;
			auto node = *nodeData.find(meshData->meshName);
			matrixData.worldMatrix = DX::XMMatrixTranspose(node.second->GetTransform().GetWorldMatrix());  // ?„ì¹˜ ?‰ë ¬ ?£ê¸°
			matrixData.viewMatrix = DX::XMMatrixTranspose(CameraObject::g_MainCameraObject->GetViewMatrix());
			matrixData.projectionMatrix = DX::XMMatrixTranspose(CameraObject::g_MainCameraObject->GetProjectionMatrix());
		
			Material* material = (*modelData->materials)[meshData->GetMaterialIndex()];
			ObjectBuffer objectData;
			objectData.metalness = material->GetMetalness();
			objectData.roughness = material->GetRoughness();

			int textureregister = 20;
			for (UINT slot = 0; slot < textureregister; ++slot)
			{
				ID3D11ShaderResourceView* nullSRV = nullptr;
				d3dDeviceContext->PSSetShaderResources(slot, 1, &nullSRV);
			}

			for (auto& textur : material->GetTextures())
			{ 
				if (!textur->GetTextureTypeIndexs().empty())
				{
					for (auto textureIndex : textur->GetTextureTypeIndexs()) // setÀ» ¹Ýº¹ÀÚ·Î ¼øÈ¸
					{
						d3dDeviceContext->PSSetShaderResources(textureIndex, 1, textur->GetTexture().GetAddressOf());
					}
				}
			}

			d3dDeviceContext->UpdateSubresource(matrixConstantBuffer.GetBuffer().Get(), 0, nullptr, &matrixData, 0, 0); // CPU -> GPU
			d3dDeviceContext->UpdateSubresource(objectBuffer.GetBuffer().Get(), 0, nullptr, &objectData, 0, 0);			// CPU -> GPU
			d3dDeviceContext->DrawIndexed(indexBuffer->GetIndexCount(), 0, 0);
		}
	}
}

void Renderer::AddRenderComponent(RenderComponent* _renderComponent)
{
	work.emplace_back(_renderComponent);
}

void Renderer::RemoveRenderComponent(RenderComponent* _renderComponent)
{
	if (_renderComponent)
	{
		work.erase(std::remove(work.begin(), work.end(), _renderComponent), work.end());
	}
}

std::pair<int, int> Renderer::GetWindowsSize()
{
	return D3DGraphics->GetWindowsSize();
}

void Renderer::SetWindowSize()
{
	D3DGraphics->ChangeWindowSize();
}

ComPtr<ID3D11ShaderResourceView> Renderer::GetImGuiImageTexture()
{
	return D3DGraphics->GetImGuiImageTexture();
}

