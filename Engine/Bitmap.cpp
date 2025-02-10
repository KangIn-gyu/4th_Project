#include "pch.h"
#include "Bitmap.h"
#include  "Helper.h"

#include "D2DClass.h"
#include "D2DLoader.h"



void Bitmap::Load(std::string_view _filePath)
{
   d2dBitmap = D2DLOADER->Load(StringConverter::StringToWide(_filePath));
   width = d2dBitmap->GetSize().width;
   height = d2dBitmap->GetSize().height;
   destRect = { xPos, yPos,width + xPos, height + yPos };
}

void Bitmap::SetSize(float _width, float _height)
{
    //destRect.right = width;
    //destRect.bottom = height;

    width = _width;
    height = _height;
    destRect = { xPos, yPos,width + xPos, height + yPos };
}

void Bitmap::SetPos(float _x, float _y)
{

    xPos = _x;
    yPos = _y;
    destRect = { xPos, yPos, width + xPos,height + yPos };
}
void Bitmap::SetBoundBox(D2D1_RECT_F _box)
{
    bitmapBoundbox = _box;
}