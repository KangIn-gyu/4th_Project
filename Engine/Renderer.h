#pragma once
#include "SingletonBase.h"
#include "D3DClass.h"
#include "ConstantBuffer.h"

#define RENDERER Renderer::GetInstance()

class RenderComponent;
struct WindowInfo;
class Renderer : public SingletonBase<Renderer>
{
	friend class SingletonBase<Renderer>;
public:
	void Initialize(WindowInfo* _windowInfo);
	void Update(float _deltaTiem);
	void Render();
	void Draw();
	void AddRenderComponent(RenderComponent* _renderComponent);
	void RemoveRenderComponent(RenderComponent* _renderComponent);
private:
	Renderer() {}
	~Renderer() { work.clear(); }
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
	ConstantBuffer objectBuffer;

	// 샘플러 : 이것도 상수버퍼처럼 돌려쓰기용
	ComPtr<ID3D11SamplerState>	linearWrapSampler;    // LINEAR 필터링
	ComPtr<ID3D11SamplerState>  pointClampSampler;    // POINT 필터링
};
// 여기서 메인 카메라 포인터로 가지게 할 수 있게 처리 하자