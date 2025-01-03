#include "pch.h"
#include "ObjectManager.h"
#include "Object.h"

void ObjectManager::Updata(float _deltaTime)
{
	for (auto& obj : Objects)
	{
		for (int i = 0; i < obj.second.size(); i++)
		{
			obj.second[i]->Update(_deltaTime);
		}
	}
}

void ObjectManager::Render()
{

}
