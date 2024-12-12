#pragma once
#include "SingletonBase.h"

#include "D3DClass.h"

#define GRAPHICS Graphics::GetInstance()
class Graphics : public SingletonBase<Graphics>
{
	friend class SingletonBase<Graphics>;
public:
	void Initialize(const int _screenWidth, const int _screenHeight, HWND _hwnd);
	void Update();
	void Render();

private:
	Graphics() {}
	~Graphics() {};
	Graphics(const Graphics& InputSystem) = delete;
	Graphics& operator=(Graphics& InputSystem) = delete;
	Graphics(Graphics&& InputSystem) = delete;
	Graphics& operator=(Graphics&& InputSystem) = delete;

public:

private:
	D3DClass* D3DGraphics {};
};

