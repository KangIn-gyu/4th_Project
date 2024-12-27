#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <directxtk/SimpleMath.h>

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

	void ChangeWindowSize(WindowInfo* _windowInfo);

	// (캡슐화 포기) 이건 그냥 주자 구조만들기 너무 힘듬
	static const ComPtr<ID3D11Device> GetD3DDevice() { return D3DDevice; }
	static const ComPtr<ID3D11DeviceContext> GetD3DDeviceContext() { return D3DDeviceContext; }

	std::unique_ptr<Viewport>& GetVieport() { return viewport; }

	void CreateSamplerState(D3D11_FILTER _filter, D3D11_TEXTURE_ADDRESS_MODE _addressMode, ComPtr<ID3D11SamplerState>& _sampler);

private:
	void InitD3D(WindowInfo* windowInfo);
	void InitDXGI(WindowInfo* windowInfo);


	DXGI_SWAP_CHAIN_DESC CreateSwapDesc(WindowInfo* windowInfo);
	void CreateDepthStencilBuffer(WindowInfo* windowInfo);

public:

private:
	static ComPtr<ID3D11Device>        D3DDevice;              // 디바이스	                      
	static ComPtr<ID3D11DeviceContext> D3DDeviceContext;       // 즉시 디바이스 컨텍스트
	ComPtr<IDXGISwapChain>		       swapChain {};           // 스왑체인

	ComPtr<ID3D11RenderTargetView>	   renderTargetView {};    // 렌더링 타겟뷰
	ComPtr<ID3D11DepthStencilView>	   depthStencilView{};     // 깊이값 처리를 위한 뎊스스텐실 뷰 
	ComPtr<ID3D11Texture2D>			   depthStencilBuffer {};  // 뎊스스텐실 버퍼
									   
	ComPtr<ID3D11BlendState>		   alphaBlendState{};

	std::unique_ptr<Viewport> viewport;
	bool presentEnabled;

	ComPtr<IDXGIFactory> DXGIFactory;
	ComPtr<IDXGIAdapter> DXGIAdapter;
	ComPtr<IDXGIDevice>  DXGIDevice;
};

// 항상 그래픽 파이프 라인을 생각하자
// IA[Input_Assember] -> VS[Vertex Shader] -> (생략가능)HS[HullShader]/TS[Tessellator]/DS[Domain Sader]/ 
// GS[Geometry Sader]-> RS[Rasterizer] -> PS[Pixel Shader] -> OM[Output-Merger]