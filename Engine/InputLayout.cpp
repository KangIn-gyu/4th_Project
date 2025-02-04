#include "pch.h"
#include "InputLayout.h"
#include "Helper.h"
#include "D3DClass.h"

#include "ResourceSystem.h"
#include "Shader.h"

void InputLayout::IASetInputLayout(const std::initializer_list<D3D11_INPUT_ELEMENT_DESC>& _elements, std::string_view _vertexShaderfilePath)
{
    VSshader = RESOURCESYSTEM->Load<Shader>(_vertexShaderfilePath);

    if (!VSshader)
    {
        std::cout << "Failed to load vertex shader" << std::endl;
        return;
    }

    ID3DBlob* vsBlod = VSshader->GetVSBlob();
    if (nullptr == vsBlod)
    {
        std::cout << "IASetInputLayout 실패" << std::endl; // 추후 로그 시스템으로 해야됨
        return;
    }

    // 3. elements를 벡터로 변환 (안전한 메모리 관리를 위해)
    std::vector<D3D11_INPUT_ELEMENT_DESC> elements(_elements.begin(), _elements.end());



    HR_T(D3DClass::GetD3DDevice()->CreateInputLayout(_elements.begin(),
                                                     static_cast<UINT>(_elements.size()),
                                                     vsBlod->GetBufferPointer(),
                                                     vsBlod->GetBufferSize(),
                                                     inputLayout.GetAddressOf()
                                                    ));
}
