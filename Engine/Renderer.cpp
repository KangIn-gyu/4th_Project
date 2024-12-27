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

	D3DGraphics->EndDraw();
}
