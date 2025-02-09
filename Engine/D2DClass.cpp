#include "pch.h"
#include "D2DClass.h"
#include "D3DClass.h"
#include "Helper.h"
#include "Declare.h"
#include "FontManager.h"

ComPtr<ID2D1DeviceContext> D2DClass::D2DDeviceContext = nullptr;
ComPtr<ID2D1SolidColorBrush> D2DClass::Brush = nullptr;
ComPtr<ID2D1Device> D2DClass::D2DDevice = nullptr;
ComPtr<IDWriteFactory5> D2DClass::DWriteFactory = nullptr;
ComPtr<ID2D1Bitmap1> D2DClass::D2DBitmap1 = nullptr;

D2DClass::~D2DClass()
{
}

void D2DClass::Initialize(WindowInfo* _windowInfo)
{
	windowInfo = _windowInfo;
	InitD2D();
	CreateD2DRenderTarget();
	FONTMANAGER->Initialize();
}

void D2DClass::BeginDraw()
{
	D2DDeviceContext->BeginDraw();
}

void D2DClass::EndDraw()
{
//	FontManager::Uninitialize();
	D2DDeviceContext->EndDraw();
}

void D2DClass::InitD2D()
{
	HR_T(D3DClass::GetD3DDevice()->QueryInterface(__uuidof(IDXGIDevice), (void**)DXGIDevice.GetAddressOf()));
	// Direct2D 팩토리 생성
	HR_T(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, D2DFactory1.GetAddressOf()));

	// DXGI 디바이스를 사용하여 Direct2D 디바이스를 생성
	HR_T(D2DFactory1.Get()->CreateDevice(DXGIDevice.Get(), D2DDevice.GetAddressOf()));

	// ID2D1DeviceContext 생성
	HR_T(D2DDevice.Get()->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, D2DDeviceContext.GetAddressOf()));

	// brush 생성
	HR_T(D2DDeviceContext.Get()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), Brush.GetAddressOf()));

	HR_T(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory5),
		reinterpret_cast<IUnknown**>(DWriteFactory.GetAddressOf())));
}

void D2DClass::CreateD2DRenderTarget()
{
	// 현재 창의 DPI(1인치당 픽셀의 개수) 설정 가져오기
	float dpiX, dpiY;
	UINT dpi = GetDpiForWindow(windowInfo->hWnd);
	dpiX = static_cast<float>(dpi);
	dpiY = static_cast<float>(dpi);
	// DXGI 표면 가져오기

	// Direct2D 비트맵 속성 정의
	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		dpiX,
		dpiY
	);	// DXGI_FORMAT_UNKNOWN을 사용하면 Direct2D가 적합한 포맷을 자동으로 선택함.

	// DXGI 표면을 기반으로 Direct2D 비트맵 생성
	HR_T(D2DDeviceContext->CreateBitmapFromDxgiSurface(
		D3DClass::GetIDXGISurface().Get(),
		&bitmapProperties,
		D2DBitmap1.GetAddressOf()
	));

	// 비트맵을 DeviceContext의 렌더 타겟으로 설정
	D2DDeviceContext->SetTarget(D2DBitmap1.Get());
}

void D2DClass::Release()
{
	Brush->Release();
	D2DDeviceContext->Release();
	D2DDevice->Release();
	D2DFactory1->Release();
	D2DBitmap1->Release();
	DWriteFactory->Release();
}
