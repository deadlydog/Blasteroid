/*****************************************************************************/
/* Globals.h                                                                 */
/* Written by Daniel Schroeder                                               */
/* Created on June 3, 2005                                                   */
/* Last Updated July 19, 2005                                                */
/*---------------------------------------------------------------------------*/
/*   This file Includes all other global data files, so only this file needs */
/* to be included from the games main file.                                  */
/*****************************************************************************/

#ifndef GLOBALS_H
#define GLOBALS_H

// Include all Globals
#include "GlobalCharacterPictures.h"
#include "GlobalCharacterInfo.h"
#include "GlobalWeaponAndBulletInfo.h"
#include "GlobalLevelInfo.h"
#include "GlobalItemInfo.h"
#include "GlobalGameInfo.h"
#include "GlobalMusicAndSounds.h"


// Texture IDs and Number of Textures
const int GiNUMBER_OF_TEXTURES_IN_GAME				= 7;

const int GiMENU_TEXTURE							= 0;
const int GiSHIP_TEXTURE							= 1;
const int GiBULLET_TEXTURE							= 2;
const int GiCONTROLS_TEXTURE						= 3;
const int GiPLAY_TEXTURE							= 4;
const int GiSPACE_STATION_TEXTURE					= 5;
const int GiITEM_TEXTURE							= 6;


#endif

