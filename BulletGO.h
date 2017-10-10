#pragma once

#include <Core/GameObjects/IGameObject.h>

class BulletGO : virtual public IGameObject
{
public:
	static constexpr IdType TypeId = "BulletGO";
	static GameObjectDataRef loadData();
	static GameObjectRef createInstance(const GameObjectDataRef &aDataRef);
};



