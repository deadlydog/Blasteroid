/*****************************************************************************/
/* GlobalLevelInfo.h                                                         */
/* Written by Daniel Schroeder                                               */
/* Created on July 15, 2005                                                  */
/* Last Updated July 19, 2005                                                */
/*---------------------------------------------------------------------------*/
/*   This file specifies all of the global data pertaining to the games      */
/* levels, such as background pictures, the number of stars to display, and  */
/* any other global data deals with the levels.                              */
/*****************************************************************************/

#ifndef GLOBAL_LEVEL_INFO_H
#define GLOBAL_LEVEL_INFO_H


// The Total Number Of Levels in the Game
const int GiTOTAL_NUMBER_OF_LEVELS_IN_GAME				= 3;


//***** General Level Globals*****

const int GiMAX_NUM_OF_BACKGROUND_STARS					= 500;

// GiMAX_VELOCITY_OF_BACKGROUND_STARS + GiBASE_STAR_BRIGHTNESS should not be greater than 255


//***** Specific Level Globals *****

// Menus info
const int GiNUM_OF_MENU_BACKGROUND_STARS				= 400;
const int GiMAX_VELOCITY_OF_MENU_BACKGROUND_STARS		= 400;
const int GiMENU_BASE_STAR_BRIGHTNESS					= 50;

const int GiNUM_OF_STORY_BACKGROUND_STARS				= 150;
const int GiNUM_OF_SPACE_STATION_BACKGROUND_STARS		= 200;

const int GiMENU_UP_DELAY_TIME							= 250;
const int GiMENU_DOWN_DELAY_TIME						= 250;
const int GiMENU_LEFT_DELAY_TIME						= 250;
const int GiMENU_RIGHT_DELAY_TIME						= 250;
const int GiMENU_SELECT_DELAY_TIME						= 500;


// Level 1 info
const int GiNUM_OF_LEVEL_1_BACKGROUND_STARS				= 100;	// Number of Background Stars
const int GiMAX_VELOCITY_OF_LEVEL_1_BACKGROUND_STARS	= 150;	// Max Background Star Velocity
const int GiLEVEL_1_BASE_STAR_BRIGHTNESS				= 85;	// Stars will be at least this bright

const int GiLEVEL_1_X_FRICTION							= 0;	// X Friction for all objects
const int GiLEVEL_1_Y_FRICTION							= 0;	// Y Friction for all objects


// Level 2 info
const int GiNUM_OF_LEVEL_2_BACKGROUND_STARS				= 200;	// Number of Background Stars
const int GiMAX_VELOCITY_OF_LEVEL_2_BACKGROUND_STARS	= 165;	// Max Background Star Velocity
const int GiLEVEL_2_BASE_STAR_BRIGHTNESS				= 70;	// Stars will be at least this bright

const int GiLEVEL_2_X_FRICTION							= 0;	// X Friction for all objects
const int GiLEVEL_2_Y_FRICTION							= 0;	// Y Friction for all objects


// Level 3 info
const int GiNUM_OF_LEVEL_3_BACKGROUND_STARS				= 250;	// Number of Background Stars
const int GiMAX_VELOCITY_OF_LEVEL_3_BACKGROUND_STARS	= 185;	// Max Background Star Velocity
const int GiLEVEL_3_BASE_STAR_BRIGHTNESS				= 50;	// Stars will be at least this bright

const int GiLEVEL_3_X_FRICTION							= 0;	// X Friction for all objects
const int GiLEVEL_3_Y_FRICTION							= 0;	// Y Friction for all objects


#endif

