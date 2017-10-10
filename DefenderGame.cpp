#include "Precompiled.h"

//=============================================================================
// EXTERNAL DECLARATIONS
//=============================================================================
#include "Engine/Game/GameEngine.h"
#include "Engine/Managers/SpawnManager.h"
#include "Engine/Tasks/SpawnTask.h"
#include "Core/Game/Game.h"
#include "Core/GameManagers/ISpawnManager.h"
#include "Core/GameObjects//GameObjectFactory.h"
#include "Engine/Components/InputComponent.h"
#include "Engine/Components/PlayerComponent.h"
#include "PxPhysicsAPI.h"
#include "PlayerBehaviour.h"

#include "Engine/Managers/RenderManager.h"
#include "Engine/Tasks/RenderTask.h"
#include "Engine/Tasks/PlayerTask.h"
#include "Engine/Tasks/InputTask.h"
#include "Engine/Tasks/AITask.h"
#include "Engine/Tasks/PhysicsTask.h"
#include "Engine/Tasks/GameRulesTask.h"
#include "Engine/Tasks/TimeTask.h"

#include "Engine/Components/ComponentFactory.h"
#include "Engine/Components/SimulationComponent.h"
#include "Engine/Components/AIComponent.h"
#include "Engine/Components/FollowPoseComponent.h"
#include "Engine/Components/RenderComponent.h"
#include "Engine/Components/RenderViewComponent.h"

#include "Engine/Managers/WindowManager.h"
#include "Engine/Managers/CameraManager.h"
#include "Engine/Managers/SimulationManager.h"
#include "Engine/Managers/PlayerManager.h"
#include "Engine/Managers/AIManager.h"
#include "Engine/Managers/GameLoopManager.h"
#include "Engine/Managers/TimeManager.h"
#include "Engine/Rendering/Camera.h"

#define _USE_MATH_DEFINES
#include <Core/Components/IFollowPoseInterface.h>
#include <math.h>
#include "PlayerGO.h"
#include "AlienGO.h"
#include "BulletGO.h"

using namespace physx;
//=============================================================================
// CLASS DefenderGame
//=============================================================================
class DefenderGame : public GameEngine
{
public:

	enum
	{
		TIMERTASK,
		INPUTTASK,
		PLAYERTASK,
		AITASK,
		PHYSICSTASK,
		GAMERULESTASK,
		RENDERTASK,
		SPAWNTASK,
	};


	DefenderGame()
	{
	}

	virtual bool init() override
	{
		GameEngine::init();

		{
			GameManagerFactory& factory = GameManagerFactory::Instance();
			factory.registerType<WindowManager>();
			factory.registerType<RenderManager>();
			factory.registerType<SimulationManager>();
			factory.registerType<SpawnManager>();
			factory.registerType<AIManager>();
			factory.registerType<PlayerManager>();
			factory.registerType<CameraManager>();
			factory.registerType<GameLoopManager>();
			factory.registerType<TimeManager>();
		}

		{
			ComponentFactory& factory = ComponentFactory::Instance();
			factory.registerType<DynamicSimulationComponent>();
			factory.registerType<StaticSimulationComponent>();
			factory.registerType<AIComponent>();
			factory.registerType<FollowPoseComponent>();
			factory.registerType<KeyboardInputComponent>();
			factory.registerType<PlayerComponent>();
			factory.registerType<RenderComponent>();
			factory.registerType<RenderViewComponent>();
		}

		{
			GameObjectFactory::registerGameObjectType(Camera::TypeId, &Camera::createInstance, &Camera::loadData);
			GameObjectFactory::registerGameObjectType(PlayerGO::TypeId, &PlayerGO::createInstance, &PlayerGO::loadData);
			GameObjectFactory::registerGameObjectType(AlienGO::TypeId, &AlienGO::createInstance, &AlienGO::loadData);
			GameObjectFactory::registerGameObjectType(BulletGO::TypeId, &BulletGO::createInstance, &BulletGO::loadData);
		}
		ensureManager<WindowManager>();
		ensureManager<RenderManager>();
		ensureManager<SimulationManager>();
		ensureManager<SpawnManager>();
		ensureManager<AIManager>();
		ensureManager<PlayerManager>();
		ensureManager<CameraManager>();
		ensureManager<GameLoopManager>();
		ensureManager<TimeManager>();

		addTask<SpawnTask>(SPAWNTASK);
		addTask<RenderTask>(RENDERTASK);
		addTask<PlayerTask>(PLAYERTASK);
		addTask<InputTask>(INPUTTASK);
		addTask<AITask>(AITASK);
		addTask<GameRulesTask>(GAMERULESTASK);
		addTask<PhysicsTask>(PHYSICSTASK);
		addTask<TimeTask>(TIMERTASK);

		loadLevel();
		return true;
	}

	virtual bool cleanup() override
	{
		unloadLevel();

		return GameEngine::cleanup();
	}


	~DefenderGame()
	{}

	void loadLevel();

	void unloadLevel()
	{
		// unspawn every remaining objects
		Game<ISpawnManager>()->unspawnAll();
		Game<ISpawnManager>()->update();
	}
};



GameEngineRef& IGameEngine::Instance()
{
	static GameEngineRef sGame(new DefenderGame);
	return sGame;
}


void DefenderGame::loadLevel()
{
	Game<ISimulationManager>()->scene().setGravity(PxVec3(0, 0, 0));
	// spawn the player
	GameObjectRef player = Game<ISpawnManager>()->spawn<PlayerGO>(PxTransform(PxVec3(0, 0, 0)));
	player->ensureComponent<KeyboardInputComponent>();
	player->ensureComponent<PlayerComponent>()->setBehaviour(IBehaviourRef(new PlayerBehaviour));

	// spawn an enemy
	Game<ISpawnManager>()->spawn<AlienGO>(PxTransform(PxVec3(0, 0, 18)));


	GameObjectRef camera = Game<ISpawnManager>()->spawn<Camera>(PxTransform(PxVec3(0, 0, 0)));

	camera->as<IFollowPoseInterface>()->setPoseAdjustment([](const physx::PxTransform &iPose) {
		PxTransform wCameraPose;
		// Look down, where x is horizontal, z is vertical and y is up/down
		wCameraPose.q = PxQuat(float(-M_PI), PxVec3(0, 1, 0)) * PxQuat(float(-M_PI) / 2, PxVec3(1, 0, 0)); // for now...
		PxVec3 wFront(0, 0, 1);
		wFront = wCameraPose.q.rotate(wFront);
		wCameraPose.p = PxVec3(0, -20, iPose.p.z + 8.0f);
		return wCameraPose;
	});
}



