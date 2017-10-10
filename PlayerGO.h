#pragma once

#include <Core/GameObjects/IGameObject.h>

class PlayerGO : virtual public IGameObject
{
public:
	static constexpr IdType TypeId = "PlayerGO";
	static GameObjectDataRef loadData();
	static GameObjectRef createInstance(const GameObjectDataRef &aDataRef);
};

