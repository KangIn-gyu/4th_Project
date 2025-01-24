#pragma once
struct WindowInfo;
class D2DClass
{
public:
	D2DClass() = default;
	~D2DClass();

	void Initialize(WindowInfo* _windowInfo);
	void BeginDraw(); // 렌더링 파이프라인에서 렌더링 작업을 시작하는 단계
	void EndDraw();

	static ComPtr<ID2D1SolidColorBrush> GetD2DBrush() { return Brush; }
	static ComPtr<ID2D1DeviceContext> GetD2DDeviceContext() { return D2DDeviceContext; }

private:
	void InitD2D();
	void CreateD2DRenderTarget();

public:

private:
	static ComPtr <ID2D1SolidColorBrush>	Brush;
	static ComPtr<ID2D1DeviceContext>		D2DDeviceContext;

	WindowInfo*					windowInfo;

	ComPtr<IDXGIDevice>			DXGIDevice;

	ComPtr<ID2D1Device>			D2DDevice;
	ComPtr<ID2D1Factory1>		D2DFactory1;
	ComPtr<ID2D1Bitmap1>		D2DBitmap1;
};

	// 항상 그래픽 파이프 라인을 생각하자
	// IA[Input_Assember] -> VS[Vertex Shader] -> (생략가능)HS[HullShader]/TS[Tessellator]/DS[Domain Sader]/ 
	// GS[Geometry Sader]-> RS[Rasterizer] -> PS[Pixel Shader] -> OM[Output-Merger]