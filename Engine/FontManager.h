#pragma once
#include <dwrite_3.h>
#include "SingletonBase.h"

#define FONTMANAGER FontManager::GetInstance()
class D2DFont;
class FontManager : public SingletonBase<FontManager>
{
	friend class SingletonBase<FontManager>;
public:
	void Initialize();
	D2DFont* LoadFont(std::string_view _fontFilePath);

private:
	FontManager() = default;
	~FontManager();

	void LoadTextFormat(std::string_view _fontFilePath, std::string_view fontName);
	void AddFont(const std::wstring& _fontName, IDWriteFontCollection1* _pFontCollection, IDWriteTextFormat** _ppTextFormat);

public:
	
private:
	int index = 0; // ÆùÆ® °¹¼ö
	IDWriteFontSetBuilder1* fontSetBuilder;
	std::unordered_map<std::string, IDWriteTextFormat*> fontMap;
	std::string basePath = "Resource/";
};

