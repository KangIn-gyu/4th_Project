#include "pch.h"
#include "Viewport.h"

Viewport::Viewport(float _x, float _y, float _width, float _height, float _minDepth, float _maxDepth)
{
	D3DViewport.TopLeftX = _x;
	D3DViewport.TopLeftY = _y;
	D3DViewport.Width = _width;
	D3DViewport.Height = _height;
	D3DViewport.MinDepth = _minDepth;
	D3DViewport.MaxDepth = _maxDepth;

	viewportSize.width = _width;
	viewportSize.height = _height;
}

void Viewport::Set(float _width, float _height, float _x, float _y, float _minDepth, float _maxDepth)
{
	D3DViewport.TopLeftX = _x;
	D3DViewport.TopLeftY = _y;
	D3DViewport.Width = _width;
	D3DViewport.Height = _height;
	D3DViewport.MinDepth = _minDepth;
	D3DViewport.MaxDepth = _maxDepth;

	viewportSize.width = _width;
	viewportSize.height = _height;
}

Viewport::ViewportSize Viewport::GetViewportSize()
{
	return viewportSize;
}

DXMath::Vector3 Viewport::Project(const DXMath::Vector3& _pos, const DXMath::Matrix& _world, const DXMath::Matrix _view, const DXMath::Matrix& _projection)
{
	DXMath::Matrix combinedMatrix = _world * _view * _projection;
	DXMath::Vector3 pos = DXMath::Vector3::Transform(_pos, combinedMatrix);

	// 스크린 좌표로 변환
	pos.x = (pos.x + 1.0f) * (D3DViewport.Width / 2) + D3DViewport.TopLeftX;
	pos.y = (-pos.y + 1.0f) * (D3DViewport.Height / 2) + D3DViewport.TopLeftY;
	pos.z = pos.z * (D3DViewport.MaxDepth - D3DViewport.MinDepth) + D3DViewport.MinDepth;
	return pos;
}

DXMath::Vector3 Viewport::UnProject(const DXMath::Vector3& _pos, const DXMath::Matrix& _world, const DXMath::Matrix _view, const DXMath::Matrix& _projection)
{
	DXMath::Vector3 pos;

	// 스크린 좌표를 정규화된 좌표로 변환
	pos.x = 2.f * (pos.x - D3DViewport.TopLeftX) / D3DViewport.Width - 1.f;
	pos.y = -2.f * (pos.y - D3DViewport.TopLeftY) / D3DViewport.Height + 1.f;
	pos.z = ((pos.z - D3DViewport.MinDepth) / (D3DViewport.MaxDepth - D3DViewport.MinDepth));

	DXMath::Matrix combinedMatrix = _world * _view * _projection;
	DXMath::Matrix combinedMatrixInvert = combinedMatrix.Invert(); // 변환 행렬의 역행렬 계산

	pos = DXMath::Vector3::Transform(pos, combinedMatrixInvert); // 역변환
	return pos;
}
