#include "pch.h"
#include "ShadowRenderer.h"
#include "ConstantBufferData.h"
#include "Shader.h"
#include "ResourceSystem.h"
#include "SkeletalMesh.h"
#include "ModelComponent.h"
#include "Renderer.h"
#include "RenderComponent.h"

bool ShadowRenderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
    // 셰도우 맵 텍스처 생성
    D3D11_TEXTURE2D_DESC shadowMapDesc = {};
    shadowMapDesc.Width = SHADOW_MAP_SIZE;
    shadowMapDesc.Height = SHADOW_MAP_SIZE;
    shadowMapDesc.MipLevels = 1;
    shadowMapDesc.ArraySize = 1;
    shadowMapDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    shadowMapDesc.SampleDesc.Count = 1;
    shadowMapDesc.SampleDesc.Quality = 0;
    shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
    shadowMapDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    shadowMapDesc.CPUAccessFlags = 0;
    shadowMapDesc.MiscFlags = 0;

    if (FAILED(device->CreateTexture2D(&shadowMapDesc, nullptr, shadowMap.GetAddressOf())))
    {
        return false;
    }

    // 뎁스 스텐실 뷰 생성
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;

    if (FAILED(device->CreateDepthStencilView(shadowMap.Get(), &dsvDesc, shadowMapDSV.GetAddressOf())))
    {
        return false;
    }

    // 래스터라이저 상태 생성
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.DepthBias = 1000;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.SlopeScaledDepthBias = 1.0f;
    device->CreateRasterizerState(&rasterDesc, shadowRasterState.GetAddressOf());

    // 깊이 스텐실 상태 생성
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    device->CreateDepthStencilState(&depthStencilDesc, shadowDepthState.GetAddressOf());

    // 셰이더 리소스 뷰 생성
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    if (FAILED(device->CreateShaderResourceView(shadowMap.Get(), &srvDesc, shadowMapSRV.GetAddressOf())))
    {
        return false;
    }

    // 상수 버퍼 생성
    D3D11_BUFFER_DESC shadowBufferDesc = {};
    shadowBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    shadowBufferDesc.ByteWidth = sizeof(ShadowBuffer);
    shadowBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    shadowBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(device->CreateBuffer(&shadowBufferDesc, nullptr, shadowCB.GetAddressOf())))
    {
        return false;
    }

    D3D11_BUFFER_DESC basicBufferDesc = {};
    basicBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    basicBufferDesc.ByteWidth = sizeof(MatrixBuffer);
    basicBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    basicBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(device->CreateBuffer(&basicBufferDesc, nullptr, basicCB.GetAddressOf())))
    {
        return false;
    }

    return true;
}

