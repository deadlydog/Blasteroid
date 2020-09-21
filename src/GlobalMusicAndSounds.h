/*****************************************************************************/
/* GlobalMusicAndSounds.h                                                    */
/* Written by Daniel Schroeder                                               */
/* Created on August 16, 2005                                                */
/* Last Updated August 17, 2005                                              */
/*---------------------------------------------------------------------------*/
/*   This file specifies all of the Music and Sound IDs, including the file  */
/* names, and how many music and sound effects there are.                    */
/*****************************************************************************/

#ifndef GLOBAL_MUSIC_AND_SOUNDS_H
#define	GLOBAL_MUSIC_AND_SOUNDS_H

// Max Sound File Name Length
const int GiMAX_SOUND_FILE_NAME_LENGTH							= 50;


// Sound Globals

// Number Of Sounds that can play Simultaneously
const int GiNUMBER_OF_SIMULTANEOUS_SOUNDS						= 32;

// Total Number Of Sounds in the Game
const int GiNUMBER_OF_SOUNDS_IN_GAME							= 10;

// Sound ID's
const int GiChangeSelectionSound								= 0;
const int GiSelectionChosenSound								= 1;
const int GiUpgradePurchasedSound								= 2;
const int GiInvalidSelectionSound								= 3;
const int GiBombExplosionSound									= 4;
const int GiPickupItemSound										= 5;
const int GiEnemyDeathSound										= 6;
const int GiEnemyBossDeathSound									= 7;
const int GiBulletHitPlayerSound								= 8;
const int GiPlayerDeathSound									= 9;

// Menu Sounds
char caChangeSelectionSound[GiMAX_SOUND_FILE_NAME_LENGTH]		= "Data\\Sounds\\Pop.wav";
char caSelectionChosenSound[GiMAX_SOUND_FILE_NAME_LENGTH]		= "Data\\Sounds\\Turn Table.wav";
char caUpgradePurchasedSound[GiMAX_SOUND_FILE_NAME_LENGTH]		= "Data\\Sounds\\Upgrade Purchased.wav";
char caInvalidSelectionSound[GiMAX_SOUND_FILE_NAME_LENGTH]		= "Data\\Sounds\\Invalid Selection.wav";

// Play Sounds
char caBombExplosionSound[GiMAX_SOUND_FILE_NAME_LENGTH]			= "Data\\Sounds\\Bomb Explosion.wav";
char caPickupItemSound[GiMAX_SOUND_FILE_NAME_LENGTH]			= "Data\\Sounds\\Pickup Item.wav";
char caEnemyDeathSound[GiMAX_SOUND_FILE_NAME_LENGTH]			= "Data\\Sounds\\Enemy Death.wav";
char caEnemyBossDeathSound[GiMAX_SOUND_FILE_NAME_LENGTH]		= "Data\\Sounds\\Boss Death.wav";
char caBulletHitPlayerSound[GiMAX_SOUND_FILE_NAME_LENGTH]		= "Data\\Sounds\\Bullet Hit Something.wav";
char caPlayerDeathSound[GiMAX_SOUND_FILE_NAME_LENGTH]			= "Data\\Sounds\\Slurp.wav";



// Music Globals

// Total Number Of Music files in the Game
const int GiNUMBER_OF_MUSIC_IN_GAME								= 8;

// Music ID's
const int GiGameIntroMusic										= 0;
const int GiMainStoryMusic										= 1;
const int GiLevel1Music											= 2;
const int GiLevel2Music											= 3;
const int GiLevel3Music											= 4;
const int GiEnemyBossMusic										= 5;
const int GiEndBossMusic										= 6;
const int GiBeatGameMusic										= 7;

// Music files
char caGameIntroMusic[GiMAX_SOUND_FILE_NAME_LENGTH]				= "Data\\Music\\Game Intro.mp3";
char caMainStoryMusic[GiMAX_SOUND_FILE_NAME_LENGTH]				= "Data\\Music\\Main Story.mid";
char caLevel1Music[GiMAX_SOUND_FILE_NAME_LENGTH]				= "Data\\Music\\Level 1.mid";
char caLevel2Music[GiMAX_SOUND_FILE_NAME_LENGTH]				= "Data\\Music\\Level 2.mid";
char caLevel3Music[GiMAX_SOUND_FILE_NAME_LENGTH]				= "Data\\Music\\Level 3.mid";
char caEnemyBossMusic[GiMAX_SOUND_FILE_NAME_LENGTH]				= "Data\\Music\\Enemy Boss.mid";
char caEndBossMusic[GiMAX_SOUND_FILE_NAME_LENGTH]				= "Data\\Music\\End Boss.mid";
char caBeatGameMusic[GiMAX_SOUND_FILE_NAME_LENGTH]				= "Data\\Music\\Beat Game.mid";


#endif

