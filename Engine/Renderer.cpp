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
#include "CameraObject.h"
#include "UserImGui.h"

#include "FontManager.h"

#include "AiNode.h"
#include "TextObject.h"
#include "TransformComponent.h"
#include "TimeSystem.h"
#include "ResourceSystem.h"
#include "InputLayout.h"

void Renderer::Initialize(WindowInfo* _windowInfo)
{
	D3DGraphics = std::make_unique<D3DClass>();
	D3DGraphics->Initialize(_windowInfo);
	IMGUI->Initialize(_windowInfo->hWnd, D3DGraphics->GetD3DDevice(), D3DGraphics->GetD3DDeviceContext());

	// 광원 설정
	DXMath::Vector3 lightTarget = DXMath::Vector3(0.0f, 0.0f, 0.0f);
	IMGUI->lightPos = DXMath::Vector3(200.0f, 200.0f, -200.0f);
	IMGUI->lightDir = DXMath::Vector3(lightTarget - IMGUI->lightPos);

	CreateOutlineStates();

	shadowRenderer.Initialize(D3DGraphics->GetD3DDevice().Get(), D3DGraphics->GetD3DDeviceContext().Get());
	shadowRenderer.InitShadowResources(D3DGraphics->GetD3DDevice().Get());
	//	m_skybox.Init();

	matrixConstantBuffer.Create(sizeof(MatrixBuffer));
	objectBuffer.Create(sizeof(ObjectBuffer));
	cameraBuffer.Create(sizeof(CameraBuffer));
	matrixPaletteBuffer.Create(sizeof(MatrixPallete));
	productBuffer.Create(sizeof(ProductBuffer));
	lightBuffer.Create(sizeof(LightBuffer), ConstantBuffer::Usage::DYNAMIC);

	SpotLightData test;
	test.position = DXMath::Vector3(0.0f, 100.0f, 0.0f);
	test.direction = DXMath::Vector3(0.0f, -1.0f, 0.0f);
	test.color = DXMath::Vector3(0.0f, 0.0f, 1.0f);
	test.range = 100.0f;
	test.innerCone = cos(DX::XMConvertToRadians(30.0f));
	test.outerCone = cos(DX::XMConvertToRadians(45.0f));
	test.intensity = 100.0f;

	AddSpotLight(test);

	SpotLightData test2;
	test2.position = DXMath::Vector3(300.0f, 100.0f, 0.0f);
	test2.direction = DXMath::Vector3(0.0f, -1.0f, 0.0f);
	test2.color = DXMath::Vector3(1.0f, 0.0f, 1.0f);
	test2.range = 100.0f;
	test2.innerCone = cos(DX::XMConvertToRadians(30.0f));
	test2.outerCone = cos(DX::XMConvertToRadians(45.0f));
	test2.intensity = 100.0f;

	AddSpotLight(test2);


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
	D3DGraphics->BeginDraw(IMGUI->GetBackGroundColor());
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

	// 명시적으로 스텐실 초기화
	if (originalDSV)
	{
		d3dDeviceContext->ClearDepthStencilView(
			originalDSV,
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f,    // depth clear value
			0        // stencil clear value
		);
	}

	// 1. 그림자 맵 패스

	shadowRenderer.BeginShadowPass(d3dDeviceContext.Get());

	ID3D11InputLayout* currentLayout1;
	d3dDeviceContext->IAGetInputLayout(&currentLayout1);

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

	// 1. 먼저 마스크 패스
	for (auto& renderComponent : work)
	{
		if (renderComponent->GetOwner()->GetEffect() == Object::Effect::OutLine)
		{
			d3dDeviceContext->OMSetDepthStencilState(outlineMaskState.Get(), 1);
			RenderObject(renderComponent, false);  // 일반 셰이더로 마스크 생성
		}
	}

	// 2. 그 다음 아웃라인 패스
	for (auto& renderComponent : work)
	{
		if (renderComponent->GetOwner()->GetEffect() == Object::Effect::OutLine)
		{
			d3dDeviceContext->OMSetDepthStencilState(outlineStencilState.Get(), 1);
			d3dDeviceContext->RSSetState(outlineRasterizerState.Get());  // 래스터라이저 상태 설정
			RenderObject(renderComponent, true);   // 아웃라인 셰이더로 렌더링
		}
	}

	// 3. 마지막으로 일반 오브젝트
	d3dDeviceContext->OMSetDepthStencilState(nullptr, 0);
	d3dDeviceContext->RSSetState(nullptr);
	for (auto& renderComponent : work)
	{
		if (renderComponent->GetOwner()->GetEffect() != Object::Effect::OutLine)
		{
			RenderObject(renderComponent, false);
		}
	}

	if (originalDSV) originalDSV->Release();
	if (originalRTV) originalRTV->Release();
}

