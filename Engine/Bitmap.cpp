#include "pch.h"
#include <wincodec.h>
#include "Bitmap.h"
#include  "Helper.h"
#include "D2DClass.h"

Bitmap::Bitmap()
{

}

Bitmap::~Bitmap()
{
}

void Bitmap::Update()
{
}

void Bitmap::Load(std::string_view _filePath)
{
    CreateBitmapFromFile(StringConverter::StringToWide(_filePath).c_str());
}
void Bitmap::CreateBitmapFromFile(const WCHAR* _filePath)
{
    if (!wicFactory)
    {
        HR_T(CoInitialize(nullptr)); // COM 초기화
        HR_T(CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(wicFactory.GetAddressOf())
        ));
    }

    // WIC 비트맵 디코더 생성
    ComPtr<IWICBitmapDecoder> decoder;
    HR_T(wicFactory->CreateDecoderFromFilename(
        _filePath,
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnDemand,
        &decoder
    ));

    // 첫 번째 프레임 가져오기
    ComPtr<IWICBitmapFrameDecode> frame;
    HR_T(decoder->GetFrame(0, &frame));

    // WIC 포맷 변환기 생성
    ComPtr<IWICFormatConverter> converter;
    HR_T(wicFactory->CreateFormatConverter(&converter));

    HR_T(converter->Initialize(
        frame.Get(),
        GUID_WICPixelFormat32bppPBGRA, // D2D가 지원하는 포맷
        WICBitmapDitherTypeNone,
        nullptr,
        0.0,
        WICBitmapPaletteTypeCustom
    ));

    // WIC → Direct2D 비트맵 변환
    HR_T(D2DClass::GetD2DDeviceContext()->CreateBitmapFromWicBitmap(
        converter.Get(),
        nullptr,
        d2dBitmap.GetAddressOf()
    ));

    width = d2dBitmap->GetSize().width;
    height = d2dBitmap->GetSize().height;
    destRect = { xPos,yPos,width,height };
}

void Bitmap::SetSize(float width, float height)
{
    destRect.right = width;
    destRect.bottom = height;
}

void Bitmap::SetPos(float x, float y)
{
    xPos = CenterX + x;
    yPos = CenterY + y;
}
