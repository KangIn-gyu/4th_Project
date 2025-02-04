#pragma once
#include "IResources.h"
class Bitmap : public IResources
{
public:
	Bitmap();
	virtual ~Bitmap();

	void Initialize(){};
	void Update();


	virtual void Load(std::string_view _filePath) override;
	void CreateBitmapFromFile(const WCHAR* _filePath);
	ComPtr<ID2D1Bitmap> GetImageData() { return d2dBitmap; }

	D2D1_RECT_F GetRect() { return  destRect; }

	void SetSize(float width, float height);
	void SetPos(float x, float y);


private:
	ComPtr<IWICImagingFactory> wicFactory;
	ComPtr<ID2D1Bitmap> d2dBitmap;

	D2D1_RECT_F destRect;
	float xPos = 0, yPos = 0, width, height;

	float CenterX = (destRect.right - destRect.left) / 2;
	float CenterY = (destRect.bottom - destRect.top) / 2;
};
