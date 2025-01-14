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

	if (false == inputProcessers.empty())
	{ // 컨테이너가 비워있지 않다면
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

void DirectXInput::RemoveInputProcesser(IinputProcesser* processer)
{ // 용도 해당 객체사라지면 원소 제거 후 정렬
	if (processer)
	{
		inputProcessers.erase(std::remove(inputProcessers.begin(), inputProcessers.end(), processer), inputProcessers.end());
	}
}

DirectXInput::~DirectXInput()
{
	inputProcessers.clear(); // 원소만 비워주면 된다. 지우는건 본인들이 알아서 할 거다
}

