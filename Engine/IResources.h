#pragma once
#include <type_traits>
#include <string>

class IResources
{
public:
	IResources() = default; // 파일 경로가 문자가 많아봐야 40개 넘을가??
	virtual ~IResources() = default;
	virtual void Load(std::wstring filePath) = 0;
};
template<class T>
concept ResourcesType = std::is_base_of<IResources, T>::value;

// 로드 함수의 기능이 애매한 부분이 많다. 좀 더 고민이 필요