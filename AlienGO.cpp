#include "Precompiled.h"

#include "AlienGO.h"
#include "Core/GameObjects/IGameObject.h"
#include "Core/GameObjects/IGameObjectData.h"
#include "Assets/Models/MeshModel.h"
#include "Core/GameManagers/IGameSimulation.h"
#include "Core/Game/Game.h"
#include "Core/GameManagers/IGameRendering.h"
#include "Core/GameObjects/IBehaviour.h"
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

class AlienGOData : public IGameObjectData
{
public:
	typedef std::auto_ptr<MeshModel> MeshModelRef;
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

		_material = Game<IGameSimulation>()->physics().createMaterial(0.5f, 0.5f, 0.1f);    //static friction, dynamic friction, restitution

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
	static IGameObject::IdType TypeId()
	{
		return "AlienGO";
	}

	//-------------------------------------------------------------------------
	//
	AlienGOImp(const IGameObjectDataRef &aDataRef)
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

		addComponent<AIComponent>()->setBehaviour(IBehaviourRef(new AlienBehaviour));
	}

	//-------------------------------------------------------------------------
	//
	virtual void onUnspawn() override
	{}
};

//-----------------------------------------------------------------------------
//
IGameObject::IdType AlienGO::TypeId()
{
	return AlienGOImp::TypeId();
}

RegisterGameObjectType<AlienGOImp> gRegisterActor;

