#include "pch.h"
#include "D3DClass.h"
#include "Helper.h"
#include "Viewport.h"
#include "Declare.h"

// 정적변수랑은 스태틱은 의미가 달라서 g_표시 안함
ComPtr<ID3D11Device>        D3DClass::D3DDevice =        nullptr; 
ComPtr<ID3D11DeviceContext> D3DClass::D3DDeviceContext = nullptr;

D3DClass::D3DClass()
{
}

D3DClass::~D3DClass()
{
	swapChain->SetFullscreenState(FALSE, nullptr);
}

void D3DClass::Initialize(WindowInfo* windowInfo)
{
	InitD3D(windowInfo);
	InitDXGI(windowInfo);
}

void D3DClass::BeginDraw(DXMath::Color _BackgroundColor)
{
	D3DDeviceContext->ClearRenderTargetView(renderTargetView.Get(), _BackgroundColor);
	D3DDeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DClass::EndDraw()
{
	if (true == presentEnabled)
	{
		swapChain->Present(1, 0);   // 화면 새로 고침 비율을 고정합니다.
	}
	else
	{
		swapChain->Present(0, 0);  // 가능한 빠르게 출력합니다
	}
}

void D3DClass::ChangeWindowSize(WindowInfo* windowInfo)
{
	if (swapChain)
	{
		swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	}

	if (depthStencilView)
	{
		depthStencilView.Reset();
	}

	DXGI_SWAP_CHAIN_DESC swapDesc = CreateSwapDesc(windowInfo);
	HR_T(DXGIFactory->CreateSwapChain(D3DDevice.Get(), &swapDesc, swapChain.GetAddressOf()));

	CreateDepthStencilBuffer(windowInfo);
}

void D3DClass::InitD3D(WindowInfo* windowInfo)
{
	HRESULT hr = 0;
	DXGI_SWAP_CHAIN_DESC swapDesc = CreateSwapDesc(windowInfo);

	// 디버그 기능 활성화
	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 1. 장치 생성.   2. 스왑체인 생성.  3. 장치 컨텍스트 생성.
	HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
		D3D11_SDK_VERSION, &swapDesc, &swapChain, &D3DDevice, NULL, &D3DDeviceContext));

	ID3D11Texture2D* BackBufferTexture = nullptr;
	HR_T(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBufferTexture));
	HR_T(D3DDevice->CreateRenderTargetView(BackBufferTexture, nullptr, &renderTargetView));
	BackBufferTexture->Release(); // 외부 참조 카운트를 감소시킨다.

	// 뷰포트 설정.	
	viewport = std::make_unique<Viewport>(0, 0, windowInfo->screenWidth, windowInfo->screenHeight, 0.0f, 1.0f);
	D3DDeviceContext->RSSetViewports(1, &viewport->Get());  // RS


	// 렌더 타겟을 최종 출력 파이프라인에 바인딩합니다.
	D3DDeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), NULL); // OM

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

	CreateDepthStencilBuffer(windowInfo);
}

void D3DClass::InitDXGI(WindowInfo* windowInfo)
{
	HR_T(D3DDevice.As(&DXGIDevice));
	HR_T(DXGIDevice->GetAdapter(&DXGIAdapter));
	HR_T(DXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(DXGIFactory.GetAddressOf())));
	HR_T(DXGIFactory->MakeWindowAssociation(windowInfo->hWnd, DXGI_MWA_NO_ALT_ENTER)); // 해당 플로그는 Alt + Enter 전환할수 없음
}

DXGI_SWAP_CHAIN_DESC D3DClass::CreateSwapDesc(WindowInfo* windowInfo)
{
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// 기본값 0  https://learn.microsoft.com/ko-kr/windows/win32/api/dxgi/ne-dxgi-dxgi_swap_effect
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

void D3DClass::CreateDepthStencilBuffer(WindowInfo* windowInfo)
{
	//6. 뎊스&스텐실 뷰 생성 (깊이 버퍼 생성)
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	depthStencilDesc.Width = windowInfo->screenWidth;
	depthStencilDesc.Height = windowInfo->screenHeight;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	HR_T(D3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer));

	// 뷰 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	descDSV.Format = depthStencilDesc.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	HR_T(D3DDevice->CreateDepthStencilView(depthStencilBuffer.Get(), &descDSV, depthStencilView.GetAddressOf()));
	D3DDeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
}
