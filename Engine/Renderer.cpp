#include "pch.h"
#include "Renderer.h"
#include "Declare.h"
#include "RenderComponent.h"
#include "D2DRenderComponent.h"
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
#include "TextObject.h"
#include "TransformComponent.h"
#include "TimeSystem.h"

void Renderer::Initialize(WindowInfo* _windowInfo)
{
	D3DGraphics = std::make_unique<D3DClass>();
	D3DGraphics->Initialize(_windowInfo);
	IMGUI->Initialize(_windowInfo->hWnd, D3DGraphics->GetD3DDevice(), D3DGraphics->GetD3DDeviceContext());

	// 광원 설정
	DXMath::Vector3 lightTarget = DXMath::Vector3(0.0f, 0.0f, 0.0f);
	IMGUI->lightPos = DXMath::Vector3(200.0f, 200.0f, -200.0f);
	IMGUI->lightDir = DXMath::Vector3(lightTarget - IMGUI->lightPos);

	shadowRenderer.Initialize(D3DGraphics->GetD3DDevice().Get(), D3DGraphics->GetD3DDeviceContext().Get());
	shadowRenderer.InitShadowResources(D3DGraphics->GetD3DDevice().Get());
	//	m_skybox.Init();

	matrixConstantBuffer.Create(sizeof(MatrixBuffer));
	objectBuffer.Create(sizeof(ObjectBuffer));
	cameraBuffer.Create(sizeof(CameraBuffer));
	matrixPaletteBuffer.Create(sizeof(MatrixPallete));
	productBuffer.Create(sizeof(ProductBuffer));

	D3DGraphics->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, linearWrapSampler);
	D3DGraphics->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, pointClampSampler);

#ifdef USE_D2D
	D2DGraphics = std::make_unique<D2DClass>();
	D2DGraphics->Initialize(_windowInfo);
#endif

}

void Renderer::Update(float _deltaTime)
{
	IMGUI->Update(_deltaTime);
}

void Renderer::Render()
{
	D3DGraphics->BeginDraw(IMGUI->GetBankGroundColor());
	//m_skybox.Render(D3DClass::GetD3DDeviceContext().Get());

	D3DDraw();

#ifdef USE_D2D
	D2DGraphics->BeginDraw();
	D2DDraw();
	D2DGraphics->EndDraw();
#endif

	D3DGraphics->ExtractFinalImage();
	IMGUI->Render();
	D3DGraphics->EndDraw();
}

