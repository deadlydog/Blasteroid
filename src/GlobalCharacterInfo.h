/*****************************************************************************/
/* GlobalCharacterInfo.h                                                     */
/* Written by Daniel Schroeder                                               */
/* Created on July 15, 2005                                                  */
/* Last Updated August 21, 2005                                              */
/*---------------------------------------------------------------------------*/
/*   This file specifies all of the global data pertaining to characters in  */
/* the game. This includes both the Players and the Enemies. This does not   */
/* include the characters Pictures or States though. This includes things    */
/* such as the characters ID, speed, health, armor, lives, etc.              */
/*****************************************************************************/

#ifndef GLOBAL_CHARACTER_INFO_H
#define GLOBAL_CHARACTER_INFO_H

#include "GlobalWeaponAndBulletInfo.h"	// Needed for Weapon/Bullet Types
#include "GlobalGameInfo.h"				// Needed for Total Number of Enemies
#include "CShip.h"						// Needed for AI Patterns


//********** General Character Type IDs **********

const int GiPLAYER										= 10000;
const int GiENEMY										= 10001;
const int GiENEMY_BOSS									= 10002;


//********** Specific Character Type IDs **********

// Players
const int GiPLAYER_1									= 0;	// Start at zero so we can use for loops to cycle through all Players
const int GiPLAYER_2									= 1;


// Total number of enemies in entire game
const int GiTOTAL_NUMBER_OF_ENEMIES_IN_GAME				= 10;

// Enemy Bosses Starting ID (All IDs above and including this are bosses)
const int GiENEMY_BOSSES_STARTING_ID					= 7;

// Enemies
const int GiENEMY_UFO_OPEN_TOP							= 0;	// Start at zero so we can use for loops to cycle through all Enemies
const int GiENEMY_UFO_CLOSED_TOP						= 1;
const int GiENEMY_UFO_SMALL								= 2;
const int GiENEMY_UFO_CURVED_WINGS						= 3;
const int GiENEMY_BATTLE_SHIP							= 4;
const int GiENEMY_SLIMY_ALIEN							= 5;
const int GiENEMY_UFO_LARGE_SAUCER						= 6;
const int GiENEMY_GHOUL									= 7;
const int GiENEMY_BLUE_HEAD								= 8;
const int GiENEMY_LIDDLE_BUNNY							= 9;



//********** General Character Information **********

// Velocities at which characters descend when dying (exploding)
const int GiDEATH_X_VELOCITY							= 0;
const int GiDEATH_Y_VELOCITY							= 100;


//********** Player Information **********

// Players default (Level 1) information
const int GiPLAYER_DEFAULT_NUMBER_OF_LIVES				= 2;
const int GiPLAYER_DEFAULT_POINTS						= 0;
const int GiPLAYER_DEFAULT_WEAPON_TYPE					= GiNORMAL_WEAPON;

const int GiPLAYER_LEVEL_1_MAX_X_VELOCITY				= 150;
const int GiPLAYER_LEVEL_1_MAX_Y_VELOCITY				= 150;
const int GiPLAYER_LEVEL_1_MAX_X_ACCELERATION			= 250;
const int GiPLAYER_LEVEL_1_MAX_Y_ACCELERATION			= 250;
const int GiPLAYER_LEVEL_1_MAX_HEALTH					= 500;
const float GfPLAYER_LEVEL_1_SHIELD_STRENGTH			= 1.0;
const float GfPLAYER_LEVEL_1_ARMOR						= 1.0;

// Players Level 2 Upgrade information
const int GiPLAYER_LEVEL_2_MAX_X_VELOCITY				= 225;
const int GiPLAYER_LEVEL_2_MAX_Y_VELOCITY				= 225;
const int GiPLAYER_LEVEL_2_MAX_X_ACCELERATION			= 325;
const int GiPLAYER_LEVEL_2_MAX_Y_ACCELERATION			= 325;
const int GiPLAYER_LEVEL_2_MAX_HEALTH					= 1000;
const float GfPLAYER_LEVEL_2_SHIELD_STRENGTH			= 1.5;
const float GfPLAYER_LEVEL_2_ARMOR						= 1.5;

