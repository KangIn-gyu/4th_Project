#include "pch.h"
#include "ConstantBuffer.h"
#include "Helper.h"
#include "D3DClass.h"

void ConstantBuffer::Create(size_t _size)
{
	D3D11_BUFFER_DESC bd = {};
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = _size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	HR_T(D3DClass::GetD3DDevice()->CreateBuffer(&bd, nullptr, constantBuffer.GetAddressOf()));
}
