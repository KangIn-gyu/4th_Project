#include "pch.h"
#include "Renderer.h"

#include "ConstantBufferData.h"

void Renderer::Initialize(WindowInfo* _windowInfo)
{
	D3DGraphics = std::make_unique<D3DClass>();
	D3DGraphics->Initialize(_windowInfo);

	// 상수 버퍼 생성
	matrixConstantBuffer.Create(sizeof(MatrixBuffer));

	// 샘플러 생성
	D3DGraphics->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, linearWrapSampler);
	D3DGraphics->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, pointClampSampler);
}


void Renderer::Render()
{
	D3DGraphics->BeginDraw({ 0.0f, 0.0f, 0.0f, 0.0f});

//  보류 상태
//	//IA 입력 어셈블러 스테이지 설정
//	D3DGraphics->GetD3DDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	D3DGraphics->GetD3DDeviceContext()->IASetVertexBuffers(0, 1, );
//	D3DGraphics->GetD3DDeviceContext()->IASetIndexBuffer(, DXGI_FORMAT_R32_UINT, 0);
//	D3DGraphics->GetD3DDeviceContext()->IASetInputLayout();
//
//	// VS 정점 셰이더 스테이지 설정
//	D3DGraphics->GetD3DDeviceContext()->VSSetShader(, nullptr, 0);
//	D3DGraphics->GetD3DDeviceContext()->VSSetConstantBuffers();
//
//	// PS 정점 셰이더 스테이지 설정
//	D3DGraphics->GetD3DDeviceContext()->PSSetShader(, nullptr, 0);
//	D3DGraphics->GetD3DDeviceContext()->PSSetConstantBuffers();
//
//	// 상수 버퍼 업데이트
//	D3DGraphics-> > GetD3DDeviceContext()->UpdateSubresource(/*상수 버퍼*/, 0, nullptr, /*구조체*/, 0, 0); // CPU -> GPU로 데이터 전송 처리
//	D3DGraphics->GetD3DDeviceContext()->DrawIndexed(, 0, 0);
	D3DGraphics->EndDraw();
}
