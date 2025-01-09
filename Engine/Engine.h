#pragma once
#include "SingletonBase.h"

// #include "ObjectManager.h"

#define ENGINE Engine::GetInstance()

class WindowApp;
struct WindowInfo;
class DirectXInput;
class Renderer;
class TimeSystem;
class ObjectManager;
class Engine : public SingletonBase<Engine>
{
	friend class SingletonBase<Engine>;
// 함수
public:
	void Initialize();
	void Loop();

	WindowInfo* GetWindowInfo() const;
	void SetWindowApp(WindowApp* _window) { clientApp = _window; }

private:
	Engine() = default;
	~Engine() = default;
	Engine(const Engine& _engine) = delete;
	Engine(Engine&& _engine) noexcept = delete;

	void Update(const float _deltaTime);
	void Render(const float _deltaTime);

	void TestCode(); // 용도 이름 그대로 테스트할 것들 넣어서 실험하는 곳
	void End();  // 테스트용
// 변수
public:

private:
	WindowApp* clientApp {}; // 더블 프리 때문에 원시 포인터로 처리함
	std::shared_ptr<Renderer> graphicsSystem {};
	std::shared_ptr<DirectXInput> inputSystem {};
	std::shared_ptr<TimeSystem> timeSystem{};

	ObjectManager* test; // 테스트
};

