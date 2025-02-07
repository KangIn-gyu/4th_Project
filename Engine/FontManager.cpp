#include "pch.h"
#include "FontManager.h"
#include "Helper.h"
#include "D2DClass.h"
#include "D2DFont.h"

std::string GetFileName(std::string_view _filePath)
{
    std::filesystem::path filePath(_filePath);
    return filePath.filename().string();  // 파일명 + 확장자 포함한 문자열
}

FontManager::~FontManager()
{
    for (auto& data : fontMap)
    {
        data.second->Release();
    }

    fontSetBuilder->Release();
}

void FontManager::Initialize()
{
    D2DClass::GetDWriteFactory()->CreateFontSetBuilder(&fontSetBuilder);
}

D2DFont* FontManager::LoadFont(std::string_view _fontFilePath)
{
    std::string filePath = basePath + _fontFilePath.data();
    std::string fileName = GetFileName(_fontFilePath);
    LoadTextFormat(filePath, fileName);

    D2DFont* newFont = new D2DFont();

    auto* format = fontMap.find(filePath)->second;
    if (nullptr != format)
    {
        newFont->SetTextFormat(format);
    }

    return newFont;
}

void FontManager::LoadTextFormat(std::string_view _fontFilePath, std::string_view fontName)
{
    if (fontMap.find(_fontFilePath.data()) != fontMap.end())
    {
        return;
    }

    IDWriteFontFile* FontFile{ nullptr };
    IDWriteFontSet* FontSet{ nullptr };
    IDWriteFontCollection1* FontCollection{ nullptr };
    IDWriteFontFamily* FontFamily{ nullptr };
    IDWriteLocalizedStrings* FontFamilyNames{ nullptr };

    HRESULT hresult = D2DClass::GetDWriteFactory()->CreateFontFileReference(
        StringConverter::StringToWide(_fontFilePath).c_str(),
        nullptr,
        &FontFile
    );

    if (FAILED(hresult)) 
    {
        MessageBoxW(nullptr, L"Failed to create font file reference.", L"Error", MB_OK);
        return;
    }

    fontSetBuilder->AddFontFile(FontFile);

    BOOL isSupported;
    DWRITE_FONT_FILE_TYPE fileType;
    UINT32 numberOfFonts;

    fontSetBuilder->CreateFontSet(&FontSet); // 폰트 세트를 생성합니다. 이 세트는 추가된 폰트 파일들을 포함합니다.

    // 폰트 세트에서 폰트 컬렉션 생성
    hresult = D2DClass::GetDWriteFactory()->CreateFontCollectionFromFontSet(FontSet, &FontCollection);

    if (FAILED(hresult))
    {
        MessageBoxW(nullptr, L"Failed to create font collection from font set.", L"Error", MB_OK);
        return;
    }

    hresult = FontCollection->GetFontFamily(index, &FontFamily);
    if (FAILED(hresult))
    {
        MessageBoxW(nullptr, L"Failed to get font family.", L"Error", MB_OK);
        return;
    }

    // 폰트 패밀리 얻기
    hresult = FontFamily->GetFamilyNames(&FontFamilyNames);  // FontFamilyNames 초기화
    if (FAILED(hresult))
    {
        MessageBoxW(nullptr, L"Failed to get font family names.", L"Error", MB_OK);
        return;
    }
    
    WCHAR familyName[MAX_PATH];
    hresult = FontFamilyNames->GetString(0, familyName, MAX_PATH);
    if (FAILED(hresult))
    {
        MessageBoxW(nullptr, L"Failed to get string.", L"Error", MB_OK);
        return;
    }

#if( _DEBUG)
    wprintf(L"Loaded Font: %s\n", familyName);  // 디버깅 출력
#endif

    IDWriteTextFormat* NewFont = nullptr;
    AddFont(familyName, FontCollection, &NewFont);

    fontMap.insert(std::make_pair(_fontFilePath.data(), NewFont));

    FontFile->Release();
    FontSet->Release();
    FontCollection->Release();
    FontFamily->Release();
    index++;
}

void FontManager::AddFont(const std::wstring& _fontName, IDWriteFontCollection1* _pFontCollection, IDWriteTextFormat** _ppTextFormat)
{
    HRESULT hresult = D2DClass::GetDWriteFactory()->CreateTextFormat(
        _fontName.c_str(),
        _pFontCollection,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        10.0f,
        L"en-us",
        _ppTextFormat
    );

    if (FAILED(hresult)) 
    {
        MessageBoxW(nullptr, L"Failed to create text format.", L"Error", MB_OK);
    }
}
