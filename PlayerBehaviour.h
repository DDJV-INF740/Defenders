#pragma once

#include "Core/GameObjects/IBehaviour.h"

class PlayerActorState
{
public:
	PlayerActorState()
		: _lastFireTime(Game<ITimeManager>()->currentTime())
	{}

	TimeManager::time_point _lastFireTime;
};


class PlayerBehaviour : public IBehaviour
{
private:
	PlayerActorState _state;

public:
	virtual void update(const GameObjectRef &iGameObject) override;
};
