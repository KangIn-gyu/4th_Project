#include "pch.h"
#include "Texture.h"
#include "Helper.h"
#include "D3DClass.h"

#include <Directxtk/DDSTextureLoader.h>
#include <Directxtk/WICTextureLoader.h>

void Texture::Load(std::string_view _filePath)
{
	size_t lastDot = _filePath.find_last_of(L'.');
	if (lastDot == std::wstring_view::npos || lastDot < 2)
	{ // 나중에 로그 시스템으로 변경 필요
		throw std::runtime_error("Invalid file path: Unable to determine Texture type.");
	}

	std::string extension = std::string(_filePath.substr(lastDot + 1));
	std::transform(extension.begin(), extension.end(), extension.begin(), ::towlower);

	// 일일히 함수 호출 하지 말고 한번에 하기 위해
	std::wstring wstrFilePaht = StringConverter::StringToWide(_filePath); 
	const wchar_t* wchFilePath = wstrFilePaht.c_str();

	if (extension == "tga" || extension == "TGA")
	{
		DX::ScratchImage scratchImage;
		DX::TexMetadata metaData;
		HR_T(DX::LoadFromTGAFile(wchFilePath, &metaData, scratchImage));
		HR_T(DX::CreateShaderResourceView(D3DClass::GetD3DDevice().Get(), scratchImage.GetImages(),
			scratchImage.GetImageCount(), metaData, texture.GetAddressOf()));
	}
	else if (extension == "dds" || extension == "DDS")
	{
		HR_T(DX::CreateDDSTextureFromFile(D3DClass::GetD3DDevice().Get(), wchFilePath,
			nullptr, texture.GetAddressOf()));
	}
	else
	{
		HR_T(DX::CreateWICTextureFromFile(D3DClass::GetD3DDevice().Get(), wchFilePath,
										  nullptr, texture.GetAddressOf()));
	}
}

void Texture::SetName(std::string_view _naem)
{
	name.assign(_naem);
}
