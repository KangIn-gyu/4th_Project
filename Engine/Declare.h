#pragma once
#include "windows.h"

struct WindowInfo
{
	HWND hWnd = nullptr;
	int screenWidth{};
	int screenHeight{};
	bool windoweMode = true;

	~WindowInfo() { DestroyWindow(hWnd); }
};

struct CameraInfo
{
	float Speed = 400.f;
	float RotationSpeed = 0.004f;

	// 트랜스폼 고민중

	// 투영 관련 변수
	float FovAngleY = 1.0f;
	float Near = 1.0f;           // 근거리 클리핑 평면
	float Far = 100.0f;          // 원거리 클리핑 평면

	bool Update = false;
	bool projectionUpdate = false;
};