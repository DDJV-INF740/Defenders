#pragma once

#include <Core/GameObjects/IGameObject.h>

class PlayerGO : virtual public IGameObject
{
public:
	static IGameObject::IdType TypeId();
};

