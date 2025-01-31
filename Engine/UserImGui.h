#pragma once
//#include <imgui.h>
#include "SingletonBase.h"
#include "HierarchyWindow.h"
#include "InspectorWindow.h"

#include <imgui.h>

#define IMGUI UserImGui::GetInstance()
class Scene;
class UserImGui : public SingletonBase<UserImGui>
{
	friend class SingletonBase<UserImGui>;
public:
	virtual ~UserImGui();

	// 먼 미래를 생각하면 디바이스랑 디바이스컨테스트는 내가 사용하는 그래픽스 라이브러리에 따른 차이에 따라 대응하게 만드는게 좋다
	void Initialize(HWND _hwnd, ComPtr<ID3D11Device> _Device, ComPtr<ID3D11DeviceContext> _DeviceContext); 
	void Update(const float _deltaTime);
	void Render();

	void SetWindowSize(int _width, int _height);
	void HierarchyCurrentSceneSetting(Scene* _currentScene);
	DXMath::Color GetBankGroundColor() { return backgroundColor; }

private:
	void MenuBar();
	void ConsoleMenu();
	void MainMenu();
	void ImGuiScene();

	void MainDockSpace(); // 보류 코드

public:
	bool debugFlag = false;

private:
	ImGuiIO* io;
	std::pair<int, int> windowsSize{};

	// 콘솔
	bool is_consoleMenu = false;
	bool is_mainMenu = false;
	DXMath::Color backgroundColor = { 0.5f, 0.5f, 0.5f, 1.0f};

	HierarchyWindow hierarchy;
	std::shared_ptr<InspectorWindow> inspector;
};

