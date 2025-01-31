#pragma once
#pragma once
#ifndef PCH_H
#define PCH_H

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

#define WIN32_LEAN_AND_MEAN 
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


#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__) // 다시 재정의 (필요한 경우)
#endif

#endif //PCH_H