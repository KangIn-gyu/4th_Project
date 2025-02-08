#pragma once
#include "SingletonBase.h"
#include "D3DClass.h"
#include "D2DClass.h"
#include "ConstantBuffer.h"

#include "SkyBox.h"
#include "ShadowRenderer.h"
#include "ConstantBufferData.h"

#define RENDERER Renderer::GetInstance()

typedef DirectX::XMFLOAT4		COLOR;
struct WindowInfo;
class RenderComponent;
class D2DRenderComponent;
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
	void D2DAddRenderComponent(D2DRenderComponent* _D2DrenderComponent);

	void RemoveRenderComponent(RenderComponent* _renderComponent);

	//void TextDraw(int x, int y, COLOR col, TCHAR* msg, ...);
	std::pair<int, int> GetWindowsSize();
	void SetWindowSize();

	ComPtr<ID3D11ShaderResourceView> GetImGuiImageTexture();

	ConstantBuffer* GetMatrixPaletteBuffer() { return &matrixPaletteBuffer; }

	DXMath::Matrix CreateShadowMatrix();
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
	std::vector<D2DRenderComponent*> D2Dwork {};
	
	ShadowRenderer shadowRenderer;


	ConstantBuffer matrixConstantBuffer;
	ConstantBuffer objectBuffer;
	ConstantBuffer cameraBuffer;
	ConstantBuffer matrixPaletteBuffer;
	ConstantBuffer productBuffer;


	ComPtr<ID3D11SamplerState>	linearWrapSampler;    // LINEAR 
	ComPtr<ID3D11SamplerState>  pointClampSampler;    // POINT 

	std::stack<int> previousTexturerProcessing;
	SkyBox m_skybox;
public:
	DXMath::Vector3 lightTarget;
	DXMath::Vector3 lightPos;
	DXMath::Vector3 lightDir;

	// 테스트 용
	ConstantBuffer lightBuffer;
	std::vector<SpotLightData> spotLights;

	void AddSpotLight(const SpotLightData& light);
	void UpdateSpotLights();
	void ClearSpotLight() { spotLights.clear(); }

	ComPtr<ID3D11DepthStencilState> outlineStencilState;
	ComPtr<ID3D11DepthStencilState> outlineMaskState;
	ComPtr<ID3D11RasterizerState> outlineRasterizerState;
	void CreateOutlineStates();

	std::shared_ptr<Shader> outlineShader;

	void RenderObject(RenderComponent* rendercomponent, bool isOutLinePass);
};
