#pragma once
#include "SingletonBase.h"
#include "D3DClass.h"

#define GRAPHICS Graphics::GetInstance()

struct WindowInfo;
class Graphics : public SingletonBase<Graphics>
{
	friend class SingletonBase<Graphics>;
public:
	void Initialize(WindowInfo* _windowInfo);
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
	std::unique_ptr<D3DClass> D3DGraphics {};
};

