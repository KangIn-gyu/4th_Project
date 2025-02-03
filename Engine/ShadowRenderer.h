#pragma once
#include "InputLayout.h"

class RenderComponent;
class ShadowRenderer
{
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void BeginShadowPass(ID3D11DeviceContext* context);
	void InitShadowResources(ID3D11Device* device);
	void RenderShadow(ID3D11DeviceContext* context, const DXMath::Matrix& lightViewProj, std::vector<RenderComponent*> rendercomponent);
	//ComPtr<ID3D11Buffer> shadowCB;
	//ComPtr<ID3D11Buffer> basicCB;
	ComPtr<ID3D11VertexShader> shadowVS;
	
	ComPtr<ID3D11SamplerState> shadowSampler;
	InputLayout shadowIA;		// 어차피 메인이랑 똑같은데 왜 또 선언해야하지?

	bool useCustomWolrdMatrix = false;

	ComPtr<ID3D11ShaderResourceView> GetShadowMapSRV() const { return shadowMapSRV.Get(); }
	ComPtr<ID3D11SamplerState> GetShadowSampler() const { return shadowSampler.Get(); }
private:
	static const UINT SHADOW_MAP_SIZE = 4096;

	ComPtr<ID3D11Texture2D> shadowMap;
	ComPtr<ID3D11DepthStencilView> shadowMapDSV;
	ComPtr<ID3D11ShaderResourceView> shadowMapSRV;
	ComPtr<ID3D11RasterizerState> shadowRasterState;
	ComPtr<ID3D11DepthStencilState> shadowDepthState;
	ComPtr<ID3D11Buffer> shadowCB;
	ComPtr<ID3D11Buffer> basicCB;
};

