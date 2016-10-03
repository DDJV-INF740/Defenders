#include "Precompiled.h"

//=============================================================================
// EXTERNAL DECLARARIONS
//=============================================================================
#include "PlayerGO.h"

#include "Engine/Components/InputComponent.h"
#include "Engine/Components/SimulationComponent.h"
#include "Engine/Components/RenderComponent.h"
#include "Engine/Components/AIComponent.h"
#include "Engine/Components/PlayerComponent.h"
#include "Engine/Managers/SpawnManager.h"
#include "Import/DotX/DotXModel.h"
#include "Assets/Models/ModelRendering.h"
#include "Core/GameObjects/IGameObjectData.h"
#include "Core/GameObjects/IGameObject.h"
#include "Core/GameManagers/IRenderManager.h"
#include "Engine/GameObjects/GameObject.h"
#include "Core/GameObjects/GameObjectFactory.h"
#include "CollisionTypes.h"
#include "Assets.h"

#include "PxPhysicsAPI.h"
#include <d3dx9.h>



using namespace physx;

//=============================================================================
// CLASS PlayerGOData
//=============================================================================
class PlayerGOData : public IGameObjectData
{
public:
	// physics
	PxMaterial *_material;
	PxShape *_actorShape;

	// rendering
	DotXModel *_model;

public:
	PlayerGOData()
		: _material(nullptr)
		, _model(nullptr)
	{}

	int load()
	{
		//---------------------------------------------------------------------
		// create the physic object
		PxPhysics &physics = Game<ISimulationManager>()->physics();

		//static friction, dynamic friction, restitution
		_material = physics.createMaterial(0.5f, 0.5f, 0.1f);

		//---------------------------------------------------------------------
		// create the render object
		_model = new DotXModel(Game<IRenderManager>()->d3dDevice(), Assets::MESH_SHIP, Assets::TEXTURE_ROOT);

		return 0;
	}
};




//=============================================================================
// CLASS PlayerGOImp
//=============================================================================
class PlayerGOImp : public PlayerGO, public GameObject<PlayerGOImp, PlayerGOData>
{
public:
	// physics
	PxShape *_actorShape;

public:
	PlayerGOImp(const GameObjectDataRef &aDataRef)
		: GameObject(aDataRef)
		, _actorShape(nullptr)
	{}

public:
	//-------------------------------------------------------------------------
	//
	static IGameObject::IdType TypeId()
	{
		return "PlayerGO";
	}

	//-------------------------------------------------------------------------
	//
	static GameObjectDataRef loadData()
	{
		PlayerGOData *data = new PlayerGOData();
		data->load();
		return GameObjectDataRef(data);
	}

	//-------------------------------------------------------------------------
	//
	virtual void onSpawn(const PxTransform &aPose) override
	{
		// create the physic object
		PxRigidDynamic &pxActor = ensureComponent<DynamicSimulationComponent>()->pxActor();
		pxActor.setGlobalPose(aPose);
		PxShape *actorShape = pxActor.createShape(PxSphereGeometry(0.5f), *_data->_material);
		pxActor.setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

		PxFilterData filterData;
		filterData.word0 = eACTOR_PLAYER;
		filterData.word1 = eACTOR_ENEMY;
		actorShape->setSimulationFilterData(filterData);


		//----------------------------------------------------------
		// create the render object

		// create the vertices using the CUSTOMVERTEX struct
		ensureComponent<RenderComponent>()->setRenderPrimitive(IRenderPrimitiveRef(new ModelRendering(*_data->_model)));

	}

	//-------------------------------------------------------------------------
	//
	virtual void onUnspawn() override
	{
	}
};

//-----------------------------------------------------------------------------
//
IGameObject::IdType PlayerGO::TypeId()
{
	return PlayerGOImp::TypeId();
}

RegisterGameObjectType<PlayerGOImp> gRegisterActor;
