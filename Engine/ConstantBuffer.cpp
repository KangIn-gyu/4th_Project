#include "pch.h"
#include "ConstantBuffer.h"
#include "Helper.h"
#include "D3DClass.h"
#include "ConstantBufferData.h"

void ConstantBuffer::Create(size_t _size, Usage usage)
{
    D3D11_BUFFER_DESC bd = {};
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

    switch (usage)
    {
    case Usage::DEFAULT:
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.CPUAccessFlags = 0;
        break;
    case Usage::DYNAMIC:
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        break;
    }

    bd.ByteWidth = _size;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    HR_T(D3DClass::GetD3DDevice()->CreateBuffer(&bd, nullptr, constantBuffer.GetAddressOf()));
}

void ConstantBuffer::Update(const void* data, size_t size)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HR_T(D3DClass::GetD3DDeviceContext()->Map(
        constantBuffer.Get(),
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedResource));

    memcpy(mappedResource.pData, data, size);

    D3DClass::GetD3DDeviceContext()->Unmap(constantBuffer.Get(), 0);
}
