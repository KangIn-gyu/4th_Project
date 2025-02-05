#include "pch.h"
#include "Component.h"



void Component::SetActive(bool _isActive)
{
	if (_isActive != isActive)
		isActive = _isActive;
}
