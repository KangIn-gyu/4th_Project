#include "pch.h"
#include "ResourceSystem.h"
#include <filesystem>
#include "IResources.h"

std::shared_ptr<IResources> ResourceSystem::Load(std::type_index _key, std::wstring_view _filePath)
{
    std::filesystem::path relativePath = _filePath;

    // 파일 존재 여부 확인
    if (!std::filesystem::exists(relativePath))
    {
        throw std::runtime_error("File does not exist: " + std::string(relativePath.string()));
        return nullptr;
    }
    
    // 맵으로 부터 확인을 한다
    auto it = resources.find(_key);
    if (it != resources.end()) // 같은게 있으면?
    {
        auto& resourceVector = it->second;
        auto vecIt = std::find_if(resourceVector.begin(), resourceVector.end(),
            [&_filePath](const std::pair<std::wstring, std::weak_ptr<IResources>>& pair)
            {
                return pair.first == _filePath; // 람다의 리턴임
            });

        if (vecIt != resourceVector.end()) // 찾은 경우 맵에 있는 값으로 준다
        {
            return vecIt->second.lock(); // weak_ptr에서 shared_ptr로 변환
        }
    }

    // 없을 경우 처리 확장자마다의 로드 구현해야 됨.
    std::wstring extension = relativePath.extension();
    if (extension == L".png" || extension == L".jpg")
    {

    }
    else if (extension == L".fbx")
    {

    }
    else if (extension == L".cso")
    {

    }
    else if (extension == L".dds")
    {

    }
}

void ResourceSystem::AllLoadFile(std::wstring_view filePath)
{
    std::filesystem::path fs(filePath);

    if (!std::filesystem::exists(fs)) // 경로에 파일이 있는지 확인
    {
        // 추후 조치가 필요 로그 시스템 이용이 필요
        return;
    }

    for (auto& file : std::filesystem::recursive_directory_iterator(fs))
    {
     //   조건 처리가 필요.. 고민 사항
     //   Load(file.path().wstring());
    }
}
