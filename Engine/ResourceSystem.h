#pragma once
#include "SingletonBase.h"

class ResourceSystem : public SingletonBase<ResourceSystem>
{
	friend class SingletonBase<ResourceSystem>;
//	using 
public:
	ResourceSystem(const ResourceSystem& File) = delete;
	ResourceSystem(ResourceSystem&& File)noexcept = delete;


private:
	explicit ResourceSystem() = default;
	~ResourceSystem() = default;

public:

private:
	
};

// 매쉬에 있는 인덱스버퍼랑 버덱스 버퍼는 한번만 만들면 매쉬를 내주면 되니깐 문제 없다
