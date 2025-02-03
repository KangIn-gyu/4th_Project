#include "pch.h"
#include "D3DClass.h"
#include "Helper.h"

#include "Declare.h"
#include <dxgidebug.h>

// 정적변수랑은 스태틱은 의미가 달라서 g_표시 안함
ComPtr<ID3D11Device>        D3DClass::D3DDevice =        nullptr; 
ComPtr<ID3D11DeviceContext> D3DClass::D3DDeviceContext = nullptr;
ComPtr<IDXGISurface> D3DClass::DXGISurface = nullptr;
std::unique_ptr<Viewport>   D3DClass::viewport = nullptr;

D3DClass::~D3DClass()
{
	swapChain->SetFullscreenState(FALSE, nullptr);

#ifdef _DEBUG
	MemoryLick();
#endif
}

void D3DClass::Initialize(WindowInfo* _windowInfo)
{
	windowInfo = _windowInfo;
	InitD3D();
	InitDXGI();
}

void D3DClass::BeginDraw(DXMath::Color _BackgroundColor)
{
	D3DDeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get()); // OM
	D3DDeviceContext->ClearRenderTargetView(renderTargetView.Get(), _BackgroundColor);
	D3DDeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DClass::EndDraw()
{
	bool isSwapChainOccluded = false;

	if (true == presentEnabled)
	{
		isSwapChainOccluded = (swapChain->Present(1, 0) == DXGI_STATUS_OCCLUDED);   // 화면 새로 고침 비율을 고정합니다.
	}
	else
	{
		isSwapChainOccluded = (swapChain->Present(0, 0) == DXGI_STATUS_OCCLUDED);  // 가능한 빠르게 출력합니다
	}

	if (true == isSwapChainOccluded)
	{
		std::cout << "스왑체인이 화면에 보이지 않거나 가려졌습니다." << std::endl;
	}
}

void D3DClass::ChangeWindowSize()
{
	renderTargetView.Reset();
	renderTargetBuffer.Reset();
	depthStencilView.Reset();
	depthStencilBuffer.Reset();

	if (swapChain)
	{
		swapChain->ResizeBuffers(0, windowInfo->screenWidth, windowInfo->screenHeight, DXGI_FORMAT_UNKNOWN, 0);
	}

	ComPtr<ID3D11Texture2D> renderTarget = nullptr;
	HR_T(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(renderTarget.GetAddressOf())));
	D3D11_TEXTURE2D_DESC desc = {};
	renderTarget->GetDesc(&desc);
	renderTargetBuffer = renderTarget;

	HR_T(D3DDevice->CreateRenderTargetView(renderTargetBuffer.Get(), nullptr, renderTargetView.GetAddressOf()));
	CreateDepthStencilBuffer();

	// 뷰포트 처리
	viewport->Set(windowInfo->screenWidth, windowInfo->screenHeight);
	D3DDeviceContext->RSSetViewports(1, &viewport->Get());
}

void D3DClass::CreateSamplerState(D3D11_FILTER _filter, D3D11_TEXTURE_ADDRESS_MODE _addressMode, ComPtr<ID3D11SamplerState>& _sampler)
{
	D3D11_SAMPLER_DESC sampDesc = {};
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = _filter;            // 선형 보간 방식
	sampDesc.AddressU = _addressMode;
	sampDesc.AddressV = _addressMode;
	sampDesc.AddressW = _addressMode;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR_T(D3DDevice.Get()->CreateSamplerState(&sampDesc, _sampler.GetAddressOf()));
}

std::pair<int, int> D3DClass::GetWindowsSize()
{
	return std::pair<int, int>(windowInfo->screenWidth, windowInfo->screenHeight);
}

void D3DClass::InitD3D()
{
	HRESULT hr = 0;
	DXGI_SWAP_CHAIN_DESC swapDesc = CreateSwapDesc();

	// 디버그 기능 활성화
	UINT creationFlags;

#ifdef _DEBUG
	// 디버그 모드에서는 디버그 플래그를 추가
	creationFlags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#else
	// 릴리즈 모드에서는 디버그 플래그를 제외
	creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	
	// 1. 장치 생성.   2. 스왑체인 생성.  3. 장치 컨텍스트 생성.
	HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
		D3D11_SDK_VERSION, &swapDesc, swapChain.GetAddressOf(), D3DDevice.GetAddressOf(), &featureLevel, D3DDeviceContext.GetAddressOf()));

	HR_T(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&renderTargetBuffer));
	// D2D에서 사용할 IDXGISurface 생성 //
	HR_T(swapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(DXGISurface.GetAddressOf())));
	HR_T(D3DDevice->CreateRenderTargetView(renderTargetBuffer.Get(), nullptr, renderTargetView.GetAddressOf()));

	// 뷰포트 설정.	
	viewport = std::make_unique<Viewport>(0, 0, windowInfo->screenWidth, windowInfo->screenHeight, 0.0f, 1.0f);
	D3DDeviceContext->RSSetViewports(1, &viewport->Get());  // RS

	// 레스터 라이저 상태 설정
	{
		ComPtr<ID3D11RasterizerState> resterizerState;
		D3D11_RASTERIZER_DESC rasterDesc = {};
		ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		//rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
		//rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.FrontCounterClockwise = FALSE;
		rasterDesc.DepthClipEnable = TRUE;
		D3DDevice->CreateRasterizerState(&rasterDesc, &resterizerState);
		D3DDeviceContext->RSSetState(resterizerState.Get());
	}

	CreateDepthStencilBuffer();
}

