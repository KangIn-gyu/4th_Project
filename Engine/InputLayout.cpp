#include "pch.h"
#include "InputLayout.h"
#include "Helper.h"
#include "D3DClass.h"

#include "ResourceSystem.h"
#include "Shader.h"

bool InputLayout::IASetInputLayout(const std::initializer_list<D3D11_INPUT_ELEMENT_DESC>& elements, std::string_view vertexShaderfilePath)
{
    VSshader = RESOURCESYSTEM->Load<Shader>(vertexShaderfilePath);
    if (!VSshader)
    {
        std::cout << "Failed to load vertex shader" << std::endl;
        return false;
    }

    ID3DBlob* vsBlob = VSshader->GetVSBlob();
    if (nullptr == vsBlob)
    {
        std::cout << "IASetInputLayout 실패" << std::endl;
        return false;
    }

    // elements를 직접 사용
    HR_T(D3DClass::GetD3DDevice()->CreateInputLayout(elements.begin(),
        static_cast<UINT>(elements.size()),
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        inputLayout.GetAddressOf()
    ));
    return true;
}
