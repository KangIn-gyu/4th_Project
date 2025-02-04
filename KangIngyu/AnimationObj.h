#pragma once
#include "../Engine/Object.h"
#include "../Engine/DirectXInput.h"

class AnimationObj : public Object, public IinputProcesser
{
public:
	AnimationObj(std::string_view _name, Object::ObjectType _type);

	virtual void Initialize();
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	virtual void OnInputProcess(const DX::Keyboard::State& _KeyState,
		const DX::Keyboard::KeyboardStateTracker& _KeyTracker,
		const DX::Mouse::State& _MouseState,
		const DX::Mouse::ButtonStateTracker& _MouseTracker);
};