void Renderer::D3DDraw()
{
	ComPtr<ID3D11DeviceContext> d3dDeviceContext = D3DGraphics->GetD3DDeviceContext();
	ComPtr <ID3D11Device> device = D3DGraphics->GetD3DDevice();

	// 현재 렌더링 상태 저장
	D3D11_VIEWPORT originalViewport;
	UINT numViewports = 1;
	d3dDeviceContext->RSGetViewports(&numViewports, &originalViewport);

	ID3D11RenderTargetView* originalRTV = nullptr;
	ID3D11DepthStencilView* originalDSV = nullptr;
	d3dDeviceContext->OMGetRenderTargets(1, &originalRTV, &originalDSV);

	d3dDeviceContext->PSSetSamplers(0, 1, &linearWrapSampler);
	d3dDeviceContext->PSSetSamplers(1, 1, &pointClampSampler);
	d3dDeviceContext->PSSetSamplers(2, 1, &shadowRenderer.GetShadowSampler());

	// 1. 그림자 맵 패스
	ID3D11InputLayout* currentLayout;
	d3dDeviceContext->IAGetInputLayout(&currentLayout);
	if (!currentLayout) {
		std::cout << "Input Layout is null before shadow pass\n";
	}

	shadowRenderer.BeginShadowPass(d3dDeviceContext.Get());

	ID3D11InputLayout* currentLayout1;
	d3dDeviceContext->IAGetInputLayout(&currentLayout1);
	if (!currentLayout1) {
		std::cout << "Input Layout is null before shadow pass\n";
	}
	// 그림자 맵 렌더링
	shadowRenderer.RenderShadow(d3dDeviceContext.Get(), CreateShadowMatrix(), work);
	IMGUI->srv = shadowRenderer.GetShadowMapSRV();
	
	// 원래의 렌더링 상태로 복구

	d3dDeviceContext->RSSetViewports(1, &originalViewport);
	d3dDeviceContext->OMSetRenderTargets(1, &originalRTV, originalDSV);

	//shadowRenderer.DebugShadowMap(device.Get(), d3dDeviceContext.Get());

	d3dDeviceContext->PSSetShaderResources(24, 1, shadowRenderer.GetShadowMapSRV().GetAddressOf());

	//ID3D11SamplerState* samplers[] = { linearWrapSampler.Get(), pointClampSampler.Get() };
	//d3dDeviceContext->PSSetSamplers(0, 2, samplers);

	d3dDeviceContext->VSSetConstantBuffers(2, 1, cameraBuffer.GetBuffer().GetAddressOf());
	d3dDeviceContext->PSSetConstantBuffers(2, 1, cameraBuffer.GetBuffer().GetAddressOf());

	// 상수버퍼 설정 (렌더 오브젝트 제외)
	CameraBuffer cameraData;
	cameraData.eyePosition = CameraObject::g_MainCameraObject->GetComponent<TransformComponent>()->GetPosition();
	cameraData.lightDirection = IMGUI->lightDir;

	ProductBuffer productData;
	productData.totalTime = TIMESYSTEM->GetTotalTime();

	d3dDeviceContext->PSSetConstantBuffers(5, 1, productBuffer.GetBuffer().GetAddressOf());

	//LightConstantBuffer cameraData;		// 다중 빛 CB (폐기)
	//DXMath::Vector3 eyePos = CameraObject::g_MainCameraObject->GetComponent<TransformComponent>()->GetPosition();
	//cameraData.eyePosition = DXMath::Vector4(eyePos.x, eyePos.y, eyePos.z, 1.0f);
	d3dDeviceContext->UpdateSubresource(cameraBuffer.GetBuffer().Get(), 0, nullptr, &cameraData, 0, 0);
	d3dDeviceContext->UpdateSubresource(productBuffer.GetBuffer().Get(), 0, nullptr, &productData, 0, 0);

	for (auto& renderComponent : work)
	{
		auto* modelData = renderComponent->GetModelData()->GetModelData();
		std::unordered_map<std::string, AiNode*>* nodeData = renderComponent->GetNodeData();
		for (auto& data : *modelData->meshs)
		{
			auto* meshData = data->GetMeshInfo();
			//IA 
			auto* vertexBuffer = meshData->vertexBuffer;
			d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			d3dDeviceContext->IASetVertexBuffers(0, 1, vertexBuffer->GetBuffer().GetAddressOf(), &vertexBuffer->vertextBufferStride, &vertexBuffer->vertextBufferOffset);
			auto* indexBuffer = meshData->indexBuffer;
			d3dDeviceContext->IASetIndexBuffer(indexBuffer->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
			// 문제점 발견
			// 한번 물어볼것 
			// meshdata를 돌면서 meshdata안에 inputlayout이 없는게 있을수 있나?
			// 메모리 손상이나 잘못된 참조
			// ComPtr나 스마트 포인터의 잘못된 관리
			// meshData->inputLayout의 수명주기 문제
			// 멀티스레딩 관련 동기화 문제
			// 제안하는 디버깅 방법 :
			//
			// meshData->inputLayout의 생성 / 소멸 시점 확인
			// GetInputLayout() 구현 검토
			// COM 참조 카운트 확인
			// ID3D11DeviceContext가 올바른 스레드에서 호출되는지 확인
			d3dDeviceContext->IASetInputLayout(meshData->inputLayout.GetInputLayout().Get());
			if (meshData->inputLayout.GetInputLayout().GetAddressOf())
			{
				// IA의 주소와 실제 인터페이스 값
				std::cout << "Layout Address: " << meshData->inputLayout.GetInputLayout().GetAddressOf()
					<< ", Interface: " << meshData->inputLayout.GetInputLayout().Get() << "\n";
			}


			// VS 
			d3dDeviceContext->VSSetShader(renderComponent->GetShader(ShaderType::VS)->GetVertexShader().Get(), nullptr, 0);
			d3dDeviceContext->VSSetConstantBuffers(0, 1, matrixConstantBuffer.GetBuffer().GetAddressOf());
			d3dDeviceContext->VSSetConstantBuffers(1, 1, objectBuffer.GetBuffer().GetAddressOf());
			d3dDeviceContext->VSSetConstantBuffers(3, 1, matrixPaletteBuffer.GetBuffer().GetAddressOf());

			// PS 
			d3dDeviceContext->PSSetShader(renderComponent->GetShader(ShaderType::PS)->GetPixelShader().Get(), nullptr, 0);
			d3dDeviceContext->PSSetConstantBuffers(0, 1, matrixConstantBuffer.GetBuffer().GetAddressOf());
			d3dDeviceContext->PSSetConstantBuffers(1, 1, objectBuffer.GetBuffer().GetAddressOf());

			MatrixBuffer matrixData;
			auto node = nodeData->find(meshData->meshName);
			matrixData.worldMatrix = DX::XMMatrixTranspose(node->second->GetTransform().GetWorldMatrix());
			matrixData.viewMatrix = DX::XMMatrixTranspose(CameraObject::g_MainCameraObject->GetViewMatrix());
			matrixData.projectionMatrix = DX::XMMatrixTranspose(CameraObject::g_MainCameraObject->GetProjectionMatrix());
			

			Material* material = (*modelData->materials)[meshData->GetMaterialIndex()];
			ObjectBuffer objectData;
			objectData.metalness = material->GetMetalness();
			objectData.roughness = material->GetRoughness();
			objectData.onOutline = false;

			if (renderComponent->GetOwner()->GetEffect() == Object::Effect::OutLine)
			{
				objectData.onOutline = true;
			}

			if (nullptr != modelData->matrixPallete)
			{
				d3dDeviceContext->UpdateSubresource(matrixPaletteBuffer.GetBuffer().Get(), 0, nullptr, &(*modelData->matrixPallete), 0, 0);
			}
			else
			{ // TODO : 이거 할필요가 있을가 고민중... 
				static DXMath::Matrix identityPallete[128];
				d3dDeviceContext->UpdateSubresource(matrixPaletteBuffer.GetBuffer().Get(), 0, nullptr, identityPallete, 0, 0);
			}

			while (!previousTexturerProcessing.empty())
			{
				ID3D11ShaderResourceView* nullSRV = nullptr;
				d3dDeviceContext->PSSetShaderResources(previousTexturerProcessing.top(), 1, &nullSRV);
				previousTexturerProcessing.pop();
			}

			for (auto& textur : material->GetTextures())
			{
				if (!textur->GetTextureTypeIndexs().empty())
				{
					for (auto textureIndex : textur->GetTextureTypeIndexs())
					{
						previousTexturerProcessing.push(textureIndex);
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

void Renderer::D2DDraw()
{
	for (auto& D2DrenderComponent : D2Dwork)
	{
		D2DrenderComponent->Draw();
	}
}

void Renderer::AddRenderComponent(RenderComponent* _renderComponent)
{
	work.emplace_back(_renderComponent);
}

void Renderer::D2DAddRenderComponent(D2DRenderComponent* _D2DRenderComponent)
{
	D2Dwork.push_back(_D2DRenderComponent);
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

DXMath::Matrix Renderer::CreateShadowMatrix()
{
	// 1. Light position은 이미 IMGUI에서 설정된 값 사용
	DXMath::Vector3 lightPos = IMGUI->lightPos;

	// 2. Light direction 계산 및 정규화
	DXMath::Vector3 lightDir = IMGUI->lightDir;
	lightDir.Normalize();
	IMGUI->lightDir = lightDir;  // 정규화된 방향을 다시 저장

	// 3. Look-At 행렬 생성
	DXMath::Vector3 upVector = DXMath::Vector3(0.0f, 1.0f, 0.0f);
	DXMath::Matrix lightView = DXMath::Matrix::CreateLookAt(
		lightPos,                    // 광원 위치
		lightPos + lightDir,         // 광원이 바라보는 지점
		upVector                     // Up vector
	);

	// 4. 직교 투영 행렬 생성 
	// (이상적으로는 이 값들도 IMGUI에서 조정 가능하게 만들면 좋습니다)
	float orthoSize = 1000.0f;        // IMGUI로 조정 가능하게 수정 권장
	float nearPlane = 0.1f;          // IMGUI로 조정 가능하게 수정 권장
	float farPlane = 10000.0f;         // IMGUI로 조정 가능하게 수정 권장

	DXMath::Matrix lightProj = DXMath::Matrix::CreateOrthographic(
		orthoSize,
		orthoSize,
		nearPlane,
		farPlane
	);

	DXMath::Matrix final = lightView * lightProj;

	return final;
}
