#include "pch.h"
#include "InputLayout.h"
#include "Helper.h"
#include "D3DClass.h"

#include "ResourceSystem.h"
#include "Shader.h"
void InputLayout::IASetInputLayout(const std::initializer_list<D3D11_INPUT_ELEMENT_DESC>& _elements, std::wstring_view _vertexShaderfilePath)
{
    std::shared_ptr<Shader> shader = RESOURCESYSTEM->Load<Shader>(_vertexShaderfilePath);
    ID3DBlob* vsBlod = shader->GetVSBlob();
    if (nullptr == vsBlod)
    {
        std::cout << "IASetInputLayout 실패" << std::endl; // 추후 로그 시스템으로 해야됨
        return;
    }

    int test = vsBlod->GetBufferSize();
    int elementsSize = static_cast<UINT>(_elements.size());

    HR_T(D3DClass::GetD3DDevice()->CreateInputLayout(_elements.begin(),
                                                     static_cast<UINT>(_elements.size()),
                                                     vsBlod->GetBufferPointer(),
                                                     vsBlod->GetBufferSize(),
                                                     inputLayout.GetAddressOf()
                                                    ));
}
