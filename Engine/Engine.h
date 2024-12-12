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
