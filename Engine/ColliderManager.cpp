#include "pch.h"
#include "ColliderManager.h"
#include "Collider.h"
#include "Object.h"

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
                if(colliders[i]->GetActive() == false || colliders[j]->GetActive() ==false)
                    continue;
                if (colliders[i]->GetOwner()->GetName() == colliders[j]->GetOwner()->GetName())
                    continue;
                if (colliders[i]->CheckCollision(colliders[j])) {
                    colliders[i]->OnBlock(colliders[i], colliders[j]);
                    colliders[j]->OnBlock(colliders[j], colliders[i]);
                }
            }
        }
    }

    if (!raycolliders.empty() && !colliders.empty())
    {
        for (auto ray : raycolliders)
        {
            if (ray->GetActive() == false)
                continue;

            for (auto col : colliders)
            {
                if (col->GetActive() == false)
                    continue;
                if (col->coltouch == Touch::None)
                    continue;
                if (ray->GetOwner()->GetName() == col->GetOwner()->GetName())
                    continue;

                if (ray->CheckCollision(col))  // 충돌 검사
                {
                    if (ray->preCollision == nullptr || ray->preCollision != col) // 이전 충돌과 다르면
                    {
                        ray->curCollision = col;
                        ray->OnRay(col);
                    }
                }
                else
                {
                    if (ray->preCollision != nullptr) // 이전에 충돌한 게 있는데 지금은 충돌이 없으면
                    {
                        ray->curCollision = nullptr;
                    }
                }
            }

            if (ray->curCollision != ray->preCollision) // 현재와 이전이 다르면
            {
                if (ray->preCollision != nullptr) // 이전에 충돌한 객체가 있다면 종료 처리
                {
                    ray->EndRay(ray->preCollision);
                }
                ray->preCollision = ray->curCollision;
            }
        }
    }
}

void ColliderManager::AddCollider(Collider* _collider, ColType type)
{
    if (type == ColType::Baisc)
        colliders.push_back(_collider);
    else if (type == ColType::Ray)
        raycolliders.push_back(_collider);
}

