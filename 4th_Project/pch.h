// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#pragma once
#ifndef PCH_H
#define PCH_H

#include "framework.h"

#include <string>
#include <string_view>

#include <map>
#include <vector>
#include <unordered_map>

#include <typeindex>

#include <iostream>


#include <directxtk/SimpleMath.h>  // #pragma comment(lib, "d3dx11.lib") 대체됨
namespace DX = DirectX;
namespace DXMath = DirectX::SimpleMath;

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//라이브러리 오류수정 근대 조금 처음 로딩이 느려진듯 논의
#ifdef _DEBUG
#undef new // new 재정의 해제
#endif

// 라이브러리 코드 (new가 재정의되지 않음)
#include "../Engine/jjson.hpp"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__) // 다시 재정의 (필요한 경우)
#endif

#endif //PCH_H
