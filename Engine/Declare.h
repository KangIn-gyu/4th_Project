#pragma once
#include "windows.h"

// 용도 : 각 클래스에서 전달할 정보들 모음 객체

struct WindowInfo
{
	HWND hWnd = nullptr;
	int screenWidth{};
	int screenHeight{};
	bool windoweMode = true;

	~WindowInfo() { DestroyWindow(hWnd); }
};

class TransformComponent;
struct CameraInfo
{
	float Speed = 400.f;
	float RotationSpeed = 0.004f;

	// 트랜스폼 고민중
	TransformComponent* cameraTransform;

	// 투영 관련 변수
	float FovAngleY = 1.0f;
	float Near = 1.0f;           // 근거리 클리핑 평면
	float Far = 1000.0f;          // 원거리 클리핑 평면

	bool Update = false;
	bool projectionUpdate = false;
};

struct MaterialInfo
{
	float metallic{};        // 기본 비금속
	float roughness = 0.5f;  // 기본 거칠기

};