const int GiPLAYER_LEVEL_2_UPGRADE_MAX_VELOCITY_COST	= 500;
const int GiPLAYER_LEVEL_2_UPGRADE_ACCELERATION_COST	= 500;
const int GiPLAYER_LEVEL_2_UPGRADE_MAX_HEALTH_COST		= 500;
const int GiPLAYER_LEVEL_2_UPGRADE_SHIELD_STRENGTH_COST	= 500;
const int GiPLAYER_LEVEL_2_UPGRADE_ARMOR_COST			= 500;

// Players Level 3 Upgrade information
const int GiPLAYER_LEVEL_3_MAX_X_VELOCITY				= 275;
const int GiPLAYER_LEVEL_3_MAX_Y_VELOCITY				= 275;
const int GiPLAYER_LEVEL_3_MAX_X_ACCELERATION			= 375;
const int GiPLAYER_LEVEL_3_MAX_Y_ACCELERATION			= 375;
const int GiPLAYER_LEVEL_3_MAX_HEALTH					= 1500;
const float GfPLAYER_LEVEL_3_SHIELD_STRENGTH			= 2.0;
const float GfPLAYER_LEVEL_3_ARMOR						= 2.0;

const int GiPLAYER_LEVEL_3_UPGRADE_MAX_VELOCITY_COST	= 1000;
const int GiPLAYER_LEVEL_3_UPGRADE_ACCELERATION_COST	= 1000;
const int GiPLAYER_LEVEL_3_UPGRADE_MAX_HEALTH_COST		= 1000;
const int GiPLAYER_LEVEL_3_UPGRADE_SHIELD_STRENGTH_COST	= 1000;
const int GiPLAYER_LEVEL_3_UPGRADE_ARMOR_COST			= 1000;

// Other Upgrade Costs
const int GiPLAYER_THREE_NORMAL_WEAPON_COST				= 600;
const int GiPLAYER_MACHINE_GUN_WEAPON_COST				= 500;
const int GiPLAYER_THREE_MACHINE_GUN_WEAPON_COST		= 2000;

// Players Max information (Should be equal to highest level Upgrade information where applicable)
const int GiPLAYER_MAX_X_VELOCITY						= 275;
const int GiPLAYER_MAX_Y_VELOCITY						= 275;
const int GiPLAYER_MAX_X_ACCELERATION					= 375;
const int GiPLAYER_MAX_Y_ACCELERATION					= 375;
const int GiPLAYER_MAX_HEALTH							= 1500;
const float GfPLAYER_MAX_SHIELD_STRENGTH				= 2.0;
const float GfPLAYER_MAX_ARMOR							= 2.0;

const int GiPLAYER_MAX_NUMBER_OF_LIVES					= 99;


//********** Enemy Information **********

// For Enemy AI Patterns see the CreateModels() function in Blasteriod.cpp

// UFO Open Top information
const int GiENEMY_UFO_OPEN_TOP_MAX_X_VELOCITY			= 200;
const int GiENEMY_UFO_OPEN_TOP_MAX_Y_VELOCITY			= 200;
const int GiENEMY_UFO_OPEN_TOP_X_ACCELERATION			= 100;
const int GiENEMY_UFO_OPEN_TOP_Y_ACCELERATION			= 100;
const int GiENEMY_UFO_OPEN_TOP_HEALTH					= 15;
const float GfENEMY_UFO_OPEN_TOP_SHIELD_STRENGTH		= 1.0;
const float GfENEMY_UFO_OPEN_TOP_ARMOR					= 1.0;
const int GiENEMY_UFO_OPEN_TOP_NUMBER_OF_LIVES			= 1;
const int GiENEMY_UFO_OPEN_TOP_POINTS					= 10;
const int GiENEMY_UFO_OPEN_TOP_WEAPON_TYPE				= GiMACHINE_GUN_WEAPON;

// UFO Closed Top information
const int GiENEMY_UFO_CLOSED_TOP_MAX_X_VELOCITY			= 100;
const int GiENEMY_UFO_CLOSED_TOP_MAX_Y_VELOCITY			= 100;
const int GiENEMY_UFO_CLOSED_TOP_X_ACCELERATION			= 200;
const int GiENEMY_UFO_CLOSED_TOP_Y_ACCELERATION			= 200;
const int GiENEMY_UFO_CLOSED_TOP_HEALTH					= 20;
const float GfENEMY_UFO_CLOSED_TOP_SHIELD_STRENGTH		= 1.0;
const float GfENEMY_UFO_CLOSED_TOP_ARMOR				= 1.0;
const int GiENEMY_UFO_CLOSED_TOP_NUMBER_OF_LIVES		= 1;
const int GiENEMY_UFO_CLOSED_TOP_POINTS					= 25;
const int GiENEMY_UFO_CLOSED_TOP_WEAPON_TYPE			= GiLASER_WEAPON;

