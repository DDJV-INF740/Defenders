#pragma once

#include <Core/GameObjects/IGameObject.h>

class AlienGO : virtual public IGameObject
{
public:
	static IGameObject::IdType TypeId();
};



