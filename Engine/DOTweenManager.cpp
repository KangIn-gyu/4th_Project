#include "pch.h"
#include "DOTweenManager.h"
#include "Dotween.h"

DOTweenManager::DOTweenManager()
{
   
}

DOTweenManager::~DOTweenManager()
{
}

//void DOTweenManager::reset()
//{
//    tweens.clear();
//    removetweens.clear();
//}

void DOTweenManager::Update(float deltaTime)
{
    if (removetweens.size() != 0)
    {
        for (auto it = removetweens.begin(); it != removetweens.end(); ++it) {
            tweens.remove(*it);
        }
    }
    removetweens.clear();
    for (auto var : tweens)
        var->Update(deltaTime);
}
