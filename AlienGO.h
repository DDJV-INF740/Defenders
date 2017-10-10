#pragma once

#include <Core/GameObjects/IGameObject.h>

class AlienGO : virtual public IGameObject
{
public:
	static constexpr IdType TypeId = "AlienGO";
	static GameObjectDataRef loadData();
	static GameObjectRef createInstance(const GameObjectDataRef &aDataRef);
};



