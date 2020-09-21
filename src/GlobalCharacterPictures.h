/*****************************************************************************/
/* GlobalCharacterPictures.h                                                 */
/* Written by Daniel Schroeder                                               */
/* Created on July 15, 2005                                                  */
/* Last Updated July 19, 2005                                                */
/*---------------------------------------------------------------------------*/
/*   This file specifies all of the global data pertaining to characters     */
/* Pictures and States (See CCharacterPicture.h).                            */
/*****************************************************************************/

#ifndef GLOBAL_CHARACTER_PICTURES_H
#define GLOBAL_CHARACTER_PICTURES_H

//********** Picture Globals **********

// Ship States
const int GiSHIP_STATE_IDLE							= 0;
const int GiSHIP_STATE_FORWARD						= 1;
const int GiSHIP_STATE_BACKWARDS					= 2;
const int GiSHIP_STATE_DEATH						= 3;


// Ship Pictures
const int GiSHIP_PICTURE_ZERO_IMPULSE				= 0;
const int GiSHIP_PICTURE_HALF_IMPULSE				= 1;
const int GiSHIP_PICTURE_FULL_IMPULSE				= 2;

// Used for both Ships and Enemies - High numbers to make sure there are no conflicts with other Pictures
const int GiSHIP_PICTURE_DEATH_1					= 1000;
const int GiSHIP_PICTURE_DEATH_2					= 1001;
const int GiSHIP_PICTURE_DEATH_3					= 1002;
const int GiSHIP_PICTURE_DEATH_4					= 1003;
const int GiSHIP_PICTURE_DEATH_5					= 1004;
const int GiSHIP_PICTURE_DEATH_6					= 1005;
const int GiSHIP_PICTURE_DEATH_7					= 1006;
const int GiSHIP_PICTURE_DEATH_DONE					= 1006;


// Enemy States
const int GiENEMY_STATE_MOVING						= 0;
const int GiENEMY_STATE_DEATH						= 1;


// Enemy Pictures
const int GiENEMY_PICTURE_1							= 0;
const int GiENEMY_PICTURE_2							= 1;
const int GiENEMY_PICTURE_3							= 2;
const int GiENEMY_PICTURE_4							= 3;
const int GiENEMY_PICTURE_5							= 4;
const int GiENEMY_PICTURE_6							= 5;
const int GiENEMY_PICTURE_7							= 6;
const int GiENEMY_PICTURE_8							= 7;
const int GiENEMY_PICTURE_9							= 8;
const int GiENEMY_PICTURE_10						= 9;
const int GiENEMY_PICTURE_11						= 10;
const int GiENEMY_PICTURE_12						= 11;


// Bullet States
const int GiBULLET_STATE_TRAVELING					= 0;
const int GiBULLET_STATE_DETONATING					= 1;


// Bullet Pictures
const int GiBULLET_PICTURE_TRAVELING_1				= 0;
const int GiBULLET_PICTURE_TRAVELING_2				= 1;
const int GiBULLET_PICTURE_TRAVELING_3				= 2;
const int GiBULLET_PICTURE_TRAVELING_4				= 3;
const int GiBULLET_PICTURE_TRAVELING_5				= 4;
const int GiBULLET_PICTURE_TRAVELING_6				= 5;
const int GiBULLET_PICTURE_TRAVELING_7				= 6;
const int GiBULLET_PICTURE_TRAVELING_8				= 7;
const int GiBULLET_PICTURE_TRAVELING_9				= 8;

const int GiBULLET_PICTURE_DETONATING_1				= 9;
const int GiBULLET_PICTURE_DETONATING_2				= 10;
const int GiBULLET_PICTURE_DETONATING_3				= 11;
const int GiBULLET_PICTURE_DETONATING_4				= 12;
const int GiBULLET_PICTURE_DETONATING_5				= 13;
const int GiBULLET_PICTURE_DETONATING_6				= 14;
const int GiBULLET_PICTURE_DETONATING_7				= 15;
const int GiBULLET_PICTURE_DETONATING_8				= 16;
const int GiBULLET_PICTURE_DETONATING_9				= 17;
const int GiBULLET_PICTURE_DETONATING_DONE			= 18;


// Space Station States
const int GiSPACE_STATION_STATE_REVOLVING			= 0;


// Space Station Pictures
const int GiSPACE_STATION_PICTURE_REVOLUTION_1		= 0;
const int GiSPACE_STATION_PICTURE_REVOLUTION_2		= 1;
const int GiSPACE_STATION_PICTURE_REVOLUTION_3		= 2;
const int GiSPACE_STATION_PICTURE_REVOLUTION_4		= 3;
const int GiSPACE_STATION_PICTURE_REVOLUTION_5		= 4;
const int GiSPACE_STATION_PICTURE_REVOLUTION_6		= 5;


#endif

