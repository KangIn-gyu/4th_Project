#include "pch.h"
#include "ImageComponent.h"
#include "Bitmap.h"
#include "ResourceSystem.h"

ImageComponent::ImageComponent(std::string_view _filePath)
{
	imageData = RESOURCESYSTEM->Load<Bitmap>(_filePath);
	float width = imageData->GetImageData().Get()->GetSize().width;
	float height = imageData->GetImageData().Get()->GetSize().height;
	destRect = { 0,0,width,height };
}

ImageComponent::~ImageComponent()
{
}

void ImageComponent::ComponentInitialize()
{
}

void ImageComponent::ComponentUpdate(const float _deltaTime)
{
}
void ImageComponent::SetSize(float width, float height)
{
	destRect.right += width;
	destRect.bottom += height;
}
void ImageComponent::SetPos(float x, float y)
{
	destRect.left += x;
	destRect.top += y;
}