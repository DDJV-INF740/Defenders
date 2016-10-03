// Precompiled.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include <cassert>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#define SAFE_RELEASE(_x) { (_x)->Release(); (_x) = nullptr; }

#ifdef _DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x) do { } while (false)
#endif


#ifndef _NOPCH

#include <stdio.h>
#include <tchar.h>

// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "PxPhysicsAPI.h"

//=============================================================================
// EXTERNAL DECLARATIONS
//=============================================================================
#include "Engine/Game/GameEngine.h"
#include "Engine/Managers/SpawnManager.h"
#include "Engine/Tasks/SpawnTask.h"
#include "Core/Game/Game.h"
#include "Core/GameManagers/ISpawnManager.h"
#include "Engine/Components/InputComponent.h"
#include "Engine/Components/PlayerComponent.h"

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

#include <Core/Components/IFollowPoseInterface.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "PxPhysicsAPI.h"
using namespace physx;
using namespace std;
using namespace engine;

#include "PxPhysicsAPI.h"
#include "PlayerBehaviour.h"

#endif // _NOPCH

