#include "pch.h"
#include "ResourceSystem.h"
#include "SkyBox.h"
#include "D3DClass.h"
#include "Vertex.h"
#include "Helper.h"
#include "InputLayout.h"

void SkyBox::Init()
{
	auto device = D3DClass::GetD3DDevice();
	CreateBuffers(device.Get());
	//CreateShaders(device);
	CreateDepthState(device.Get());
	CreateSamplerState(device.Get());

	LoadEnvironmentMap("Map/SkyBlueEnvHDR.dds");
	LoadIrradianceMap("Map/SkyBlueDiffuseHDR.dds");
	LoadSpecularMap("Map/SkyBlueSpecularHDR.dds");
	LoadBRDFLUT("Map/SkyBlueBrdf.dds");

	SetShaders(device.Get(), L"Resource/Shaders/SkyBoxVS.hlsl", L"Resource/Shaders/SkyBoxPS.hlsl");
}

void SkyBox::Render(ID3D11DeviceContext* context)
{
	// 깊이 버퍼 상태 설정
	ComPtr<ID3D11DepthStencilState> originalDepthState;
	UINT originalStencilRef;
	context->OMGetDepthStencilState(originalDepthState.GetAddressOf(), &originalStencilRef);
	context->OMSetDepthStencilState(m_depthState.Get(), 0);

	ComPtr<ID3D11RasterizerState> originalRasterizerState;
	context->RSGetState(originalRasterizerState.GetAddressOf());

	// 환경맵 텍스처 설정
	auto envMap = m_environmentMap->GetTexture();

	if (!envMap) {
		std::cout << ("ERROR: 환경맵 텍스처 NULL\n");
		return;
	}
	
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	//envMap->GetDesc(&srvDesc);
	//std::cout << (std::format("환경맵 Format: {}\n", (int)srvDesc.Format).c_str());
	//
	//std::cout << (std::format("ViewDimension: {}\n", (int)srvDesc.ViewDimension).c_str());

	context->PSSetShaderResources(20, 1, m_environmentMap->GetTexture().GetAddressOf());

	// 샘플러 스테이트 설정
	context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

	// 렌더링 설정
	UINT stride = sizeof(DXMath::Vector3);
	UINT offset = 0;
	context->IASetInputLayout(m_inputLayout.Get());
	context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 셰이더 설정
	//context->VSSetShader(m_shader->GetVertexShader().Get(), nullptr, 0);
	//context->PSSetShader(m_shader->GetPixelShader().Get(), nullptr, 0);

	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	// 드로우 콜
	context->DrawIndexed(m_indexCount, 0, 0);

	// 원래 깊이 상태 복원
	context->OMSetDepthStencilState(originalDepthState.Get(), originalStencilRef);

	// 리소스 클리어
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	context->PSSetShaderResources(20, 1, nullSRV);
}

void SkyBox::SetShaders(ID3D11Device* device, std::wstring vsPath, std::wstring psPath)
{
	//InputLayout input;
	//std::initializer_list<D3D11_INPUT_ELEMENT_DESC> elements = {
	//   { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//};
	//input.IASetInputLayout(elements, "Shader/SkyBoxVS.hlsl");

	// 셰이더 생성 resourcemanager생성전까지 임시 보류
	//m_shader = RESOURCESYSTEM->Load<Shader>("Shader/SkyBoxPS.hlsl");
	//m_shader = RESOURCESYSTEM->Load<Shader>("Shader/SkyBoxVS.hlsl");

	// 사용하는 부분:
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;

	// 버텍스 셰이더 컴파일
	HR_T(CompileShaderFromFile(vsPath, "main", "vs_5_0", &vsBlob));

	// 픽셀 셰이더 컴파일
	HR_T(CompileShaderFromFile(psPath, "main", "ps_5_0", &psBlob));

	// 셰이더 생성
	HR_T(device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		nullptr,
		m_vertexShader.GetAddressOf()));

	HR_T(device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		nullptr,
		m_pixelShader.GetAddressOf()));

	D3D11_INPUT_ELEMENT_DESC layout[] = {
	   { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	HR_T(device->CreateInputLayout(
		layout,
		ARRAYSIZE(layout),
		vsBlob->GetBufferPointer(),    // vsData 대신 vsBlob 사용
		vsBlob->GetBufferSize(),       // vsData 대신 vsBlob 사용
		m_inputLayout.GetAddressOf()));

	// Blob 해제
	if (vsBlob) vsBlob->Release();
	if (psBlob) psBlob->Release();
}

void SkyBox::LoadEnvironmentMap(const std::string_view& filename)
{
	m_environmentMap = RESOURCESYSTEM->Load<Texture>(filename);
}

void SkyBox::LoadIrradianceMap(const std::string_view& filename)
{
	m_irradianceMap = RESOURCESYSTEM->Load<Texture>(filename);
}

void SkyBox::LoadSpecularMap(const std::string_view& filename)
{
	m_specularMap = RESOURCESYSTEM->Load<Texture>(filename);
}

void SkyBox::LoadBRDFLUT(const std::string_view& filename)
{
	m_brdfLUT = RESOURCESYSTEM->Load<Texture>(filename);
}


void SkyBox::CreateBuffers(ID3D11Device* device)
{
	struct SkyboxVertex {
		DXMath::Vector3 pos;
	};

	SkyboxVertex vertices[] = {
		{ DXMath::Vector3(-1.0f,  1.0f, -1.0f) },
		{ DXMath::Vector3(1.0f,  1.0f, -1.0f) },
		{ DXMath::Vector3(1.0f,  1.0f,  1.0f) },
		{ DXMath::Vector3(-1.0f,  1.0f,  1.0f) },
		{ DXMath::Vector3(-1.0f, -1.0f, -1.0f) },
		{ DXMath::Vector3(1.0f, -1.0f, -1.0f) },
		{ DXMath::Vector3(1.0f, -1.0f,  1.0f) },
		{ DXMath::Vector3(-1.0f, -1.0f,  1.0f) }
	};

	UINT indices[] = {
		// 위
		0, 1, 2,
		2, 3, 0,
		// 아래
		4, 7, 6,
		6, 5, 4,
		// 앞
		3, 2, 6,
		6, 7, 3,
		// 뒤
		1, 0, 4,
		4, 5, 1,
		// 왼쪽
		0, 3, 7,
		7, 4, 0,
		// 오른쪽
		2, 1, 5,
		5, 6, 2
	};
	m_indexCount = ARRAYSIZE(indices);

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	HR_T(device->CreateBuffer(&bd, &initData, m_vertexBuffer.GetAddressOf()));

	bd.ByteWidth = sizeof(indices);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	initData.pSysMem = indices;
	HR_T(device->CreateBuffer(&bd, &initData, m_indexBuffer.GetAddressOf()));
}

void SkyBox::CreateDepthState(ID3D11Device* device)
{
	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = TRUE;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	HR_T(device->CreateDepthStencilState(&depthDesc, m_depthState.GetAddressOf()));
}

void SkyBox::CreateSamplerState(ID3D11Device* device)
{
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR_T(device->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf()));
}

HRESULT SkyBox::CompileShaderFromFile(const std::wstring& path, LPCSTR entry, LPCSTR target, ID3DBlob** blob)
{
	ID3DBlob* errorBlob = nullptr;
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif

	HRESULT hr = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry,
		target,
		flags,
		0,
		blob,
		&errorBlob
	);

	if (FAILED(hr)) {
		if (errorBlob) {
			OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
			errorBlob->Release();
		}
		return hr;
	}
	if (errorBlob) errorBlob->Release();
	return S_OK;
}