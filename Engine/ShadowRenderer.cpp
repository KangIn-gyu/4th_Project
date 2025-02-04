#include "pch.h"
#include "ShadowRenderer.h"
#include "ConstantBufferData.h"
#include "Shader.h"
#include "ResourceSystem.h"
#include "SkeletalMesh.h"
#include "ModelComponent.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "Object.h"
#include "IndexBuffer.h"

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
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 1000;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.SlopeScaledDepthBias = 1.0f;
    device->CreateRasterizerState(&rasterDesc, shadowRasterState.GetAddressOf());

    // 깊이 스텐실 상태 생성
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
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
    basicBufferDesc.ByteWidth = sizeof(MatrixPallete);
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

    //std::cout << "Shadow DSV valid: " << (shadowMapDSV != nullptr) << std::endl;

    ID3D11RenderTargetView* nullRTV = nullptr;
    ID3D11DepthStencilView* nullDSV = nullptr;
    ID3D11ShaderResourceView* nullSRV = nullptr;
    context->OMSetRenderTargets(1, &nullRTV, nullDSV);
    context->PSSetShaderResources(24, 1, &nullSRV);

    context->Flush();
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


    context->OMSetRenderTargets(1, &nullRTV, shadowMapDSV.Get());
    // 깊이 버퍼 클리어
    context->ClearDepthStencilView(shadowMapDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
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

    shadowVS = RESOURCESYSTEM->Load<Shader>("Shaders/ShadowVS.hlsl");

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

    if (!shadowIA.GetInputLayout().Get())
    {
        std::cout << "Failed to create shadow input layout!" << std::endl;
        return;
    }



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
    sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
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

    D3D11_SAMPLER_DESC basicSampDesc = {};
    basicSampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;  // 기본 필터링
    basicSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    basicSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    basicSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    basicSampDesc.MinLOD = 0;
    basicSampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    device->CreateSamplerState(&basicSampDesc, basicSampler.GetAddressOf());

}

