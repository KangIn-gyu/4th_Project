#include "pch.h"
#include "ResourceSystem.h"
#include "IResources.h"
// #include "Shader.h" //일단 보류 12.27

void ResourceSystem::AllLoadFile(std::wstring_view _filePath)
{
    std::filesystem::path fs(basePath + _filePath.data());

    if (!std::filesystem::exists(fs)) // 경로에 파일이 있는지 확인
    {
        // 추후 조치가 필요 로그 시스템 이용이 필요
        std::cout << "파일 전부 로드 실패" << '\n';
        return;
    }

    for (auto& file : std::filesystem::recursive_directory_iterator(fs))
    {
     //   조건 처리가 필요.. 고민 사항
     //   Load(file.path().wstring());
    }
}

void ResourceSystem::Show()
{
    for (auto& it : resources)
    {
        std::cout << "첫 번째 키값 :" << it.first.name() << '\n';
        for (auto& it2 : it.second)
        {
            std::wstring filePath = it2.first;
            auto weakResource = it2.second;
            auto sharedResource = weakResource.lock();
            if (sharedResource) // 리소스가 유효하다면
            {
                std::wcout << L"Path:" << filePath << '\n';
            }
        }
    }
}
