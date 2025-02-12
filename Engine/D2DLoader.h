#pragma once
#include "SingletonBase.h"
#include <wincodec.h>  
#include <wrl/client.h> 
#pragma comment(lib, "Windowscodecs.lib")

#define D2DLOADER D2DLoader::GetInstance()
class D2DLoader : public SingletonBase<D2DLoader>
{
	friend class SingletonBase<D2DLoader>;
public:
	ID2D1Bitmap* Load(std::wstring_view _filePath);

private:
	D2DLoader();
	~D2DLoader();

public:

private:
	ComPtr<IWICImagingFactory> wicFactory;
	std::unordered_map<std::wstring ,ID2D1Bitmap*> bitmaps;
	std::wstring basePath = L"Resource/";
};

