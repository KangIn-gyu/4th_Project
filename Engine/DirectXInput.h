#pragma once
#include <directXTK/Mouse.h>
#include <directXTK/Keyboard.h>

#include "SingletonBase.h"

#define DXINPUT DirectXInput::GetInstance()

class IinputProcesser;
class DirectXInput : public SingletonBase <DirectXInput>
{
	friend class SingletonBase<DirectXInput>;
// 함수
public:
	void Initialize(const HWND _hWnd);
	void Update(const float _deltaTime);
	void InputProcesserAdd(IinputProcesser* _InputProcesser);

private:
	DirectXInput() : mouseState(), keyboardState() {}
	~DirectXInput() {};
	DirectXInput(const DirectXInput& InputSystem) = delete;
	DirectXInput& operator=(DirectXInput& InputSystem) = delete;
	DirectXInput(DirectXInput&& InputSystem) = delete;
	DirectXInput& operator=(DirectXInput&& InputSystem) = delete;

// 변수
public:
	std::unique_ptr<DirectX::Keyboard>          keyboard{};
	std::unique_ptr<DirectX::Mouse>             mouse{};

private:
	DirectX::Mouse::State                       mouseState{};
	DirectX::Mouse::ButtonStateTracker          mouseStateTracker{};

	DirectX::Keyboard::State                    keyboardState{};
	DirectX::Keyboard::KeyboardStateTracker     keyboardStateTracker{};

	std::vector<IinputProcesser*>				inputProcessers;
};

class IinputProcesser
{
public:
	IinputProcesser() { DXINPUT->InputProcesserAdd(this); }
	virtual void OnInputProcess(const DirectX::Keyboard::State& KeyState,
		const DirectX::Keyboard::KeyboardStateTracker& KeyTracker,
		const DirectX::Mouse::State& MouseState,
		const DirectX::Mouse::ButtonStateTracker& MouseTracker) = 0;
};
