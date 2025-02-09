#include "pch.h"
#include "D2DBaseObj.h"
#include "../Engine/D2DRenderComponent.h"

D2DBaseObj::D2DBaseObj(std::string_view _name, ObjectType _type) : Object(_name, _type)
{
	CreateComponent<D2DRenderComponent>();
}

D2DBaseObj::D2DBaseObj(std::string_view _name, ObjectType _type, std::vector<std::string> _bitmapFilePath) : bitmapFilePath(_bitmapFilePath), Object(_name, _type)
{
	CreateComponent<D2DRenderComponent>();
}

D2DBaseObj::D2DBaseObj(std::string_view _name, ObjectType _type, std::string _fontFilePath) :fontFilePath(_fontFilePath), Object(_name, _type)
{
	CreateComponent<D2DRenderComponent>();
}

D2DBaseObj::D2DBaseObj(std::string_view _name, ObjectType _type, std::vector<std::string> _bitmapFilePath, std::string _fontFilePath) : bitmapFilePath(_bitmapFilePath), fontFilePath(_fontFilePath), Object(_name, _type)
{
	CreateComponent<D2DRenderComponent>();
}

D2DBaseObj::D2DBaseObj(std::string_view _name, ObjectType _type, int _start, int _end, std::string _fontFilePath, std::string _CsvFilePath, std::string _sceneName) :start(_start), end(_end), fontFilePath(_fontFilePath), CsvFilePath(_CsvFilePath), sceneName(_sceneName), Object(_name, _type)
{
	CreateComponent<D2DRenderComponent>();
}

void D2DBaseObj::Initialize()
{

}

void D2DBaseObj::SetD2DLayerOrder(int _index)
{
	GetComponent<D2DRenderComponent>()->bitmapLayerOrder = _index;
}
