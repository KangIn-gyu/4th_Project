#pragma once
#include "SingletonBase.h"
#include "D3DClass.h"
#include "ConstantBuffer.h"

#define RENDERER Renderer::GetInstance()

struct WindowInfo;
class Renderer : public SingletonBase<Renderer>
{
	friend class SingletonBase<Renderer>;
public:
	void Initialize(WindowInfo* _windowInfo);
	void Render();

private:
	Renderer() {}
	~Renderer() {};
	Renderer(const Renderer& InputSystem) = delete;
	Renderer& operator=(Renderer& InputSystem) = delete;
	Renderer(Renderer&& InputSystem) = delete;
	Renderer& operator=(Renderer&& InputSystem) = delete;

public:

private:
	std::unique_ptr<D3DClass> D3DGraphics {}; // 그래픽스
	ConstantBuffer matrixConstantBuffer;
//	std::vector<> // 추후 렌더링 컴포넌트를 수집해서 한번에 그림을 그리게 할 것이다.
};

