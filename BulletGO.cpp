#include "Precompiled.h"

#include "BulletGO.h"
#include "Core/GameObjects/IGameObject.h"
#include "Core/GameObjects/IGameObjectData.h"
#include "Assets/Models/MeshModel.h"
#include "Core/GameManagers/IGameSimulation.h"
#include "Core/Game/Game.h"
#include "Core/GameManagers/IGameRendering.h"
#include "Core/GameObjects/IBehaviour.h"
#include "Core/GameManagers/IGameTime.h"
#include "Core/GameManagers/IGameSpawner.h"
#include "Engine/GameObjects/GameObject.h"
#include "Engine/Components/RenderComponent.h"
#include "Assets/Models/ModelRendering.h"
#include "Engine/Components/SimulationComponent.h"
#include "Engine/Components/AIComponent.h"
#include "Core/GameObjects/GameObjectFactory.h"
#include "Loader/Models/ModelFactory.h"
#include "CollisionTypes.h"
#include "PxPhysicsAPI.h"

#include <d3dx9.h>

using namespace physx;

class BulletGOData : public IGameObjectData
{
public:
	typedef std::auto_ptr<MeshModel> MeshModelRef;
	MeshModelRef _model;
	PxMaterial *_material;
	float _radius;

public:
	BulletGOData()
		: _material(nullptr)

	{}

	int load()
	{
		const float kRadius = 0.25f;
		_radius = kRadius;

		// create the physic object

		_material = Game<IGameSimulation>()->physics().createMaterial(0.5f, 0.5f, 0.1f);    //static friction, dynamic friction, restitution

																							//----------------------------------------------------------
																							// create the render object
		_model = MeshModelRef(ModelFactory::createSphere(kRadius));

		return 0;
	}

	~BulletGOData()
	{
		// cleanup physics objects
		if (_material)
		{
			_material->release();
			_material = nullptr;
		}
	}
};

class BulletGOState
{
public:
	BulletGOState()
		: _spawnTime(0)
	{}

public:
	double _spawnTime;
};

class BulletBehaviour : virtual public IBehaviour
{
private:
	BulletGOState _state;


public:
	BulletBehaviour()
	{
		_state._spawnTime = Game<IGameTime>()->currentTime();
	}

	virtual void update(const GameObjectRef &iGameObject) override
	{
		if (Game<IGameTime>()->currentTime() - _state._spawnTime > 10)
			Game<IGameSpawner>()->unspawn(iGameObject);
	}
};

class BulletGOImp : public BulletGO, public GameObject<BulletGOImp, BulletGOData>
{

public:
	static IGameObject::IdType TypeId()
	{
		return "BulletGO";
	}

	//-------------------------------------------------------------------------
	//
	BulletGOImp(const IGameObjectDataRef &aDataRef)
		: GameObject(aDataRef)
	{}

public:
	virtual void onSpawn(const PxTransform &aPose) override
	{
		addComponent<RenderComponent>()->setRenderPrimitive(IRenderPrimitiveRef(new ModelRendering(*_data->_model)));

		// setup simulation component
		auto simulationComponent = addComponent<DynamicSimulationComponent>();
		PxRigidDynamic &pxActor = simulationComponent->pxActor();
		pxActor.setGlobalPose(aPose);

		PxShape *actorShape = pxActor.createShape(PxSphereGeometry(_data->_radius), *_data->_material);

		PxFilterData filterData;
		filterData.word0 = eACTOR_BULLET;
		filterData.word1 = eACTOR_ENEMY | eACTOR_BULLET;
		actorShape->setSimulationFilterData(filterData);

		addComponent<AIComponent>()->setBehaviour(IBehaviourRef(new BulletBehaviour));
	}

	//-------------------------------------------------------------------------
	//
	virtual void onUnspawn() override
	{}
};

//-----------------------------------------------------------------------------
//
IGameObject::IdType BulletGO::TypeId()
{
	return BulletGOImp::TypeId();
}

RegisterGameObjectType<BulletGOImp> gRegisterActor;

