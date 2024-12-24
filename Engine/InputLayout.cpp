#include "pch.h"
#include "InputLayout.h"
#include "Helper.h"
#include "D3DClass.h"

void InputLayout::IASetInputLayout(const std::initializer_list<D3D11_INPUT_ELEMENT_DESC>& _elements, const ComPtr<ID3DBlob>& _shaderBuffer)
{
    if (nullptr == _shaderBuffer.Get())
    {
        return;
    }

    HR_T(D3DClass::GetD3DDevice()->CreateInputLayout(_elements.begin(),
                                                     static_cast<UINT>(_elements.size()),
                                                     _shaderBuffer->GetBufferPointer(),
                                                     _shaderBuffer->GetBufferSize(),
                                                     inputLayout.GetAddressOf()));
}
