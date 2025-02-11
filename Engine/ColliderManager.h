#pragma once
#include "SingletonBase.h"
#include "Collider.h"
#include "RayCollier.h"
class Collider;
class RayCollider;
#define CollidersManager ColliderManager::GetInstance().get()

enum class ColType
{
	Baisc,
	Ray,
};
class ColliderManager : public SingletonBase<ColliderManager>
{
	friend class SingletonBase<ColliderManager>;
	ColliderManager();
	virtual ~ColliderManager() = default;
	std::vector<Collider*>  colliders;
	std::vector<Collider*> raycolliders;
public:
	void CollisionCheck();
	void AddCollider(Collider* _collider, ColType type = ColType::Baisc);  //블럭 인것만  추가하면될듯
};

