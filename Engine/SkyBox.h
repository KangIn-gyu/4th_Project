#pragma once
#include "Texture.h"
#include "Shader.h"
class SkyBox
{
public:
    void Init();
    void Render(ID3D11DeviceContext* context);
    void SetShaders(ID3D11Device* device, std::wstring vsPath, std::wstring psPath);

    void LoadEnvironmentMap(const std::string_view& filename);

    void LoadIrradianceMap(const std::string_view& filename);

    void LoadSpecularMap(const std::string_view& filename);

    void LoadBRDFLUT(const std::string_view& filename);
    
private:
    void CreateBuffers(ID3D11Device* device);
    void CreateDepthState(ID3D11Device* device);
    void CreateSamplerState(ID3D11Device* device);


    HRESULT CompileShaderFromFile(const std::wstring& path, LPCSTR entry, LPCSTR target, ID3DBlob** blob);

    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    std::shared_ptr<Shader> m_shader;
    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11InputLayout> m_inputLayout;
    ComPtr<ID3D11DepthStencilState> m_depthState;
    UINT m_indexCount = 0;

    ComPtr<ID3D11SamplerState> m_samplerState;

    std::shared_ptr<Texture> m_environmentMap;
    std::shared_ptr<Texture> m_irradianceMap; 
    std::shared_ptr<Texture> m_specularMap;   
    std::shared_ptr<Texture> m_brdfLUT;       

};
