#pragma once
#include "SingletonBase.h"

class IResources;
class ResourceSystem : public SingletonBase<ResourceSystem>
{
	friend class SingletonBase<ResourceSystem>;
	using Resource_vec_pair = std::vector<std::pair<std::wstring, std::weak_ptr<IResources>>>; // 자료형 너무 길어서 별칭 만듬
public:
	std::shared_ptr<IResources> Load(std::type_index _key ,std::wstring_view _filePath);
	void AllLoadFile(std::wstring_view filePath);

private:
	explicit ResourceSystem() = default;
	~ResourceSystem() = default;
	ResourceSystem(const ResourceSystem& _sameClassObj) = delete;
	ResourceSystem(ResourceSystem&& _sameClassObj)noexcept = delete;


public:

private:
	std::unordered_map<std::type_index, Resource_vec_pair> resources;
};


// 매쉬에 있는 인덱스버퍼랑 버덱스 버퍼는 한번만 만들면 매쉬를 내주면 되니깐 문제 없다
