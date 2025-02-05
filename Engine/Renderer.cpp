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

	//if (!work.empty() && work[0]->GetModelData()->GetModelData()->meshs->size() > 0)
	//{
	//	// 모든 메쉬를 순회하면서 유효한 첫 번째 InputLayout을 찾음
	//	for (auto& mesh : *work[0]->GetModelData()->GetModelData()->meshs)
	//	{
	//		if (mesh->GetMeshInfo() && mesh->GetMeshInfo()->inputLayout.GetInputLayout())
	//		{
	//			std::cout << "Found valid layout in mesh\n";
	//			d3dDeviceContext->IASetInputLayout(mesh->GetMeshInfo()->inputLayout.GetInputLayout().Get());
	//			break;
	//		}
	//	}
	//}

	// 1. 그림자 맵 패스
	ID3D11InputLayout* currentLayout;
	d3dDeviceContext->IAGetInputLayout(&currentLayout);
	//if (!currentLayout) {
	//	std::cout << "Input Layout is null before shadow pass\n";
	//}

	shadowRenderer.BeginShadowPass(d3dDeviceContext.Get());

	ID3D11InputLayout* currentLayout1;
	d3dDeviceContext->IAGetInputLayout(&currentLayout1);
	//if (!currentLayout1) {
	//	std::cout << "Input Layout is null before shadow pass\n";
	//}
	// 그림자 맵 렌더링
	shadowRenderer.RenderShadow(d3dDeviceContext.Get(), CreateShadowMatrix(), work);
	IMGUI->srv = shadowRenderer.GetShadowMapSRV();
	//d3dDeviceContext->IASetInputLayout(nullptr);
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

	bool hasSetLayout = false;
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
			//	comptr을 항상 맹신하지 말것
			//	InputLayout같은걸 계속 유지시키면서 재할당 하는 식으로 해야하는데
			//	문제점은 그냥 comptr의 임시객체를 생성해서 넣어버려서
			//	문장이 끝나면 바로 소멸되서 참조카운트가 불안정해져서 원래있던 InputLayout를 날린다는것
			//	meshData->inputLayout.GetInputLayout().Get(); 이걸 강제로 문장에 때려박으면 날라갈수도있다는거
			//	전에 되었던 이유는 input layout을 여기서만 할당하기때문에 참조카운팅이 날라갈 이유가 없고
			//	지금은 ia를 날렸다가 다시 할당했다가 날렸다가 메인 렌더링 루프에서 재할당을 하기때문에 가지고 있던
			//	문제가 생겼던것
			//	comptr은 무적이 아니다 comptr의 자동참조 카운트 관리가 문제를 일으킬 가능성이 있다.
			//	렌더링 파이프라인중에서 comptr의 스코프가 끝나면서 자동으로 release가 호출되었거나
			//	여러곳에서 같은 리소스를 참조할 때 comptr의 참조 카운트관리가 의도치 않게 작동할 가능성이 있다.
			//	그러므로 comptr은 무적이 아니다.
			auto layout = meshData->inputLayout.GetInputLayout().Get();
			if (!hasSetLayout && layout)
			{
				d3dDeviceContext->IASetInputLayout(layout);
				hasSetLayout = true;
			}
			//if (layout)
			//{
			//	// IA의 주소와 실제 인터페이스 값
			//	std::cout << "Layout Address: " << meshData->inputLayout.GetInputLayout().GetAddressOf()
			//		<< ", Interface: " << meshData->inputLayout.GetInputLayout().Get() << "\n";
			//}

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
