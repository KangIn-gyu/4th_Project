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
	void RemoveInputProcesser(IinputProcesser* processer);

private:
	DirectXInput() : mouseState(), keyboardState() {}
	virtual ~DirectXInput();
	DirectXInput(const DirectXInput& InputSystem) = delete;
	DirectXInput& operator=(DirectXInput& InputSystem) = delete;
	DirectXInput(DirectXInput&& InputSystem) = delete;
	DirectXInput& operator=(DirectXInput&& InputSystem) = delete;

// 변수
public:
	std::unique_ptr<DX::Keyboard>          keyboard{};
	std::unique_ptr<DX::Mouse>             mouse{};
	DX::Mouse::State                       mouseState{};

private:
	DX::Mouse::ButtonStateTracker          mouseStateTracker{};

	DX::Keyboard::State                    keyboardState{};
	DX::Keyboard::KeyboardStateTracker     keyboardStateTracker{};

	std::vector<IinputProcesser*>		   inputProcessers;
};


class IinputProcesser
{
public:

	IinputProcesser() { DXINPUT->InputProcesserAdd(this); }
	virtual ~IinputProcesser()
	{  
		DXINPUT->RemoveInputProcesser(this);
	}

	virtual void OnInputProcess(const DX::Keyboard::State& _KeyState,
		const DX::Keyboard::KeyboardStateTracker& _KeyTracker,
		const DX::Mouse::State& _MouseState,
		const DX::Mouse::ButtonStateTracker& _MouseTracker) = 0;

};
