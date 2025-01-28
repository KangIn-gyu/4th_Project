#pragma once
#include "SingletonBase.h"

#define ENGINE Engine::GetInstance()

class WindowApp;
struct WindowInfo;
class DirectXInput;
class Renderer;
class TimeSystem;
class SceneManager;
class Engine : public SingletonBase<Engine>
{
	friend class SingletonBase<Engine>;
// 함수
public:
	void Initialize();
	void Loop();

	WindowInfo* GetWindowInfo() const;
	void SetWindowApp(WindowApp* _window) { clientApp = _window; }

	void SetWindowSize(int _width, int _height);
	DXMath::Vector2 GetWindowSize();
	void ChangeScene(std::string_view _SceneName);

private:
	Engine() = default;
	~Engine() = default;
	Engine(const Engine& _engine) = delete;
	Engine(Engine&& _engine) noexcept = delete;

	void Update(const float _deltaTime);
	void RateUpdate(const float _deltaTime);
	void Render(const float _deltaTime);

// 변수
public:

private:
	WindowApp* clientApp {}; // 더블 프리 때문에 원시 포인터로 처리함
	std::shared_ptr<Renderer> graphicsSystem {};
	std::shared_ptr<DirectXInput> inputSystem {};
	std::shared_ptr<TimeSystem> timeSystem{};
	std::shared_ptr<SceneManager> sceneManager{};

};

