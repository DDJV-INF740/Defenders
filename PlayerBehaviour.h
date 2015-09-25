#include "Core/GameObjects/IBehaviour.h"

class PlayerActorState
{
public:
	PlayerActorState()
		: _lastFireTime(0)
	{}

	double _lastFireTime;
};


class PlayerBehaviour : public IBehaviour
{
private:
	PlayerActorState _state;

public:
	virtual void update(const GameObjectRef &iGameObject) override;
};