// UFO Small information
const int GiENEMY_UFO_SMALL_MAX_X_VELOCITY				= 300;
const int GiENEMY_UFO_SMALL_MAX_Y_VELOCITY				= 300;
const int GiENEMY_UFO_SMALL_X_ACCELERATION				= 500;
const int GiENEMY_UFO_SMALL_Y_ACCELERATION				= 500;
const int GiENEMY_UFO_SMALL_HEALTH						= 10;
const float GfENEMY_UFO_SMALL_SHIELD_STRENGTH			= 1.0;
const float GfENEMY_UFO_SMALL_ARMOR						= 1.0;
const int GiENEMY_UFO_SMALL_NUMBER_OF_LIVES				= 1;
const int GiENEMY_UFO_SMALL_POINTS						= 15;
const int GiENEMY_UFO_SMALL_WEAPON_TYPE					= GiNORMAL_WEAPON;

// UFO Curved Wings information
const int GiENEMY_UFO_CURVED_WINGS_MAX_X_VELOCITY		= 200;
const int GiENEMY_UFO_CURVED_WINGS_MAX_Y_VELOCITY		= 200;
const int GiENEMY_UFO_CURVED_WINGS_X_ACCELERATION		= 200;
const int GiENEMY_UFO_CURVED_WINGS_Y_ACCELERATION		= 200;
const int GiENEMY_UFO_CURVED_WINGS_HEALTH				= 40;
const float GfENEMY_UFO_CURVED_WINGS_SHIELD_STRENGTH	= 1.0;
const float GfENEMY_UFO_CURVED_WINGS_ARMOR				= 1.0;
const int GiENEMY_UFO_CURVED_WINGS_NUMBER_OF_LIVES		= 1;
const int GiENEMY_UFO_CURVED_WINGS_POINTS				= 50;
const int GiENEMY_UFO_CURVED_WINGS_WEAPON_TYPE			= GiMACHINE_GUN_WEAPON;

// Battle Ship information
const int GiENEMY_BATTLE_SHIP_MAX_X_VELOCITY			= 200;
const int GiENEMY_BATTLE_SHIP_MAX_Y_VELOCITY			= 200;
const int GiENEMY_BATTLE_SHIP_X_ACCELERATION			= 100;
const int GiENEMY_BATTLE_SHIP_Y_ACCELERATION			= 100;
const int GiENEMY_BATTLE_SHIP_HEALTH					= 75;
const float GfENEMY_BATTLE_SHIP_SHIELD_STRENGTH			= 1.0;
const float GfENEMY_BATTLE_SHIP_ARMOR					= 1.0;
const int GiENEMY_BATTLE_SHIP_NUMBER_OF_LIVES			= 1;
const int GiENEMY_BATTLE_SHIP_POINTS					= 75;
const int GiENEMY_BATTLE_SHIP_WEAPON_TYPE				= GiFIREBALL_WEAPON;

// Slimy Alien information
const int GiENEMY_SLIMY_ALIEN_MAX_X_VELOCITY			= 200;
const int GiENEMY_SLIMY_ALIEN_MAX_Y_VELOCITY			= 200;
const int GiENEMY_SLIMY_ALIEN_X_ACCELERATION			= 100;
const int GiENEMY_SLIMY_ALIEN_Y_ACCELERATION			= 100;
const int GiENEMY_SLIMY_ALIEN_HEALTH					= 60;
const float GfENEMY_SLIMY_ALIEN_SHIELD_STRENGTH			= 1.0;
const float GfENEMY_SLIMY_ALIEN_ARMOR					= 1.0;
const int GiENEMY_SLIMY_ALIEN_NUMBER_OF_LIVES			= 1;
const int GiENEMY_SLIMY_ALIEN_POINTS					= 60;
const int GiENEMY_SLIMY_ALIEN_WEAPON_TYPE				= GiLASER_WEAPON;

