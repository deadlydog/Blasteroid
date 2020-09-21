/*****************************************************************************/
/* GlobalGameInfo.h                                                          */
/* Written by Daniel Schroeder                                               */
/* Created on July 15, 2005                                                  */
/* Last Updated July 19, 2005                                                */
/*---------------------------------------------------------------------------*/
/*   This file specifies all of the general global game data. Things such as */
/* the max number of players, the total number of enemies in the game, and   */
/* any other game related data that does not fit in the other Global*.h files*/
/*****************************************************************************/

#ifndef GLOBAL_GAME_INFO_H
#define GLOBAL_GAME_INFO_H

//********** Game Globals **********

// Max number of simultaneous players
const int GiMAX_NUMBER_OF_PLAYERS					= 2;

// Time delay before accepting another Escape key press
const int GiESC_KEY_PRESS_DELAY						= 500;

// Time delay before accepting another ShowFPS key press
const int GiSHOW_FPS_KEY_PRESS_DELAY				= 500;


#endif

