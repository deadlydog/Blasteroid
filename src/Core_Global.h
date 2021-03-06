/**************************************************
WinMain.cpp
GameCore Component

Programming Role-Playing Games with DirectX
by Jim Adams (01 Jan 2002)
**************************************************/

/*
	Don't forget to include: 
	d3d8.lib d3dx8.lib d3dxof.lib dxguid.lib dinput8.lib dsound.lib winmm.lib dplay.lib wsock32.lib
*/

#ifndef _CORE_GLOBAL_H_
#define _CORE_GLOBAL_H_

// Windows includes
#include <windows.h>

// Standard ANSI-C includes
#include <stdio.h>

// Define Direct Input Version
#define DIRECTINPUT_VERSION 0x0800

// DirectX includes
#include "d3d8.h"
#include "d3dx8.h"
#include "dmusici.h"
#include "dsound.h"
#include "dplay8.h"
#include "dpaddr.h"
#include "dinput.h"

// Core includes
#include "Core_System.h"
#include "Core_Graphics.h"
#include "Core_Input.h"
#include "Core_Sound.h"
#include "Core_Network.h"

#endif
