#include "pch.h"
#include "GameManager.h"
#include "Engine.h"

GameManager::GameManager()
{
	ENGINE->CollectionGameManager(this);
}
