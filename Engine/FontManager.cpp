#include "pch.h"
#include "FontManager.h"

#include "D2DClass.h"
#include "Helper.h"

FontManager::~FontManager()
{
    for (auto& font : fontMap) {
        font.second->Release();
    }
    fontMap.clear();
}

void FontManager::LoadFont(const std::wstring& fontFilePath, const std::wstring& fontName)
{
    if (fontMap.find(fontName) != fontMap.end()) {
        OutputDebugString(L"Font already loaded. Skipping.\n");
        return;
    }

    IDWriteFontFile* FontFile = nullptr;
    HRESULT hresult = D2DClass::GetDWriteFactory()->CreateFontFileReference(
        fontFilePath.c_str(),
        nullptr,
        &FontFile
    );
    if (FAILED(hresult)) {
        MessageBoxW(nullptr, L"Failed to create font file reference.", L"Error", MB_OK);
        return;
    }

    hresult = D2DClass::GetDWriteFactory()->CreateFontSetBuilder(&FontSetBuilder);
    if (FAILED(hresult))
    {
        MessageBoxW(nullptr, L"Failed to create font set builder.", L"Error", MB_OK);
        return;
    }
    FontSetBuilder->AddFontFile(FontFile);

    IDWriteFontSet* FontSet = nullptr;
    hresult = FontSetBuilder->CreateFontSet(&FontSet);
    if (FAILED(hresult)) {
        MessageBoxW(nullptr, L"Failed to create font set.", L"Error", MB_OK);
        FontFile->Release();
        return;
    }

    IDWriteFontCollection1* FontCollection = nullptr;
    hresult = D2DClass::GetDWriteFactory()->CreateFontCollectionFromFontSet(FontSet, &FontCollection);
    if (FAILED(hresult)) {
        MessageBoxW(nullptr, L"Failed to create font collection from font set.", L"Error", MB_OK);
        FontSet->Release();
        FontFile->Release();
        return;
    }

    IDWriteTextFormat* NewFont = nullptr;
    AddFont(fontName, FontCollection, &NewFont);
    if (NewFont) {
        fontMap[fontName] = NewFont;
    }

    FontSet->Release();
    FontFile->Release();
    FontCollection->Release();
}

void FontManager::AddFont(const std::wstring& fontName, IDWriteFontCollection1* pFontCollection, IDWriteTextFormat** ppTextFormat)
{
    HRESULT hresult = D2DClass::GetDWriteFactory()->CreateTextFormat(
        fontName.c_str(),
        pFontCollection,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        10.0f,
        L"en-us",
        ppTextFormat
    );

    if (FAILED(hresult)) {
        MessageBoxW(nullptr, L"Failed to create text format.", L"Error", MB_OK);
    }
}

IDWriteTextFormat* FontManager::FindFont(const std::wstring& keyName)
{
    auto it = fontMap.find(keyName);
    return (it != fontMap.end()) ? it->second : nullptr;
}
