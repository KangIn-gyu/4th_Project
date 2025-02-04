#pragma once
// 용도 : 오브젝트 그룹이라고 생각하면 될 듯
class Object;
class Layer
{
public:
	enum class Tag // 이거 수정하면 오브젝트 타입도 수정해야됨.
	{
		Background,
		Camera,
		Light,
		Basic,
		UI,
		End
	};

	Layer(Tag _tag) : tag(_tag) {};
	~Layer();

	void Initialize();
	void Update(const float _deltaTime);
	void LateUpdate(const float _deltaTime);
	void Destroy();

	void AddGameObjcet(Object* _gameObject);
	void EraseGameObject(Object* _eraseGameObj);

	int GetSize();
	Object* GetGameObject(std::string_view _name);
	Object* GetGameObject(int _index = 0);

	Tag GetTag() { return tag; }
	std::vector<Object*> NextObjects();

private:

public:

private:
	Tag tag;
	std::vector<Object*> objects;

};

