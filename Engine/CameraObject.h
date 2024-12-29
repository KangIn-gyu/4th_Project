#pragma once
#include "Object.h"

struct WindowInfo;
class CameraObject : public Object
{
public:
	CameraObject();
	virtual ~CameraObject();

	void SetWindowInfo(WindowInfo* _windowInfo);
	std::pair<int, int> GetWindowSize() const;
private:

public:

private:
	WindowInfo* windowInfo; // 여기서 삭제할 필요 없음 어차피 윈도우에서 삭제함.
};

