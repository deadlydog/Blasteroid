/*****************************************************************************/
/* Blasteriod.h (and Blasteroid.cpp)                                         */
/* Written by Daniel Schroeder                                               */
/* Created in June 2004                                                      */
/* Last Updated August 21, 2005                                              */
/*---------------------------------------------------------------------------*/
/*   These files control all of the game major game functions and states.    */
/* If you are looking to change something about the way the game play and    */
/* game flow work, these are the files to change.                            */
/*****************************************************************************/

#ifndef BLASTERIOD_H
#define BLASTERIOD_H

#include "Core_Global.h"
#include "CStateManager.h"
#include "CTimer.h"
#include "CCharacterPicture.h"
#include "CShip.h"
#include "CShipList.h"
#include "CBullet.h"
#include "CBulletList.h"
#include "CItem.h"
#include "CItemList.h"
#include "CObjectPrototypeList.h"
#include "CHighScores.h"
#include "Globals.h"
#include "resource.h"

// Include FMOD headers for sound
#include "fmod.hpp"
#include "fmod_errors.h"


//********** Application Globals **********

const int GiWINDOW_WIDTH	= 640;
const int GiWINDOW_HEIGHT	= 480;


//********** Structures **********

// Define Vertex Structure and associated VertexFVF
struct SVertex
{
    float fX, fY, fZ, fRHW;
    D3DCOLOR diffuse;
    float fXVelocity, fYVelocity;
};
#define VERTEXFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

// Define Texture Vertex Structure and associated VertexFVF
struct STextureVertex
{
    float fX, fY, fZ, fRHW;		// xy Coordinates for 2D
    float tu, tv;
};
#define TEXTURE_VERTEXFVF (D3DFVF_XYZRHW | D3DFVF_TEX1)

// Class that holds all Game info
class CApp : public cApplication
{
	public:
		
		CApp();						// Constructor
   
		BOOL Init();				// Initializes game resources and devices
		BOOL Shutdown();			// Shuts down game resources and devices
		BOOL Frame();				// Performs work for this frame

		bool mbFullScreen;			// Tells if the game should run in Full Screen or Windowed mode
		bool mbUsingMouse;			// Tells if the user is currently using the Mouse or Keyboard
		bool mbUsingMouseLastFrame;	// Used to tell if the Mouse was showing last frame or not 

		// Create Players' Ships
		CShip mcaPlayer[GiMAX_NUMBER_OF_PLAYERS];
		
		CShipList				mcActiveShipList;		// Holds all Ships active on the screen (including Players)
		CObjectPrototypeList	mcInactiveEnemyList;	// Holds Enemies and Items not yet on the screen
		CObjectPrototypeList	mcRandomEnemyList;		// Holds a List of Random Enemies to choose from
		
		CTimer	mcNewScriptedEnemyTimer;	// Used to tell when to generate a new scripted enemy
		CTimer	mcNewRandomEnemyTimer;		// Used to tell when to generate a new random enemy
   
		CBulletList	mcFiredBulletsList;		// Keeps track of all Bullets that have been fired

		CItemList	mcActiveItemList;		// Keeps track of Items on the screen

		// Input Timers - used to make sure input for same key is not taken too often
		CTimer	mcUpDelayTimer;
		CTimer	mcDownDelayTimer;
		CTimer	mcLeftDelayTimer;
		CTimer	mcRightDelayTimer;
		CTimer	mcSelectDelayTimer;
		CTimer	mcESCDelayTimer;
   
		// Bullet 'models'
		CBullet mcaBulletModel[GiTOTAL_NUMBER_OF_WEAPONS_IN_GAME];

		// Item 'models'
		CItem mcaItemModel[GiTOTAL_NUMBER_OF_TYPES_OF_ITEMS_IN_GAME];

		// Players Ship 'models'
		CShip	mcaPlayerShipModel[GiMAX_NUMBER_OF_PLAYERS];
   
		// Enemy Ship 'models'
		CShip	mcaEnemyModel[GiTOTAL_NUMBER_OF_ENEMIES_IN_GAME];


