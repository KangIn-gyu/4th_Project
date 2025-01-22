#include "pch.h"
#include "Renderer.h"
#include "Declare.h"
#include "RenderComponent.h"
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
#include <tchar.h>

void Renderer::Initialize(WindowInfo* _windowInfo)
{
	//D3D 초기화
	D3DGraphics = std::make_unique<D3DClass>();
	D3DGraphics->Initialize(_windowInfo);

	IMGUI->Initialize(_windowInfo->hWnd, D3DGraphics->GetD3DDevice(), D3DGraphics->GetD3DDeviceContext());

	// 상수 버퍼 생성
	matrixConstantBuffer.Create(sizeof(MatrixBuffer));
	objectBuffer.Create(sizeof(ObjectBuffer));

	// 샘플러 생성
	D3DGraphics->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, linearWrapSampler);
	D3DGraphics->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, pointClampSampler);
}

void Renderer::Update(float _deltaTime)
{
	IMGUI->Update(_deltaTime);
}

void Renderer::Render()
{
	// OMSetRenderTarget(새로 만든 타겟)
	// 이 타겟에 기존 렌더링 하고
	// OmSetRenderTarget(기존 백버퍼 타겟(스왑체인 생성하면서 같이 만든 타겟)
	// 위에서 그린 그림의 ShaderResourceView를 PSSetShaderResource(SRV);

	D3DGraphics->BeginDraw(IMGUI->GetBankGroundColor());
	Draw();
	TextDraw(300, 300, { 0,0,0,1 }, (TCHAR*)"fks");
	D3DGraphics->ExtractFinalImage();
	IMGUI->Render();
	D3DGraphics->EndDraw();
}

int Renderer::FontCreate(ID3D11Device* pDev, ID3D11DeviceContext* pContext)
{
	// ASCII 0~255 + 특수문자 '■' + Unicode 한글 완성형 총 11,440 글자, 크기: 9
	const TCHAR* filename = _T("Resource/Font/DNFBitBitv2.ttf");

	try {
		// SpriteBatch 초기화
		g_pFontBatch = new DirectX::SpriteBatch(pContext);

		// SpriteFont 객체 생성
		g_pFont = new DirectX::SpriteFont(pDev, filename);

		// 폰트 설정
		g_pFont->SetLineSpacing(14.0f);  // 폰트 9 기준, 줄간격 설정
		g_pFont->SetDefaultCharacter('_');  // 출력 글자값 미검색 시 대체 출력 키값
	}
	catch (std::exception& e)
	{
		TCHAR msg[1024] = _T("");
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, msg, e.what(), strlen(e.what()));
		
		return FALSE;
	}
	return TRUE;
}
// 텍스트 출력 함수
void Renderer::TextDraw(int x, int y, COLOR col, TCHAR* msg, ...)
{
	TCHAR buff[2048] = _T("");
	va_list vl;
	va_start(vl, msg);
	_vstprintf_s(buff, msg, vl);
	va_end(vl);

	// D3D11에서는 CommandList를 설정할 필요 없음
	g_pFontBatch->Begin();

	// 문자열 출력
	g_pFont->DrawString(
		g_pFontBatch,
		buff,
		DirectX::SimpleMath::Vector2((float)x, (float)y),
		DirectX::XMVECTORF32{ col.x, col.y, col.z, col.w } // 색상
	);

	g_pFontBatch->End();
}
void Renderer::Draw()
{
	// 디바이스 컨테스트 받기
	ComPtr<ID3D11DeviceContext> d3dDeviceContext = D3DGraphics->GetD3DDeviceContext();
	d3dDeviceContext->PSSetSamplers(0, 1, &linearWrapSampler); // TODO: 샘플러 일단 보류
	d3dDeviceContext->PSSetSamplers(1, 1, &pointClampSampler);

	FontCreate(D3DGraphics->GetD3DDevice().Get(), d3dDeviceContext.Get());
	for (auto& renderComponent : work)
	{
		auto* modelData = renderComponent->GetModelData()->GetModelData();
		for (auto& data : *modelData->meshs)
		{
			auto* MeshData= data->GetMeshInfo();
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
			matrixData.worldMatrix = DX::XMMatrixTranspose(MeshData->transform->GetWorldMatrix());  // 전치 행렬 넣기
			matrixData.viewMatrix = DX::XMMatrixTranspose(CameraObject::g_MainCameraObject->GetViewMatrix());
			matrixData.projectionMatrix = DX::XMMatrixTranspose(CameraObject::g_MainCameraObject->GetProjectionMatrix());
		
			Material* material = (*modelData->materials)[MeshData->GetMeshIndex()]; // 매쉬 인덱스랑 메터리얼 인덱스가 같다

			ObjectBuffer objectData;
			objectData.metalness = material->GetMetalness();
			objectData.roughness = material->GetRoughness();

			for (auto& textur : material->GetTextures())
			{ // 예전 코드에서 문제점인 스위치문으로 해서 더러웠지만 텍스처가 해당하는 레지스터 인덱스를 가지고 있어서 텍스처수만큼만 반복하면 됨.
				if(textur->GetTextureTypeIndex() >= 0)
				d3dDeviceContext->PSSetShaderResources(textur->GetTextureTypeIndex(), 1 , textur->GetTexture().GetAddressOf());
			}

			// 상수 버퍼 업데이트
			d3dDeviceContext->UpdateSubresource(matrixConstantBuffer.GetBuffer().Get(), 0, nullptr, &matrixData, 0, 0); // CPU -> GPU로 데이터 전송 처리
			d3dDeviceContext->UpdateSubresource(objectBuffer.GetBuffer().Get(), 0, nullptr, &objectData, 0, 0);			// CPU -> GPU로 데이터 전송 처리
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