void D3DClass::InitDXGI()
{
	HR_T(D3DDevice.As(&DXGIDevice));
	HR_T(DXGIDevice->GetAdapter(DXGIAdapter.GetAddressOf()));
	HR_T(DXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(DXGIFactory.GetAddressOf())));
	HR_T(DXGIFactory->MakeWindowAssociation(windowInfo->hWnd, DXGI_MWA_NO_ALT_ENTER)); // 해당 플로그는 Alt + Enter 전환할수 없음
}

void D3DClass::MemoryLick()
{
#ifdef _DEBUG
	HMODULE dxgiDebugDll = GetModuleHandleW(L"dxgidebug.dll");
	if (dxgiDebugDll == nullptr)
	{
		OutputDebugStringW(L"Failed to load dxgidebug.dll\n");
		return;
	}

	decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgiDebugDll, "DXGIGetDebugInterface"));

	IDXGIDebug* debug = nullptr;
	GetDebugInterface(IID_PPV_ARGS(&debug));

	OutputDebugStringW(L"----------Starting Live Direct3D Object Dump----------\r\n");
	debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY); // https://learn.microsoft.com/ko-kr/windows/win32/api/dxgidebug/ne-dxgidebug-dxgi_debug_rlo_flags
	OutputDebugStringW(L"----------Completed Live Direct3D Object Dump----------\r\n");
	debug->Release();
#endif
}

DXGI_SWAP_CHAIN_DESC D3DClass::CreateSwapDesc()
{
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 2;  // imgui 때문에 수정 1.14 1로 수정해야 될수도 있음
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// 기본값 0  https://learn.microsoft.com/ko-kr/windows/win32/api/dxgi/ne-dxgi-dxgi_swap_effect
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 더블 버퍼링 및 3중 버퍼링도 있다
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //   https://learn.microsoft.com/ko-kr/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format
	// 백버퍼(텍스처)의 가로/세로 크기 설정.
	swapDesc.OutputWindow = windowInfo->hWnd;	// 스왑체인 출력할 창 핸들 값.
	swapDesc.Windowed = windowInfo->windoweMode;		// 창 모드 여부 설정.
	swapDesc.BufferDesc.Width = windowInfo->screenWidth;
	swapDesc.BufferDesc.Height = windowInfo->screenHeight;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // 창모드, 전체모드 전환을 허용할 것인가
	// 화면 주사율 설정.
	swapDesc.BufferDesc.RefreshRate.Numerator = 60; // 최대 프레임 갯수 144 모니터가 좋아서 가능
	swapDesc.BufferDesc.RefreshRate.Denominator = 1; // 갱순 주기, 프레임의 분자
	// 화면 출력 방식
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // 지정되지 않은 스케일링
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //기본값인 위에서부터 한줄씩 차례로 출력하는 방식을 사용하겠다는 것
	// 샘플링 관련 설정. (안티앨리어싱과 관련된 부분)
	swapDesc.SampleDesc.Count = 1; // 샘플 갯수 샘플링을 몇번 할 것인지?
	swapDesc.SampleDesc.Quality = 0; // 퀄리티 수준, 0을 주면 안티앨리어싱을 사용하지 않겠다는 의미 
	
	return swapDesc;
}

void D3DClass::CreateDepthStencilBuffer()
{
	//6. 뎊스&스텐실 뷰 생성 (깊이 버퍼 생성)
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilDesc.Width = windowInfo->screenWidth;
	depthStencilDesc.Height = windowInfo->screenHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HR_T(D3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.GetAddressOf()));

	// 뷰 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	descDSV.Format = depthStencilDesc.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// 깊이 스텐실 뷰 생성
	HR_T(D3DDevice->CreateDepthStencilView(depthStencilBuffer.Get(), &descDSV, depthStencilView.GetAddressOf()));
	// 렌더 타켓 설정
//	D3DDeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
}

void D3DClass::ExtractFinalImage()
{
	if (nullptr == SRV)
	{
		// 1. 먼저 renderTargetBuffer에서 데이터를 읽을 수 있도록 준비합니다.
		D3D11_TEXTURE2D_DESC desc;
		renderTargetBuffer->GetDesc(&desc);

		// 2. D3D11_USAGE_STAGING 텍스처로 복사할 텍스처 생성
		D3D11_TEXTURE2D_DESC stagingDesc = desc;
		stagingDesc.Usage = D3D11_USAGE_DEFAULT; // D3D11_USAGE_STAGING
		stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // CPU에서 읽을 수 있게 설정
		stagingDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		stagingDesc.MiscFlags = 0;
		// stagingDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		HR_T(D3DDevice->CreateTexture2D(&stagingDesc, nullptr, stagingTexture.GetAddressOf()));

		// 3. 텍스처 복사 (GPU에서 CPU로 복사)
		D3DDeviceContext->CopyResource(stagingTexture.Get(), renderTargetBuffer.Get());

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		HR_T(D3DDevice->CreateShaderResourceView(stagingTexture.Get(), &srvDesc, SRV.GetAddressOf()));
	}
	else
	{
		D3DDeviceContext->CopyResource(stagingTexture.Get(), renderTargetBuffer.Get());
	}
}