void Renderer::D2DDraw()
{
	for (auto& D2DrenderComponent : D2Dwork)
	{
		if (true == D2DrenderComponent->GetActive())
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

void Renderer::AddSpotLight(const SpotLightData& light)
{
	if (spotLights.size() < 7)
	{
		spotLights.push_back(light);
	}
}

void Renderer::UpdateSpotLights()
{
	LightBuffer lightData;
	lightData.LIGHT_NUM = static_cast<int>(spotLights.size());

	for (size_t i = 0; i < spotLights.size(); i++)
	{
		lightData.spotLights[i] = spotLights[i];
	}
	lightBuffer.Update(&lightData, sizeof(LightBuffer));
}

void Renderer::CreateOutlineStates()
{
	// 1. 마스크 생성을 위한 스텐실 상태
	D3D11_DEPTH_STENCIL_DESC maskDesc = {};
	maskDesc.DepthEnable = true;
	maskDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	maskDesc.DepthFunc = D3D11_COMPARISON_LESS;  // 기본 깊이 테스트

	// 스텐실 설정
	maskDesc.StencilEnable = true;
	maskDesc.StencilReadMask = 0xFF;
	maskDesc.StencilWriteMask = 0xFF;

	// 전면 페이스 스텐실 설정
	maskDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	maskDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	maskDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	maskDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	// 후면 페이스도 동일하게 설정
	maskDesc.BackFace = maskDesc.FrontFace;

	// 마스크 스테이트 생성
	HRESULT hr = D3DGraphics->GetD3DDevice()->CreateDepthStencilState(&maskDesc, &outlineMaskState);
	if (FAILED(hr)) {
		// 에러 처리
		// LOG_ERROR("Failed to create outline mask state");
		return;
	}

	// 2. 아웃라인 렌더링을 위한 스텐실 상태
	D3D11_DEPTH_STENCIL_DESC outlineDesc = {};
	outlineDesc.DepthEnable = true;
	outlineDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;  // 깊이 쓰기 비활성화
	outlineDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;             // 정확한 깊이 테스트

	// 스텐실 설정
	outlineDesc.StencilEnable = true;
	outlineDesc.StencilReadMask = 0xFF;
	outlineDesc.StencilWriteMask = 0xFF;

	// 스텐실이 마킹되지 않은 영역에만 아웃라인 그리기
	outlineDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
	outlineDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	outlineDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	outlineDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	// 후면 페이스도 동일하게 설정
	outlineDesc.BackFace = outlineDesc.FrontFace;

	// 아웃라인 스테이트 생성
	hr = D3DGraphics->GetD3DDevice()->CreateDepthStencilState(&outlineDesc, &outlineStencilState);
	if (FAILED(hr)) {
		// 에러 처리
		// LOG_ERROR("Failed to create outline stencil state");
		return;
	}

	// 3. 아웃라인용 래스터라이저 상태 생성
	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.DepthBias = 5000;              // Z-fighting 방지를 위한 깊이 바이어스
	rastDesc.DepthBiasClamp = 0.0f;
	rastDesc.SlopeScaledDepthBias = 1.0f;
	rastDesc.MultisampleEnable = true;       // MSAA 활성화

	hr = D3DGraphics->GetD3DDevice()->CreateRasterizerState(&rastDesc, &outlineRasterizerState);
	if (FAILED(hr)) {
		// 에러 처리
		// LOG_ERROR("Failed to create outline rasterizer state");
		return;
	}

	// 4. 쉐이더 로드
	outlineShader = RESOURCESYSTEM->Load<Shader>("Shaders/OutLineVS.hlsl");
	if (outlineShader) {
		outlineShader->Load("Resource/Shaders/OutLinePS.hlsl");
	}
}

void Renderer::RenderObject(RenderComponent* renderComponent, bool isOutlinePass)
{
	if (renderComponent->GetActive() == false)
		return;
	auto d3dDeviceContext = D3DGraphics->GetD3DDeviceContext();
	auto* modelData = renderComponent->GetModelData()->GetModelData();
	std::unordered_map<std::string, AiNode*>* nodeData = renderComponent->GetNodeData();

	for (auto& data : *modelData->meshs)
	{
		auto* meshData = data->GetMeshInfo();

		// IA Stage Setup
		auto* vertexBuffer = meshData->vertexBuffer;
		d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3dDeviceContext->IASetVertexBuffers(0, 1, vertexBuffer->GetBuffer().GetAddressOf(),
			&vertexBuffer->vertextBufferStride, &vertexBuffer->vertextBufferOffset);

		auto* indexBuffer = meshData->indexBuffer;
		d3dDeviceContext->IASetIndexBuffer(indexBuffer->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
		auto layout = meshData->inputLayout.GetInputLayout().Get();
		if (layout)
		{
			d3dDeviceContext->IASetInputLayout(layout);
		}

		// VS Stage Setup
		if (isOutlinePass)
		{
			d3dDeviceContext->VSSetShader(outlineShader->GetVertexShader().Get(), nullptr, 0);
		}
		else
		{
			d3dDeviceContext->VSSetShader(renderComponent->GetShader(ShaderType::VS)->GetVertexShader().Get(), nullptr, 0);
		}

		d3dDeviceContext->VSSetConstantBuffers(0, 1, matrixConstantBuffer.GetBuffer().GetAddressOf());
		d3dDeviceContext->VSSetConstantBuffers(1, 1, objectBuffer.GetBuffer().GetAddressOf());
		d3dDeviceContext->VSSetConstantBuffers(3, 1, matrixPaletteBuffer.GetBuffer().GetAddressOf());

		// PS Stage Setup
		if (isOutlinePass)
		{
			d3dDeviceContext->PSSetShader(outlineShader->GetPixelShader().Get(), nullptr, 0);
		}
		else
		{
			d3dDeviceContext->PSSetShader(renderComponent->GetShader(ShaderType::PS)->GetPixelShader().Get(), nullptr, 0);
		}

		d3dDeviceContext->PSSetConstantBuffers(0, 1, matrixConstantBuffer.GetBuffer().GetAddressOf());
		d3dDeviceContext->PSSetConstantBuffers(1, 1, objectBuffer.GetBuffer().GetAddressOf());

		// Update Constant Buffers
		MatrixBuffer matrixData;
		auto node = nodeData->find(meshData->meshName);
		matrixData.worldMatrix = DX::XMMatrixTranspose(node->second->GetTransform().GetWorldMatrix());
		matrixData.viewMatrix = DX::XMMatrixTranspose(CameraObject::g_MainCameraObject->GetViewMatrix());
		matrixData.projectionMatrix = DX::XMMatrixTranspose(CameraObject::g_MainCameraObject->GetProjectionMatrix());

		Material* material = (*modelData->materials)[meshData->GetMaterialIndex()];
		ObjectBuffer objectData;
		objectData.metalness = material->GetMetalness();
		objectData.roughness = material->GetRoughness();
		objectData.onOutline = isOutlinePass;

		// Update Matrix Palette if needed
		if (nullptr != modelData->matrixPallete)
		{
			d3dDeviceContext->UpdateSubresource(matrixPaletteBuffer.GetBuffer().Get(), 0, nullptr,
				&(*modelData->matrixPallete), 0, 0);
		}
		else
		{
			static DXMath::Matrix identityPallete[128];
			d3dDeviceContext->UpdateSubresource(matrixPaletteBuffer.GetBuffer().Get(), 0, nullptr,
				identityPallete, 0, 0);
		}

		// Handle Textures
		if (!isOutlinePass)  // 아웃라인 패스에서는 텍스처가 필요 없음
		{
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
		}

		// Update and Draw
		d3dDeviceContext->UpdateSubresource(matrixConstantBuffer.GetBuffer().Get(), 0, nullptr, &matrixData, 0, 0);
		d3dDeviceContext->UpdateSubresource(objectBuffer.GetBuffer().Get(), 0, nullptr, &objectData, 0, 0);
		d3dDeviceContext->DrawIndexed(indexBuffer->GetIndexCount(), 0, 0);
	}
}
