#include "pch.h"
#include "ResourceSystem.h"
#include "IResources.h"
// #include "Shader.h" //일단 보류 12.27

void ResourceSystem::AllLoadFile(std::wstring_view filePath)
{
    std::filesystem::path fs(filePath);

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