// UFO Large Saucer information
const int GiENEMY_UFO_LARGE_SAUCER_MAX_X_VELOCITY		= 200;
const int GiENEMY_UFO_LARGE_SAUCER_MAX_Y_VELOCITY		= 200;
const int GiENEMY_UFO_LARGE_SAUCER_X_ACCELERATION		= 100;
const int GiENEMY_UFO_LARGE_SAUCER_Y_ACCELERATION		= 100;
const int GiENEMY_UFO_LARGE_SAUCER_HEALTH				= 90;
const float GfENEMY_UFO_LARGE_SAUCER_SHIELD_STRENGTH	= 1.0;
const float GfENEMY_UFO_LARGE_SAUCER_ARMOR				= 1.0;
const int GiENEMY_UFO_LARGE_SAUCER_NUMBER_OF_LIVES		= 1;
const int GiENEMY_UFO_LARGE_SAUCER_POINTS				= 100;
const int GiENEMY_UFO_LARGE_SAUCER_WEAPON_TYPE			= GiFIREBALL_WEAPON;

// Ghoul information
const int GiENEMY_GHOUL_MAX_X_VELOCITY					= 200;
const int GiENEMY_GHOUL_MAX_Y_VELOCITY					= 200;
const int GiENEMY_GHOUL_X_ACCELERATION					= 100;
const int GiENEMY_GHOUL_Y_ACCELERATION					= 100;
const int GiENEMY_GHOUL_HEALTH							= 750;
const float GfENEMY_GHOUL_SHIELD_STRENGTH				= 1.0;
const float GfENEMY_GHOUL_ARMOR							= 1.0;
const int GiENEMY_GHOUL_NUMBER_OF_LIVES					= 1;
const int GiENEMY_GHOUL_POINTS							= 750;
const int GiENEMY_GHOUL_WEAPON_TYPE						= GiLASER_WEAPON;

// Blue Top enemy information
const int GiENEMY_BLUE_HEAD_MAX_X_VELOCITY				= 100;
const int GiENEMY_BLUE_HEAD_MAX_Y_VELOCITY				= 100;
const int GiENEMY_BLUE_HEAD_X_ACCELERATION				= 200;
const int GiENEMY_BLUE_HEAD_Y_ACCELERATION				= 200;
const int GiENEMY_BLUE_HEAD_HEALTH						= 1500;
const float GfENEMY_BLUE_HEAD_SHIELD_STRENGTH			= 1.0;
const float GfENEMY_BLUE_HEAD_ARMOR						= 1.0;
const int GiENEMY_BLUE_HEAD_NUMBER_OF_LIVES				= 1;
const int GiENEMY_BLUE_HEAD_POINTS						= 1000;
const int GiENEMY_BLUE_HEAD_WEAPON_TYPE					= GiFIREBALL_WEAPON;

// Liddle Bunny information
const int GiENEMY_LIDDLE_BUNNY_MAX_X_VELOCITY			= 300;
const int GiENEMY_LIDDLE_BUNNY_MAX_Y_VELOCITY			= 300;
const int GiENEMY_LIDDLE_BUNNY_X_ACCELERATION			= 400;
const int GiENEMY_LIDDLE_BUNNY_Y_ACCELERATION			= 400;
const int GiENEMY_LIDDLE_BUNNY_HEALTH					= 4000;
const float GfENEMY_LIDDLE_BUNNY_SHIELD_STRENGTH		= 1.0;
const float GfENEMY_LIDDLE_BUNNY_ARMOR					= 1.0;
const int GiENEMY_LIDDLE_BUNNY_NUMBER_OF_LIVES			= 1;
const int GiENEMY_LIDDLE_BUNNY_POINTS					= 2000;
const int GiENEMY_LIDDLE_BUNNY_WEAPON_TYPE				= GiBOMB_WEAPON;

// NOTE: When adding new enemies don't foget to add their info to the Arrays below


//********* Enemy Information Arrays **********

// The array contents must stay in the same order as the Enemy ID's
// These are used to fill the Enemy Model information in a loop

// Enemies Max X Velocity
const int GiENEMY_MAX_X_VELOCITY[GiTOTAL_NUMBER_OF_ENEMIES_IN_GAME] = 
{GiENEMY_UFO_OPEN_TOP_MAX_X_VELOCITY, GiENEMY_UFO_CLOSED_TOP_MAX_X_VELOCITY, 
 GiENEMY_UFO_SMALL_MAX_X_VELOCITY, GiENEMY_UFO_CURVED_WINGS_MAX_X_VELOCITY, 
 GiENEMY_BATTLE_SHIP_MAX_X_VELOCITY, GiENEMY_SLIMY_ALIEN_MAX_X_VELOCITY, 
 GiENEMY_UFO_LARGE_SAUCER_MAX_X_VELOCITY, GiENEMY_GHOUL_MAX_X_VELOCITY, 
 GiENEMY_BLUE_HEAD_MAX_X_VELOCITY, GiENEMY_LIDDLE_BUNNY_MAX_X_VELOCITY};

