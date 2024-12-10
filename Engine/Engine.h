#pragma once
#include "SingletonBase.h"

#define ENGINE Engine::GetInstance().get()

class WindowManager;
class Engine : public SingletonBase<Engine>
{
	friend class SingletonBase<Engine>;
// 함수
public:
	Engine(const Engine& engine) = delete;
	Engine(Engine&& engine) noexcept = delete;

	void Initialize();
	void Loop();

	void SetWindow(WindowManager* window) { m_pWindowManager = window; }
private:
	Engine() = default;
	~Engine() = default;

// 변수
public:

private:
	WindowManager* m_pWindowManager {};
};


// 게임 루프에 대해서는 고민좀 더 하도록

// 12-10 해야 할 것
// 다중 모니터 대응할 수 있도록 하기
// 클라이언트 부족한 부분 채우기
// 유틸리티 좀 더 가져오기