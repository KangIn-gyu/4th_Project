#include "pch.h"
#include "Texture.h"
#include "Helper.h"
#include "D3DClass.h"

#include <Directxtk/DDSTextureLoader.h>
#include <Directxtk/WICTextureLoader.h>

void Texture::Load(std::wstring_view _filePath)
{
	size_t lastDot = _filePath.find_last_of(L'.');
	if (lastDot == std::wstring_view::npos || lastDot < 2)
	{
		throw std::runtime_error("Invalid file path: Unable to determine Texture type.");
	}

	std::wstring extension = std::wstring(_filePath.substr(lastDot + 1));
	std::transform(extension.begin(), extension.end(), extension.begin(), ::towlower);

	if (extension == L"tga" || extension == L"TGA")
	{
		DX::ScratchImage scratchImage;
		DX::TexMetadata metaData;
		HR_T(DX::LoadFromTGAFile(_filePath.data(), &metaData, scratchImage));
		HR_T(DX::CreateShaderResourceView(D3DClass::GetD3DDevice().Get(), scratchImage.GetImages(),
			scratchImage.GetImageCount(), metaData, texture.GetAddressOf()));
	}
	else if (extension == L"dds" || extension == L"DDS")
	{
		HR_T(DX::CreateDDSTextureFromFile(D3DClass::GetD3DDevice().Get(), _filePath.data(), 
										  nullptr, texture.GetAddressOf()));
	}
	else
	{
		HR_T(DX::CreateWICTextureFromFile(D3DClass::GetD3DDevice().Get(), _filePath.data(), 
										  nullptr, texture.GetAddressOf()));
	}
}
