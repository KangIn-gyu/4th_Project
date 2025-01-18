#pragma once
class EditorWindow
{
public:
	enum class State
	{
		Disable,  // 비활성화
		Active,   // 활성화
		Destroy,  // 삭제
	};

	EditorWindow() = default;
	virtual ~EditorWindow() = default;

	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void Run() {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};
	virtual void OnDestroy() {};
	
	ImGuiWindowFlags GetFlag() const { return flag; }
	void SetFlags(ImGuiWindowFlags _flags) { flag = _flags; }
	State GetState() const { return state; }
	void SetState(State _state) { this->state = _state; }
	ImVec2 GetSize() { return size; }
	void SetSize(ImVec2 _size) { this->size = _size; }
	const std::string GetName() { return name; }
protected:
	void SetName(const std::string& _name) { name = _name; }

private:
	ImGuiWindowFlags flag = ImGuiWindowFlags_None;
	State state;
	ImVec2 size;
	std::string name;
};

