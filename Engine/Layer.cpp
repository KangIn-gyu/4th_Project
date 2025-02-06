#include "pch.h"
#include "Layer.h"
#include "Object.h"
#include "Helper.h"
#include "Script.h"

void Layer::Initialize()
{
	for (auto& obj : objects)
	{
		obj->Initialize();

		if (obj->IsScript())
		{
			obj->ComponentSetting();
		}
	}
}

void Layer::Update(const float _deltaTime)
{
	auto UpdateObj = objects | std::ranges::views::filter([](auto& obj) { return obj->IsActive(); });

	for (auto& obj : UpdateObj)
	{
		obj->Update(_deltaTime);
		obj->ComponentsUpdate(_deltaTime);
	}
}

void Layer::LateUpdate(const float _deltaTime)
{
	for (auto& obj : objects)
	{ // TODO : 보류 사항 레이트업데이트 추후 만들어야 됨
		obj->LateUpdate();
	}
}

void Layer::ResetInformation()
{
	for (auto& obj : objects)
	{ // TODO : 보류 사항 레이트업데이트 추후 만들어야 됨
		obj->ResetInformation();
	}
}

void Layer::Destroy()
{
	// TODO : 오브젝트 삭제 어떻게 구현할지 고민
}

Layer::~Layer()
{
	SafeExtinction::SAFE_CLEAR_CONTAINER(objects);
}

void Layer::AddGameObjcet(Object* _gameObject)
{
	if (_gameObject == nullptr)
		return;

	objects.push_back(_gameObject);

	if (objects.size() > 2)
	{
		std::ranges::sort(objects, [](const Object* lhs, const Object* rhs) {return lhs < rhs;});
	}
}

void Layer::EraseGameObject(Object* _eraseGameObj)
{

	std::erase_if(objects,
		[=](Object* gameObj)
		{
			return gameObj == _eraseGameObj;
		});
}

int Layer::GetSize()
{
	return objects.size();
}

Object* Layer::GetGameObject(std::string_view _name)
{
	auto it = std::find_if(objects.begin(), objects.end(), [_name](Object* object) { return object->GetName() == _name; });

	if (it != objects.end()) 
	{
		return *it; // 오브젝트 반환
	}

	std::cout << "GetGameObject의 해당하는 이름의 오브젝트가 없습니다.\n";
	return nullptr;
}

Object* Layer::GetGameObject(int _index)
{
	if (_index >= 0 && _index < objects.size())
	{
		return objects[_index];
	}
	else
	{
		std::cout << "GetGameObject의 잘못된 인덱스를 입력하였습니다.\n";
	}
}

std::vector<Object*> Layer::NextObjects()
{
	std::vector<Object*> nextObjects;  // State::Next 상태인 오브젝트들을 담을 벡터

	// 레인지 기반 루프를 사용하여 objects 벡터 순회
	for (auto* obj : objects)  // objects의 각 요소를 순회
	{
		if (obj->GetState() == Object::State::Next)  // 상태가 Next인 경우
		{
			nextObjects.push_back(obj);
		}
	}

	return nextObjects;
}
