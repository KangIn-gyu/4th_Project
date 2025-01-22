#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include "Viewport.h"

struct WindowInfo;
class D3DClass // 그래픽스 클래스이다.
{
public:
	D3DClass() = default;
	~D3DClass();

	void Initialize(WindowInfo* _windowInfo);
	void BeginDraw(DXMath::Color _BackgroundColor); // 렌더링 파이프라인에서 렌더링 작업을 시작하는 단계
	void EndDraw();

	void ChangeWindowSize();
	void MemoryLick();

	// (캡슐화 포기) 이건 그냥 주자 구조만들기 너무 힘듬
	static ComPtr<ID3D11Device> GetD3DDevice() { return D3DDevice; }
	static ComPtr<ID3D11DeviceContext> GetD3DDeviceContext() { return D3DDeviceContext; }
	static ComPtr<ID2D1SolidColorBrush> GetD2DBrush() { return Brush; }
	static ComPtr<ID2D1DeviceContext> GetD2DDeviceContext() { return D2DDeviceContext; }
	static std::unique_ptr<Viewport>& GetViewport() { return viewport; }

	void CreateSamplerState(D3D11_FILTER _filter, D3D11_TEXTURE_ADDRESS_MODE _addressMode, ComPtr<ID3D11SamplerState> _sampler);
	std::pair<int, int> GetWindowsSize();
	void CreateD2DRenderTarget();

	void ExtractFinalImage(); //  프론트 버퍼 렌더링
	ComPtr<ID3D11ShaderResourceView> GetImGuiImageTexture() { return SRV; }
private:
	void InitD3D();
	void InitDXGI();
	void InitD2D();

	DXGI_SWAP_CHAIN_DESC CreateSwapDesc();
	void CreateDepthStencilBuffer();

public:

private:
	static ComPtr<ID3D11Device>				D3DDevice;           // 디바이스	                      
	static ComPtr<ID3D11DeviceContext>		D3DDeviceContext;    // 즉시 디바이스 컨텍스트
	static std::unique_ptr<Viewport>		viewport;
	static ComPtr <ID2D1SolidColorBrush>	Brush;
	static ComPtr<ID2D1DeviceContext>		D2DDeviceContext;

	ComPtr<IDXGISwapChain>		       swapChain;           // 스왑체인

	ComPtr<ID3D11RenderTargetView>	   renderTargetView;    // 렌더링 타겟뷰
	ComPtr<ID3D11Texture2D>			   renderTargetBuffer;
	ComPtr<ID3D11DepthStencilView>	   depthStencilView;    // 깊이값 처리를 위한 뎊스스텐실 뷰 
	ComPtr<ID3D11Texture2D>			   depthStencilBuffer;  // 뎊스스텐실 버퍼

	ComPtr<ID3D11BlendState>		   alphaBlendState;

	WindowInfo* windowInfo;

	bool presentEnabled;

	ComPtr<IDXGIFactory> DXGIFactory;
	ComPtr<IDXGIAdapter> DXGIAdapter;
	ComPtr<IDXGIDevice>  DXGIDevice;
	ComPtr<IDXGISurface>  DXGISurface;	// Direct3D와 Direct2D 간의 데이터 교환을 가능하게 해줌

	ComPtr<ID3D11Texture2D> stagingTexture; // ImGui에 넘길 텍스쳐
	ComPtr<ID3D11ShaderResourceView> SRV;

	ComPtr<ID2D1Device>			D2DDevice = nullptr;
	ComPtr<ID2D1Factory1>		D2DFactory1 = nullptr;
	ComPtr<ID2D1Bitmap1>		D2DBitmap1 = nullptr;
};

// 항상 그래픽 파이프 라인을 생각하자
// IA[Input_Assember] -> VS[Vertex Shader] -> (생략가능)HS[HullShader]/TS[Tessellator]/DS[Domain Sader]/ 
// GS[Geometry Sader]-> RS[Rasterizer] -> PS[Pixel Shader] -> OM[Output-Merger]