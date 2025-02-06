#include "pch.h"
#include "D2DLoader.h"
#include "Helper.h"
#include "D2DClass.h"
#include <wincodec.h>

ID2D1Bitmap* D2DLoader::Load(std::wstring_view _filePath)
{
    std::wstring filePath = basePath + _filePath.data();
    if (bitmaps.find(std::wstring(filePath)) != bitmaps.end())
    {
        return bitmaps.find(std::wstring(filePath))->second;
    }

    ComPtr<IWICBitmapDecoder> decoder;
    HR_T(wicFactory->CreateDecoderFromFilename(
        filePath.c_str(),
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnDemand,
        &decoder
    ));

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
    ID2D1Bitmap* newBitmap = nullptr;
    HR_T(D2DClass::GetD2DDeviceContext()->CreateBitmapFromWicBitmap(
        converter.Get(),
        nullptr,
        &newBitmap
    ));

    bitmaps[std::wstring(_filePath)] = newBitmap;

    return newBitmap;
}

D2DLoader::D2DLoader()
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
}

D2DLoader::~D2DLoader()
{
   for (auto& pair : bitmaps)
   {
       if (pair.second)
       {
           pair.second->Release();
       }
   }
   bitmaps.clear();
}
