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

// 이것을 통해서 리소스를 로드를 할 예정