		// Space Station Model
		CCharacterPicture mcSpaceStationModel;


		// Returns how much time has passed since the last Frame
		unsigned long ReturnAmountOfTimeSinceLastFrame();
   
	private:

		// Graphics and Font devices
		cGraphics       mcGraphics;
		cTexture		mcTexture[GiNUMBER_OF_TEXTURES_IN_GAME];
		cFont           mcFontTitle;			// Used to display "Daniel Schroeder" on Main Menu
		cFont			mcFontRegular;			// Used for all normal text
		cFont			mcFPSFont;				// Used to display FPS info
		cVertexBuffer	mcTextureVertexBuffer;
		cVertexBuffer	mcVertexBuffer;

		// Input devices
		cInput          mcInput;
		cInputDevice    mcKeyboard;
		cInputDevice    mcMouse;

		// Sound device and channels
		FMOD::System*	mcpSoundSystem;
		FMOD::Sound*	mcpSound[GiNUMBER_OF_SOUNDS_IN_GAME];
		FMOD::Sound*	mcpMusic[GiNUMBER_OF_MUSIC_IN_GAME];
		FMOD::Channel*  mcpSoundChannel;
		FMOD::Channel*	mcpMusicChannel;
/*
		cSound          mcSound;
		cSoundChannel   mcSoundChannel[GiNUMBER_OF_SOUND_CHANNELS];
		cMusicChannel   mcMusicChannel;
  
		// Networking Devices
		cNetworkAdapter	mcNetworkAdapter;
		cNetworkServer	mcNetworkServer;
		cNetworkClient	mcNetworkClient;
*/   
		// State Managers
		CStateManager	mcGameStateManager;
   
		// Specify Clock pointer - Used to get how much time elapsed between Frames and by CTimers
		CClock*			mcpClock;

		// High Scores Holder
		CHighScores		mcHighScores;

		// Controls
		unsigned char	mucKeyboardUp;
		unsigned char	mucKeyboardDown;
		unsigned char	mucKeyboardLeft;
		unsigned char	mucKeyboardRight;
		unsigned char	mucKeyboardFire;
		unsigned char	mucKeyboardNextWeapon;
		unsigned char	mucKeyboardPreviousWeapon;
		unsigned char	mucKeyboardDetonateBullet;

		unsigned char	mucMouseFire;
		unsigned char	mucMouseNextWeapon;
		unsigned char	mucMouseDetonateBullet;

		// Number of Players in game
		int miNumberOfPlayers;

		// Level Player is Currently on
		int miCurrentLevel;

		// Tells if Levels Boss has been killed or not
		bool mbLevelComplete;

		// Delta time since last Frame
		unsigned long	mulAmountOfTimeSinceLastFrame;

		// FPS (Frame Per Second) objects
		CTimer	mcShowFPSTimer;			// Used to make sure enough time has passed before accepting ShowFPS key press
		CTimer	mcFPSTimer;				// Used in calculating the FPS
		CTimer	mcLastFrameTimer;		// Used in calculating how much time has passed since last frame
		bool	mbShowFPS;				// Tells whether FPS should be displayed or not
		EPurpose meFPSPurpose;			// Tells the Purpose of the ShowFPS() function
		unsigned int muiBestFPS;		// Holds the Best FPS so far
		unsigned int muiWorstFPS;		// Holds the Worst FPS so far
		void	ShowFPS(void* vpApp, EPurpose ePurpose);// Displays the FPS (Frame Per Second)

		// Random Enemy variables
		unsigned long mulNewRandomEnemyTime;
		unsigned long mulMinRandomEnemyTime;
		unsigned long mulMaxRandomEnemyTime;
		int miRandomEnemyLikeliness;

		// Level Friction variables
		int miLevelXFriction;
		int miLevelYFriction;

		//***** Game State Functions *****

		// Displays the Start Menu
		static void GameStateMainMenu(void* vpOptionalDataToPass, EPurpose ePurpose);
		
