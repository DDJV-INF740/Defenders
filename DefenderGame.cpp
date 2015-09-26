#include "Precompiled.h"

//=============================================================================
// EXTERNAL DECLARATIONS
//=============================================================================
#include "Engine/Game/GameEngine.h"
#include "Engine/Managers/SpawnManager.h"
#include "Engine/Tasks/SpawnTask.h"
#include "Core/Game/Game.h"
#include "Core/GameManagers/IGameSpawner.h"
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

		addComponent<WindowManager>();
		addComponent<RenderManager>();
		addComponent<SimulationManager>();
		addComponent<SpawnManager>();
		addComponent<AIManager>();
		addComponent<PlayerManager>();
		addComponent<CameraManager>();
		addComponent<GameLoopManager>();
		addComponent<TimeManager>();

		addTask(new SpawnTask, SPAWNTASK);
		addTask(new RenderTask, RENDERTASK);
		addTask(new PlayerTask, PLAYERTASK);
		addTask(new InputTask, INPUTTASK);
		addTask(new AITask, AITASK);
		addTask(new GameRulesTask, GAMERULESTASK);
		addTask(new PhysicsTask, PHYSICSTASK);
		addTask(new TimeTask, TIMERTASK);

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
		Game<IGameSpawner>()->unspawnAll();
		Game<IGameSpawner>()->update();
	}
};



GameEngineRef IGameEngine::Instance()
{
	static GameEngineRef sGame(new DefenderGame);
	return sGame;
}


void DefenderGame::loadLevel()
{
	Game<IGameSimulation>()->scene().setGravity(PxVec3(0, 0, 0));
	// spawn the player
	GameObjectRef player = Game<IGameSpawner>()->spawn<PlayerGO>(PxTransform(PxVec3(0, 0, 0)));
	player->addComponent<KeyboardInputComponent>();
	player->addComponent<PlayerComponent>()->setBehaviour(IBehaviourRef(new PlayerBehaviour));

	// spawn an enemy
	Game<IGameSpawner>()->spawn<AlienGO>(PxTransform(PxVec3(0, 0, 18)));


	GameObjectRef camera = Game<IGameSpawner>()->spawn<ICamera>(PxTransform(PxVec3(0, 0, 0)));

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



