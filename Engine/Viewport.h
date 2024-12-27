#pragma once
#include <directxtk/SimpleMath.h>
namespace DXMath = DirectX::SimpleMath;

class Viewport 
{
public:
	Viewport(float _x, float _y, float _width, float _height, float _minDepth, float _maxDepth);
	~Viewport() {}

	struct ViewportSize;

	D3D11_VIEWPORT& Get() { return D3DViewport; }
	void Set(float _width, float _height, float _x = 0, float _y = 0, float _minDepth = 0, float _maxDepth = 0);
	ViewportSize GetViewportSize(); 

	// 3D ÁÂÇ¥¸¦ 2D È­¸é ÁÂÇ¥·Î ÇÁ·ÎÁ§¼Ç
	DXMath::Vector3 Project(const DXMath::Vector3& _pos, const DXMath::Matrix& _world, 
							const DXMath::Matrix _view, const DXMath::Matrix& _projection);

	// 2D È­¸é ÁÂÇ¥¸¦ 3D ÁÂÇ¥·Î ¿ªÇÁ·ÎÁ§¼Ç
	DXMath::Vector3 UnProject(const DXMath::Vector3& _pos, const DXMath::Matrix& _world, 
							  const DXMath::Matrix _view, const DXMath::Matrix& _projection);

private:

public:
	struct ViewportSize
	{
		float width{};
		float height{};
	};

private:
	D3D11_VIEWPORT D3DViewport;
	ViewportSize viewportSize;
};

