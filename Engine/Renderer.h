#pragma once
#include "SingletonBase.h"
#include "D3DClass.h"
#include "D2DClass.h"
#include "ConstantBuffer.h"

#include "SkyBox.h"

#define RENDERER Renderer::GetInstance()

typedef DirectX::XMFLOAT4		COLOR;
class RenderComponent;
struct WindowInfo;
class Renderer : public SingletonBase<Renderer>
{
	friend class SingletonBase<Renderer>;
	friend class TextObject;
	friend class SFont;
public:
	void Initialize(WindowInfo* _windowInfo);
	void Update(float _deltaTiem);
	void Render();
	void D3DDraw();
	void D2DDraw();
	void AddRenderComponent(RenderComponent* _renderComponent);
	void RemoveRenderComponent(RenderComponent* _renderComponent);

	//void TextDraw(int x, int y, COLOR col, TCHAR* msg, ...);
	std::pair<int, int> GetWindowsSize();
	void SetWindowSize();

	ComPtr<ID3D11ShaderResourceView> GetImGuiImageTexture();

private:
	Renderer() {}
	~Renderer() { work.clear(); }
	Renderer(const Renderer& InputSystem) = delete;
	Renderer& operator=(Renderer& InputSystem) = delete;
	Renderer(Renderer&& InputSystem) = delete;
	Renderer& operator=(Renderer&& InputSystem) = delete;
	//int FontCreate(ID3D11Device* pDev, ID3D11DeviceContext* pContext);
	
public:

private:
	std::unique_ptr<D3DClass> D3DGraphics {}; 
	std::unique_ptr<D2DClass> D2DGraphics {}; 
	std::vector<RenderComponent*> work {};
	

	ConstantBuffer matrixConstantBuffer;
	ConstantBuffer objectBuffer;
	ConstantBuffer cameraBuffer;
	ConstantBuffer matrixPaletteBuffer;

	ComPtr<ID3D11SamplerState>	linearWrapSampler;    // LINEAR 
	ComPtr<ID3D11SamplerState>  pointClampSampler;    // POINT 

	std::stack<int> previousTexturerProcessing;
	SkyBox m_skybox;

};
