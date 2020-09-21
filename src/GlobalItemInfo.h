/*****************************************************************************/
/* GlobalLevelInfo.h                                                         */
/* Written by Daniel Schroeder                                               */
/* Created on July 26, 2005                                                  */
/* Last Updated July 26, 2005                                                */
/*---------------------------------------------------------------------------*/
/*   This file specifies all of the global data pertaining to the games      */
/* levels, such as background pictures, the number of stars to display, and  */
/* any other global data deals with the levels.                              */
/*****************************************************************************/

#ifndef GLOBAL_ITEM_INFO_H
#define GLOBAL_ITEM_INFO_H

// Items Y Velocity
const int GiITEM_Y_VELOCITY							= 100;

// Total number of different Types of Items in the game
const int GiTOTAL_NUMBER_OF_TYPES_OF_ITEMS_IN_GAME	= 11;

// Item ID's
const int GiITEM_FREE_SHIP							= 0;
const int GiITEM_RESTORE_HEALTH_100					= 1;
const int GiITEM_RESTORE_HEALTH_500					= 2;
const int GiITEM_RESTORE_HEALTH_1000				= 3;
const int GiITEM_RESTORE_HEALTH_FULL				= 4;
const int GiITEM_INCREASE_ARMOR						= 5;
const int GiITEM_INCREASE_MAX_VELOCITY				= 6;
const int GiITEM_INCREASE_ACCELERATION				= 7;
const int GiITEM_FIREBALL_WEAPON_50					= 8;
const int GiITEM_LASER_WEAPON_75					= 9;
const int GiITEM_BOMB_WEAPON_15						= 10;


// Items only have one Picture
const int GiITEM_PICTURE							= 0;

#endif

