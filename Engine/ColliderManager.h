#pragma once
#include "SingletonBase.h"
#include "Collider.h"
class Collider;

#define CollidersManager ColliderManager::GetInstance().get()

class ColliderManager : public SingletonBase<ColliderManager>
{
	friend class SingletonBase<ColliderManager>;
	ColliderManager();
	virtual ~ColliderManager() = default;
	std::vector<Collider*>  colliders;
public:
	void CollisionCheck();
	void AddCollider(Collider* _collider);  //블럭 인것만  추가하면될듯
};

