#pragma once

class GameManager
{
public:
	GameManager();
	~GameManager() =default;

	virtual void Update(const float _deltaTime) {};
};

