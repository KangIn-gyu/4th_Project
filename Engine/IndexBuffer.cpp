#include "pch.h"
#include "IndexBuffer.h"
#include "D3DClass.h"
#include "Helper.h"

void IndexBuffer::Create(const std::vector<DWORD>& _indices)
{
	indices = std::move(_indices); // °í¹ÎÁß

	D3D11_BUFFER_DESC ibDesc = {};
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));
	ibDesc.ByteWidth = sizeof(DWORD) * indices.size();
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices.data();
	HR_T(D3DClass::GetD3DDevice()->CreateBuffer(&ibDesc, &ibData, indexBuffer.GetAddressOf()));
}