// Enemies Max Y Velocity
const int GiENEMY_MAX_Y_VELOCITY[GiTOTAL_NUMBER_OF_ENEMIES_IN_GAME] = 
{GiENEMY_UFO_OPEN_TOP_MAX_Y_VELOCITY, GiENEMY_UFO_CLOSED_TOP_MAX_Y_VELOCITY, 
 GiENEMY_UFO_SMALL_MAX_Y_VELOCITY, GiENEMY_UFO_CURVED_WINGS_MAX_Y_VELOCITY, 
 GiENEMY_BATTLE_SHIP_MAX_Y_VELOCITY, GiENEMY_SLIMY_ALIEN_MAX_Y_VELOCITY, 
 GiENEMY_UFO_LARGE_SAUCER_MAX_Y_VELOCITY, GiENEMY_GHOUL_MAX_Y_VELOCITY, 
 GiENEMY_BLUE_HEAD_MAX_Y_VELOCITY, GiENEMY_LIDDLE_BUNNY_MAX_Y_VELOCITY};

// Enemies Max X Acceleration
const int GiENEMY_X_ACCELERATION[GiTOTAL_NUMBER_OF_ENEMIES_IN_GAME] =
{GiENEMY_UFO_OPEN_TOP_X_ACCELERATION, GiENEMY_UFO_CLOSED_TOP_X_ACCELERATION, 
 GiENEMY_UFO_SMALL_X_ACCELERATION, GiENEMY_UFO_CURVED_WINGS_X_ACCELERATION, 
 GiENEMY_BATTLE_SHIP_X_ACCELERATION, GiENEMY_SLIMY_ALIEN_X_ACCELERATION, 
 GiENEMY_UFO_LARGE_SAUCER_X_ACCELERATION, GiENEMY_GHOUL_X_ACCELERATION, 
 GiENEMY_BLUE_HEAD_X_ACCELERATION, GiENEMY_LIDDLE_BUNNY_X_ACCELERATION};

// Enemies Max Y Acceleration
const int GiENEMY_Y_ACCELERATION[GiTOTAL_NUMBER_OF_ENEMIES_IN_GAME] =
{GiENEMY_UFO_OPEN_TOP_Y_ACCELERATION, GiENEMY_UFO_CLOSED_TOP_Y_ACCELERATION, 
 GiENEMY_UFO_SMALL_Y_ACCELERATION, GiENEMY_UFO_CURVED_WINGS_Y_ACCELERATION, 
 GiENEMY_BATTLE_SHIP_Y_ACCELERATION, GiENEMY_SLIMY_ALIEN_Y_ACCELERATION, 
 GiENEMY_UFO_LARGE_SAUCER_Y_ACCELERATION, GiENEMY_GHOUL_Y_ACCELERATION, 
 GiENEMY_BLUE_HEAD_Y_ACCELERATION, GiENEMY_LIDDLE_BUNNY_Y_ACCELERATION};

// Enemies Health
const int GiENEMY_HEALTH[GiTOTAL_NUMBER_OF_ENEMIES_IN_GAME] = 
{GiENEMY_UFO_OPEN_TOP_HEALTH, GiENEMY_UFO_CLOSED_TOP_HEALTH, 
 GiENEMY_UFO_SMALL_HEALTH, GiENEMY_UFO_CURVED_WINGS_HEALTH, 
 GiENEMY_BATTLE_SHIP_HEALTH, GiENEMY_SLIMY_ALIEN_HEALTH, 
 GiENEMY_UFO_LARGE_SAUCER_HEALTH, GiENEMY_GHOUL_HEALTH, 
 GiENEMY_BLUE_HEAD_HEALTH, GiENEMY_LIDDLE_BUNNY_HEALTH};

