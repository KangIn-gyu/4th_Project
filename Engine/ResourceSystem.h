#pragma once
#include "SingletonBase.h"
#include <filesystem>
#define RESOURCESYSTEM ResourceSystem::GetInstance()

class IResources;
class ResourceSystem : public SingletonBase<ResourceSystem>
{
	friend class SingletonBase<ResourceSystem>;
	using Resource_unMap = std::unordered_map<std::wstring, std::weak_ptr<IResources>>; // 자료형 너무 길어서 별칭 만듬
public:
	template<typename T>
	std::shared_ptr<T> Load(const std::wstring_view& _filePath);
	void AllLoadFile(std::wstring_view filePath);

private:
	explicit ResourceSystem() = default;
	~ResourceSystem() = default;
	ResourceSystem(const ResourceSystem& _sameClassObj) = delete;
	ResourceSystem(ResourceSystem&& _sameClassObj)noexcept = delete;


public:

private:
	std::unordered_map<std::type_index, Resource_unMap> resources;
};

// 매쉬에 있는 인덱스버퍼랑 버덱스 버퍼는 한번만 만들면 매쉬를 내주면 되니깐 문제 없다

template <typename T>
inline std::shared_ptr<T> ResourceSystem::Load(const std::wstring_view& _filePath)
{
    std::filesystem::path relativePath = _filePath;

    // 파일 존재 여부 확인
    if (!std::filesystem::exists(relativePath))
    {
        throw std::runtime_error("File does not exist: " + std::string(relativePath.string()));
        return nullptr;
    }

    std::type_index key = typeid(T);
    std::cout << key.name() << std::endl; 
    // 맵으로 부터 확인을 한다
    auto it = resources.find(key);  //
    if (it != resources.end()) // 같은게 있으면?
    {
        std::unordered_map<std::wstring, std::weak_ptr<IResources>>& resourceUnMap = it->second; // std::unordered_map<std::wstring, std::weak_ptr<IResources>>
        auto it2 = resourceUnMap.find(_filePath.data());
        if (it2 != resourceUnMap.end())
        {
            auto weak_ptr = it2->second;
            return std::static_pointer_cast<T>(weak_ptr.lock());
        }
    }

    // 없을 경우 처리 확장자마다의 로드 구현해야 됨.
    std::wstring extension = relativePath.extension();
    if (extension == L".png" || extension == L".jpg") // 추후 텍스쳐 추가하면 처리
    {

    }
    else if (extension == L".fbx")
    {

    }
    else if (extension == L".dds")
    {

    }
    else if (extension == L".hlsl" || extension == L".cso")
    {
        std::shared_ptr<T> newShader = std::make_shared<T>();
        newShader->CreateShader(_filePath);
        std::type_index shaderType = typeid(T);      // 타입 인덱스 가져오기
        auto& resourceUnMap = resources[shaderType];
        resourceUnMap.emplace(_filePath, newShader);     // 새로운 리소스 추가
        return newShader;
    }
    // 추후 사운드도 추가 해야 한다.
}
