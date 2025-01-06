#pragma once
#include "IResources.h"

class Shader : public IResources
{
public:
	Shader() = default;
	virtual ~Shader() = default;

	virtual void Load(std::string_view _filePath) override;

	ID3DBlob* GetVSBlob();
	ComPtr<ID3D11VertexShader> GetVertexShader() { return vertexShader; }

	ID3DBlob* GetPSBlob();
	ComPtr<ID3D11PixelShader> GetPixelShader() { return pixelShader; }

private:

public:

private:
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;

	ComPtr<ID3DBlob> VSBlob;
	ComPtr<ID3DBlob> PSBlob;
	// 추후 다른 셰이더 추가될 수 있음 참고
};
// ComPtr은 스마트 포인터로 초기화되지 않은 상태에서는 내부적으로 리소스를 소유하지 않늗나. 그래서 메모리 낭비는 발생하지 않는다
