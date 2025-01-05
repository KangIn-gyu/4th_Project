#include "pch.h"
#include "Shader.h"
#include "Helper.h"
#include "D3DClass.h"
#include "ResourceSystem.h"

void Shader::Load(std::wstring_view _filePath)
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // 함수 아래 참조
#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	// 파일명에서 마지막 '.'의 위치 찾기
	size_t lastDot = _filePath.find_last_of(L'.');
	if (lastDot == std::wstring_view::npos || lastDot < 2)
	{
		throw std::runtime_error("Invalid file path: Unable to determine shader type.");
	}

	std::wstring shaderPrefix(_filePath.substr(lastDot - 2, 2)); // 어떤 타입 쉐이더인지? 확인함 확장자 이전 2개의 문자 확인해서
	std::wstring extension = std::wstring(_filePath.substr(lastDot + 1));

	ID3DBlob* errorBlob = nullptr;

	// 고민을 많이함 각 셰이더 별로 각자 만들게 할 것인지 통합으로 할 것인지
	// 가독성과 속도로 보면 개별로 만드는게 좋다. 이유 if 확인할 필요가 없기 때문에
	// 통합 처리로 하였다. 이유는 리소스 시스템을 통해서 셰이더를 만든다고 했을때
	// (통합함수가 아닌 개별일때)내가 어떤 셰이더를 만들어야지 일일히 함수를 바꾸면서 하기 귀찮을거 같아서 함수하나에 통합을 했다.
	auto compileShader = [&dwShaderFlags](const std::wstring_view& filePath, const std::string& shaderModel,
		ComPtr<ID3DBlob>& shaderBuffer, ID3DBlob* errorBuffer) {
			HR_T(D3DCompileFromFile(
				filePath.data(),					// 셰이더 파일 경로
				nullptr,							// 셰이더 매크로 정의 (없음)
				D3D_COMPILE_STANDARD_FILE_INCLUDE,  // 포함 파일 처리기 
				"main",								// 셰이더 엔트리 포인트 함수
				shaderModel.c_str(),				// 셰이더 모델 
				dwShaderFlags,						// 컴파일 플래그
				0,									// 이펙트 플래그 (기본값)
				shaderBuffer.GetAddressOf(),		// 컴파일 결과	
				&errorBuffer			            // 오류 메시지
			)); };


	if (shaderPrefix == L"VS")
	{
		if (extension == L"hlsl")
		{
			compileShader(_filePath, "vs_5_0", VSBlob, errorBlob);
		}
		else if (extension == L"cso")
		{
			HR_T(D3DReadFileToBlob(_filePath.data(), VSBlob.GetAddressOf()));
		}

		HR_T(D3DClass::GetD3DDevice()->CreateVertexShader(VSBlob->GetBufferPointer(),
														  VSBlob->GetBufferSize(), nullptr,
														  vertexShader.GetAddressOf()));
	}
	else if (shaderPrefix == L"PS")
	{
		if (extension == L"hlsl")
		{
			compileShader(_filePath, "ps_5_0", PSBlob, errorBlob);
		}
		else if (extension == L"cso")
		{
			HR_T(D3DReadFileToBlob(_filePath.data(), PSBlob.GetAddressOf()));
		}
		HR_T(D3DClass::GetD3DDevice()->CreatePixelShader(PSBlob->GetBufferPointer(),
														 PSBlob->GetBufferSize(), nullptr,
														 pixelShader.GetAddressOf()));
	}

	if (errorBlob)
	{
		MessageBoxA(NULL, (char*)errorBlob->GetBufferPointer(), "CompileShaderFromFile", MB_OK);
		errorBlob->Release();
	}
}

ID3DBlob* Shader::GetVSBlob()
{
	if (nullptr != VSBlob.Get())
	{
		return VSBlob.Get();;
	}
	return nullptr; // 나중에 로그 시스템 처리
}

ID3DBlob* Shader::GetPSBlob()
{
	if (nullptr != PSBlob.Get())
	{
		return PSBlob.Get();
	}
	return nullptr; // 나중에 로그 시스템 처리
}

// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
// Setting this flag improves the shader debugging experience, but still allows 
// the shaders to be optimized and to run exactly the way they will run in 
// the release configuration of this program.
// Disable optimizations to further improve shader debugging