#pragma once
#include "SingletonBase.h"
#include <filesystem>

#include "IResources.h"
#include "FBXLoader.h"
#define RESOURCESYSTEM ResourceSystem::GetInstance()

class Model;
class ResourceSystem : public SingletonBase<ResourceSystem>
{
	friend class SingletonBase<ResourceSystem>;
	using Resource_unMap = std::unordered_map<std::string, std::weak_ptr<IResources>>; // 자료형 너무 길어서 별칭 만듬
public:
	template<ResourcesType T>
	std::shared_ptr<T> Load(std::string_view _filePath);
	void AllLoadFile(std::string_view _filePath);
    void Show(); // 리소스 매니저 테스트용

private:
	explicit ResourceSystem() = default;
	~ResourceSystem() = default;
	ResourceSystem(const ResourceSystem& _sameClassObj) = delete;
	ResourceSystem(ResourceSystem&& _sameClassObj)noexcept = delete;

public:

private:
    FBXLoader fbxLoader;
	std::unordered_map<std::type_index, Resource_unMap> resources;
    std::string basePath = "Resource/";
};

template <ResourcesType T>
std::shared_ptr<T> ResourceSystem::Load(std::string_view _filePath)
{
    std::string filePath = basePath + _filePath.data();
    std::filesystem::path relativePath = filePath;

    // 파일 존재 여부 확인
    if (!std::filesystem::exists(relativePath))
    {
        // 추후 로그 시스템 필요
        throw std::runtime_error("File does not exist: " + std::string(relativePath.string()));
        return nullptr;
    }

    std::type_index key = typeid(T);

    //// 맵으로 부터 확인을 한다
    auto it = resources.find(key);  //
    if (it != resources.end()) // 같은게 있으면?
    {
        std::unordered_map<std::string, std::weak_ptr<IResources>>& resourceUnMap = it->second; // std::unordered_map<std::wstring, std::weak_ptr<IResources>>
        auto it2 = resourceUnMap.find(filePath);
        if (it2 != resourceUnMap.end())
        {
            auto weak_ptr = it2->second;
            if (!weak_ptr.expired())
            {
                return std::static_pointer_cast<T>(weak_ptr.lock());
            }
            else
            {
                resourceUnMap.erase(it2);
            }
        }
    }

    if constexpr (false == std::is_same_v<T, Model>)
    {
        std::shared_ptr<T> newResources = std::make_shared<T>();
        newResources->Load(filePath);
        std::type_index type = typeid(T);        // 타입 인덱스 가져오기
        auto& resourceUnMap = resources[type];
        resourceUnMap.emplace(filePath, newResources);     // 새로운 리소스 추가
        return newResources;
    }
    else // 모델일 경우 예외
    {
        std::type_index type = typeid(T);
        auto& resourceUnMap = resources[type];
        auto sharedPtr = fbxLoader.FBXLoad(filePath);
        resourceUnMap.emplace(filePath, sharedPtr);
        return sharedPtr;
    }
}
