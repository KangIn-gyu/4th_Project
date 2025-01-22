#pragma once
//마우스를 해당오브젝트 위에올렸을때 쓸거
class IOnmouse
{
public:
	virtual void OnMouse() = 0;
	virtual ~IOnmouse() = default;
};