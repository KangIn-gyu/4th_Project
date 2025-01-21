#pragma once

class IClickAble
{
public:
	virtual void OnClick() = 0;
	virtual ~IClickAble() = default;
};
