#pragma once

class InputLayout
{
public:
	InputLayout() {};
	~InputLayout() {};

	struct Layuout
	{
		enum class Value
		{
			Int = 4,
			Float = 4,
			Vector2 = 8,
			Vector3 = 12,
			Vector4 = 16,
			Color = 16
		};

		UINT AlignedByteOffset{};
		UINT Byte(Value _byte)
		{
			UINT value = AlignedByteOffset;
			AlignedByteOffset += static_cast<UINT>(_byte);
			return value;
		}
	};

	 void IASetInputLayout(const std::initializer_list<D3D11_INPUT_ELEMENT_DESC>& _elements, const ComPtr<ID3DBlob>& _shaderBuffer);
private:

public:

private:
	ComPtr<ID3D11InputLayout> inputLayout;
};

// float4 -> R32G32B32A32 / float3 -> R32G32B32 / float2 -> R32G32
//	std::initializer_list<D3D11_INPUT_ELEMENT_DESC> elements = 
// {
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	};