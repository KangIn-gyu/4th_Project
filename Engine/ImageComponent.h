#pragma once
#include "Component.h"

class Bitmap;

class ImageComponent: public Component
{
public:

	ImageComponent(std::string_view _filePath);
	virtual ~ImageComponent();
	virtual void ComponentInitialize() override;                       // 초기화용
	virtual void ComponentUpdate(const float _deltaTime) override;	   // 업데이트

	void SetSize(float width, float height);
	void SetPos(float x, float y);

	std::shared_ptr<Bitmap> GetImage() { return imageData; }
	D2D1_RECT_F GetDestRect() { return destRect; }

private:
	std::shared_ptr<Bitmap> imageData;
	D2D1_RECT_F destRect;
};