void ShadowRenderer::BeginShadowPass(ID3D11DeviceContext* context)
{
    // 뷰포트 설정
    D3D11_VIEWPORT shadowViewport = {};
    shadowViewport.TopLeftX = 0.0f;
    shadowViewport.TopLeftY = 0.0f;
    shadowViewport.Width = 4096.f;
    shadowViewport.Height = 4096.f;
    shadowViewport.MinDepth = 0.0f;
    shadowViewport.MaxDepth = 1.0f;
    context->RSSetViewports(1, &shadowViewport);

    // 렌더 타겟을 null로 설정하고 깊이 버퍼만 사용
    ID3D11RenderTargetView* nullRTV = nullptr;
    context->OMSetRenderTargets(1, &nullRTV, shadowMapDSV.Get());

    // 깊이 버퍼 클리어
    context->ClearDepthStencilView(shadowMapDSV.Get(), D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void ShadowRenderer::InitShadowResources(ID3D11Device* device)
{
	// 로드 하는 다른 방법이 있는지 내일 물어볼것
	D3D11_BUFFER_DESC shadowBufferDesc = {};
	shadowBufferDesc.ByteWidth = sizeof(ShadowBuffer);
	shadowBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	shadowBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	shadowBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	device->CreateBuffer(&shadowBufferDesc, nullptr, shadowCB.GetAddressOf());

	InputLayout::Layout offset;
	// float4 -> R32G32B32A32 / float3 -> R32G32B32 / float2 -> R32G32
	std::initializer_list<D3D11_INPUT_ELEMENT_DESC> elements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,		 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,		 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BLENDWEIGHT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// 설정하는걸
	shadowIA.IASetInputLayout(elements, "Shaders/ShadowVS.hlsl");
	auto temp = RESOURCESYSTEM->Load<Shader>("Shaders/ShadowVS.hlsl");
	shadowVS = temp->GetVertexShader();

	/*
	update
	그림자맵을 먼저그려요 (연산)
	그림자맵 초기화
	gbuffer
	그림자맵 리소스 붙여넣고
	디퍼드 오브젝트를 여기서 그리기
	gbuffer끝내서 정상적인 상태로 돌리기 (포워드 상태)
	라이팅 패스를 여기서 그려요(빛 맵)
	포워드 오브젝트 그리기
	*/

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.BorderColor[0] = 1.0f;
	sampDesc.BorderColor[1] = 1.0f;
	sampDesc.BorderColor[2] = 1.0f;
	sampDesc.BorderColor[3] = 1.0f;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDesc, shadowSampler.GetAddressOf());
}

//이거 어캐함?
void ShadowRenderer::RenderShadow(ID3D11DeviceContext* context, const DXMath::Matrix& lightViewProj, std::vector<RenderComponent*> rendercomponent)
{
    if (!shadowMapSRV || !shadowMapDSV)
    {
        std::cout << "NO SRV or DSV\n";
        return;
    }

    // srv 초기화
    ID3D11ShaderResourceView* nullSRV = nullptr;
    context->PSSetShaderResources(24, 1, &nullSRV);
    context->RSSetState(shadowRasterState.Get());
    context->OMSetDepthStencilState(shadowDepthState.Get(), 0);
    context->VSSetShader(shadowVS.Get(), nullptr, 0);
    context->PSSetShader(nullptr, nullptr, 0);
    context->IASetInputLayout(shadowIA.GetInputLayout().Get());
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 본 매트릭스 버퍼 바인딩
    auto temp = RENDERER->GetMatrixPaletteBuffer();
    context->VSSetConstantBuffers(3, 1, temp->GetBuffer().GetAddressOf());

    for (auto* renderComp : rendercomponent)
    {
        auto* modelData = renderComp->GetModelData()->GetModelData();
        if (!modelData) {
            std::cout << "ModelData is null!" << std::endl;
            continue;
        }
        std::unordered_map<std::string, AiNode*>* nodeData = renderComp->GetNodeData();
        if (!nodeData) {
            std::cout << "NodeData is null!" << std::endl;
            continue;
        }

        // 모든 메시에 대해 순회
        for (auto& mesh : *modelData->meshs)
        {
            auto* meshInfo = mesh->GetMeshInfo();
            if (!meshInfo) continue;

            // IA 설정
            auto* vertexBuffer = meshInfo->vertexBuffer;
            context->IASetVertexBuffers(0, 1,
                vertexBuffer->GetBuffer().GetAddressOf(),
                &vertexBuffer->vertextBufferStride,
                &vertexBuffer->vertextBufferOffset);

            auto* indexBuffer = meshInfo->indexBuffer;
            context->IASetIndexBuffer(
                indexBuffer->GetBuffer().Get(),
                DXGI_FORMAT_R32_UINT,
                0);

            // 해당 메시의 노드 찾기
            auto nodeIter = nodeData->find(meshInfo->meshName);
            if (nodeIter == nodeData->end()) continue;

            // World Matrix 가져오기
            DXMath::Matrix worldMatrix = nodeIter->second->GetTransform().GetWorldMatrix();

            // 쉐도우 상수 버퍼 업데이트
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            context->Map(shadowCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            ShadowBuffer* shadowData = (ShadowBuffer*)mappedResource.pData;
            shadowData->lightviewproj = XMMatrixTranspose(lightViewProj);
            context->Unmap(shadowCB.Get(), 0);

            // 기본 변환 매트릭스 버퍼 업데이트
            D3D11_MAPPED_SUBRESOURCE basicMappedResource;
            context->Map(basicCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &basicMappedResource);
            MatrixBuffer* basicData = (MatrixBuffer*)basicMappedResource.pData;
            basicData->worldMatrix = XMMatrixTranspose(worldMatrix);
            context->Unmap(basicCB.Get(), 0);

            // 버퍼 바인딩
            ID3D11Buffer* basicBuffer = basicCB.Get();
            context->VSSetConstantBuffers(0, 1, &basicBuffer);
            ID3D11Buffer* shadowBuffer = shadowCB.Get();
            context->VSSetConstantBuffers(4, 1, &shadowBuffer);

            // 드로우 콜
            context->DrawIndexed(indexBuffer->GetIndexCount(), 0, 0);
        }
    }

    context->Flush();

    context->PSSetShaderResources(24, 1, shadowMapSRV.GetAddressOf());
    context->RSSetState(nullptr);
    context->OMSetDepthStencilState(nullptr, 0);
}