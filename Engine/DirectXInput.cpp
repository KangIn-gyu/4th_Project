#include "pch.h"
#include "DirectXInput.h"

// constexpr float ROTATION_GAIN = 0.004f;
// constexpr float MOVEMENT_GAIN = 0.07f;

void DirectXInput::Initialize(const HWND _hWnd)
{
	keyboard = std::make_unique<DirectX::Keyboard>();
	mouse = std::make_unique<DirectX::Mouse>();
	mouse->SetWindow(_hWnd);
}

void DirectXInput::Update(const float _DeltaTime)
{
	mouseState = mouse->GetState();
	mouseStateTracker.Update(mouseState);

	keyboardState = keyboard->GetState();
	keyboardStateTracker.Update(keyboardState);

	if (inputProcessers.empty())
	{
		for (auto& processer : inputProcessers)
		{
			processer->OnInputProcess(keyboardState, keyboardStateTracker, mouseState, mouseStateTracker);
		}
	}
}

void DirectXInput::InputProcesserAdd(IinputProcesser* _InputProcesser)
{
	inputProcessers.emplace_back(_InputProcesser);
}

