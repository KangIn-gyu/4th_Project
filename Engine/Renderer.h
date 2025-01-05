#pragma once
#include "SingletonBase.h"
#include "D3DClass.h"
#include "ConstantBuffer.h"

#define RENDERER Renderer::GetInstance()

struct WindowInfo;
class RenderComponent;
class Renderer : public SingletonBase<Renderer>
{
	friend class SingletonBase<Renderer>;
public:
	void Initialize(WindowInfo* _windowInfo);
	void Render();
	void AddRenderComponent(RenderComponent* _renderComponent);

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
	std::vector<RenderComponent*> work {};

	// 상수 버퍼
	ConstantBuffer matrixConstantBuffer;

	// 샘플러 : 이것도 상수버퍼처럼 돌려쓰기용
	ComPtr<ID3D11SamplerState>	linearWrapSampler{};    // LINEAR 필터링
	ComPtr<ID3D11SamplerState>  pointClampSampler{};    // POINT 필터링

//	std::vector<> // 추후 렌더링 컴포넌트를 수집해서 한번에 그림을 그리게 할 것이다.
};

