#pragma once
#include "../Engine/Object.h"
#include "../Engine/IClick.h"

class Button : public Object , public IClick
{
public:

	
	Button(std::string_view _name, Object::ObjectType _type,std::function<void(void)> _func);

	//실험용 버튼생성자
	Button(std::string_view _name, Object::ObjectType _type, DXMath::Vector3 _pos, std::function<void(void)> _func);
	virtual void Start();
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	virtual void OnClick() override;
private:
	std::function<void(void)> clickFunc;
};

