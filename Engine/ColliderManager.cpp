#include "pch.h"
#include "ColliderManager.h"
#include "Collider.h"


ColliderManager::ColliderManager()
{

}


void ColliderManager::CollisionCheck()
{
    if (!colliders.empty())
    {
        for (size_t i = 0; i < colliders.size(); ++i) {
            for (size_t j = i + 1; j < colliders.size(); ++j) {
                // 
                if (colliders[i]->CheckCollision(colliders[j])) {
                    colliders[i]->OnBlock(colliders[i], colliders[j]);
                    colliders[j]->OnBlock(colliders[j], colliders[i]);
                }
            }
        }
    }
}

void ColliderManager::AddCollider(Collider* _collider)
{
	colliders.push_back(_collider);
}

