#include "pch.h"
#include "FontManager.h"
#include "Helper.h"

FontManager::~FontManager()
{
	//SAFE_RELEASE(FontSetBuilder);
	//SAFE_RELEASE(DWriteFactory);
	for (auto& font : fontMap)
	{
		if (font.second != nullptr) font.second->Release();
	}
}

void FontManager::InitializeDWrite()
{
	HR_T(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory5),
		reinterpret_cast<IUnknown**>(&DWriteFactory)
	));

	HR_T(DWriteFactory->CreateFontSetBuilder(&FontSetBuilder));
}

void FontManager::LoadFont(std::wstring fontFilePath, std::wstring fontName)
{
	if (fontMap.find(fontName) != fontMap.end()) { return; }

	IDWriteFontFile* FontFile{ nullptr };
	IDWriteFontSet* FontSet{ nullptr };
	IDWriteFontCollection1* FontCollection{ nullptr };
	IDWriteFontFamily* FontFamily{ nullptr };
	IDWriteLocalizedStrings* FontFamilyNames{ nullptr };

	// 폰트 파일 참조 생성
	HR_T(DWriteFactory->CreateFontFileReference(
		fontFilePath.c_str(),
		nullptr,
		&FontFile));

	// 폰트 파일을 FontSetBuilder에 추가
	FontSetBuilder->AddFontFile(FontFile);

	BOOL isSupported;
	DWRITE_FONT_FILE_TYPE fileType;
	UINT32 numberOfFonts;

	FontSetBuilder->CreateFontSet(&FontSet); // 폰트 세트를 생성합니다. 이 세트는 추가된 폰트 파일들을 포함합니다.

	// 폰트 세트에서 폰트 컬렉션 생성
	HR_T(DWriteFactory->CreateFontCollectionFromFontSet(FontSet, &FontCollection));

	HR_T(FontCollection->GetFontFamily(index, &FontFamily));

	// 폰트 패밀리 얻기
	HR_T(FontFamily->GetFamilyNames(&FontFamilyNames));  // FontFamilyNames 초기화

	// 폰트 패밀리의 이름을 얻기 위해 로컬라이즈된 문자열 컬렉션을 가져옵니다.
	WCHAR familyName[MAX_PATH];
	HR_T(FontFamilyNames->GetString(0, familyName, MAX_PATH));

#if( _DEBUG)
	wprintf(L"Loaded Font: %s\n", familyName);  // 디버깅 출력
#endif

	IDWriteTextFormat* NewFont = nullptr;
	AddFont(familyName, FontCollection, &NewFont);

	fontMap.insert(std::make_pair(fontName, NewFont));

	FontFile->Release();
	FontSet->Release();
	FontCollection->Release();
	FontFamily->Release();
	index++;
}

void FontManager::AddFont(std::wstring fontName, IDWriteFontCollection1* pFontCollection,
	IDWriteTextFormat** ppTextFormat)
{
	HR_T(DWriteFactory->CreateTextFormat(
		fontName.c_str(),
		pFontCollection,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		10.f,
		L"en-us",
		ppTextFormat
	));
}

IDWriteTextFormat* FontManager::FindFont(const std::wstring& keyName)
{
	auto it = fontMap.find(keyName);

	if (it != fontMap.end())
	{
		return it->second;
	}
	return nullptr;
}
