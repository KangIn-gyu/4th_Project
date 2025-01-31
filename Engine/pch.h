// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

// 문자열
#include <string>

// 컨테이너
#include <map>
#include <vector>
#include <unordered_map>
#include <set>

// 포인터 관련
#include <memory>
#include <wrl/client.h>
using namespace Microsoft::WRL;

// 타입 관련
#include <typeindex>

// 스레드

// 입출력 
#include <iostream>

// 유틸
#include <algorithm>
#include <functional>

// Direct
#include <d3d11.h>
#include <directxtk/SimpleMath.h>  // #pragma comment(lib, "d3dx11.lib") 대체됨
#include <d3dcompiler.h>  // 일단 보류
#include <DirectXTex.h>
#include <dxgi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DirectXTex.lib")

// D2D
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#ifndef DEBUG
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib,"dxgi.lib")  
#endif
#include <directxtk/SpriteFont.h>

namespace DX = DirectX;
namespace DXMath = DirectX::SimpleMath;

#include <fstream>

// 여기에 미리 컴파일하려는 헤더를 추가하세요
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC  // C 런타임의 디버깅 도구를 활성화 파일 이름과 라인 번호 추적
#include <crtdbg.h>

#include <imgui.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//라이브러리 오류수정테스트
#ifdef _DEBUG
#undef new // new 재정의 해제
#endif

// 라이브러리 코드 (new가 재정의되지 않음)
#include "jjson.hpp"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__) // 다시 재정의 (필요한 경우)
#endif


#endif //PCH_H
