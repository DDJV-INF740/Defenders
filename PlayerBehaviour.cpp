#include "Precompiled.h"

#include "PlayerBehaviour.h"
#include "BulletGO.h"

#include "Core/GameObjects/IGameObject.h"
#include "Engine/Components/InputComponent.h"
#include "Engine/Managers/SpawnManager.h"
#include "Core/Game/Game.h"
#include "Core/GameManagers/ITimeManager.h"
#include "Engine/Components/SimulationComponent.h"
#include "PxPhysicsAPI.h"

using namespace physx;

void PlayerBehaviour::update(const GameObjectRef &iGameObject)
{
	std::shared_ptr<IKeyboardInputInterface> keyboardInput = iGameObject->as<IKeyboardInputInterface>();
	std::shared_ptr<IPoseInterface> poseInterface = iGameObject->as<IPoseInterface>();

	{
		// 			// make sure the actor always has an up vector
		// 			// point at (0, 1, 0)
		// 			PxTransform pose = _pxActor->getGlobalPose();
		// 			PxReal angle;
		// 			PxVec3 axis;
		// 			pose.q.toRadiansAndUnitAxis(angle, axis);
		// 			pose.q = PxQuat(angle, PxVec3(0, axis.y > 0 ? 1 : -1, 0));
		// 			_pxActor->setGlobalPose(pose);
	}

	static float kRotateStep = 0.02f; // angle in radians
	if (keyboardInput->isPressed(ControllerKey::KEY_RIGHT))
	{
		PxTransform transform = poseInterface->pose();
		transform.p.x -= 0.1f;
		poseInterface->setPose(transform);
	}
	else if (keyboardInput->isPressed(ControllerKey::KEY_LEFT))
	{
		PxTransform transform = poseInterface->pose();
		transform.p.x += 0.1f;
		poseInterface->setPose(transform);
	}
	if (keyboardInput->isPressed(ControllerKey::KEY_ABUTTON))
	{
		const int kMaxFireRate = 10;
		if (chrono::duration_cast<chrono::milliseconds>(Game<ITimeManager>()->currentTime() - _state._lastFireTime).count() > 1000 / kMaxFireRate)
		{
			PxTransform pose = poseInterface->pose();
			PxVec3 frontVec = pose.q.rotate(PxVec3(0, 0, 1));
			PxVec3 spawnPos = pose.p + frontVec * 1;
			GameObjectRef ball = Game<ISpawnManager>()->spawn<BulletGO>(PxTransform(spawnPos, pose.q));

			PxVec3 ballSpeed = pose.q.rotate(PxVec3(0, 0, 15.0f));
			ball->as<IDynamicSimulationInterface>()->pxActor().setLinearVelocity(PxVec3(ballSpeed.x, ballSpeed.y, ballSpeed.z));
			_state._lastFireTime = Game<ITimeManager>()->currentTime();
		}
	}

	if (keyboardInput->isPressed(ControllerKey::KEY_UP))
	{
		PxTransform transform = poseInterface->pose();
		PxVec3 frontVector = transform.q.rotate(PxVec3(0, 0, 1));
		// 			_pxActor->addForce(frontVector);
		transform.p += frontVector / 10;
		poseInterface->setPose(transform);
	}
	else if (keyboardInput->isPressed(ControllerKey::KEY_DOWN))
	{
		PxTransform transform = poseInterface->pose();
		PxVec3 frontVector = transform.q.rotate(PxVec3(0, 0, 1));
		// 			_pxActor->addForce(frontVector);
		transform.p -= frontVector / 10;
		poseInterface->setPose(transform);
	}
	else // not moving
	{
		// 			PxVec3 v = _pxActor->getLinearVelocity();
		// 			v.x /= 2;
		// 			v.z /= 2;
		// 			_pxActor->setLinearVelocity(v);
	}

	if (keyboardInput->isPressed(ControllerKey::KEY_SHIFTBUTTON))
	{
		PxTransform transform = poseInterface->pose();
		transform.p.y += 0.1f;
		poseInterface->setPose(transform);
	}

	if (keyboardInput->isPressed(ControllerKey::KEY_CTRLBUTTON))
	{
		PxTransform transform = poseInterface->pose();
		transform.p.y -= 0.1f;
		poseInterface->setPose(transform);
	}
}