		// Starts the game
		static void GameStatePlay(void* vpOptionalDataToPass, EPurpose ePurpose);
		
		// Displays the Main Storyline
		static void GameStateMainStory(void* vpOptionalDataToPass, EPurpose ePurpose);
		
		// Displays the High Scores
		static void GameStateDisplayHighScores(void* vpOptionalDataToPass, EPurpose ePurpose);

		// Allows Player to Enter a High Score
		static void GameStateEnterHighScore(void* vpOptionalDataToPass, EPurpose ePurpose);

		// Displays the controls
		static void GameStateControls(void* vpOptionalDataToPass, EPurpose ePurpose);

		// Displays mini Storylines encountered while playing
		static void GameStateSideStory(void* vpOptionalDataToPass, EPurpose ePurpose);
		
		// Displays Space Station to buy stuff
		static void GameStateSpaceStation(void* vpOptionalDataToPass, EPurpose ePurpose);

		// Pauses the Game (Can only pause from GameStatePlay)
		static void GameStatePause(void* vpOptionalDataToPass, EPurpose ePurpose);

		// Displays the End Story
		static void GameStatePlayerBeatGame(void* vpOptionalDataToPass, EPurpose ePurpose);


		//***** Game State Play() Functions *****

		// Displays level background and generates new enemies
		void DoLevelStuff(void* vpApp, EPurpose ePurpose = eNormal);

		// Gets and Processes Players Input
		void GetAndProcessInput(void* vpApp, EPurpose ePurpose = eNormal);

		// Updates all Objects (Players, Enemies, items, etc)
		void UpdateObjects(void* vpApp, EPurpose ePurpose = eNormal);

		// Tests for Collisions and Processes the results
		void ProcessCollisionsAndObjects(void* vpApp, EPurpose ePurpose = eNormal);

		// Displays Screen Information, such as players current weapon, score, lives left, etc
		void DisplayScreenInfo(void* vpApp, EPurpose ePurpose = eNormal);


		//***** Do Level Stuff() Functions *****
		
		// Level Functions - Sets up specific levels
		void Level1(void* vpApp, EPurpose ePurpose = eNormal);
		void Level2(void* vpApp, EPurpose ePurpose = eNormal);
		void Level3(void* vpApp, EPurpose ePurpose = eNormal);

		// Loads Enemies from File into mcInactiveEnemyList
		void LoadEnemiesFromFile(void* vpApp, char* cpFileName);


		//***** Other Functions *****

		// Set and Return Number of Players (either 1 or 2)
		bool SetNumberOfPlayers(int iNumOfPlayers = 1);
		int ReturnNumberOfPlayers() const;

		// Set/Return/Increment Level player is currently on
		void SetCurrentLevel(int iCurrentLevel);
		int ReturnCurrentLevel() const;
		void IncrementCurrentLevel();

		// Returns True if there is a Collision
		bool Collision(CCharacterPicture* cpObject1, CCharacterPicture* cpObject2);

		// Set Object Pictures, States, and info
		void CreateModels(void* vpApp);

		// Returns how far an object should travel, given it's Velocity (pixels per second) and Time (milliseconds)
		float GetFloatDistanceTravelled(float fVelocity, unsigned long ulTime);
		int GetIntDistanceTravelled(float fVelocity, unsigned long ulTime);

		// Plays a Sound
		// Loop = -1 means loop forever, 0 means just play once, 1 means loop once, etc
		void PlaySoundEffect(int iSoundToPlay, float fVolumePercent = 100.0, int iLoopCount = 0);

		// Stops current music and Plays specified Music
		// Loop = -1 means loop forever, 0 means just play once, 1 means loop once, etc
		void PlayMusic(int iMusicToPlay, float fVolumePercent = 100.0, int iLoopCount = -1);

		// Stops currently playing Music
		void StopMusic();
};

struct SChangingNumber
{
	float fX;
	float fY;
	float fZ;
	float fXChange;
	float fYChange;
	float fZChange;
};


// FMOD Error Check function
void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}


#endif

