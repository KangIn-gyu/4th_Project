#pragma once

class IClick
{
public:
	virtual void OnClick() = 0;
	virtual ~IClick() = default;
};
