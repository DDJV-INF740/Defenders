#include "Precompiled.h"

#include "AlienGO.h"
#include "Core/GameObjects/IGameObject.h"
#include "Core/GameObjects/IGameObjectData.h"
#include "Assets/Models/MeshModel.h"
#include "Core/GameManagers/ISimulationManager.h"
#include "Core/Game/Game.h"
#include "Core/GameManagers/IRenderManager.h"
#include "Core/GameObjects/IBehaviour.h"
#include "Core/GameManagers/ISpawnManager.h"
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

class AlienGOData : public IGameObjectData
{
public:
	typedef std::unique_ptr<MeshModel> MeshModelRef;
	MeshModelRef _model;
	PxMaterial *_material;
	float _radius;

public:
	AlienGOData()
		: _material(nullptr)

	{}

	int load()
	{
		const float kRadius = 2.0f;
		_radius = kRadius;

		// create the physic object

		_material = Game<ISimulationManager>()->physics().createMaterial(0.5f, 0.5f, 0.1f);    //static friction, dynamic friction, restitution

																							//----------------------------------------------------------
																							// create the render object
		_model = MeshModelRef(ModelFactory::createSphere(kRadius));

		return 0;
	}

	~AlienGOData()
	{
		// cleanup physics objects
		if (_material)
		{
			_material->release();
			_material = nullptr;
		}
	}
};

class AlienGOState
{
public:
	AlienGOState()
	{}
};

class AlienBehaviour : virtual public IBehaviour
{
private:
	AlienGOState _state;


public:
	AlienBehaviour()
	{}

	virtual void update(const GameObjectRef &iGameObject) override
	{}
};

class AlienGOImp : public AlienGO, public GameObject<AlienGOImp, AlienGOData>
{

public:
	//-------------------------------------------------------------------------
	//
	AlienGOImp(const GameObjectDataRef &aDataRef)
		: GameObject(aDataRef)
	{}

	static GameObjectDataRef loadData()
	{
		return GameObject<AlienGOImp, AlienGOData>::loadData();
	}

	static GameObjectRef createInstance(const GameObjectDataRef &aDataRef)
	{
		return GameObject<AlienGOImp, AlienGOData>::createInstance(aDataRef);
	}

public:
	virtual void onSpawn(const PxTransform &aPose) override
	{
		ensureComponent<RenderComponent>()->setRenderPrimitive(IRenderPrimitiveRef(new ModelRendering(*_data->_model)));

		// setup simulation component
		auto simulationComponent = ensureComponent<DynamicSimulationComponent>();
		PxRigidDynamic &pxActor = simulationComponent->pxActor();
		pxActor.setGlobalPose(aPose);

		PxShape *actorShape = pxActor.createShape(PxSphereGeometry(_data->_radius), *_data->_material);

		ensureComponent<AIComponent>()->setBehaviour(IBehaviourRef(new AlienBehaviour));
	}

	//-------------------------------------------------------------------------
	//
	virtual void onUnspawn() override
	{}
};


GameObjectDataRef AlienGO::loadData()
{
	return AlienGOImp::loadData();
}

GameObjectRef AlienGO::createInstance(const GameObjectDataRef &aDataRef)
{
	return AlienGOImp::createInstance(aDataRef);
}

