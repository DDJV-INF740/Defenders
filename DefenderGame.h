#pragma once

#include <Core/Game/Game.h>

class Defenders: public Game
{
public:
	Defenders(LPDIRECT3DDEVICE9 aD3DDevice);
	~Defenders();

public:
	void init();
	void update();
	void cleanup();
	LPDIRECT3DDEVICE9 d3dDevice();

	void setScene(physx::PxScene *aScene);
	physx::PxScene* scene();

	void requestExit();
	bool shouldExit() const;

	Camera* camera();
	 
private:
	class DefendersImp;
	DefendersImp *_imp;
};

