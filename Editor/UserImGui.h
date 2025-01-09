#pragma once
#include <imgui.h>

class UserImGui
{
public:
	UserImGui() = default;
	~UserImGui();

	// 먼 미래를 생각하면 디바이스랑 디바이스컨테스트는 내가 사용하는 그래픽스 라이브러리에 따른 차이에 따라 대응하게 만드는게 좋다
	void Initialize(HWND _hwnd, ComPtr<ID3D11Device> _Device, ComPtr<ID3D11DeviceContext> _DeviceContext); 
	void Update(const float _deltaTime);
	void Render();
private:

public:

private:
	ImGuiIO* io;
};

