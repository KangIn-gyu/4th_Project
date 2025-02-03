#pragma once
#include "IResources.h"
class Bitmap : public IResources
{
public:
	Bitmap();
	virtual ~Bitmap();

	void Initialize(){};

	virtual void Load(std::string_view _filePath) override;
	void CreateBitmapFromFile(const WCHAR* _filePath);
	ComPtr<ID2D1Bitmap> GetImageData() { return d2dBitmap; }
private:
	ComPtr<IWICImagingFactory> wicFactory;
	ComPtr<ID2D1Bitmap> d2dBitmap;
};
