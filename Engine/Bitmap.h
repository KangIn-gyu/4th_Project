#pragma once
#include "IResources.h"
class Bitmap 
{
public:
	Bitmap() = default;
	~Bitmap() =default;

	void Initialize(){};
	void Update() {};

	void Load(std::string_view _filePath);
	ID2D1Bitmap* GetImageData() { return d2dBitmap; }
	D2D1_RECT_F GetRect() { return  destRect; }

	void SetSize(float width, float height);
	void SetPos(float x, float y);

private:
	ID2D1Bitmap* d2dBitmap; // 한번에 여러개 넣어 주기 위해서            

	float xPos = 0, yPos = 0, width{}, height;
	D2D1_RECT_F destRect = { xPos,yPos,width+ xPos,height+ yPos };

	float CenterX = (destRect.right - destRect.left) / 2;
	float CenterY = (destRect.bottom - destRect.top) / 2;
};
