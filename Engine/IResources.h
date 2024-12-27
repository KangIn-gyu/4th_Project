#pragma once
#include <type_traits>
#include <string>

class IResources
{
public:
	IResources() = default; // 파일 경로가 문자가 많아봐야 40개 넘을가??
	virtual ~IResources() = default;
	virtual void Load(std::wstring_view _filePath) = 0; // 어차피 읽기용으로만 할건데 wstring필요가 있을가?
};
template<class T>
concept ResourcesType = std::derived_from<T, IResources>;

// 로드 함수의 기능이 애매한 부분이 많다. 좀 더 고민이 필요