// Enemies Shield Strength
const float GfENEMY_SHIELD_STRENGTH[GiTOTAL_NUMBER_OF_ENEMIES_IN_GAME] = 
{GfENEMY_UFO_OPEN_TOP_SHIELD_STRENGTH, GfENEMY_UFO_CLOSED_TOP_SHIELD_STRENGTH, 
 GfENEMY_UFO_SMALL_SHIELD_STRENGTH, GfENEMY_UFO_CURVED_WINGS_SHIELD_STRENGTH, 
 GfENEMY_BATTLE_SHIP_SHIELD_STRENGTH, GfENEMY_SLIMY_ALIEN_SHIELD_STRENGTH, 
 GfENEMY_UFO_LARGE_SAUCER_SHIELD_STRENGTH, GfENEMY_GHOUL_SHIELD_STRENGTH, 
 GfENEMY_BLUE_HEAD_SHIELD_STRENGTH, GfENEMY_LIDDLE_BUNNY_SHIELD_STRENGTH};

// Enemies Armor
const float GfENEMY_ARMOR[GiTOTAL_NUMBER_OF_ENEMIES_IN_GAME] = 
{GfENEMY_UFO_OPEN_TOP_ARMOR, GfENEMY_UFO_CLOSED_TOP_ARMOR, 
 GfENEMY_UFO_SMALL_ARMOR, GfENEMY_UFO_CURVED_WINGS_ARMOR, 
 GfENEMY_BATTLE_SHIP_ARMOR, GfENEMY_SLIMY_ALIEN_ARMOR, 
 GfENEMY_UFO_LARGE_SAUCER_ARMOR, GfENEMY_GHOUL_ARMOR, 
 GfENEMY_BLUE_HEAD_ARMOR, GfENEMY_LIDDLE_BUNNY_ARMOR};

// Enemies Number Of Lives
const int GiENEMY_NUMBER_OF_LIVES[GiTOTAL_NUMBER_OF_ENEMIES_IN_GAME] = 
{GiENEMY_UFO_OPEN_TOP_NUMBER_OF_LIVES, GiENEMY_UFO_CLOSED_TOP_NUMBER_OF_LIVES, 
 GiENEMY_UFO_SMALL_NUMBER_OF_LIVES, GiENEMY_UFO_CURVED_WINGS_NUMBER_OF_LIVES, 
 GiENEMY_BATTLE_SHIP_NUMBER_OF_LIVES, GiENEMY_SLIMY_ALIEN_NUMBER_OF_LIVES, 
 GiENEMY_UFO_LARGE_SAUCER_NUMBER_OF_LIVES, GiENEMY_GHOUL_NUMBER_OF_LIVES, 
 GiENEMY_BLUE_HEAD_NUMBER_OF_LIVES, GiENEMY_LIDDLE_BUNNY_NUMBER_OF_LIVES};

// Enemies Number Of Points they are worth
const int GiENEMY_POINTS[GiTOTAL_NUMBER_OF_ENEMIES_IN_GAME] = 
{GiENEMY_UFO_OPEN_TOP_POINTS, GiENEMY_UFO_CLOSED_TOP_POINTS, 
 GiENEMY_UFO_SMALL_POINTS, GiENEMY_UFO_CURVED_WINGS_POINTS, 
 GiENEMY_BATTLE_SHIP_POINTS, GiENEMY_SLIMY_ALIEN_POINTS, 
 GiENEMY_UFO_LARGE_SAUCER_POINTS, GiENEMY_GHOUL_POINTS, 
 GiENEMY_BLUE_HEAD_POINTS, GiENEMY_LIDDLE_BUNNY_POINTS};

// Enemies Weapon Types
const int GiENEMY_WEAPON_TYPE[GiTOTAL_NUMBER_OF_ENEMIES_IN_GAME] = 
{GiENEMY_UFO_OPEN_TOP_WEAPON_TYPE, GiENEMY_UFO_CLOSED_TOP_WEAPON_TYPE, 
 GiENEMY_UFO_SMALL_WEAPON_TYPE, GiENEMY_UFO_CURVED_WINGS_WEAPON_TYPE, 
 GiENEMY_BATTLE_SHIP_WEAPON_TYPE, GiENEMY_SLIMY_ALIEN_WEAPON_TYPE, 
 GiENEMY_UFO_LARGE_SAUCER_WEAPON_TYPE, GiENEMY_GHOUL_WEAPON_TYPE, 
 GiENEMY_BLUE_HEAD_WEAPON_TYPE, GiENEMY_LIDDLE_BUNNY_WEAPON_TYPE};


#endif
