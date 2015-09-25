#pragma once

#include <Core/GameObjects/IGameObject.h>

class BulletGO : virtual public IGameObject
{
public:
	static IGameObject::IdType TypeId();
};