//이거 어캐함?
void ShadowRenderer::RenderShadow(ID3D11DeviceContext* context, const DXMath::Matrix& lightViewProj, std::vector<RenderComponent*> rendercomponent)
{
    if (!shadowMapSRV || !shadowMapDSV)
    {
        std::cout << "NO SRV or DSV\n";
        return;
    }

    // 1. 렌더링 시작할 때 파이프라인 상태 체크
    ID3D11RenderTargetView* boundRTV;
    ID3D11DepthStencilView* boundDSV;
    context->OMGetRenderTargets(1, &boundRTV, &boundDSV);
    //std::cout << "RTV bound: " << (boundRTV == nullptr) << " (should be null)" << std::endl;
    //std::cout << "DSV matches: " << (boundDSV == shadowMapDSV.Get()) << " (should be true)" << std::endl;
    if (boundRTV) boundRTV->Release();
    if (boundDSV) boundDSV->Release();

    // 2. 라이트 뷰프로젝션 행렬 체크
    //std::cout << "Light View-Proj Matrix: " << std::endl;
    //for (int i = 0; i < 4; i++) {
    //    std::cout << lightViewProj.m[i][0] << ", "
    //        << lightViewProj.m[i][1] << ", "
    //        << lightViewProj.m[i][2] << ", "
    //        << lightViewProj.m[i][3] << std::endl;
    //}

    // srv 초기화
    ID3D11ShaderResourceView* nullSRV = nullptr;
    context->PSSetShaderResources(24, 1, &nullSRV);

    // 잠시 대기하여 리소스 해제가 완료되도록 함
    context->Flush();

    context->RSSetState(shadowRasterState.Get());
    context->OMSetDepthStencilState(shadowDepthState.Get(), 0);

    ID3D11SamplerState* nullSampler = nullptr;
    context->PSSetSamplers(0, 1, basicSampler.GetAddressOf());
    context->PSSetSamplers(2, 1, shadowSampler.GetAddressOf());

    //auto inputLayout = shadowIA.GetInputLayout();
    //if (!inputLayout)
    //{
    //    std::cout << "Shadow Input Layout is null! Check if InitShadowResources was called." << std::endl;
    //    return;
    //}

    context->VSSetShader(shadowVS->GetVertexShader().Get(), nullptr, 0);
    context->PSSetShader(nullptr, nullptr, 0);

    //auto tmp = shadowIA.GetInputLayout().GetAddressOf();
    
    context->IASetInputLayout(shadowIA.GetInputLayout().Get());
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // input layout이 제대로 설정되었는지 확인
    ID3D11InputLayout* boundLayout;
    context->IAGetInputLayout(&boundLayout);
    if (!boundLayout)
    {
        std::cout << "Failed to bind Input Layout!" << std::endl;
    }
    if (boundLayout) boundLayout->Release();

    // 본 매트릭스 버퍼 바인딩
    auto temp = RENDERER->GetMatrixPaletteBuffer();
    context->VSSetConstantBuffers(3, 1, temp->GetBuffer().GetAddressOf());

    for (auto* renderComp : rendercomponent)
    {
        auto* modelData = renderComp->GetModelData()->GetModelData(); 

        if (renderComp->GetOwner()->GetObjectType() == Object::ObjectType::Background)
        {
            continue;
        }

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

            // 해당 메시의 노드 찾기
            auto nodeIter = nodeData->find(meshInfo->meshName);
            if (nodeIter == nodeData->end()) continue;

            UINT stride = sizeof(Vertex); // 정점 구조체의 크기
            UINT offset = 0;
            ID3D11Buffer* vertexBuffer = meshInfo->vertexBuffer->GetBuffer().Get();
            context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

            // Index Buffer도 설정
            context->IASetIndexBuffer(meshInfo->indexBuffer->GetBuffer().Get(),
                DXGI_FORMAT_R32_UINT, 0);

            auto* indexBuffer = meshInfo->indexBuffer;
            // World Matrix 가져오기
            DXMath::Matrix worldMatrix = nodeIter->second->GetTransform().GetWorldMatrix();

            // 쉐도우 상수 버퍼 업데이트
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            context->Map(shadowCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            ShadowBuffer* shadowData = (ShadowBuffer*)mappedResource.pData;
            shadowData->lightviewproj = XMMatrixTranspose(lightViewProj);
            context->Unmap(shadowCB.Get(), 0);

            //float* data = (float*)mappedResource.pData;
            //for (int i = 0; i < 10; i++) {
            //    std::cout << "Depth value " << i << ": " << data[i] << std::endl;  // 주석 해제
            //}

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

           //UINT indexCount = indexBuffer->GetIndexCount();
           //std::cout << "Drawing mesh with " << indexCount << " indices" << std::endl;
           //
           //ID3D11Buffer* vb;
           //UINT stride, offset;
           //context->IAGetVertexBuffers(0, 1, &vb, &stride, &offset);
           //std::cout << "Vertex buffer bound: " << (vb != nullptr) << std::endl;
           //if (vb) vb->Release();
           //
           //ID3D11Buffer* ib;
           //context->IAGetIndexBuffer(&ib, nullptr, nullptr);
           //std::cout << "Index buffer bound: " << (ib != nullptr) << std::endl;
           //if (ib) ib->Release();

             // 월드 행렬 체크
            //std::cout << "World Matrix: " << std::endl;
            //for (int i = 0; i < 4; i++) {
            //    std::cout << worldMatrix.m[i][0] << ", "
            //        << worldMatrix.m[i][1] << ", "
            //        << worldMatrix.m[i][2] << ", "
            //        << worldMatrix.m[i][3] << std::endl;
            //}

            // DrawIndexed 호출 직전에 바인딩된 상수버퍼 확인
            ID3D11Buffer* boundCB;
            context->VSGetConstantBuffers(4, 1, &boundCB);
            //std::cout << "Shadow CB bound before draw: " << (boundCB != nullptr) << std::endl;
            if (boundCB) boundCB->Release();


            // 드로우 콜
            context->DrawIndexed(indexBuffer->GetIndexCount(), 0, 0);
        }
    }

    context->Flush();
    //auto device = D3DClass::GetD3DDevice();
    //DebugShadowMap(device.Get(), context);

    ID3D11RenderTargetView* nullRTV = nullptr;
    context->OMSetRenderTargets(1, &nullRTV, nullptr);
    context->Flush();
    context->PSSetShaderResources(24, 1, shadowMapSRV.GetAddressOf());
    context->RSSetState(nullptr);
    context->OMSetDepthStencilState(nullptr, 0);
}

void ShadowRenderer::DebugShadowMap(ID3D11Device* device, ID3D11DeviceContext* context)
{
    // 그림자맵의 실제 내용을 확인하기 위한 임시 버퍼 생성
    D3D11_TEXTURE2D_DESC desc;
    shadowMap->GetDesc(&desc);

    // 스테이징 텍스처 생성
    desc.Usage = D3D11_USAGE_STAGING;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    desc.BindFlags = 0;

    ID3D11Texture2D* stagingTex = nullptr;
    device->CreateTexture2D(&desc, nullptr, &stagingTex);

    // 데이터 복사
    context->CopyResource(stagingTex, shadowMap.Get());

    // 데이터 읽기
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    context->Map(stagingTex, 0, D3D11_MAP_READ, 0, &mappedResource);

    // 첫 few rows의 데이터 출력
    float* data = (float*)mappedResource.pData;
    for (int i = 0; i < 10; i++) {
        //std::cout << "Depth value " << i << ": " << data[i] << std::endl;
    }

    context->Unmap(stagingTex, 0);
    stagingTex->Release();
}
