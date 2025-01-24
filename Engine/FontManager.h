#pragma once
#include "SingletonBase.h"
#include <dwrite_3.h>

class FontManager:public SingletonBase<FontManager>
{
public:
	virtual ~FontManager();
	void InitializeDWrite();
	void LoadFont(std::wstring fontFilePath, std::wstring fontName);
	void AddFont(std::wstring fontName, IDWriteFontCollection1* pFontCollection, IDWriteTextFormat** ppTextFormat);

	IDWriteTextFormat* FindFont(const std::wstring& keyName);

private:
	int index = 0; // ÆùÆ® °¹¼ö

	IDWriteFactory5* DWriteFactory;
	IDWriteFontSetBuilder1* FontSetBuilder;

	std::unordered_map<std::wstring, IDWriteTextFormat*> fontMap;
};

