#pragma once
class Editor
{
public:
	enum class State
	{
		Paused,   // 일시중지  비호출할때
		Active,   // 실행
		Disabled, // 비활성화 상태
		Destroyed // 파괴 상태
	};
	Editor() = default;
	virtual ~Editor() = default;

	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void OnEnable() {};  // 편집기가 활성화될 때 처리해야 할 로직입니다.
	virtual void OnDisable() {}; // 편집기가 비활성화될 때 처리할 로직입니다.
	virtual void OnDestroy() {}; // 편집기가 파괴될 때 처리할 로직입니다

	std::string& GetName() { return name; }
protected:
	void SetName(const std::string& name) { this->name = name; }

public:

protected:
	State state;

private:
	std::string name;
};

