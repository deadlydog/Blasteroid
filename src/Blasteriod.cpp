// Blasteroid.cpp

#include "Blasteriod.h"
#include <stdlib.h>			// Needed for the random numbers
#include <time.h>			// Needed for the random number
#include <cmath>			// Needed for floorf() function
#include <fstream>			// Needed for Reading/Writing files
using namespace std;

//********** Class Function Declarations **********

// Main()
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow)
{
	// Initialize cApp
	CApp cApp;
	
	// Ask whether user wants to run game in Full Screen or not
	if (MessageBox(NULL, "Run in full screen mode?", "Run in full screen mode?", MB_YESNO) == IDYES)
	{
		cApp.mbFullScreen = true;
	}else
	{
		cApp.mbFullScreen = false;
	}

	// Run cApp
	return cApp.Run();
}


// Setup Window parameters and style
CApp::CApp()
{
	m_Width		= GiWINDOW_WIDTH;
	m_Height	= GiWINDOW_HEIGHT;
	m_XPos		= 0;
	m_YPos		= 0;
	m_Style		= WS_SYSMENU | WS_VISIBLE;

	m_wcex.hIcon	= LoadIcon(GethInst(), MAKEINTRESOURCE(GAME_ICON));
	m_wcex.hIconSm	= LoadIcon(GethInst(), MAKEINTRESOURCE(GAME_ICON));
	m_wcex.hCursor	= LoadCursor(GethInst(), MAKEINTRESOURCE(TARGET_CURSOR));

	strcpy(m_Class, "Full Screen");
	strcpy(m_Caption, "Daniel Schroeder's Blasteriod");
}

// Initializes all needed Devices
BOOL CApp::Init()
{
	int iIndex = 0;		// Temp counting variable

	// Variables used by FMOD
	FMOD_RESULT       Result;
    unsigned int      uiVersion;


	// Seed the random number generator
	srand((unsigned int)time(NULL));


	// ****** Initialize Graphics Devices ******
	mcGraphics.Init();
	mcGraphics.SetMode(GethWnd(), !mbFullScreen /*FALSE = Full Screen Mode*/, FALSE, GiWINDOW_WIDTH, GiWINDOW_HEIGHT, 16);

	// Enable the Alpha color for transparency
	mcGraphics.EnableAlphaBlending(TRUE);
	mcGraphics.EnableAlphaTesting(TRUE);

	// Load Fonts
	mcFontTitle.Create(&mcGraphics, "Clearblock circular - 3DFX", 25);
	mcFontRegular.Create(&mcGraphics, "Crystal clear");
	mcFPSFont.Create(&mcGraphics, "Clearblock circular - 3DFX");

	// Load Textures (Pure White will be transparent)
	mcTexture[GiMENU_TEXTURE].Load(&mcGraphics, "Data\\Images\\MainMenuIcons.png", D3DCOLOR_RGBA(255, 255, 255, 255));
	mcTexture[GiSHIP_TEXTURE].Load(&mcGraphics, "Data\\Images\\Ships.png", D3DCOLOR_RGBA(255, 255, 255, 255));
	mcTexture[GiBULLET_TEXTURE].Load(&mcGraphics, "Data\\Images\\Bullets.png", D3DCOLOR_RGBA(255, 255, 255, 255));
	mcTexture[GiCONTROLS_TEXTURE].Load(&mcGraphics, "Data\\Images\\ControlsScreen.png", D3DCOLOR_RGBA(255, 255, 255, 255));
	mcTexture[GiPLAY_TEXTURE].Load(&mcGraphics, "Data\\Images\\GamePlayIcons.png", D3DCOLOR_RGBA(255, 255, 255, 255));
	mcTexture[GiSPACE_STATION_TEXTURE].Load(&mcGraphics, "Data\\Images\\SpaceStationIcons.png", D3DCOLOR_RGBA(255, 255, 255, 255));
	mcTexture[GiITEM_TEXTURE].Load(&mcGraphics, "Data\\Images\\ItemIcons.png", D3DCOLOR_RGBA(255, 255, 255, 255));

	// Create the Vertex Buffer
	mcVertexBuffer.Create(&mcGraphics, GiMAX_NUM_OF_BACKGROUND_STARS, VERTEXFVF, sizeof(SVertex));
	mcTextureVertexBuffer.Create(&mcGraphics, GiMAX_NUM_OF_BACKGROUND_STARS, TEXTURE_VERTEXFVF, sizeof(STextureVertex));


	// ********** Initialize Input Devices **********
	mcInput.Init(GethWnd(), GethInst());
	mcKeyboard.Create(&mcInput, KEYBOARD/*, !bFULL_SCREEN*/);
	mcMouse.Create(&mcInput, MOUSE/*, !bFULL_SCREEN*/);


	// ********** Initialize Sound Devices **********
	
	// Create and Initialize the FMOD system device

	// Create the Sound System
	Result = FMOD::System_Create(&mcpSoundSystem);
    ERRCHECK(Result);

	// Get the FMOD Version
    Result = mcpSoundSystem->getVersion(&uiVersion);
    ERRCHECK(Result);

	// Check the FMOD Version
    if (uiVersion < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", uiVersion, FMOD_VERSION);
        return 0;
    }

	// Initialize the Sound System
    Result = mcpSoundSystem->init(GiNUMBER_OF_SIMULTANEOUS_SOUNDS, FMOD_INIT_NORMAL, 0);
    ERRCHECK(Result);

	// Create Sounds
	mcpSoundSystem->createSound(caChangeSelectionSound, FMOD_DEFAULT, 0, &mcpSound[GiChangeSelectionSound]);
	mcpSoundSystem->createSound(caSelectionChosenSound, FMOD_DEFAULT, 0, &mcpSound[GiSelectionChosenSound]);
	mcpSoundSystem->createSound(caUpgradePurchasedSound, FMOD_DEFAULT, 0, &mcpSound[GiUpgradePurchasedSound]);
	mcpSoundSystem->createSound(caInvalidSelectionSound, FMOD_DEFAULT, 0, &mcpSound[GiInvalidSelectionSound]);
	mcpSoundSystem->createSound(caBombExplosionSound, FMOD_DEFAULT, 0, &mcpSound[GiBombExplosionSound]);
	mcpSoundSystem->createSound(caPickupItemSound, FMOD_DEFAULT, 0, &mcpSound[GiPickupItemSound]);
	mcpSoundSystem->createSound(caEnemyDeathSound, FMOD_DEFAULT, 0, &mcpSound[GiEnemyDeathSound]);
	mcpSoundSystem->createSound(caEnemyBossDeathSound, FMOD_DEFAULT, 0, &mcpSound[GiEnemyBossDeathSound]);
	mcpSoundSystem->createSound(caBulletHitPlayerSound, FMOD_DEFAULT, 0, &mcpSound[GiBulletHitPlayerSound]);
	mcpSoundSystem->createSound(caPlayerDeathSound, FMOD_DEFAULT, 0, &mcpSound[GiPlayerDeathSound]);

	// Create Music
	mcpSoundSystem->createStream(caGameIntroMusic, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &mcpMusic[GiGameIntroMusic]);
	mcpSoundSystem->createStream(caMainStoryMusic, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &mcpMusic[GiMainStoryMusic]);
	mcpSoundSystem->createStream(caLevel1Music, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &mcpMusic[GiLevel1Music]);
	mcpSoundSystem->createStream(caLevel2Music, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &mcpMusic[GiLevel2Music]);
	mcpSoundSystem->createStream(caLevel3Music, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &mcpMusic[GiLevel3Music]);
	mcpSoundSystem->createStream(caEnemyBossMusic, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &mcpMusic[GiEnemyBossMusic]);
	mcpSoundSystem->createStream(caEndBossMusic, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &mcpMusic[GiEndBossMusic]);
	mcpSoundSystem->createStream(caBeatGameMusic, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &mcpMusic[GiBeatGameMusic]);
	
	// Initialize Channels
	mcpMusicChannel = 0;
	mcpSoundChannel = 0;

/*
	mcSound.Init(GethWnd());
	mcMusicChannel.Create(&mcSound);
	
	// Setup all Sound Channels
	for (iIndex = 0; iIndex < GiNUMBER_OF_SOUND_CHANNELS; iIndex++)
	{
		mcSoundChannel[iIndex].Create(&mcSound);
	}


	// ********** Initialize Networking Devices **********
	mcNetworkAdapter.Init();
	mcNetworkServer.Init();
	mcNetworkClient.Init();
*/


	// ********** Do other Init work **********

	mbFullScreen = true;			// Make Full Screen default
	mbUsingMouse = true;			// Show Mouse to start with
	mbUsingMouseLastFrame = true;	

	// Initialize Clock
	mcpClock = CClock::ReturnClassInstance();

	// Initialize Timers and if they are Affected by Scaling the Clock or not
	mcNewScriptedEnemyTimer.TimerIsAffectedByClockScaleFactor(true);	
	mcNewRandomEnemyTimer.TimerIsAffectedByClockScaleFactor(true);
	
	mcUpDelayTimer.TimerIsAffectedByClockScaleFactor(false);		
	mcDownDelayTimer.TimerIsAffectedByClockScaleFactor(false);
	mcLeftDelayTimer.TimerIsAffectedByClockScaleFactor(false);
	mcRightDelayTimer.TimerIsAffectedByClockScaleFactor(false);
	mcSelectDelayTimer.TimerIsAffectedByClockScaleFactor(false);
	mcESCDelayTimer.TimerIsAffectedByClockScaleFactor(false);
	
	mcShowFPSTimer.TimerIsAffectedByClockScaleFactor(false);
	mcFPSTimer.TimerIsAffectedByClockScaleFactor(false);
	mcLastFrameTimer.TimerIsAffectedByClockScaleFactor(false);

	// Initialize Time Between Frames holder
	mulAmountOfTimeSinceLastFrame	= 0;


	// TODO: Make controls changable and save them in a file for next time they play
	// Initialize Controls
	mucKeyboardUp				= DIK_UP;
	mucKeyboardDown				= DIK_DOWN;
	mucKeyboardLeft				= DIK_LEFT;
	mucKeyboardRight			= DIK_RIGHT;
	mucKeyboardFire				= DIK_D;
	mucKeyboardNextWeapon		= DIK_S;
	mucKeyboardPreviousWeapon	= DIK_W;
	mucKeyboardDetonateBullet	= DIK_F;

	mucMouseFire				= MOUSE_LBUTTON;
	mucMouseNextWeapon			= MOUSE_RBUTTON;
	mucMouseDetonateBullet		= MOUSE_MBUTTON;

	// Initialize game with 1 player
	miNumberOfPlayers	= 1;

	// Initialize game to start on Level 1
	miCurrentLevel		= 1;

	// Mark that the Level is not finished yet
	mbLevelComplete		= false;

	// Initialize FPS variables
	mbShowFPS	= false;		// FPS not displayed by default
	muiBestFPS	= 0;			// Initialize to low number
	muiWorstFPS	= 30000;		// Initialize to high number
	meFPSPurpose= eInitialize;	// Set FPS Purpose to Initialize

	// Initialize Random Enemy variables
	mulNewRandomEnemyTime	= 0;
	mulMinRandomEnemyTime	= 0;
	mulMaxRandomEnemyTime	= 0;
	miRandomEnemyLikeliness	= 0;

	// Initialize Level Friction variables
	miLevelXFriction	= 0;
	miLevelYFriction	= 0;

	// Create object "Models" (Sets Pictures, States and other info)
	CreateModels(this);

	// Set StateManager to Start Menu
	mcGameStateManager.SetState(GameStateMainMenu, this);

	return TRUE;
}

// Shuts down all Initialized Devices
BOOL CApp::Shutdown()
{
	int iIndex	= 0;

	// Shutdown all of the Game States in case they have not already been
	mcGameStateManager.ShutdownAllStates(this);

/*
	// Shutdown Networking Devices
	if (mcNetworkClient.IsConnected())
	{
		mcNetworkClient.Disconnect();
	}
	
	if (mcNetworkServer.IsConnected())
	{
		mcNetworkServer.Disconnect();
	}
	
	mcNetworkClient.Shutdown();
	mcNetworkServer.Shutdown();
	mcNetworkAdapter.Shutdown();


	// Stop all Audio and Shutdown Sound and Music Devices
	mcMusicChannel.Stop();
	mcMusicChannel.Free();
	
	for (iIndex = 0; iIndex < GiNUMBER_OF_SOUND_CHANNELS; iIndex++)
	{
		mcSoundChannel[iIndex].Stop();
		mcSoundChannel[iIndex].Free();
	}

	mcSound.Shutdown();
*/

	// Shutdown all sounds, music, and the sound system

	// Loop through all sounds and release them
	for (iIndex = 0; iIndex < GiNUMBER_OF_SOUNDS_IN_GAME; iIndex++)
	{
		mcpSound[iIndex]->release();
	}

	// Loop through all music and release them
	for (iIndex = 0; iIndex < GiNUMBER_OF_MUSIC_IN_GAME; iIndex++)
	{
		mcpMusic[iIndex]->release();
	}

	mcpSoundSystem->close();
	mcpSoundSystem->release();

	// Shutdown Input Devices
	mcMouse.Free();
	mcKeyboard.Free();
	mcInput.Shutdown();

	// Shutdown Font and Graphics Devices
	mcTextureVertexBuffer.Free();
	mcVertexBuffer.Free();

	for (iIndex = 0; iIndex < GiNUMBER_OF_TEXTURES_IN_GAME; iIndex++)
	{
		if (mcTexture[iIndex].IsLoaded())
		{
			mcTexture[iIndex].Free();
		}
	}

	mcFontRegular.Free();
	mcFPSFont.Free();
	mcFontTitle.Free();
	mcGraphics.Shutdown();

	return TRUE;
}

// Performs per Frame work
BOOL CApp::Frame()
{
	// Clear keyboard/mouse input and wait a little before getting input again
	mcKeyboard.Clear();
	mcMouse.Clear();
	
	// Acquire Input Devices and Read Input
	mcKeyboard.Acquire(TRUE);
	mcKeyboard.Read();
	mcMouse.Acquire(TRUE);
	mcMouse.Read();

	// Update sounds
	mcpSoundSystem->update();

	// If Mouse Show status has changed
	// NOTE: This setup is needed to make sure ShowMouse is only called when necessary, as it
	//		 increments/decrements a counter each time it is called and only displays the
	//		 mouse if the counter is greater than or equal to 0. It is initialized to 0
	if (mbUsingMouse != mbUsingMouseLastFrame)
	{
		// Show Mouse if we are supposed to
		ShowMouse(mbUsingMouse);

		// Mark the show status of this frame
		mbUsingMouseLastFrame = mbUsingMouse;
	}

	// If user is pressing Escape
	if (mcKeyboard.GetKeyState(DIK_ESCAPE))
	{
		// Make sure when user presses Escape it does not escape everything (half second delay)
		if (mcESCDelayTimer.XTimeHasPassedUsingSavedTime(GiESC_KEY_PRESS_DELAY))
		{
			// If the game is not in the Main Menu state
			if (mcGameStateManager.ReturnCurrentStateFunction() != GameStateMainMenu)
			{
				// Shut Down the Current State and set the Main Menu as the Current State
				mcGameStateManager.ShutdownCurrentStateAndSetNextState(GameStateMainMenu, this);
			}
			// Else we are already in the Main Menu so shut the game down
			else
			{
				// Exit game
				mcGameStateManager.ShutdownAllStates(this);
			}
		}
	}

	// If user is pressing Tab, then toggle showing the FPS
	if (mcKeyboard.GetKeyState(DIK_TAB))
	{
		// Give some time before Show FPS button press can register again
		if (mcShowFPSTimer.XTimeHasPassedUsingSavedTime(GiSHOW_FPS_KEY_PRESS_DELAY))
		{
			// Toggle Showing the FPS
			if (mbShowFPS)
			{
				meFPSPurpose = eShutdown;
			}else
			{
				mbShowFPS = true;
				meFPSPurpose = eInitialize;
			}
		}
	}

	// Save how much time has passed since last frame
	mulAmountOfTimeSinceLastFrame = mcpClock->ReturnElapsedTimeAndResetClock();

	// Process the current State
	// If something went wrong while processing current state, Exit Game
	if (!mcGameStateManager.Process())
	{
		return FALSE;			// Exit Game
	}

	// If we should be showing the FPS
	if (mbShowFPS)
	{
		ShowFPS(this, meFPSPurpose);
	}

	// Display the graphics
	mcGraphics.Display();

	return TRUE;				// Return everything fine
}

// Returns how much time has passed since last Frame
unsigned long CApp::ReturnAmountOfTimeSinceLastFrame()
{
	return mulAmountOfTimeSinceLastFrame;
}

// Set the number of players (either 1 or 2)
bool CApp::SetNumberOfPlayers(int iNumOfPlayers)
{
	// If specified number of players is invalid
	if (iNumOfPlayers > GiMAX_NUMBER_OF_PLAYERS || iNumOfPlayers < 1)
	{
		return false;	// Return error
	}

	// Else set the Number of Players
	miNumberOfPlayers = iNumOfPlayers;

	return true;		// Return that everything is good
}

// Return the number of players
int CApp::ReturnNumberOfPlayers() const
{
	return miNumberOfPlayers;
}

// Set Level player is currently on
void CApp::SetCurrentLevel(int iCurrentLevel)
{
	miCurrentLevel = iCurrentLevel;
}

// Return Level player is currently on
int CApp::ReturnCurrentLevel() const
{
	return miCurrentLevel;
}

// Increment Level player is currently on
void CApp::IncrementCurrentLevel()
{
	++miCurrentLevel;
}

// Displays the Frame Per Second
void CApp::ShowFPS(void* vpApp, EPurpose ePurpose)
{
	CApp* cpApp = (CApp*)vpApp;

	static unsigned int SuiFPS = 0;		// Used to count the Frames Per Second
	static char ScaFPS[15];				// Used to display the Frames Per Second
	static char ScaBestFPS[15];			// Used to display the Best Frame Per Second
	static char ScaWorstFPS[15];		// Used to display the Worst Frame Per Second

	unsigned long ulTimeSinceLastFrame;	// Used to hold how much time has elapsed between frames
	char cLastFrame[40];				// Used to display how much time has elapsed between frames

	// Text color
	D3DCOLOR sFPSTextColor = D3DCOLOR_RGBA(255, 255, 0, 255);

	// If function is being called for the first time
	if (ePurpose == eInitialize)
	{
		// Reset FPS strings
		sprintf(ScaFPS,"");
		sprintf(ScaBestFPS,"");
		sprintf(ScaWorstFPS,"");

		// Reset FPS counters
		SuiFPS		= 0;
		muiBestFPS	= 0;		// Reset BestFPS to a low number
		muiWorstFPS	= 30000;	// Reset WorstFPS to a high number

		mcFPSTimer.RestartTimerUsingCurrentTime();	// Restart the Timer

		// Reset Purpose to Normal
		cpApp->meFPSPurpose = eNormal;
	}
	// Else if function is being called for last time
	else if (ePurpose == eShutdown)
	{
		cpApp->mbShowFPS = false;
	}

	SuiFPS++;

	// If one second has passed
	if (cpApp->mcFPSTimer.XTimeHasPassedUsingCurrentTime(1000))
	{		
		// Save highest FPS
		if (SuiFPS > muiBestFPS)
		{
			muiBestFPS = SuiFPS;
		}

		// Save worst FPS
		if (SuiFPS < muiWorstFPS && SuiFPS > 1)
		{
			muiWorstFPS = SuiFPS;
		}

		// Copy FPS, BestFPS, and WorstFPS into Buffers
		sprintf(ScaFPS,"FPS = %d", SuiFPS);
		sprintf(ScaBestFPS,"Best = %d", muiBestFPS);
		sprintf(ScaWorstFPS,"Worst = %d", muiWorstFPS);

		// Restart the FPS counter
		SuiFPS = 0;
	}

	// Get time passed since last Frame
	ulTimeSinceLastFrame = cpApp->ReturnAmountOfTimeSinceLastFrame();	
	sprintf(cLastFrame, "Time Since Last Frame = %dms", ulTimeSinceLastFrame);

	// Allow new graphics to be displayed
	mcGraphics.BeginScene();

	mcFPSFont.Begin();
	mcFPSFont.Print(ScaFPS, 10, 390, 0, 0, sFPSTextColor);
	mcFPSFont.Print(ScaBestFPS, 10, 410, 0, 0, sFPSTextColor);
	mcFPSFont.Print(ScaWorstFPS, 10, 430, 0, 0, sFPSTextColor);
	mcFPSFont.Print(cLastFrame, 10, 450, 0, 0, sFPSTextColor);
	mcFPSFont.End();

	// Lock Graphics
	mcGraphics.EndScene();
}


// Displays the Start Menu
void CApp::GameStateMainMenu(void* vpOptionalDataToPass, EPurpose ePurpose)
{
	//********** Initialize Variables **********
	CApp* cpApp	= (CApp*)vpOptionalDataToPass;		// cpApp = our CApp class

	// ID's of options to choose from
	const int iPLAY			= 0;
	const int iSTORY		= 1;
	const int iCONTROLS		= 2;
	const int iHIGH_SCORES	= 3;

	static CShip* ScpPlayer;

	static SChangingNumber SsPlayScale;
	static SChangingNumber SsStoryScale;
	static SChangingNumber SsControlsScale;
	static SChangingNumber SsHighScoresScale;

	static SVertex SsStar[GiNUM_OF_MENU_BACKGROUND_STARS];	// Holds background star properties

	static int SiCurrentChoice		= iPLAY;	// Current 'highlighted' Choice
	static int SiLastChoice			= iPLAY;	// Used to tell if Choice was changed
	static bool SbChoiceHasBeenMade	= false;	// Tells if user has chosen yet

	static int SiIntroSoundChannel	= 0;		// Holds which Sound Channel the Intro is playing on

	int iIndex		= 0;		// Temp counter variable
	int iBrightness = 0;		// Temp variable used to hold brightness of Stars

	// Text Positions (Default letter size is about 8 pixels)
	int iPlayTextX				= (GiWINDOW_WIDTH / 2) - (8 * (strlen("Play") / 2));
	int iPlayTextY				= 165;
	int iHighScoresTextX		= (GiWINDOW_WIDTH / 2) - (8 * (strlen("High Scores") / 2));
	int iHighScoresTextY		= 365;
	int iStoryTextX				= (GiWINDOW_WIDTH / 4) - (8 * (strlen("Story") / 2));
	int iStoryTextY				= 265;
	int iControlsTextX			= ((GiWINDOW_WIDTH / 4) * 3) - (8 * (strlen("Controls") / 2));
	int iControlsTextY			= 265;
	int iTitleTextX				= (GiWINDOW_WIDTH / 2) - (15 * (strlen("Daniel Schroeder's") / 2));
	int iTitleTextY				= 3;
	int iExitTextX				= (GiWINDOW_WIDTH / 2) - (9 * (strlen("Press Esc at any time to exit the game") / 2));
	int iExitTextY				= 450;

	// Picture Positions
	int iPlayPictureX			= 305;
	int iPlayPictureY			= 190;
	int iHighScoresPictureX		= 305;
	int iHighScoresPictureY		= 390;
	int iStoryPictureX			= 148;
	int iStoryPictureY			= 290;
	int iControlsPictureX		= 460;
	int iControlsPictureY		= 285;
	int iTitlePictureX			= 75;
	int iTitlePictureY			= 40;

	// Tells if the mouse is currently over an option or not
	bool bMouseIsOverOption[4] = {false, false, false, false};


	//********** Do Startup/Shutdown Stuff **********

	// If first time running this function
	if (ePurpose == eInitialize)
	{
		//***** Do Game Initialization Stuff *****

		SiCurrentChoice = SiLastChoice = iPLAY;

		// Create Players Ships
		for (iIndex = 0; iIndex < cpApp->ReturnNumberOfPlayers(); iIndex++)
		{
			cpApp->mcaPlayer[iIndex].CopyFrom(cpApp->mcaPlayerShipModel[iIndex]);
		}

		// Set Current Level
		cpApp->miCurrentLevel = 1;


		//***** Do Main Menu Initialization Stuff *****

		// Create and Position Ship for Play picture
		ScpPlayer = new CShip(cpApp->mcaPlayerShipModel[GiPLAYER_1]);
		ScpPlayer->MoveTo(iPlayPictureX, iPlayPictureY);
		ScpPlayer->SetVelocities(0, 0);
		ScpPlayer->SetAccelerations(0, 0);

		// Initialize Ship Pictures in Backwards State so they appear "Off"
		ScpPlayer->mcPicture.SetCurrentState(GiSHIP_STATE_BACKWARDS);

		// Set Scale Rates
		SsPlayScale.fX = SsStoryScale.fX = SsControlsScale.fX = SsHighScoresScale.fX = 1.0;
		SsPlayScale.fY = SsStoryScale.fY = SsControlsScale.fY = SsHighScoresScale.fY = 1.0;
		SsPlayScale.fXChange = SsStoryScale.fXChange = SsControlsScale.fXChange = SsHighScoresScale.fXChange = 1.0;		
		SsPlayScale.fYChange = SsStoryScale.fYChange = SsControlsScale.fYChange = SsHighScoresScale.fYChange = 1.0;


		// Initialize Stars
		for (iIndex = 0; iIndex < GiNUM_OF_MENU_BACKGROUND_STARS; iIndex++)
		{
			SsStar[iIndex].fX		= rand() % GiWINDOW_WIDTH;
			SsStar[iIndex].fY		= rand() % GiWINDOW_HEIGHT;
			SsStar[iIndex].fZ		= 0;
			SsStar[iIndex].fRHW		= 1;
			SsStar[iIndex].fXVelocity= rand() % GiMAX_VELOCITY_OF_MENU_BACKGROUND_STARS;

			// Slow down half of the Stars that are going fast
			if (SsStar[iIndex].fXVelocity > (GiMAX_VELOCITY_OF_MENU_BACKGROUND_STARS / 2))
			{
				// Only True 1/2 the time
				if (rand() % 2 == 0)
				{
					SsStar[iIndex].fXVelocity = rand() % (GiMAX_VELOCITY_OF_MENU_BACKGROUND_STARS / 2);
				}
				// If they were not slowed down, freeze some more of them
				else if (rand() % 4 == 0)
				{
					SsStar[iIndex].fXVelocity = 0;
				}
			}

			// Make slower stars dimmer so they appear farther away using their Velocity as Alpha value
			// Add extra value so they aren't too dim
			if ((iBrightness = (int)floorf((SsStar[iIndex].fXVelocity / 2) + GiMENU_BASE_STAR_BRIGHTNESS)) > 255)
			{
				iBrightness = 255;
			}
			SsStar[iIndex].diffuse = D3DCOLOR_RGBA(255, 255, 255, iBrightness);
		}

		// Reset Players Choice
		SiCurrentChoice = iPLAY;
		SbChoiceHasBeenMade	= false;

		// Play Intro Music once
		cpApp->PlayMusic(GiGameIntroMusic, 100.0, 0);

		return;
	}
	// If closing this function down
	else if (ePurpose == eShutdown)
	{
		// Delete allocated objects
		delete ScpPlayer;

		return;
	}
	// Else if user is Returning to this State from another state
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else Purpose == eNORMAL so do following code



	//********** Clear the Display **********

	// Clear the Back Buffer to Black
	cpApp->mcGraphics.ClearDisplay(D3DCOLOR_RGBA(0, 0, 0, 255));

	// Lock the Back Buffer and do the drawing
	cpApp->mcGraphics.BeginScene();


	
	//********** Draw Main Menu **********

	// Move background stars
	for (iIndex = 0; iIndex < GiNUM_OF_MENU_BACKGROUND_STARS; iIndex++)
	{
		// Move Star
		SsStar[iIndex].fX += cpApp->GetFloatDistanceTravelled(SsStar[iIndex].fXVelocity, cpApp->ReturnAmountOfTimeSinceLastFrame());

		// Make star wrap around screen
		if (SsStar[iIndex].fX > GiWINDOW_WIDTH)
		{
			SsStar[iIndex].fX = -50;
			SsStar[iIndex].fY = rand() % GiWINDOW_HEIGHT;
		}
	}

	// Place Star into Vertex Buffer and Draw them
	cpApp->mcVertexBuffer.Set(0, GiNUM_OF_MENU_BACKGROUND_STARS, SsStar);
	cpApp->mcVertexBuffer.Render(0, GiNUM_OF_MENU_BACKGROUND_STARS, D3DPT_POINTLIST);


	// Display Title
	cpApp->mcFontTitle.Print("Daniel Schroeder's", iTitleTextX, iTitleTextY, 0, 0, D3DCOLOR_RGBA(255, 0, 0, 255));
	cpApp->mcTexture[GiMENU_TEXTURE].Blit(iTitlePictureX, iTitlePictureY, 2, 0, 488, 83);


	// Diplay the Main Menu Options
	cpApp->mcFontRegular.Begin();

	// Display "Play"
	cpApp->mcFontRegular.Print("Play", iPlayTextX, iPlayTextY, 0, 0, D3DCOLOR_RGBA(0, 0, 255, 255));

	// Display "High Scores"
	cpApp->mcFontRegular.Print("High Scores", iHighScoresTextX, iHighScoresTextY, 0, 0, D3DCOLOR_RGBA(0, 0, 255, 255));

	// Display "Story"
	cpApp->mcFontRegular.Print("Story", iStoryTextX, iStoryTextY, 0, 0, D3DCOLOR_RGBA(255, 0, 0, 255));

	// Display "Controls"
	cpApp->mcFontRegular.Print("Controls", iControlsTextX, iControlsTextY, 0, 0, D3DCOLOR_RGBA(255, 0, 0, 255));

	// Display how to exit the game
	cpApp->mcFontRegular.Print("Press Esc at any time to exit the game", iExitTextX, iExitTextY, 0, 0, D3DCOLOR_RGBA(255, 0, 0, 255));

	// Finished writing text
	cpApp->mcFontRegular.End();


	//********** Get Input **********

	// If the user has not selected a choice yet
	if (!SbChoiceHasBeenMade)
	{
		// If user is using the Mouse
		if (cpApp->mcMouse.GetXDelta() != 0 || cpApp->mcMouse.GetYDelta() != 0 ||
			cpApp->mcMouse.GetButtonState(cpApp->mucMouseFire) || cpApp->mcMouse.GetButtonState(cpApp->mucMouseNextWeapon) ||
			cpApp->mcMouse.GetButtonState(cpApp->mucMouseDetonateBullet))
		{
			// Show the Mouse
			cpApp->mbUsingMouse = true;
		}

		// If user is using the Keyboard
		if (cpApp->mcKeyboard.GetKeyState(DIK_UP) || cpApp->mcKeyboard.GetKeyState(DIK_DOWN) ||
			cpApp->mcKeyboard.GetKeyState(DIK_LEFT) || cpApp->mcKeyboard.GetKeyState(DIK_RIGHT))
		{
			// Hide the Mouse
			cpApp->mbUsingMouse = false;
		}

		// If Play is highlighted
		if ((cpApp->mcKeyboard.GetKeyState(DIK_UP) || (bMouseIsOverOption[iPLAY] = (cpApp->mbUsingMouse &&
			cpApp->mcMouse.GetXPos() > 275 && cpApp->mcMouse.GetXPos() < 355 &&
			cpApp->mcMouse.GetYPos() > 150 && cpApp->mcMouse.GetYPos() < 230))) &&
			cpApp->mcUpDelayTimer.XTimeHasPassedUsingSavedTime(GiMENU_UP_DELAY_TIME))
		{
			// Set the Current Choice to Play
			SiCurrentChoice = iPLAY;
		}

		// If High Scores is highlighted
		if ((cpApp->mcKeyboard.GetKeyState(DIK_DOWN) || (bMouseIsOverOption[iHIGH_SCORES] = (cpApp->mbUsingMouse &&
			cpApp->mcMouse.GetXPos() > 275 && cpApp->mcMouse.GetXPos() < 355 &&
			cpApp->mcMouse.GetYPos() > 360 && cpApp->mcMouse.GetYPos() < 430))) &&
			cpApp->mcDownDelayTimer.XTimeHasPassedUsingSavedTime(GiMENU_DOWN_DELAY_TIME))
		{
			// Set the Current Choice to High Scores
			SiCurrentChoice = iHIGH_SCORES;
		}

		// If Story is highlighted
		if ((cpApp->mcKeyboard.GetKeyState(DIK_LEFT) || (bMouseIsOverOption[iSTORY] = (cpApp->mbUsingMouse &&
			cpApp->mcMouse.GetXPos() > 120 && cpApp->mcMouse.GetXPos() < 195 &&
			cpApp->mcMouse.GetYPos() > 260 && cpApp->mcMouse.GetYPos() < 335))) &&
			cpApp->mcLeftDelayTimer.XTimeHasPassedUsingSavedTime(GiMENU_LEFT_DELAY_TIME))
		{
			// Set the Current Choice to Story
			SiCurrentChoice = iSTORY;
		}

		// If Controls is highlighted
		if ((cpApp->mcKeyboard.GetKeyState(DIK_RIGHT) || (bMouseIsOverOption[iCONTROLS] = (cpApp->mbUsingMouse &&
			cpApp->mcMouse.GetXPos() > 430 && cpApp->mcMouse.GetXPos() < 510 &&
			cpApp->mcMouse.GetYPos() > 255 && cpApp->mcMouse.GetYPos() < 340))) &&
			cpApp->mcRightDelayTimer.XTimeHasPassedUsingSavedTime(GiMENU_RIGHT_DELAY_TIME))
		{
			// Set the Current Choice to Controls
			SiCurrentChoice = iCONTROLS;
		}

		// If user Selects a Choice
		if ((cpApp->mcKeyboard.GetKeyState(DIK_RETURN) || 
			(cpApp->mbUsingMouse && cpApp->mcMouse.GetButtonState(MOUSE_LBUTTON) && bMouseIsOverOption[SiCurrentChoice])) &&
			cpApp->mcSelectDelayTimer.XTimeHasPassedUsingSavedTime(GiMENU_SELECT_DELAY_TIME))
		{
			// Mark that a Choice has been chosen
			SbChoiceHasBeenMade = true;

			// Play the sound of making a Choice
			cpApp->PlaySoundEffect(GiSelectionChosenSound);
		}

		// If Choice has changed
		if (SiCurrentChoice != SiLastChoice)
		{
			// Play the sound of changing Choices
			cpApp->PlaySoundEffect(GiChangeSelectionSound);

			// Set Last Choice to this Choice
			SiLastChoice = SiCurrentChoice;
		}
	}

	
	//********** Process Input **********

	// If user has not yet chosen an option
	if (!SbChoiceHasBeenMade)
	{
		// If Play is 'highlighted'
		if (SiCurrentChoice == iPLAY)
		{
			// Reverse Scale size change if too small or too large
			if (SsPlayScale.fX < 1.0)
			{
				SsPlayScale.fX = 1.0;
				SsPlayScale.fXChange *= -1;
			}else if (SsPlayScale.fX > 1.5)
			{
				SsPlayScale.fX = 1.5;
				SsPlayScale.fXChange *= -1;
			}
		
			// Adjust Scale size
			SsPlayScale.fX += cpApp->GetFloatDistanceTravelled(SsPlayScale.fXChange, cpApp->ReturnAmountOfTimeSinceLastFrame());		

			// Display Scaled Play picture
			cpApp->mcTexture[GiSHIP_TEXTURE].Blit(ScpPlayer->ReturnXCoordinate(), ScpPlayer->ReturnYCoordinate(), ScpPlayer->mcPicture.ReturnLeft(), ScpPlayer->mcPicture.ReturnTop(), ScpPlayer->mcPicture.ReturnWidth(), ScpPlayer->mcPicture.ReturnHeight(), SsPlayScale.fX, SsPlayScale.fX);
		}else
		{
			// Display normal Play picture
			cpApp->mcTexture[GiSHIP_TEXTURE].Blit(ScpPlayer->ReturnXCoordinate(), ScpPlayer->ReturnYCoordinate(), ScpPlayer->mcPicture.ReturnLeft(), ScpPlayer->mcPicture.ReturnTop(), ScpPlayer->mcPicture.ReturnWidth(), ScpPlayer->mcPicture.ReturnHeight());

			// Reset Scale size
			SsPlayScale.fX			= SsPlayScale.fY;
			SsPlayScale.fXChange	= SsPlayScale.fYChange;
		}
		
		// If High Scores is 'highlighted'
		if (SiCurrentChoice == iHIGH_SCORES)
		{
			// Reverse Scale size change if too small or too large
			if (SsHighScoresScale.fX < 1.0)
			{
				SsHighScoresScale.fX = 1.0;
				SsHighScoresScale.fXChange *= -1;
			}else if (SsHighScoresScale.fX > 1.5)
			{
				SsHighScoresScale.fX = 1.5;
				SsHighScoresScale.fXChange *= -1;
			}
		
			// Adjust Scale size
			SsHighScoresScale.fX += cpApp->GetFloatDistanceTravelled(SsHighScoresScale.fXChange, cpApp->ReturnAmountOfTimeSinceLastFrame());

			// Display Scaled High Scores picture
			cpApp->mcTexture[GiMENU_TEXTURE].Blit(iHighScoresPictureX, iHighScoresPictureY, 39, 87, 32, 32, SsHighScoresScale.fX, SsHighScoresScale.fX);
		}else
		{
			// Display normal High Scores picture
			cpApp->mcTexture[GiMENU_TEXTURE].Blit(iHighScoresPictureX, iHighScoresPictureY, 39, 87, 32, 32);
	
			// Reset Scale size
			SsHighScoresScale.fX		= SsHighScoresScale.fY;
			SsHighScoresScale.fXChange	= SsHighScoresScale.fYChange;
		}

		// If Story is 'highlighted'
		if (SiCurrentChoice == iSTORY)
		{
			// Reverse Scale size change if too small or too large and Options hasn't been chosen yet
			if (SsStoryScale.fX < 1.0)
			{
				SsStoryScale.fX = 1.0;
				SsStoryScale.fXChange *= -1;
			}else if (SsStoryScale.fX > 1.5)
			{
				SsStoryScale.fX = 1.5;
				SsStoryScale.fXChange *= -1;
			}
		
			// Adjust Scale sizing
			SsStoryScale.fX += cpApp->GetFloatDistanceTravelled(SsStoryScale.fXChange, cpApp->ReturnAmountOfTimeSinceLastFrame());

			// Display Scaled Story picture
			cpApp->mcTexture[GiMENU_TEXTURE].Blit(iStoryPictureX, iStoryPictureY, 74, 87, 32, 32, SsStoryScale.fX, SsStoryScale.fX);
		}else
		{
			// Display normal Story picture
			cpApp->mcTexture[GiMENU_TEXTURE].Blit(iStoryPictureX, iStoryPictureY, 74, 87, 32, 32);

			// Reset Scale size
			SsStoryScale.fX			= SsStoryScale.fY;
			SsStoryScale.fXChange	= SsStoryScale.fYChange;
		}
		
		// If Controls is 'highlighted'
		if (SiCurrentChoice == iCONTROLS)
		{
			// Reverse Scale size change if too small or too large
			if (SsControlsScale.fX < 1.0)
			{
				SsControlsScale.fX = 1.0;
				SsControlsScale.fXChange *= -1;
			}else if (SsControlsScale.fX > 1.5)
			{
				SsControlsScale.fX = 1.5;
				SsControlsScale.fXChange *= -1;
			}
		
			// Adjust Scale sizing
			SsControlsScale.fX += cpApp->GetFloatDistanceTravelled(SsControlsScale.fXChange, cpApp->ReturnAmountOfTimeSinceLastFrame());

			// Display Scaled Credits picture
			cpApp->mcTexture[GiMENU_TEXTURE].Blit(iControlsPictureX, iControlsPictureY, 2, 87, 32, 32, SsControlsScale.fX, SsControlsScale.fX);
		}else
		{
			// Display normal Credits picture
			cpApp->mcTexture[GiMENU_TEXTURE].Blit(iControlsPictureX, iControlsPictureY, 2, 87, 32, 32);

			// Reset Scale size
			SsControlsScale.fX			= SsControlsScale.fY;
			SsControlsScale.fXChange	= SsControlsScale.fYChange;
		}
	}
	// Else user has chosen an option
	else
	{
		//********** Perform actions based on users choice **********

		// If Play was chosen
		if (SiCurrentChoice == iPLAY)
		{
			// If Ship has moved off the screen
			if (ScpPlayer->ReturnYCoordinate() < -ScpPlayer->mcPicture.ReturnHeight())
			{
				// Reset the users choice and start the game
				SbChoiceHasBeenMade = false;
				cpApp->mcGameStateManager.ShutdownCurrentStateAndSetNextState(GameStatePlay, cpApp);
			}

			// If Ship hasn't moved downwards yet
			if (ScpPlayer->mcPicture.ReturnCurrentState() == GiSHIP_STATE_BACKWARDS)
			{
				// Make Ship move down
				ScpPlayer->SetYAcceleration(200);
			}

			// If Ship has backed up enough, make it fly forward
			if (ScpPlayer->ReturnYCoordinate() > 210)
			{
				// Move Ship forward
				ScpPlayer->SetYAcceleration(-400);

				// Change Ship Picture to moving forward
				if (ScpPlayer->mcPicture.ReturnCurrentState() == GiSHIP_STATE_BACKWARDS)
				{
					ScpPlayer->mcPicture.SetCurrentState(GiSHIP_STATE_FORWARD);
				}
			}		

			// Display the Play picture
			cpApp->mcTexture[GiSHIP_TEXTURE].Blit(ScpPlayer->ReturnXCoordinate(), ScpPlayer->ReturnYCoordinate(), ScpPlayer->mcPicture.ReturnLeft(), ScpPlayer->mcPicture.ReturnTop(), ScpPlayer->mcPicture.ReturnWidth(), ScpPlayer->mcPicture.ReturnHeight());
		}

		// If High Scores was chosen
		if (SiCurrentChoice == iHIGH_SCORES)
		{
			// Reset users choice and go to High Scores
			SbChoiceHasBeenMade = false;
			cpApp->mcGameStateManager.ShutdownCurrentStateAndSetNextState(GameStateDisplayHighScores, cpApp);
		}

		// If Story was chosen
		if (SiCurrentChoice == iSTORY)
		{
			// Reset users choice and go to Story
			SbChoiceHasBeenMade = false;
			cpApp->mcGameStateManager.ShutdownCurrentStateAndSetNextState(GameStateMainStory, cpApp);
		}

		// If Controls was chosen
		if (SiCurrentChoice == iCONTROLS)
		{
			// Reset users choice and go to Credits
			SbChoiceHasBeenMade = false;
			cpApp->mcGameStateManager.ShutdownCurrentStateAndSetNextState(GameStateControls, cpApp);
		}
	}

	// Update Ship Pictures
	ScpPlayer->Update(cpApp->ReturnAmountOfTimeSinceLastFrame());


	//********** Lock Graphics into the Back Buffer **********

	// Unlock the Back Buffer
	cpApp->mcGraphics.EndScene();
}


// Play the Game
void CApp::GameStatePlay(void* vpOptionalDataToPass, EPurpose ePurpose)
{
	//********** Initialize Variables **********
	CApp* cpApp	= (CApp*)vpOptionalDataToPass;		// cpApp = our CApp class


	//********** Do Startup/Shutdown Stuff **********

	// If this is the first time running this function
	if (ePurpose == eInitialize)
	{
// Turn the Mouse Movement Off
cpApp->mbUsingMouse = false;

		// Reset Players Movement, Position, Status, and Current Weapon
		cpApp->mcaPlayer[GiPLAYER_1].SetAccelerations(0, 0);
		cpApp->mcaPlayer[GiPLAYER_1].SetVelocities(0, 0);
		cpApp->mcaPlayer[GiPLAYER_1].MoveTo(GiWINDOW_WIDTH / 2 - (cpApp->mcaPlayer[GiPLAYER_1].mcPicture.ReturnWidth() / 2), GiWINDOW_HEIGHT - cpApp->mcaPlayer[GiPLAYER_1].mcPicture.ReturnHeight());
		cpApp->mcaPlayer[GiPLAYER_1].SetStatus(GiSHIP_STATUS_NONE);

		// If Player has Normal Gun Weapon
		if (cpApp->mcaPlayer[GiPLAYER_1].WeaponIsInList(GiNORMAL_WEAPON))
		{
			// Set the Normal Weapon as the Current Weapon
			cpApp->mcaPlayer[GiPLAYER_1].SetCurrentWeapon(GiNORMAL_WEAPON);
		}
		// Else they should have the Three Normal Gun Weapon
		else
		{
			// Set the Three Normal Weapon as the Current Weapon
			cpApp->mcaPlayer[GiPLAYER_1].SetCurrentWeapon(GiTHREE_NORMAL_WEAPON);
		}

		// Set that the Level is not Complete yet
        cpApp->mbLevelComplete = false;
		
		// Call all Game State Play() functions with Initialization Purpose
		cpApp->DoLevelStuff(cpApp, eInitialize);
		cpApp->GetAndProcessInput(cpApp, eInitialize);
		cpApp->UpdateObjects(cpApp, eInitialize);
		cpApp->ProcessCollisionsAndObjects(cpApp, eInitialize);
		cpApp->DisplayScreenInfo(cpApp, eInitialize);

		return;
	}
	// Else if this function is Shutting Down
	else if (ePurpose == eShutdown)
	{
		// Call all Game State Play() functions with Shutdown Purpose
		cpApp->DoLevelStuff(cpApp, eShutdown);
		cpApp->GetAndProcessInput(cpApp, eShutdown);
		cpApp->UpdateObjects(cpApp, eShutdown);
		cpApp->ProcessCollisionsAndObjects(cpApp, eShutdown);
		cpApp->DisplayScreenInfo(cpApp, eShutdown);

		// Increase Level to next Level
		cpApp->miCurrentLevel++;

		return;
	}


	//********** Clear the Display **********

	// Clear the Back Buffer to Black
	cpApp->mcGraphics.ClearDisplay(D3DCOLOR_RGBA(0, 0, 0, 255));

	// Lock the Back Buffer and do the drawing
	cpApp->mcGraphics.BeginScene();


	//********** Do Background Stuff **********

	cpApp->DoLevelStuff(cpApp);


	//********** Get and Process Input **********

	// If Player is not Dying or Dead and they are not colliding with an Enemy
	if (cpApp->mcaPlayer[GiPLAYER_1].ReturnStatus() != GiSHIP_STATUS_DYING && 
		cpApp->mcaPlayer[GiPLAYER_1].ReturnStatus() != GiSHIP_STATUS_DEAD/* &&
		!cpApp->mcaPlayer[GiPLAYER_1].ReturnIfShipIsColliding()*/)
	{
		// If Status is to Leave Screen
		if (cpApp->mcaPlayer[GiPLAYER_1].ReturnStatus() == GiSHIP_STATUS_LEAVE_SCREEN)
		{
			// Move Ship up off Screen
			cpApp->mcaPlayer[GiPLAYER_1].SetYAcceleration(-cpApp->mcaPlayer[GiPLAYER_1].ReturnNormalYAcceleration());
			cpApp->mcaPlayer[GiPLAYER_1].SetXVelocity(0);
		}
		// Else Get and Process Players Input
		else
		{
			// Get and Process Players Input
			cpApp->GetAndProcessInput(cpApp);
		}
	}
	

	//********** Update All Objects **********

	cpApp->UpdateObjects(cpApp);

	
	//********** Process Collisions and Display All Objects **********

	cpApp->ProcessCollisionsAndObjects(cpApp);	


	//********** Display Headings (Score, Current Bullet, etc) **********

	cpApp->DisplayScreenInfo(cpApp);


	//********** Lock Graphics into the Back Buffer **********

	// Unlock the Back Buffer
	cpApp->mcGraphics.EndScene();
}

// Displays the Main Storyline
void CApp::GameStateMainStory(void* vpOptionalDataToPass, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpOptionalDataToPass;		// cpApp = our CApp class

	static SVertex SsStar[GiNUM_OF_STORY_BACKGROUND_STARS];	// Holds background star properties
	static SChangingNumber SsTextPosition;			// Holds Text Position

	int iDefaultTextSpeed = 15;	// Default Text Speed
	int iFastTextSpeed = 150;	// Text Speed when going Fast
	
	int iIndex = 0;				// Temp counting variable
	int iBrightness = 0;		// Temp variable to hold Stars Brightness


	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		SsTextPosition.fX = 10;
		SsTextPosition.fXChange = 0;
		SsTextPosition.fY = GiWINDOW_HEIGHT;
		SsTextPosition.fYChange = iDefaultTextSpeed;

		// Initialize Stars
		for (iIndex = 0; iIndex < GiNUM_OF_STORY_BACKGROUND_STARS; iIndex++)
		{
			SsStar[iIndex].fX		= rand() % GiWINDOW_WIDTH;
			SsStar[iIndex].fY		= rand() % GiWINDOW_HEIGHT;
			SsStar[iIndex].fZ		= 0;
			SsStar[iIndex].fRHW		= 1;

			// Give Stars a random Brightness
			iBrightness = rand() % 255;
			SsStar[iIndex].diffuse = D3DCOLOR_RGBA(255, 255, 255, iBrightness);
		}

		// Start Main Story Music
		cpApp->PlayMusic(GiMainStoryMusic);

		// Hide the Mouse
		cpApp->mbUsingMouse = false;

		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		// Stop Music
		cpApp->StopMusic();

		return;
	}
	// Else this state is just doing a normal run


	//********** Clear the Display **********

	// Clear the Back Buffer to Black
	cpApp->mcGraphics.ClearDisplay(D3DCOLOR_RGBA(0, 0, 0, 255));

	// Lock the Back Buffer and do the drawing
	cpApp->mcGraphics.BeginScene();


	//********** Draw the background Stars **********

	// Place Star into Vertex Buffer and Draw them
	cpApp->mcVertexBuffer.Set(0, GiNUM_OF_STORY_BACKGROUND_STARS, SsStar);
	cpApp->mcVertexBuffer.Render(0, GiNUM_OF_STORY_BACKGROUND_STARS, D3DPT_POINTLIST);


	//********** Get and Process Players Input **********

	// If Player is pressing Fire on Keyboard or Mouse
	if (cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardFire) ||
		cpApp->mcMouse.GetButtonState(cpApp->mucMouseFire))
	{
		// Speed up Text
		SsTextPosition.fYChange = iFastTextSpeed;
	}
	// Else if Player is pressing Next Weapon on Keyboard or Mouse
	else if (cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardNextWeapon) ||
			 cpApp->mcMouse.GetButtonState(cpApp->mucMouseNextWeapon))
	{
		SsTextPosition.fYChange = 0;
	}
	// Else Player is not pressing Fire
	else
	{
		// Make sure Text is moving at Default Speed
		SsTextPosition.fYChange = iDefaultTextSpeed;
	}

	// If Text is done displaying or Player presses Enter or Mouse Detonate Weapon (Middle Click)
	if ((SsTextPosition.fY < -700 || cpApp->mcKeyboard.GetKeyState(DIK_RETURN) ||
		cpApp->mcMouse.GetButtonState(cpApp->mucMouseDetonateBullet)) &&
		cpApp->mcSelectDelayTimer.XTimeHasPassedUsingSavedTime(GiMENU_SELECT_DELAY_TIME))
	{
		// Go back to the Main Menu
		cpApp->mcGameStateManager.ShutdownCurrentStateAndSetNextState(GameStateMainMenu, cpApp);
	}


	//********** Display Story **********

	// Move the text
	SsTextPosition.fY -= cpApp->GetFloatDistanceTravelled(SsTextPosition.fYChange, cpApp->ReturnAmountOfTimeSinceLastFrame());

	cpApp->mcFontRegular.Begin();

	cpApp->mcFontRegular.Print("    Good day officer Gibberish Threephood. Sorry to disturb", SsTextPosition.fX, SsTextPosition.fY);
	cpApp->mcFontRegular.Print("you on your vacation on Moonkey Island, but as you are probably", SsTextPosition.fX, SsTextPosition.fY + 20);
	cpApp->mcFontRegular.Print("aware, the alien species we made contact with 2 days ago were", SsTextPosition.fX, SsTextPosition.fY + 40);
	cpApp->mcFontRegular.Print("not as pleasant and trust worthy as we had hoped. They managed", SsTextPosition.fX, SsTextPosition.fY + 60);
	cpApp->mcFontRegular.Print("to destroy half of Regina, Saskatchewan before our fighter jets", SsTextPosition.fX, SsTextPosition.fY + 80);
	cpApp->mcFontRegular.Print("were able to bring them down. That however is the good news.", SsTextPosition.fX, SsTextPosition.fY + 100);
	cpApp->mcFontRegular.Print("The bad news is that there is an entire colony on its way to", SsTextPosition.fX, SsTextPosition.fY + 120);
	cpApp->mcFontRegular.Print("Earth as we speak. The aliens have made their intentions clear.", SsTextPosition.fX, SsTextPosition.fY + 140);
	cpApp->mcFontRegular.Print("They intend to rob the Earth of all vegetables! For what purpose", SsTextPosition.fX, SsTextPosition.fY + 160);
	cpApp->mcFontRegular.Print("we are not certain.", SsTextPosition.fX, SsTextPosition.fY + 180);

	cpApp->mcFontRegular.Print("We tried compromising with them, saying we would trade our", SsTextPosition.fX, SsTextPosition.fY + 220);
	cpApp->mcFontRegular.Print("vegetables for some knowledge or technology, but they would not", SsTextPosition.fX, SsTextPosition.fY + 240);
	cpApp->mcFontRegular.Print("hear of it. They said either surrender all of the vegetables and", SsTextPosition.fX, SsTextPosition.fY + 260);
	cpApp->mcFontRegular.Print("seeds on Earth or they would take them by force. Obviously we", SsTextPosition.fX, SsTextPosition.fY + 280);
	cpApp->mcFontRegular.Print("cannot simply surrender all Earths vegetables into extinction.", SsTextPosition.fX, SsTextPosition.fY + 300);
	cpApp->mcFontRegular.Print("How would our children grow to be tall, and what would become of", SsTextPosition.fX, SsTextPosition.fY + 320);
	cpApp->mcFontRegular.Print("the lovable fictional character The Green Giant. He would turn", SsTextPosition.fX, SsTextPosition.fY + 340);
	cpApp->mcFontRegular.Print("into a myth.", SsTextPosition.fX, SsTextPosition.fY + 360);

	cpApp->mcFontRegular.Print("As you can see, the price of giving up all vegetables on Earth", SsTextPosition.fX, SsTextPosition.fY + 400);
	cpApp->mcFontRegular.Print("is simply too high and would be a tragety. This is why we need", SsTextPosition.fX, SsTextPosition.fY + 420);
	cpApp->mcFontRegular.Print("you, our top pilot, to go into space and intercept and destroy", SsTextPosition.fX, SsTextPosition.fY + 440);
	cpApp->mcFontRegular.Print("the alien mothership.", SsTextPosition.fX, SsTextPosition.fY + 460);
	
	cpApp->mcFontRegular.Print("Our intelligence has gathered that the mothership has some type", SsTextPosition.fX, SsTextPosition.fY + 500);
	cpApp->mcFontRegular.Print("of device capable of gathering and removing all of the vegetables", SsTextPosition.fX, SsTextPosition.fY + 520);
	cpApp->mcFontRegular.Print("on Earth within a matter of days! We believe that if we eliminate", SsTextPosition.fX, SsTextPosition.fY + 540);
	cpApp->mcFontRegular.Print("the mothership the aliens will have no reason to continue on to", SsTextPosition.fX, SsTextPosition.fY + 560);
	cpApp->mcFontRegular.Print("Earth, since their vegetable harvister will be destroyed. So we", SsTextPosition.fX, SsTextPosition.fY + 580);
	cpApp->mcFontRegular.Print("need that mothership destroyed! We would definately appreciate", SsTextPosition.fX, SsTextPosition.fY + 600);
	cpApp->mcFontRegular.Print("you destroying any other alien ships you encounter along the", SsTextPosition.fX, SsTextPosition.fY + 620);
	cpApp->mcFontRegular.Print("way, but destroying the mothership is your main objective.", SsTextPosition.fX, SsTextPosition.fY + 640);
	
	cpApp->mcFontRegular.Print("Be careful and good luck!", SsTextPosition.fX, SsTextPosition.fY + 680);

	cpApp->mcFontRegular.End();


	//********** Lock Graphics into the Back Buffer **********

	// Unlock the Back Buffer
	cpApp->mcGraphics.EndScene();
}

// Displays the controls
void CApp::GameStateDisplayHighScores(void* vpOptionalDataToPass, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpOptionalDataToPass;		// cpApp = our CApp class

	static char* ScpName = 0;	// Holds Names of players
	
	unsigned int uiScore = 0;	// Holds Scores of players
	int iIndex = 0;				// Temp counting variable

	D3DCOLOR sHighScoresTitleColor = D3DCOLOR_RGBA(255, 0, 0, 255);
	D3DCOLOR sHighScoresHeadingsColor = D3DCOLOR_RGBA(0, 0, 200, 255);
	D3DCOLOR sScoresColor = D3DCOLOR_RGBA(200, 200, 0, 255);

	int iTitleTextX				= 275;
	int iTitleTextY				= 5;
	int iEasyHeadingTextX		= 250;
	int iEasyHeadingTextY		= 50;
	int iNormalHeadingTextX		= 250;
	int iNormalHeadingTextY		= 200;
	int iHardHeadingTextX		= 250;
	int iHardHeadingTextY		= 350;
	int iScoresTextX			= 150;
	int iFirstPlaceTextY		= 80;
	int iSecondPlaceTextY		= 110;
	int iThirdPlaceTextY		= 140;
	int iModeTextYDiffererence	= 150;	// Y Difference between Easy/Norma/Hard Mode text

	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		ScpName = new char[GiMAX_NAME_LENGTH];

		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		// Delete allocated Name memory
		delete ScpName;

		return;
	}
	// Else this state is just doing a normal run


	//********** Clear the Display **********

	// Clear the Back Buffer to Black
	cpApp->mcGraphics.ClearDisplay(D3DCOLOR_RGBA(0, 0, 0, 255));

	// Lock the Back Buffer and do the drawing
	cpApp->mcGraphics.BeginScene();


	//********** Get and Process Input **********

	// If user is using the Mouse
	if (cpApp->mcMouse.GetXDelta() != 0 || cpApp->mcMouse.GetYDelta() != 0 ||
		cpApp->mcMouse.GetButtonState(cpApp->mucMouseFire) || cpApp->mcMouse.GetButtonState(cpApp->mucMouseNextWeapon) ||
		cpApp->mcMouse.GetButtonState(cpApp->mucMouseDetonateBullet))
	{
		// Show the Mouse
		cpApp->mbUsingMouse = true;
	}

	// If user is using the Keyboard
	if (cpApp->mcKeyboard.GetKeyState(DIK_UP) || cpApp->mcKeyboard.GetKeyState(DIK_DOWN) ||
		cpApp->mcKeyboard.GetKeyState(DIK_LEFT) || cpApp->mcKeyboard.GetKeyState(DIK_RIGHT))
	{
		// Hide the Mouse
		cpApp->mbUsingMouse = false;
	}

	// If user presses Enter, Right clicks, Middle clicks, or Left clicks Back To Main Menu
	if ((cpApp->mcKeyboard.GetKeyState(DIK_RETURN) || cpApp->mcMouse.GetButtonState(MOUSE_RBUTTON) ||
		cpApp->mcMouse.GetButtonState(MOUSE_MBUTTON) ||
		(cpApp->mcMouse.GetXPos() > 540 && cpApp->mcMouse.GetXPos() < 630 &&
		 cpApp->mcMouse.GetYPos() > 430 && cpApp->mcMouse.GetYPos() < 470 &&
		 cpApp->mcMouse.GetButtonState(MOUSE_LBUTTON))) &&
		 cpApp->mcSelectDelayTimer.XTimeHasPassedUsingSavedTime(GiMENU_SELECT_DELAY_TIME))
	{
		cpApp->mcGameStateManager.ShutdownCurrentStateAndSetNextState(GameStateMainMenu, cpApp);
	}

	//********** Display Headings **********

	// Start Fonts
	cpApp->mcFontRegular.Begin();

	// Display Headings
	cpApp->mcFontRegular.Print("High Scores", iTitleTextX, iTitleTextY, 0, 0, sHighScoresTitleColor);
	cpApp->mcFontRegular.Print("Easy Mode Scores", iEasyHeadingTextX, iEasyHeadingTextY, 0, 0, sHighScoresHeadingsColor);
	cpApp->mcFontRegular.Print("Normal Mode Scores", iNormalHeadingTextX, iNormalHeadingTextY, 0, 0, sHighScoresHeadingsColor);
	cpApp->mcFontRegular.Print("Hard Mode Scores", iHardHeadingTextX, iHardHeadingTextY, 0, 0, sHighScoresHeadingsColor);


	//********** Read in and Display Easy Mode High Scores

	// Display Scores

	// Loop through each mode
	for (iIndex = 0; iIndex < 3; iIndex++)
	{
		// Check which difficulty mode Scores to load
		switch (iIndex)
		{
			// If we should load the Easy Mode Scores
			case 0:

				// Read in Ease Mode High Scores
				cpApp->mcHighScores.Load("Data\\High Scores\\EasyHighScores.dat");

			break;

			// If we should load the Normal Mode Scores
			case 1:

				// Read in Normal Mode High Scores
				cpApp->mcHighScores.Load("Data\\High Scores\\NormalHighScores.dat");

			break;

			// If we should load the Hard Mode Scores
			case 2:

				// Read in Hard Mode High Scores
				cpApp->mcHighScores.Load("Data\\High Scores\\HardHighScores.dat");

			break;

			// If something went wrong
			default:
				MessageBox(cpApp->GethWnd(), "Error loading High Scores", "Error loading high scores", MB_OK);
			break;
		};

		// If there is someone in first place
		if (cpApp->mcHighScores.ReturnName(1, ScpName))
		{
			// Get their Score
			uiScore = cpApp->mcHighScores.ReturnScore(1);

			// Display the High Score
			cpApp->mcFontRegular.Print("1.", iScoresTextX, iFirstPlaceTextY + (iIndex * iModeTextYDiffererence), 0, 0, sScoresColor);
			cpApp->mcFontRegular.Print(ScpName, iScoresTextX + 20, iFirstPlaceTextY + (iIndex * iModeTextYDiffererence), 0, 0, sScoresColor);
		}

		// If there is someone in second place
		if (cpApp->mcHighScores.ReturnName(2, ScpName))
		{
			// Get their Score
			uiScore = cpApp->mcHighScores.ReturnScore(2);

			// Display the High Score
			cpApp->mcFontRegular.Print("2.", iScoresTextX, iSecondPlaceTextY + (iIndex * iModeTextYDiffererence), 0, 0, sScoresColor);
			cpApp->mcFontRegular.Print(ScpName, iScoresTextX + 20, iSecondPlaceTextY + (iIndex * iModeTextYDiffererence), 0, 0, sScoresColor);
		}

		// If there is someone in third place
		if (cpApp->mcHighScores.ReturnName(3, ScpName))
		{
			// Get their Score
			uiScore = cpApp->mcHighScores.ReturnScore(3);

			// Display the High Score
			cpApp->mcFontRegular.Print("3.", iScoresTextX, iThirdPlaceTextY + (iIndex * iModeTextYDiffererence), 0, 0, sScoresColor);
			cpApp->mcFontRegular.Print(ScpName, iScoresTextX + 20, iThirdPlaceTextY + (iIndex * iModeTextYDiffererence), 0, 0, sScoresColor);
		}
	}


	//********** Lock Graphics into the Back Buffer **********

	// Stop Fonts
	cpApp->mcFontRegular.End();

	// Unlock the Back Buffer
	cpApp->mcGraphics.EndScene();
}

// Allows Player to Enter a High Score
void CApp::GameStateEnterHighScore(void* vpOptionalDataToPass, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpOptionalDataToPass;		// cpApp = our CApp class


	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		return;
	}
	// Else this state is just doing a normal run

	//********** Clear the Display **********

	// Clear the Back Buffer to Black
	cpApp->mcGraphics.ClearDisplay(D3DCOLOR_RGBA(0, 0, 0, 255));

	// Lock the Back Buffer and do the drawing
	cpApp->mcGraphics.BeginScene();



	//********** Lock Graphics into the Back Buffer **********

	// Unlock the Back Buffer
	cpApp->mcGraphics.EndScene();
}

// Displays the Controls
void CApp::GameStateControls(void* vpOptionalDataToPass, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpOptionalDataToPass;		// cpApp = our CApp class


	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		return;
	}
	// Else this state is just doing a normal run

	// If user is using the Mouse
	if (cpApp->mcMouse.GetXDelta() != 0 || cpApp->mcMouse.GetYDelta() != 0 ||
		cpApp->mcMouse.GetButtonState(cpApp->mucMouseFire) || cpApp->mcMouse.GetButtonState(cpApp->mucMouseNextWeapon) ||
		cpApp->mcMouse.GetButtonState(cpApp->mucMouseDetonateBullet))
	{
		// Show the Mouse
		cpApp->mbUsingMouse = true;
	}

	// If user is using the Keyboard
	if (cpApp->mcKeyboard.GetKeyState(DIK_UP) || cpApp->mcKeyboard.GetKeyState(DIK_DOWN) ||
		cpApp->mcKeyboard.GetKeyState(DIK_LEFT) || cpApp->mcKeyboard.GetKeyState(DIK_RIGHT))
	{
		// Hide the Mouse
		cpApp->mbUsingMouse = false;
	}

	// If user presses Enter, Right clicks, Middle clicks, or Left clicks Back To Main Menu
	if ((cpApp->mcKeyboard.GetKeyState(DIK_RETURN) || cpApp->mcMouse.GetButtonState(MOUSE_RBUTTON) ||
		cpApp->mcMouse.GetButtonState(MOUSE_MBUTTON) ||
		(cpApp->mcMouse.GetXPos() > 540 && cpApp->mcMouse.GetXPos() < 630 &&
		 cpApp->mcMouse.GetYPos() > 430 && cpApp->mcMouse.GetYPos() < 470 &&
		 cpApp->mcMouse.GetButtonState(MOUSE_LBUTTON))) &&
		 cpApp->mcSelectDelayTimer.XTimeHasPassedUsingSavedTime(GiMENU_SELECT_DELAY_TIME))
	{
		cpApp->mcGameStateManager.ShutdownCurrentStateAndSetNextState(GameStateMainMenu, cpApp);
	}

	//********** Clear the Display **********

	// Clear the Back Buffer to Black
	cpApp->mcGraphics.ClearDisplay(D3DCOLOR_RGBA(0, 0, 0, 255));

	// Lock the Back Buffer and do the drawing
	cpApp->mcGraphics.BeginScene();


	//********** Display the Controls and Background **********

	cpApp->mcTexture[GiCONTROLS_TEXTURE].Blit(0, 0, 0, 0, 640, 480);


	//********** Lock Graphics into the Back Buffer **********

	// Unlock the Back Buffer
	cpApp->mcGraphics.EndScene();
}

// Displays mini Storylines encountered while playing
void CApp::GameStateSideStory(void* vpOptionalDataToPass, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpOptionalDataToPass;		// cpApp = our CApp class


	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		return;
	}
	// Else this state is just doing a normal run


	//********** Clear the Display **********

	// Clear the Back Buffer to Black
	cpApp->mcGraphics.ClearDisplay(D3DCOLOR_RGBA(0, 0, 0, 255));

	// Lock the Back Buffer and do the drawing
	cpApp->mcGraphics.BeginScene();










	//********** Lock Graphics into the Back Buffer **********

	// Unlock the Back Buffer
	cpApp->mcGraphics.EndScene();
}

// Displays Space Station to buy stuff
void CApp::GameStateSpaceStation(void* vpOptionalDataToPass, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpOptionalDataToPass;		// cpApp = our CApp class

	
	//***** Stars and Space Station Picture Variables *****

	static SVertex SsStar[GiNUM_OF_SPACE_STATION_BACKGROUND_STARS];	// Holds background star properties
	static CCharacterPicture* cpSpaceStationPicture;

	int iIndex = 0;					// Temp counting variable
	int iBrightness = 0;			// Temp variable to hold Stars Brightness
	bool bChoiceIsValid = false;	// Temp variable to tell if Selected Choice is Valid


	//***** Purchasing Variables *****

	const int iTOP_CHOICE								= 0;
	const int iREPAIR_SHIP								= 0;
	const int iUPGRADE_HEALTH							= 1;
	const int iUPGRADE_ARMOR							= 2;
	const int iUPGRADE_MAX_VELOCITY						= 3;
	const int iUPGRADE_ACCELERATION						= 4;
	const int iUPGRADE_NORMAL_WEAPON					= 5;
	const int iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON	= 6;
	const int iLEAVE_SPACE_STATION						= 7;
	const int iBOTTOM_CHOICE							= 7;

	static int SiCurrentChoice	= iREPAIR_SHIP;	// Holds which option is currently Selected
	static int SiLastChoice = iREPAIR_SHIP;		// Used to tell if Choice was changed or not
	
	int iaCost[8];					// Holds how much each Item costs to Upgrade
	bool baIsFullyUpgraded[8];		// Holds which options are Fully Upgraded already

	char caString[20];				// Temp String holder

	// Text Positions
	int iaTextXPosition[8] = {50, 50, 50, 50, 50, 50, 50, 50};
	int iaTextYPosition[8] = {50, 80, 110, 140, 170, 200, 230, 300};

	// Text Colors
	D3DCOLOR sSpaceStationTitleTextColor = D3DCOLOR_RGBA(200, 0, 100, 255);
	D3DCOLOR sCurrentLevelTextColor = D3DCOLOR_RGBA(0, 0, 255, 255);
	D3DCOLOR sOtherLevelTextColor = D3DCOLOR_RGBA(255, 255, 255, 255);
	D3DCOLOR sCanUpgradeTextColor = D3DCOLOR_RGBA(0, 200, 0, 255);
	D3DCOLOR sNotEnoughPointsToUpgradeTextColor = D3DCOLOR_RGBA(200, 0, 0, 255);
	D3DCOLOR sFullyUpgradedTextColor = D3DCOLOR_RGBA(255, 255, 0, 255);
	D3DCOLOR sNormalTextColor = D3DCOLOR_RGBA(200, 50, 50, 255);
	D3DCOLOR sTempTextColor = D3DCOLOR_RGBA(255, 255, 255, 255);

	// Initialize arrays
	for (iIndex = 0; iIndex < 8; iIndex++)
	{
		iaCost[iIndex] = 0;
		baIsFullyUpgraded[iIndex] = false;
	}


	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		// Create Space Station Picture
		cpSpaceStationPicture = new CCharacterPicture(cpApp->mcSpaceStationModel);

		// Initialize Stars
		for (iIndex = 0; iIndex < GiNUM_OF_SPACE_STATION_BACKGROUND_STARS; iIndex++)
		{
			SsStar[iIndex].fX		= rand() % GiWINDOW_WIDTH;
			SsStar[iIndex].fY		= rand() % GiWINDOW_HEIGHT;
			SsStar[iIndex].fZ		= 0;
			SsStar[iIndex].fRHW		= 1;

			// Give Stars a random Brightness
			iBrightness = rand() % 255;
			SsStar[iIndex].diffuse = D3DCOLOR_RGBA(255, 255, 255, iBrightness);
		}

		// Set the initial choice
		SiCurrentChoice = SiLastChoice = iREPAIR_SHIP;

		// Stop any other Music from playing
		cpApp->StopMusic();

		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		// Delete the Space Station Picture
		delete cpSpaceStationPicture;

		return;
	}
	// Else this state is just doing a normal run


	//********** Clear the Display **********

	// Clear the Back Buffer to Black
	cpApp->mcGraphics.ClearDisplay(D3DCOLOR_RGBA(0, 0, 0, 255));

	// Lock the Back Buffer and do the drawing
	cpApp->mcGraphics.BeginScene();


	//********** Draw the Background Stars and Space Station **********

	// Place Star into Vertex Buffer and Draw them
	cpApp->mcVertexBuffer.Set(0, GiNUM_OF_SPACE_STATION_BACKGROUND_STARS, SsStar);
	cpApp->mcVertexBuffer.Render(0, GiNUM_OF_SPACE_STATION_BACKGROUND_STARS, D3DPT_POINTLIST);

	// Update and Draw the Space Station
	cpSpaceStationPicture->UpdatePicture(cpApp->ReturnAmountOfTimeSinceLastFrame());
	cpApp->mcTexture[GiSPACE_STATION_TEXTURE].Blit(450, 20, cpSpaceStationPicture->ReturnLeft(), cpSpaceStationPicture->ReturnTop(), cpSpaceStationPicture->ReturnWidth(), cpSpaceStationPicture->ReturnHeight()); 


	//********** Display the list of things the Player can purchase **********

	cpApp->mcFontRegular.Begin();

	// Print Name of Space Station
	cpApp->mcFontRegular.Print("Galaxy Service Center", 125, 5, 0, 0, sSpaceStationTitleTextColor);


	// Display Repair Ship Stuff

	// If Hit Points are full
	if (cpApp->mcaPlayer[GiPLAYER_1].ReturnHealth() == cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxHealth())
	{
		// Set Text Color to Fully Upgraded
		sTempTextColor = sFullyUpgradedTextColor;

		// Mark that this option is Fully Upgraded
		baIsFullyUpgraded[iREPAIR_SHIP] = true;
	}
	// Else Hit Points are not full
	else
	{
		// Check the Number of Points needed to fully recover Health
		iaCost[iREPAIR_SHIP] = (cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxHealth() - cpApp->mcaPlayer[GiPLAYER_1].ReturnHealth());

		// If Player has some Points
		if (cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() > 0)
		{
			// Set Text Color to Can Upgrade
			sTempTextColor = sCanUpgradeTextColor;
		}
		// Else Player doesn't have any Points
		else
		{
			// Set Text Color to Not Enough Points To Upgrade
			sTempTextColor = sNotEnoughPointsToUpgradeTextColor;
		}

		// Mark that this option is not Fully Upgraded
		baIsFullyUpgraded[iREPAIR_SHIP] = false;
	}

	// Display "Repair Ship"
	cpApp->mcFontRegular.Print("Repair Ship", iaTextXPosition[iREPAIR_SHIP], iaTextYPosition[iREPAIR_SHIP], 0, 0, sTempTextColor);

	// Display HP
	itoa(cpApp->mcaPlayer[GiPLAYER_1].ReturnHealth(), caString, 10);
	cpApp->mcFontRegular.Print(caString, 300, 50, 0, 0, sTempTextColor);
	cpApp->mcFontRegular.Print("/", 350, 50, 0, 0, sTempTextColor);
	itoa(cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxHealth(), caString, 10);
	cpApp->mcFontRegular.Print(caString, 370, 50, 0, 0, sTempTextColor);


	// Display Upgrade Total Health Stuff

	// If Health has been Upgraded to the Max
	if (cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxHealth() == GiPLAYER_MAX_HEALTH)
	{
		// Set Text Color to Fully Upgraded
		sTempTextColor = sFullyUpgradedTextColor;

		// Mark that this option is Fully Upgraded
		baIsFullyUpgraded[iUPGRADE_HEALTH] = true;
	}
	// Else Health can still be Upgraded
	else
	{
		// Find out how much it will Cost to Upgrade Health to next Level and display Current Level
		switch (cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxHealth())
		{
			case GiPLAYER_LEVEL_1_MAX_HEALTH:

				// Set the Cost to Upgrade
				iaCost[iUPGRADE_HEALTH] = GiPLAYER_LEVEL_2_UPGRADE_MAX_HEALTH_COST;

				// Display 3 Health Levels and highlight Current one
				cpApp->mcFontRegular.Print("1", 300, 80, 0, 0, sCurrentLevelTextColor);
				cpApp->mcFontRegular.Print("2", 320, 80, 0, 0, sOtherLevelTextColor);
				cpApp->mcFontRegular.Print("3", 340, 80, 0, 0, sOtherLevelTextColor);
			break;

			case GiPLAYER_LEVEL_2_MAX_HEALTH:

				// Set the Cost to Upgrade
				iaCost[iUPGRADE_HEALTH] = GiPLAYER_LEVEL_3_UPGRADE_MAX_HEALTH_COST;

				// Display 3 Health Levels and highlight Current one
				cpApp->mcFontRegular.Print("1", 300, 80, 0, 0, sOtherLevelTextColor);
				cpApp->mcFontRegular.Print("2", 320, 80, 0, 0, sCurrentLevelTextColor);
				cpApp->mcFontRegular.Print("3", 340, 80, 0, 0, sOtherLevelTextColor);
			break;

			case GiPLAYER_LEVEL_3_MAX_HEALTH:

				// Set the Cost to Upgrade
				iaCost[iUPGRADE_HEALTH] = 0;

				// Display 3 Health Levels and highlight Current one
				cpApp->mcFontRegular.Print("1", 300, 80, 0, 0, sOtherLevelTextColor);
				cpApp->mcFontRegular.Print("2", 320, 80, 0, 0, sOtherLevelTextColor);
				cpApp->mcFontRegular.Print("3", 340, 80, 0, 0, sCurrentLevelTextColor);
			break;

			default:
				MessageBox(cpApp->GethWnd(), "Error in Displaying Health", "Unrecognized case", MB_OK);
			break;
		};

		// If Player has enough Points to Upgrade
		if (cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() >= iaCost[iUPGRADE_HEALTH])
		{
			// Set Text Color to Can Upgrade
			sTempTextColor = sCanUpgradeTextColor;
		}
		// Else Player does not have enough Points to Upgrade
		else
		{
			// Set Text Color to Not Enough Points To Upgrade
			sTempTextColor = sNotEnoughPointsToUpgradeTextColor;
		}

		// Mark that this option is not Fully Upgraded
		baIsFullyUpgraded[iUPGRADE_HEALTH] = false;
	}

	// Display "Upgrade Total Health"
	cpApp->mcFontRegular.Print("Upgrade Total Health", iaTextXPosition[iUPGRADE_HEALTH], iaTextYPosition[iUPGRADE_HEALTH], 0, 0, sTempTextColor);


	// Display Upgrade Total Armor Stuff

	// If Armor has been Upgraded to the Max
	if (cpApp->mcaPlayer[GiPLAYER_1].ReturnArmor() == GfPLAYER_MAX_ARMOR)
	{
		// Set Text Color to Fully Upgraded
		sTempTextColor = sFullyUpgradedTextColor;

		// Mark that this option is Fully Upgraded
		baIsFullyUpgraded[iUPGRADE_ARMOR] = true;
	}
	// Else Armor can still be Upgraded
	else
	{
		// Find out how much it will Cost to Upgrade Armor to next Level and display Current Level
		if (cpApp->mcaPlayer[GiPLAYER_1].ReturnArmor() == GfPLAYER_LEVEL_1_ARMOR)
		{
			// Set the Cost to Upgrade
			iaCost[iUPGRADE_ARMOR] = GiPLAYER_LEVEL_2_UPGRADE_ARMOR_COST;

			// Display 3 Armor Levels and highlight Current one
			cpApp->mcFontRegular.Print("1", 300, 110, 0, 0, sCurrentLevelTextColor);
			cpApp->mcFontRegular.Print("2", 320, 110, 0, 0, sOtherLevelTextColor);
			cpApp->mcFontRegular.Print("3", 340, 110, 0, 0, sOtherLevelTextColor);
		}
		else if (cpApp->mcaPlayer[GiPLAYER_1].ReturnArmor() == GfPLAYER_LEVEL_2_ARMOR)
		{
			// Set the Cost to Upgrade
			iaCost[iUPGRADE_ARMOR] = GiPLAYER_LEVEL_3_UPGRADE_ARMOR_COST;

			// Display 3 Armor Levels and highlight Current one
			cpApp->mcFontRegular.Print("1", 300, 110, 0, 0, sOtherLevelTextColor);
			cpApp->mcFontRegular.Print("2", 320, 110, 0, 0, sCurrentLevelTextColor);
			cpApp->mcFontRegular.Print("3", 340, 110, 0, 0, sOtherLevelTextColor);
		}
		else if (cpApp->mcaPlayer[GiPLAYER_1].ReturnArmor() == GfPLAYER_LEVEL_3_ARMOR)
		{
			// Set the Cost to Upgrade
			iaCost[iUPGRADE_ARMOR] = 0;

			// Display 3 Armor Levels and highlight Current one
			cpApp->mcFontRegular.Print("1", 300, 110, 0, 0, sOtherLevelTextColor);
			cpApp->mcFontRegular.Print("2", 320, 110, 0, 0, sOtherLevelTextColor);
			cpApp->mcFontRegular.Print("3", 340, 110, 0, 0, sCurrentLevelTextColor);
		}
		else
		{
			MessageBox(cpApp->GethWnd(), "Error in Displaying Armor", "Unrecognized case", MB_OK);	
		}

		// If Player has enough Points to Upgrade
		if (cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() >= iaCost[iUPGRADE_ARMOR])
		{
			// Set Text Color to Can Upgrade
			sTempTextColor = sCanUpgradeTextColor;
		}
		// Else Player does not have enough Points to Upgrade
		else
		{
			// Set Text Color to Not Enough Points To Upgrade
			sTempTextColor = sNotEnoughPointsToUpgradeTextColor;
		}

		// Mark that this option is not Fully Upgraded
		baIsFullyUpgraded[iUPGRADE_ARMOR] = false;
	}

	// Display "Upgrade Total Armor"
	cpApp->mcFontRegular.Print("Upgrade Total Armor", iaTextXPosition[iUPGRADE_ARMOR], iaTextYPosition[iUPGRADE_ARMOR], 0, 0, sTempTextColor);


	// Display Upgrade Max Velocity Stuff

	// If Max Velocity has been Upgraded to the Max
	if (cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxXVelocity() == GiPLAYER_MAX_X_VELOCITY)
	{
		// Set Text Color to Fully Upgraded
		sTempTextColor = sFullyUpgradedTextColor;

		// Mark that this option is Fully Upgraded
		baIsFullyUpgraded[iUPGRADE_MAX_VELOCITY] = true;
	}
	// Else Max Velocity can still be Upgraded
	else
	{
		// Find out how much it will Cost to Upgrade Max Velocity to next Level and display Current Level
		switch ((int)floorf(cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxXVelocity()))
		{
			case GiPLAYER_LEVEL_1_MAX_X_VELOCITY:

				// Set the Cost to Upgrade
				iaCost[iUPGRADE_MAX_VELOCITY] = GiPLAYER_LEVEL_2_UPGRADE_MAX_VELOCITY_COST;

				// Display 3 Max Velocity Levels and highlight Current one
				cpApp->mcFontRegular.Print("1", 300, 140, 0, 0, sCurrentLevelTextColor);
				cpApp->mcFontRegular.Print("2", 320, 140, 0, 0, sOtherLevelTextColor);
				cpApp->mcFontRegular.Print("3", 340, 140, 0, 0, sOtherLevelTextColor);
			break;

			case GiPLAYER_LEVEL_2_MAX_X_VELOCITY:

				// Set the Cost to Upgrade
				iaCost[iUPGRADE_MAX_VELOCITY] = GiPLAYER_LEVEL_3_UPGRADE_MAX_VELOCITY_COST;

				// Display 3 Max Velocity Levels and highlight Current one
				cpApp->mcFontRegular.Print("1", 300, 140, 0, 0, sOtherLevelTextColor);
				cpApp->mcFontRegular.Print("2", 320, 140, 0, 0, sCurrentLevelTextColor);
				cpApp->mcFontRegular.Print("3", 340, 140, 0, 0, sOtherLevelTextColor);
			break;

			case GiPLAYER_LEVEL_3_MAX_X_VELOCITY:

				// Set the Cost to Upgrade
				iaCost[iUPGRADE_MAX_VELOCITY] = 0;

				// Display 3 Max Velocity Levels and highlight Current one
				cpApp->mcFontRegular.Print("1", 300, 140, 0, 0, sOtherLevelTextColor);
				cpApp->mcFontRegular.Print("2", 320, 140, 0, 0, sOtherLevelTextColor);
				cpApp->mcFontRegular.Print("3", 340, 140, 0, 0, sCurrentLevelTextColor);
			break;

			default:
				MessageBox(cpApp->GethWnd(), "Error in Displaying Max Velocity", "Unrecognized case", MB_OK);
			break;
		};

		// If Player has enough Points to Upgrade
		if (cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() >= iaCost[iUPGRADE_MAX_VELOCITY])
		{
			// Set Text Color to Can Upgrade
			sTempTextColor = sCanUpgradeTextColor;
		}
		// Else Player does not have enough Points to Upgrade
		else
		{
			// Set Text Color to Not Enough Points To Upgrade
			sTempTextColor = sNotEnoughPointsToUpgradeTextColor;
		}

		// Mark that this option is not Fully Upgraded
		baIsFullyUpgraded[iUPGRADE_MAX_VELOCITY] = false;
	}

	// Display "Upgrade Top Speed"
	cpApp->mcFontRegular.Print("Upgrade Top Speed", iaTextXPosition[iUPGRADE_MAX_VELOCITY], iaTextYPosition[iUPGRADE_MAX_VELOCITY], 0, 0, sTempTextColor);


	// Display Upgrade Acceleration Stuff

	// If Acceleration has been Upgraded to the Max
	if (cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxXAcceleration() == GiPLAYER_MAX_X_ACCELERATION)
	{
		// Set Text Color to Fully Upgraded
		sTempTextColor = sFullyUpgradedTextColor;

		// Mark that this option is Fully Upgraded
		baIsFullyUpgraded[iUPGRADE_ACCELERATION] = true;
	}
	// Else Acceleration can still be Upgraded
	else
	{
		// Find out how much it will Cost to Upgrade Acceleration to next Level and display Current Level
		switch ((int)floorf(cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxXAcceleration()))
		{
			case GiPLAYER_LEVEL_1_MAX_X_ACCELERATION:

				// Set the Cost to Upgrade
				iaCost[iUPGRADE_ACCELERATION] = GiPLAYER_LEVEL_2_UPGRADE_ACCELERATION_COST;

				// Display 3 Acceleration Levels and highlight Current one
				cpApp->mcFontRegular.Print("1", 300, 170, 0, 0, sCurrentLevelTextColor);
				cpApp->mcFontRegular.Print("2", 320, 170, 0, 0, sOtherLevelTextColor);
				cpApp->mcFontRegular.Print("3", 340, 170, 0, 0, sOtherLevelTextColor);
			break;

			case GiPLAYER_LEVEL_2_MAX_X_ACCELERATION:

				// Set the Cost to Upgrade
				iaCost[iUPGRADE_ACCELERATION] = GiPLAYER_LEVEL_3_UPGRADE_ACCELERATION_COST;

				// Display 3 Acceleration Levels and highlight Current one
				cpApp->mcFontRegular.Print("1", 300, 170, 0, 0, sOtherLevelTextColor);
				cpApp->mcFontRegular.Print("2", 320, 170, 0, 0, sCurrentLevelTextColor);
				cpApp->mcFontRegular.Print("3", 340, 170, 0, 0, sOtherLevelTextColor);
			break;

			case GiPLAYER_LEVEL_3_MAX_X_ACCELERATION:

				// Set the Cost to Upgrade
				iaCost[iUPGRADE_ACCELERATION] = 0;

				// Display 3 Acceleration Levels and highlight Current one
				cpApp->mcFontRegular.Print("1", 300, 170, 0, 0, sOtherLevelTextColor);
				cpApp->mcFontRegular.Print("2", 320, 170, 0, 0, sOtherLevelTextColor);
				cpApp->mcFontRegular.Print("3", 340, 170, 0, 0, sCurrentLevelTextColor);
			break;

			default:
				MessageBox(cpApp->GethWnd(), "Error in Displaying Acceleration", "Unrecognized case", MB_OK);
			break;
		};

		// If Player has enough Points to Upgrade
		if (cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() >= iaCost[iUPGRADE_ACCELERATION])
		{
			// Set Text Color to Can Upgrade
			sTempTextColor = sCanUpgradeTextColor;
		}
		// Else Player does not have enough Points to Upgrade
		else
		{
			// Set Text Color to Not Enough Points To Upgrade
			sTempTextColor = sNotEnoughPointsToUpgradeTextColor;
		}

		// Mark that this option is not Fully Upgraded
		baIsFullyUpgraded[iUPGRADE_ACCELERATION] = false;
	}

	// Display "Upgrade Acceleration"
	cpApp->mcFontRegular.Print("Upgrade Acceleration", iaTextXPosition[iUPGRADE_ACCELERATION], iaTextYPosition[iUPGRADE_ACCELERATION], 0, 0, sTempTextColor);


	// Display Upgrade Normal Weapon Stuff

	// If Normal Weapon has been Upgraded
	if (cpApp->mcaPlayer[GiPLAYER_1].WeaponIsInList(GiTHREE_NORMAL_WEAPON))
	{
		// Set Text Color to Fully Upgraded
		sTempTextColor = sFullyUpgradedTextColor;

		// Mark that this option is Fully Upgraded
		baIsFullyUpgraded[iUPGRADE_NORMAL_WEAPON] = true;
	}
	// Else Normal Weapon can still be Upgraded
	else
	{
		// Find out how much it will Cost to Upgrade Normal Weapon
		iaCost[iUPGRADE_NORMAL_WEAPON] = GiPLAYER_THREE_NORMAL_WEAPON_COST;

		// If Player has enough Points to Upgrade
		if (cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() >= iaCost[iUPGRADE_NORMAL_WEAPON])
		{
			// Set Text Color to Can Upgrade
			sTempTextColor = sCanUpgradeTextColor;
		}
		// Else Player does not have enough Points to Upgrade
		else
		{
			// Set Text Color to Not Enough Points To Upgrade
			sTempTextColor = sNotEnoughPointsToUpgradeTextColor;
		}

		// Mark that this option is not Fully Upgraded
		baIsFullyUpgraded[iUPGRADE_NORMAL_WEAPON] = false;
	}

	// Display "Upgrade Normal Weapon"
	cpApp->mcFontRegular.Print("Upgrade Normal Weapon", iaTextXPosition[iUPGRADE_NORMAL_WEAPON], iaTextYPosition[iUPGRADE_NORMAL_WEAPON], 0, 0, sTempTextColor);


	// Display Purchase/Upgrade Machine Gun Weapon Stuff

	// If Player has the Machine Gun Weapon or Three Machine Gun Weapon
	if (cpApp->mcaPlayer[GiPLAYER_1].WeaponIsInList(GiMACHINE_GUN_WEAPON) ||
		cpApp->mcaPlayer[GiPLAYER_1].WeaponIsInList(GiTHREE_MACHINE_GUN_WEAPON))
	{
		// If Player has the Three Machine Gun Weapon
		if (cpApp->mcaPlayer[GiPLAYER_1].WeaponIsInList(GiTHREE_MACHINE_GUN_WEAPON))
		{
			// Set Text Color to Fully Upgraded
			sTempTextColor = sFullyUpgradedTextColor;

			// Mark that this option is Fully Upgraded
			baIsFullyUpgraded[iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON] = true;
		}
		// Else Machine Gun Weapon can still be Purchased
		else
		{
			// Find out how much it will Cost to Upgrade Normal Weapon
			iaCost[iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON] = GiPLAYER_THREE_MACHINE_GUN_WEAPON_COST;

			// If Player has enough Points to Upgrade
			if (cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() >= iaCost[iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON])
			{
				// Set Text Color to Can Upgrade
				sTempTextColor = sCanUpgradeTextColor;
			}
			// Else Player does not have enough Points to Upgrade
			else
			{
				// Set Text Color to Not Enough Points To Upgrade
				sTempTextColor = sNotEnoughPointsToUpgradeTextColor;
			}

			// Mark that this option is not Fully Upgraded
			baIsFullyUpgraded[iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON] = false;
		}

		// Display "Upgrade Machine Gun Weapon"
		cpApp->mcFontRegular.Print("Upgrade Machine Gun Weapon", iaTextXPosition[iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON], iaTextYPosition[iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON], 0, 0, sTempTextColor);
	}
	// Else Machine Gun Weapon can still be Purchased
	else
	{
		// Find out how much it will Cost to Upgrade Normal Weapon
		iaCost[iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON] = GiPLAYER_MACHINE_GUN_WEAPON_COST;

		// If Player has enough Points to Upgrade
		if (cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() >= iaCost[iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON])
		{
			// Set Text Color to Can Upgrade
			sTempTextColor = sCanUpgradeTextColor;
		}
		// Else Player does not have enough Points to Upgrade
		else
		{
			// Set Text Color to Not Enough Points To Upgrade
			sTempTextColor = sNotEnoughPointsToUpgradeTextColor;
		}

		// Display "Purchase Machine Gun Weapon"
		cpApp->mcFontRegular.Print("Purchase Machine Gun Weapon", iaTextXPosition[iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON], iaTextYPosition[iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON], 0, 0, sTempTextColor);

		// Mark that this option is not Fully Upgraded
		baIsFullyUpgraded[iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON] = false;
	}


	// Move to next Choice down that is not Fully Upgraded
	while (baIsFullyUpgraded[SiCurrentChoice])
	{
		// Move to next Choice down
		SiCurrentChoice++;

		// If we were already at the bottom Choice
		if (SiCurrentChoice > iBOTTOM_CHOICE)
		{
			// Wrap to the top Choice
			SiCurrentChoice = iTOP_CHOICE;
		}
	}
	
	//********** Display Leave Space Station **********

	cpApp->mcFontRegular.Print("Leave Space Station", 50, 300, 0, 0, sCanUpgradeTextColor);

	cpApp->mcFontRegular.End();


	//********** Get and Process Input **********

	// If user is using the Mouse
	if (cpApp->mcMouse.GetXDelta() != 0 || cpApp->mcMouse.GetYDelta() != 0 ||
		cpApp->mcMouse.GetButtonState(cpApp->mucMouseFire) || cpApp->mcMouse.GetButtonState(cpApp->mucMouseNextWeapon) ||
		cpApp->mcMouse.GetButtonState(cpApp->mucMouseDetonateBullet))
	{
		// Show the Mouse
		cpApp->mbUsingMouse = true;
	}

	// If user is using the Keyboard
	if (cpApp->mcKeyboard.GetKeyState(DIK_UP) || cpApp->mcKeyboard.GetKeyState(DIK_DOWN) ||
		cpApp->mcKeyboard.GetKeyState(DIK_LEFT) || cpApp->mcKeyboard.GetKeyState(DIK_RIGHT))
	{
		// Hide the Mouse
		cpApp->mbUsingMouse = false;
	}

	// If Up is pressed
	if (cpApp->mcKeyboard.GetKeyState(DIK_UP) &&
		cpApp->mcUpDelayTimer.XTimeHasPassedUsingSavedTime(GiMENU_UP_DELAY_TIME))
	{
		// Move to next Choice Up that is not Fully Upgraded
		do
		{
			// Move to next Choice up
			SiCurrentChoice--;

			// If we were already at the top Choice
			if (SiCurrentChoice < iTOP_CHOICE)
			{
				// Wrap to the bottom Choice
				SiCurrentChoice = iBOTTOM_CHOICE;
			}
		} while (baIsFullyUpgraded[SiCurrentChoice]);
	}

	// If Down is pressed
	if (cpApp->mcKeyboard.GetKeyState(DIK_DOWN) &&
		cpApp->mcUpDelayTimer.XTimeHasPassedUsingSavedTime(GiMENU_DOWN_DELAY_TIME))
	{
		// Move to next Choice down that is not Fully Upgraded
		do
		{
			// Move to next Choice down
			SiCurrentChoice++;

			// If we were already at the bottom Choice
			if (SiCurrentChoice > iBOTTOM_CHOICE)
			{
				// Wrap to the top Choice
				SiCurrentChoice = iTOP_CHOICE;
			}
		} while (baIsFullyUpgraded[SiCurrentChoice]);
	}

	// If Choice was changed
	if (SiCurrentChoice != SiLastChoice)
	{
		// Make the sound of changing Choices
		cpApp->PlaySoundEffect(GiChangeSelectionSound);

		// Save the Current Choice
		SiLastChoice = SiCurrentChoice;
	}

	// If user is using the Mouse
	if (cpApp->mbUsingMouse)
	{
		// Check if the Mouse is over any of the options

		// Loop through each Choice
		for (iIndex = iTOP_CHOICE; iIndex < iBOTTOM_CHOICE + 1; iIndex++)
		{
			// If Mouse is over this Choice and it is not Fully Upgraded already
			if (cpApp->mcMouse.GetXPos() < 425 && 
				cpApp->mcMouse.GetYPos() > iaTextYPosition[iIndex] &&
				cpApp->mcMouse.GetYPos() < (iaTextYPosition[iIndex] + 25) &&
				!baIsFullyUpgraded[iIndex])
			{
				// Set this as the Current Choice
				SiCurrentChoice = iIndex;
			}		
		}
	}

	// If user Selects a Choice
	if ((cpApp->mcKeyboard.GetKeyState(DIK_RETURN) || 
		(cpApp->mbUsingMouse && cpApp->mcMouse.GetButtonState(MOUSE_LBUTTON))) &&
		cpApp->mcSelectDelayTimer.XTimeHasPassedUsingSavedTime(GiMENU_SELECT_DELAY_TIME))
	{
		// Check which option is selected and process it
		switch (SiCurrentChoice)
		{
			case iREPAIR_SHIP:

				// If Ships Health is not full and Player has some Points
				if (cpApp->mcaPlayer[GiPLAYER_1].ReturnHealth() != cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxHealth() &&
					cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() > 0)
				{
					// If Ship has enough Points to fully recover Health
					if (cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() >= iaCost[iREPAIR_SHIP])
					{
						// Recover Ships Health
						cpApp->mcaPlayer[GiPLAYER_1].SetHealthRelative(iaCost[iREPAIR_SHIP]);

						// Subtract Cost from Points
						cpApp->mcaPlayer[GiPLAYER_1].SetNumberOfPointsRelative(-iaCost[iREPAIR_SHIP]);
					}
					// Else recover Health as much as possible
					else
					{
						// Recover Ships Health as much as possible
						cpApp->mcaPlayer[GiPLAYER_1].SetHealthRelative(cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints());

						// Subtract Cost from Points
						cpApp->mcaPlayer[GiPLAYER_1].SetNumberOfPointsRelative(-cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints());
					}

					// Mark that the selected Choice was valid
					bChoiceIsValid = true;
				}
				// Else Player did not have enough Points or Upgrade was maxed out
				else
				{
					// Mark that the selected Choice was not valid
					bChoiceIsValid = false;
				}

			break;

			case iUPGRADE_HEALTH:

				// If this option is not Fully Upgraded and Player has enough points
				if (!baIsFullyUpgraded[iUPGRADE_HEALTH] &&
					cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() >= iaCost[iUPGRADE_HEALTH])
				{
					// Check which Level Max Health is currently at
					switch (cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxHealth())
					{
						case GiPLAYER_LEVEL_1_MAX_HEALTH:

							// Upgrade this option to next level
							cpApp->mcaPlayer[GiPLAYER_1].SetMaxHealth(GiPLAYER_LEVEL_2_MAX_HEALTH);

							// Add Upgrade Health amount to Players current Health
							cpApp->mcaPlayer[GiPLAYER_1].SetHealthRelative(GiPLAYER_LEVEL_2_MAX_HEALTH - GiPLAYER_LEVEL_1_MAX_HEALTH);

						break;

						case GiPLAYER_LEVEL_2_MAX_HEALTH:

							// Upgrade this option to next level
							cpApp->mcaPlayer[GiPLAYER_1].SetMaxHealth(GiPLAYER_LEVEL_3_MAX_HEALTH);

							// Add Upgrade Health amount to Players current Health
							cpApp->mcaPlayer[GiPLAYER_1].SetHealthRelative(GiPLAYER_LEVEL_3_MAX_HEALTH - GiPLAYER_LEVEL_2_MAX_HEALTH);

						break;

						// If something went wrong
						default:
							MessageBox(cpApp->GethWnd(), "Error when Upgrading Health", "Unrecognized Max Health", MB_OK);
						break;
					};

					// Subtract Cost from Players Points
					cpApp->mcaPlayer[GiPLAYER_1].SetNumberOfPointsRelative(-iaCost[iUPGRADE_HEALTH]);

					// Mark that the selected Choice was valid
					bChoiceIsValid = true;
				}
				// Else Player did not have enough Points or Upgrade was maxed out
				else
				{
					// Mark that the selected Choice was not valid
					bChoiceIsValid = false;
				}

			break;

			case iUPGRADE_ARMOR:

				// If this option is not Fully Upgraded and Player has enough points
				if (!baIsFullyUpgraded[iUPGRADE_ARMOR] &&
					cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() >= iaCost[iUPGRADE_ARMOR])
				{
					// Check which level Players Armor is currently at
					if (cpApp->mcaPlayer[GiPLAYER_1].ReturnArmor() == GfPLAYER_LEVEL_1_ARMOR)
					{
						// Upgrade this option to next level
						cpApp->mcaPlayer[GiPLAYER_1].SetArmor(GfPLAYER_LEVEL_2_ARMOR);
					}
					else if (cpApp->mcaPlayer[GiPLAYER_1].ReturnArmor() == GfPLAYER_LEVEL_2_ARMOR)
					{
						// Upgrade this option to next level
						cpApp->mcaPlayer[GiPLAYER_1].SetArmor(GfPLAYER_LEVEL_3_ARMOR);
					}
					// Else If something went wrong
					else
					{
						MessageBox(cpApp->GethWnd(), "Error when Upgrading Armor", "Unrecognized Armor Level", MB_OK);
					};

					// Subtract Cost from Players Points
					cpApp->mcaPlayer[GiPLAYER_1].SetNumberOfPointsRelative(-iaCost[iUPGRADE_ARMOR]);

					// Mark that the selected Choice was valid
					bChoiceIsValid = true;
				}
				// Else Player did not have enough Points or Upgrade was maxed out
				else
				{
					// Mark that the selected Choice was not valid
					bChoiceIsValid = false;
				}

			break;

			case iUPGRADE_MAX_VELOCITY:

				// If this option is not Fully Upgraded and Player has enough points
				if (!baIsFullyUpgraded[iUPGRADE_MAX_VELOCITY] &&
					cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() >= iaCost[iUPGRADE_MAX_VELOCITY])
				{
					// Check which level Players MaxVelocity is currently at
					switch ((int)floorf(cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxXVelocity()))
					{
						case GiPLAYER_LEVEL_1_MAX_X_VELOCITY:

							// Upgrade this option to next level
							cpApp->mcaPlayer[GiPLAYER_1].SetMaxVelocities(GiPLAYER_LEVEL_2_MAX_X_VELOCITY, GiPLAYER_LEVEL_2_MAX_Y_VELOCITY);
							cpApp->mcaPlayer[GiPLAYER_1].SetMinVelocities(-GiPLAYER_LEVEL_2_MAX_X_VELOCITY, -GiPLAYER_LEVEL_2_MAX_Y_VELOCITY);

						break;

						case GiPLAYER_LEVEL_2_MAX_X_VELOCITY:

							// Upgrade this option to next level
							cpApp->mcaPlayer[GiPLAYER_1].SetMaxVelocities(GiPLAYER_LEVEL_3_MAX_X_VELOCITY, GiPLAYER_LEVEL_3_MAX_Y_VELOCITY);
							cpApp->mcaPlayer[GiPLAYER_1].SetMinVelocities(-GiPLAYER_LEVEL_3_MAX_X_VELOCITY, -GiPLAYER_LEVEL_3_MAX_Y_VELOCITY);

						break;

						// If something went wrong
						default:
							MessageBox(cpApp->GethWnd(), "Error when Upgrading Max Velocity", "Unrecognized Max Velocity Level", MB_OK);
						break;
					};

					// Subtract Cost from Players Points
					cpApp->mcaPlayer[GiPLAYER_1].SetNumberOfPointsRelative(-iaCost[iUPGRADE_MAX_VELOCITY]);

					// Mark that the selected Choice was valid
					bChoiceIsValid = true;
				}
				// Else Player did not have enough Points or Upgrade was maxed out
				else
				{
					// Mark that the selected Choice was not valid
					bChoiceIsValid = false;
				}

			break;

			case iUPGRADE_ACCELERATION:

				// If this option is not Fully Upgraded and Player has enough points
				if (!baIsFullyUpgraded[iUPGRADE_ACCELERATION] &&
					cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() >= iaCost[iUPGRADE_ACCELERATION])
				{
					// Check which level Players MaxAcceleration is currently at
					switch ((int)floorf(cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxXAcceleration()))
					{
						case GiPLAYER_LEVEL_1_MAX_X_ACCELERATION:

							// Upgrade this option to next level
							cpApp->mcaPlayer[GiPLAYER_1].SetMaxAccelerations(GiPLAYER_LEVEL_2_MAX_X_ACCELERATION, GiPLAYER_LEVEL_2_MAX_Y_ACCELERATION);
							cpApp->mcaPlayer[GiPLAYER_1].SetMinAccelerations(-GiPLAYER_LEVEL_2_MAX_X_ACCELERATION, -GiPLAYER_LEVEL_2_MAX_Y_ACCELERATION);
							cpApp->mcaPlayer[GiPLAYER_1].SetNormalAccelerations(GiPLAYER_LEVEL_2_MAX_X_ACCELERATION, GiPLAYER_LEVEL_2_MAX_Y_ACCELERATION);

						break;

						case GiPLAYER_LEVEL_2_MAX_X_ACCELERATION:

							// Upgrade this option to next level
							cpApp->mcaPlayer[GiPLAYER_1].SetMaxAccelerations(GiPLAYER_LEVEL_3_MAX_X_ACCELERATION, GiPLAYER_LEVEL_3_MAX_Y_ACCELERATION);
							cpApp->mcaPlayer[GiPLAYER_1].SetMinAccelerations(-GiPLAYER_LEVEL_3_MAX_X_ACCELERATION, -GiPLAYER_LEVEL_3_MAX_Y_ACCELERATION);
							cpApp->mcaPlayer[GiPLAYER_1].SetNormalAccelerations(GiPLAYER_LEVEL_3_MAX_X_ACCELERATION, GiPLAYER_LEVEL_3_MAX_Y_ACCELERATION);

						break;

						// If something went wrong
						default:
							MessageBox(cpApp->GethWnd(), "Error when Upgrading Max Acceleration", "Unrecognized Max Acceleration Level", MB_OK);
						break;
					};

					// Subtract Cost from Players Points
					cpApp->mcaPlayer[GiPLAYER_1].SetNumberOfPointsRelative(-iaCost[iUPGRADE_ACCELERATION]);

					// Mark that the selected Choice was valid
					bChoiceIsValid = true;
				}
				// Else Player did not have enough Points or Upgrade was maxed out
				else
				{
					// Mark that the selected Choice was not valid
					bChoiceIsValid = false;
				}

			break;

			case iUPGRADE_NORMAL_WEAPON:

				// If this option is not Fully Upgraded and Player has enough points
				if (!baIsFullyUpgraded[iUPGRADE_NORMAL_WEAPON] &&
					cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() >= iaCost[iUPGRADE_NORMAL_WEAPON])
				{
					// Add the Three Normal Gun Weapon to the Players Weapon List and set it as the Current Weapon
					cpApp->mcaPlayer[GiPLAYER_1].AddWeapon(cpApp->mcaBulletModel[GiTHREE_NORMAL_WEAPON]);
					cpApp->mcaPlayer[GiPLAYER_1].SetCurrentWeapon(GiTHREE_NORMAL_WEAPON);

                    // Remove the Normal Gun Weapon from the Players Weapon List
					cpApp->mcaPlayer[GiPLAYER_1].DeleteWeapon(GiNORMAL_WEAPON);

					// Subtract the Cost from the Players Points
					cpApp->mcaPlayer[GiPLAYER_1].SetNumberOfPointsRelative(-iaCost[iUPGRADE_NORMAL_WEAPON]);

					// Mark that the selected Choice was valid
					bChoiceIsValid = true;
				}
				// Else Player did not have enough Points or Upgrade was maxed out
				else
				{
					// Mark that the selected Choice was not valid
					bChoiceIsValid = false;
				}

			break;

			case iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON:

				// If this option is not Fully Upgraded and Player has enough points
				if (!baIsFullyUpgraded[iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON] &&
					cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints() >= iaCost[iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON])
				{
					// If Player does not have the Machine Gun Weapon yet
					if (!cpApp->mcaPlayer[GiPLAYER_1].WeaponIsInList(GiMACHINE_GUN_WEAPON))
					{
						// Add the Machine Gun Weapon to the Players Weapon List
						cpApp->mcaPlayer[GiPLAYER_1].AddWeapon(cpApp->mcaBulletModel[GiMACHINE_GUN_WEAPON]);
					}
					// Else Upgrade the Machine Gun Weapon to the Three Machine Gun Weapon
					else
					{
						// Add the Three Machine Gun Weapon to the Players Weapon List and set it as the Current Weapon
						cpApp->mcaPlayer[GiPLAYER_1].AddWeapon(cpApp->mcaBulletModel[GiTHREE_MACHINE_GUN_WEAPON]);
						cpApp->mcaPlayer[GiPLAYER_1].SetCurrentWeapon(GiTHREE_MACHINE_GUN_WEAPON);

						// Remove the normal Machine Gun Weapon
						cpApp->mcaPlayer[GiPLAYER_1].DeleteWeapon(GiMACHINE_GUN_WEAPON);
					}

					// Subtract the Cost from the Players Points
					cpApp->mcaPlayer[GiPLAYER_1].SetNumberOfPointsRelative(-iaCost[iPURCHASE_OR_UPGRADE_MACHINE_GUN_WEAPON]);

					// Mark that the selected Choice was valid
					bChoiceIsValid = true;
				}
				// Else Player did not have enough Points or Upgrade was maxed out
				else
				{
					// Mark that the selected Choice was not valid
					bChoiceIsValid = false;
				}

			break;

			case iLEAVE_SPACE_STATION:
				
				// Change Game State back to Playing Game
				cpApp->mcGameStateManager.ShutdownCurrentStateAndSetNextState(GameStatePlay, cpApp);

				// Mark that the selected Choice was valid
				bChoiceIsValid = true;

			break;

			default:
				MessageBox(cpApp->GethWnd(), "Invalid Selection", "Unrecognized Choice", MB_OK);
			break;
		};

		// Play appropriate selection sound
		
		// If selected Choice was able to be performed
		if (bChoiceIsValid)
		{
			cpApp->PlaySoundEffect(GiUpgradePurchasedSound);
		}
		// Else selected Choice could not be performed
		else
		{
			cpApp->PlaySoundEffect(GiInvalidSelectionSound);
		}
	}


	//********** Display Current Points and Cost of Currently Selected Item **********

	cpApp->mcFontRegular.Print("Money:", 450, 200, 0, 0, sNormalTextColor);
	itoa(cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints(), caString, 10);
	cpApp->mcFontRegular.Print(caString, 525, 200, 0, 0, sNormalTextColor);

	cpApp->mcFontRegular.Print("Cost:", 450, 230, 0, 0, sNormalTextColor);
	itoa(iaCost[SiCurrentChoice], caString, 10);
	cpApp->mcFontRegular.Print(caString, 525, 230, 0, 0, sNormalTextColor);

	
	//********** Display Arrow based on which option is selected **********

	cpApp->mcTexture[GiSPACE_STATION_TEXTURE].Blit(10, iaTextYPosition[SiCurrentChoice] + 4, 1, 361, 26, 17);


	//********** Lock Graphics into the Back Buffer **********

	// Unlock the Back Buffer
	cpApp->mcGraphics.EndScene();
}

// Pauses the Game (Can only pause from GameStatePlay)
void CApp::GameStatePause(void* vpOptionalDataToPass, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpOptionalDataToPass;		// cpApp = our CApp class


	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		return;
	}
	// Else this state is just doing a normal run


}

// Displays the End Story
void CApp::GameStatePlayerBeatGame(void* vpOptionalDataToPass, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpOptionalDataToPass;		// cpApp = our CApp class

	static SVertex SsStar[GiNUM_OF_STORY_BACKGROUND_STARS];	// Holds background star properties
	static SChangingNumber SsTextPosition;			// Holds Text Position

	int iDefaultTextSpeed = 15;	// Default Text Speed
	int iFastTextSpeed = 150;	// Text Speed when going Fast
	
	int iIndex = 0;				// Temp counting variable
	int iBrightness = 0;		// Temp variable to hold Stars Brightness

	// Text colors
	D3DCOLOR sCreditTitleColor = D3DCOLOR_RGBA(255, 0, 0, 255);
	D3DCOLOR sCreditsHeadingColor = D3DCOLOR_RGBA(0, 0, 255, 255);
	D3DCOLOR sCreditsColor = D3DCOLOR_RGBA(100, 255, 0, 150);
	D3DCOLOR sTheEndColor = D3DCOLOR_RGBA(255, 0, 100, 255);


	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		SsTextPosition.fX = 10;
		SsTextPosition.fXChange = 0;
		SsTextPosition.fY = GiWINDOW_HEIGHT;
		SsTextPosition.fYChange = iDefaultTextSpeed;

		// Initialize Stars
		for (iIndex = 0; iIndex < GiNUM_OF_STORY_BACKGROUND_STARS; iIndex++)
		{
			SsStar[iIndex].fX		= rand() % GiWINDOW_WIDTH;
			SsStar[iIndex].fY		= rand() % GiWINDOW_HEIGHT;
			SsStar[iIndex].fZ		= 0;
			SsStar[iIndex].fRHW		= 1;

			// Give Stars a random Brightness
			iBrightness = rand() % 255;
			SsStar[iIndex].diffuse = D3DCOLOR_RGBA(255, 255, 255, iBrightness);
		}

		// Play the Beat Game Music
		cpApp->PlayMusic(GiBeatGameMusic);

		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		return;
	}
	// Else this state is just doing a normal run


	//********** Clear the Display **********

	// Clear the Back Buffer to Black
	cpApp->mcGraphics.ClearDisplay(D3DCOLOR_RGBA(0, 0, 0, 255));

	// Lock the Back Buffer and do the drawing
	cpApp->mcGraphics.BeginScene();


	//********** Draw the background Stars **********

	// Place Star into Vertex Buffer and Draw them
	cpApp->mcVertexBuffer.Set(0, GiNUM_OF_STORY_BACKGROUND_STARS, SsStar);
	cpApp->mcVertexBuffer.Render(0, GiNUM_OF_STORY_BACKGROUND_STARS, D3DPT_POINTLIST);


	//********** Get and Process Players Input **********

	// If Player is pressing Fire on Keyboard or Mouse
	if (cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardFire) ||
		cpApp->mcMouse.GetButtonState(cpApp->mucMouseFire))
	{
		// Speed up Text
		SsTextPosition.fYChange = iFastTextSpeed;
	}
	// Else if Player is pressing Next Weapon on Keyboard or Mouse
	else if (cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardNextWeapon) ||
			 cpApp->mcMouse.GetButtonState(cpApp->mucMouseNextWeapon))
	{
		SsTextPosition.fYChange = 0;
	}
	// Else Player is not pressing Fire
	else
	{
		// Make sure Text is moving at Default Speed
		SsTextPosition.fYChange = iDefaultTextSpeed;
	}

	// If Text is done displaying or Player presses Enter or Mouse Detonate Weapon (Middle Click)
	if ((SsTextPosition.fY < -1500 || cpApp->mcKeyboard.GetKeyState(DIK_RETURN) ||
		cpApp->mcMouse.GetButtonState(cpApp->mucMouseDetonateBullet)) &&
		cpApp->mcSelectDelayTimer.XTimeHasPassedUsingSavedTime(GiMENU_SELECT_DELAY_TIME))
	{
		// Go back to the Main Menu
		cpApp->mcGameStateManager.ShutdownCurrentStateAndSetNextState(GameStateMainMenu, cpApp);
	}


	//********** Display Story **********

	// Move the text
	SsTextPosition.fY -= cpApp->GetFloatDistanceTravelled(SsTextPosition.fYChange, cpApp->ReturnAmountOfTimeSinceLastFrame());

	cpApp->mcFontRegular.Begin();

	// Display End Story
	cpApp->mcFontRegular.Print("    You've done it. It turns out that there was no mothership.", SsTextPosition.fX, SsTextPosition.fY);
	cpApp->mcFontRegular.Print("We did not properly translate the aliens messages until now.", SsTextPosition.fX, SsTextPosition.fY + 20);
	cpApp->mcFontRegular.Print("The vegetable harvister was actually the giant rabbit you just", SsTextPosition.fX, SsTextPosition.fY + 40);
	cpApp->mcFontRegular.Print("destroyed. The aliens were doing genetic experiments with", SsTextPosition.fX, SsTextPosition.fY + 60);
	cpApp->mcFontRegular.Print("animals when the bunny they were experimenting with suddenly", SsTextPosition.fX, SsTextPosition.fY + 80);
	cpApp->mcFontRegular.Print("began growing at an astonishing rate. Within hours the bunny,", SsTextPosition.fX, SsTextPosition.fY + 100);
	cpApp->mcFontRegular.Print("whose name ironically was Liddle, grew to be larger than their", SsTextPosition.fX, SsTextPosition.fY + 120);
	cpApp->mcFontRegular.Print("tallest buildings and soon was unstoppable. In addition, Liddle", SsTextPosition.fX, SsTextPosition.fY + 140);
	cpApp->mcFontRegular.Print("acquired the ability to mentally control the aliens due to their", SsTextPosition.fX, SsTextPosition.fY + 160);
	cpApp->mcFontRegular.Print("their genetic make-up. Luckily, humans were not susceptible to", SsTextPosition.fX, SsTextPosition.fY + 180);
	cpApp->mcFontRegular.Print("Liddles mind control, which is why you were unaffected.  This", SsTextPosition.fX, SsTextPosition.fY + 200);
	cpApp->mcFontRegular.Print("is also why the aliens would not negotiate when they initially", SsTextPosition.fX, SsTextPosition.fY + 220);
	cpApp->mcFontRegular.Print("came to Earth wanting our vegetables. When Liddle began getting", SsTextPosition.fX, SsTextPosition.fY + 240);
	cpApp->mcFontRegular.Print("hungry, she began getting angry and started hopping around, killing", SsTextPosition.fX, SsTextPosition.fY + 260);
	cpApp->mcFontRegular.Print("hundreds of aliens with a single hop. To calm her down they gave", SsTextPosition.fX, SsTextPosition.fY + 280);
	cpApp->mcFontRegular.Print("her vegetables to eat, but within days their planet was exhausted", SsTextPosition.fX, SsTextPosition.fY + 300);
	cpApp->mcFontRegular.Print("of all vegetables which is why they came to Earth and needed our", SsTextPosition.fX, SsTextPosition.fY + 320);
	cpApp->mcFontRegular.Print("vegetables. Now that Liddle is no longer a threat the aliens are", SsTextPosition.fX, SsTextPosition.fY + 340);
	cpApp->mcFontRegular.Print("returning to their home planet. We have given them some of our", SsTextPosition.fX, SsTextPosition.fY + 360);
	cpApp->mcFontRegular.Print("vegetables seeds so they can begin planting vegetables again.", SsTextPosition.fX, SsTextPosition.fY + 380);

	cpApp->mcFontRegular.Print("    You saved an alien civilization and all of Earths vegetables", SsTextPosition.fX, SsTextPosition.fY + 420);
	cpApp->mcFontRegular.Print("The world is once again safe! But for how long...", SsTextPosition.fX, SsTextPosition.fY + 440);


	// Display Credits info

	cpApp->mcFontRegular.Print("Credits", 275, SsTextPosition.fY + 620, 0, 0, sCreditTitleColor);

	cpApp->mcFontRegular.Print("Programming", 250, SsTextPosition.fY + 680, 0, 0, sCreditsHeadingColor);
	cpApp->mcFontRegular.Print("Lead Programmer: Daniel Schroeder", 160, SsTextPosition.fY + 720, 0, 0, sCreditsColor);
	cpApp->mcFontRegular.Print("Game Engine provided by: Jim Adams", 155, SsTextPosition.fY + 760, 0, 0, sCreditsColor);

	cpApp->mcFontRegular.Print("Graphics", 265, SsTextPosition.fY + 820, 0, 0, sCreditsHeadingColor);
	cpApp->mcFontRegular.Print("Daniel Schroeder", 230, SsTextPosition.fY + 860, 0, 0, sCreditsColor);
	cpApp->mcFontRegular.Print("Vanessa Schoenthal", 220, SsTextPosition.fY + 900, 0, 0, sCreditsColor);
	cpApp->mcFontRegular.Print("Anonymous Internet Sources", 180, SsTextPosition.fY + 940, 0, 0, sCreditsColor);

	cpApp->mcFontRegular.Print("Sounds", 275, SsTextPosition.fY + 1000, 0, 0, sCreditsHeadingColor);
	cpApp->mcFontRegular.Print("Daniel Schroeder", 230, SsTextPosition.fY + 1040, 0, 0, sCreditsColor);
	cpApp->mcFontRegular.Print("Anonymous Internet Sources", 180, SsTextPosition.fY + 1080, 0, 0, sCreditsColor);

	cpApp->mcFontRegular.Print("Music", 275, SsTextPosition.fY + 1140, 0, 0, sCreditsHeadingColor);
	cpApp->mcFontRegular.Print("Anonymous Internet Sources", 180, SsTextPosition.fY + 1180, 0, 0, sCreditsColor);

	cpApp->mcFontRegular.Print("Story", 275, SsTextPosition.fY + 1240, 0, 0, sCreditsHeadingColor);
	cpApp->mcFontRegular.Print("Daniel Schroeder", 230, SsTextPosition.fY + 1280, 0, 0, sCreditsColor);

	
	cpApp->mcFontRegular.Print("The End", 275, SsTextPosition.fY + 1480, 0, 0, sTheEndColor);


	cpApp->mcFontRegular.End();


	//********** Lock Graphics into the Back Buffer **********

	// Unlock the Back Buffer
	cpApp->mcGraphics.EndScene();
}



//********** Game State Play Functions **********

// Displays level background and generates new enemies
void CApp::DoLevelStuff(void* vpApp, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpApp;					// cpApp = our CApp class

	CShip* cpShip = 0;							// Temp pointer to Ships
	SObjectPrototype* spShipPrototype	= 0;	// Temp pointer to Ship Prototypes
	int iIndex = 0;								// Temp counting variable


	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		// Clear enemy lists
		cpApp->mcActiveShipList.Purge();
		cpApp->mcInactiveEnemyList.Purge();
		cpApp->mcRandomEnemyList.Purge();

		// Add Player to ActiveShipList
		cpApp->mcActiveShipList.AddShip(&cpApp->mcaPlayer[GiPLAYER_1], false);

		// Restart the New Enemy Timers
		cpApp->mcNewScriptedEnemyTimer.RestartTimerUsingSavedTime();
		cpApp->mcNewRandomEnemyTimer.RestartTimerUsingSavedTime();

		// Initialize appropriate Level with Initialization purpose
		switch (cpApp->miCurrentLevel)
		{
			case 1:
				cpApp->Level1(cpApp, eInitialize);
			break;

			case 2:
				cpApp->Level2(cpApp, eInitialize);
			break;

			case 3:
				cpApp->Level3(cpApp, eInitialize);
			break;

			// Display an error has occured
			default:
				MessageBox(cpApp->GethWnd(), "Unknown level Initialization called", "Unknown Level", MB_OK);
			break;
		};
        
		// Set New Random Enemy Time
		cpApp->mulNewRandomEnemyTime = ((rand() % cpApp->mulMaxRandomEnemyTime) + cpApp->mulMinRandomEnemyTime);

		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		// Clear enemy lists
		cpApp->mcActiveShipList.Purge();
		cpApp->mcInactiveEnemyList.Purge();
		cpApp->mcRandomEnemyList.Purge();

		// Shutdown current level
		switch (cpApp->miCurrentLevel)
		{
			case 1:
				cpApp->Level1(cpApp, eShutdown);
			break;

			case 2:
				cpApp->Level2(cpApp, eShutdown);
			break;

			case 3:
				cpApp->Level3(cpApp, eShutdown);
			break;

			default:
				MessageBox(cpApp->GethWnd(), "Unknown level Shutdown called", "Unknown Level", MB_OK);
			break;
		};

		return;
	}
	// Else this state is just doing a normal run

	// Call the appropriate level
	switch (cpApp->miCurrentLevel)
	{
		case 1:
			cpApp->Level1(cpApp);
		break;

		case 2:
			cpApp->Level2(cpApp);
		break;

		case 3:
			cpApp->Level3(cpApp);
		break;

		default:
			MessageBox(cpApp->GethWnd(), "Unknown level called", "Unknown Level", MB_OK);
		break;
	};
    

	//********** Add new Enemies if it's time **********

	// If it's time to add a New Scripted Enemy
	while (cpApp->mcNewScriptedEnemyTimer.XTimeHasPassedUsingSavedTimeWithoutRestartingTimer(cpApp->mcInactiveEnemyList.ReturnLowestTimeInList())
		   && !cpApp->mcInactiveEnemyList.IsEmpty())
	{
		// Get the Prototype with the Lowest Time
		spShipPrototype = cpApp->mcInactiveEnemyList.ReturnObjectPrototypeWithLowestTime();

		// Create the New Enemy
		cpShip = new CShip(cpApp->mcaEnemyModel[spShipPrototype->iType]);

		// Position New Enemy according to Prototypes Position
		cpShip->MoveTo(spShipPrototype->fX, spShipPrototype->fY);

		// If this enemy is a boss
		if (spShipPrototype->iType >= GiENEMY_BOSSES_STARTING_ID)
		{
			// Set that this Enemy is a boss
			cpShip->SetGeneralShipType(GiENEMY_BOSS);

			// Clear the Random Enemy List
			cpApp->mcRandomEnemyList.Purge();

			// If this is not the Games End Boss
			if (cpApp->miCurrentLevel != GiTOTAL_NUMBER_OF_LEVELS_IN_GAME)
			{
				// Play normal Boss Music
				cpApp->PlayMusic(GiEnemyBossMusic);
			}
			// Else this is the Games End Boss
			else
			{
				// Play the End Boss Music
				cpApp->PlayMusic(GiEndBossMusic);
			}
		}

		// Set Ship Frictions for the current level
		cpShip->SetFrictions(cpApp->miLevelXFriction, cpApp->miLevelYFriction);

		// Add New Enemy to Active Enemy List
		cpApp->mcActiveShipList.AddShip(cpShip);

		// If this Type of Enemy is not already in the Random Enemy List and it's not a boss
		if (!cpApp->mcRandomEnemyList.IsTypeInList(spShipPrototype->iType) &&
			spShipPrototype->iType < GiENEMY_BOSSES_STARTING_ID)
		{
			// Add this Type of Enemy to the Random Enemy List
			cpApp->mcRandomEnemyList.AddObjectPrototype(spShipPrototype->iType);
		}

		// Delete the Enemy Prototype from the List
		cpApp->mcInactiveEnemyList.RemoveObjectPrototype(spShipPrototype->iPrototypeID);
	}

	// If it's time to add a Random Enemy and the Scripted Enemy List is not empty (since they will then be fighting a boss)
	// and the Random Enemy List is not empty
	if ((cpApp->mcNewRandomEnemyTimer.XTimeHasPassedUsingSavedTime(cpApp->mulNewRandomEnemyTime) &&
		!cpApp->mcInactiveEnemyList.IsEmpty() && !cpApp->mcRandomEnemyList.IsEmpty())
		|| cpApp->mcKeyboard.GetKeyState(DIK_J))
	{
// TODO: Comment out above 'OR' Check for J code. It is used for testing to add a new random enemy

		// Make sure MaxRandomEnemyTime is not zero
		if (cpApp->mulMaxRandomEnemyTime == 0)
		{
			cpApp->mulMaxRandomEnemyTime = 1;
		}

		// Reset the New Random Enemy Time
		if ((cpApp->mulNewRandomEnemyTime = (rand() % cpApp->mulMaxRandomEnemyTime)) < cpApp->mulMinRandomEnemyTime)
		{
			// If New Random Enemy Time was too low, set it to the Minimum Time
			cpApp->mulNewRandomEnemyTime = cpApp->mulMinRandomEnemyTime;
		}

// TODO: Comment out below if case to check for J code. It is used for testing to add a new random enemy
int old;
if (cpApp->mcKeyboard.GetKeyState(DIK_J))
{
	old = cpApp->miRandomEnemyLikeliness;
	cpApp->miRandomEnemyLikeliness = 500;
}

		// Make sure RandomEnemyLikeliness is not zero (or else no random enemies should be created)
		if (cpApp->miRandomEnemyLikeliness != 0)
		{
			// If a New Random Enemy should be created
			if ((rand() % cpApp->miRandomEnemyLikeliness) == 0)
			{

// TODO: Comment out below if case to check for J code. It is used for testing to add a new random enemy
if (cpApp->mcKeyboard.GetKeyState(DIK_J))
{
	cpApp->miRandomEnemyLikeliness = old;
}

				// Create a new Ship
				cpShip = new CShip;

				// Move to a Random Enemy in the List
				for (iIndex = 0; iIndex < ((rand() % 5) + 1); iIndex++)
				{
					// Move to Next Prototype
					cpApp->mcRandomEnemyList.MoveToNextObjectPrototypeWithWrap();
				}

				// Get the Type of Ship to create			
				spShipPrototype = cpApp->mcRandomEnemyList.ReturnCurrentObjectPrototype();

				// If a Ship Prototype was returned
				if (spShipPrototype != 0)
				{
					// Copy Ship data from Model
					cpShip->CopyFrom(cpApp->mcaEnemyModel[spShipPrototype->iType]);

					// Move Ship into random X position above screen
					cpShip->MoveTo(rand() % GiWINDOW_WIDTH, -cpShip->mcPicture.ReturnHeight());

					// Set Ship Frictions for the current level
					cpShip->SetFrictions(cpApp->miLevelXFriction, cpApp->miLevelYFriction);

					// Have ship move around Randomly since it's a Random Enemy
					cpShip->ChoosePatternsRandomly(true);

					// Add Ship to the Active Enemy List
					cpApp->mcActiveShipList.AddShip(cpShip);
				}
			}
		}
	}
}

// Gets and Processes Players Input
void CApp::GetAndProcessInput(void* vpApp, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpApp;		// cpApp = our CApp class

	CShip* cpShip;					// Temp pointer to Ships
	CBullet* cpBullet;				// Temp pointer to Bullets
	int iDistance	= 0;			// Used to hold the Mouses distance from the Ship

	const int iMOUSE_MOVEMENT_SENSITIVITY = 1;


	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		return;
	}
	// Else this state is just doing a normal run


	//********** Get and Process Keyboard Input **********

	// If Mouse is being used
	if (cpApp->mcMouse.GetXDelta() != 0 || cpApp->mcMouse.GetYDelta() != 0 ||
		cpApp->mcMouse.GetButtonState(cpApp->mucMouseFire) || cpApp->mcMouse.GetButtonState(cpApp->mucMouseNextWeapon) ||
		cpApp->mcMouse.GetButtonState(cpApp->mucMouseDetonateBullet))
	{
//		cpApp->mbUsingMouse = true;
	}

	// If Keyboard is being used
	if (cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardUp) || cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardDown) ||
		cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardLeft) || cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardRight) ||
		cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardFire) || cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardDetonateBullet) ||
		cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardNextWeapon) || cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardPreviousWeapon))
	{
		cpApp->mbUsingMouse = false;
	}

	// If user is pressing Up
	if (cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardUp) || 
		(cpApp->mbUsingMouse && cpApp->mcMouse.GetYPos() < cpApp->mcaPlayer[GiPLAYER_1].ReturnYCoordinate()))
	{
		// If user is using the Keyboard
		if (!cpApp->mbUsingMouse)
		{
			// Move Player Up at Normal Acceleration
			cpApp->mcaPlayer[GiPLAYER_1].SetYAcceleration(-cpApp->mcaPlayer[GiPLAYER_1].ReturnNormalYAcceleration());
		}
		// Else they are using the Mouse
		else
		{
			// If Mouse Y distance from Ship is greater than the Ships Normal Y Acceleration
			if ((iDistance = (abs(cpApp->mcMouse.GetYPos() - cpApp->mcaPlayer[GiPLAYER_1].ReturnYCoordinate()) * iMOUSE_MOVEMENT_SENSITIVITY)) > cpApp->mcaPlayer[GiPLAYER_1].ReturnNormalYAcceleration())
			{
				iDistance = cpApp->mcaPlayer[GiPLAYER_1].ReturnNormalYAcceleration();
			}
			
			// Move Player Up at Acceleration
			cpApp->mcaPlayer[GiPLAYER_1].SetYAcceleration(-iDistance);
		}

		// Change Picture State to Moving Forward
		if (cpApp->mcaPlayer[GiPLAYER_1].mcPicture.ReturnCurrentState() != GiSHIP_STATE_FORWARD)
		{
			cpApp->mcaPlayer[GiPLAYER_1].mcPicture.SetCurrentState(GiSHIP_STATE_FORWARD);
		}
	}
	// Else if user is pressing Down
	else if (cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardDown) || 
			 (cpApp->mbUsingMouse && cpApp->mcMouse.GetYPos() > cpApp->mcaPlayer[GiPLAYER_1].ReturnYCoordinate()))
	{
		// If user is using the Keyboard
		if (!cpApp->mbUsingMouse)
		{
			// Move Player Down at Normal Acceleration
			cpApp->mcaPlayer[GiPLAYER_1].SetYAcceleration(cpApp->mcaPlayer[GiPLAYER_1].ReturnNormalYAcceleration());
		}
		// Else they are using the Mouse
		else
		{
			// If Mouse Y distance from Ship is greater than the Ships Normal Y Acceleration
			if ((iDistance = (abs(cpApp->mcMouse.GetYPos() - cpApp->mcaPlayer[GiPLAYER_1].ReturnYCoordinate()) * iMOUSE_MOVEMENT_SENSITIVITY)) > cpApp->mcaPlayer[GiPLAYER_1].ReturnNormalYAcceleration())
			{
				iDistance = cpApp->mcaPlayer[GiPLAYER_1].ReturnNormalYAcceleration();
			}
				
			// Move Player Down at a slower Acceleration
			cpApp->mcaPlayer[GiPLAYER_1].SetYAcceleration(iDistance);
		}

		// Change Picture State to Moving Backwards
		if (cpApp->mcaPlayer[GiPLAYER_1].mcPicture.ReturnCurrentState() != GiSHIP_STATE_BACKWARDS)
		{
			cpApp->mcaPlayer[GiPLAYER_1].mcPicture.SetCurrentState(GiSHIP_STATE_BACKWARDS);
		}
	}
	// Else if user is not pressing Up or Down
	else
	{
		// Stop accelerating
		cpApp->mcaPlayer[GiPLAYER_1].SetYAcceleration(0);

		// Change Picture State to Idle
		if (cpApp->mcaPlayer[GiPLAYER_1].mcPicture.ReturnCurrentState() != GiSHIP_STATE_IDLE)
		{
			cpApp->mcaPlayer[GiPLAYER_1].mcPicture.SetCurrentState(GiSHIP_STATE_IDLE);
		}
	}

	// If user is pressing Left
	if (cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardLeft) || 
		(cpApp->mbUsingMouse && cpApp->mcMouse.GetXPos() < cpApp->mcaPlayer[GiPLAYER_1].ReturnXCoordinate()))
	{
		// If user is using the Keyboard
		if (!cpApp->mbUsingMouse)
		{
			// Move Player Left at Normal Acceleration
			cpApp->mcaPlayer[GiPLAYER_1].SetXAcceleration(-cpApp->mcaPlayer[GiPLAYER_1].ReturnNormalXAcceleration());
		}
		// Else they are using the Mouse
		else
		{
			// If Mouse X distance from Ship is greater than the Ships Normal Y Acceleration
			if ((iDistance = (abs(cpApp->mcMouse.GetXPos() - cpApp->mcaPlayer[GiPLAYER_1].ReturnXCoordinate()) * iMOUSE_MOVEMENT_SENSITIVITY)) > cpApp->mcaPlayer[GiPLAYER_1].ReturnNormalXAcceleration())
			{
				iDistance = cpApp->mcaPlayer[GiPLAYER_1].ReturnNormalXAcceleration();
			}

			// Move Player Left at a slower Acceleration
			cpApp->mcaPlayer[GiPLAYER_1].SetXAcceleration(-iDistance);
		}
	}
	// Else if user is pressing Right
	else if (cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardRight) || 
			 (cpApp->mbUsingMouse && cpApp->mcMouse.GetXPos() > cpApp->mcaPlayer[GiPLAYER_1].ReturnXCoordinate()))
	{
		// If user is using the Keyboard
		if (!cpApp->mbUsingMouse)
		{
			// Move Player Right at Normal Acceleration
			cpApp->mcaPlayer[GiPLAYER_1].SetXAcceleration(cpApp->mcaPlayer[GiPLAYER_1].ReturnNormalXAcceleration());
		}
		// Else they are using the Mouse
		else
		{
			// If Mouse X distance from Ship is greater than the Ships Normal Y Acceleration
			if ((iDistance = (abs(cpApp->mcMouse.GetXPos() - cpApp->mcaPlayer[GiPLAYER_1].ReturnXCoordinate()) * iMOUSE_MOVEMENT_SENSITIVITY)) > cpApp->mcaPlayer[GiPLAYER_1].ReturnNormalXAcceleration())
			{
				iDistance = cpApp->mcaPlayer[GiPLAYER_1].ReturnNormalXAcceleration();
			}

			// Move Player Right at a slower Acceleration
			cpApp->mcaPlayer[GiPLAYER_1].SetXAcceleration(iDistance);
		}
	}
	// Else if user is not pressing Left or Right
	else
	{
		// Stop accelerating player
		cpApp->mcaPlayer[GiPLAYER_1].SetXAcceleration(0);
	}

	// If user is pressing Fire
	if (cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardFire) || cpApp->mcMouse.GetButtonState(cpApp->mucMouseFire))
	{
		// Mark that the Player is trying to Fire
		cpApp->mcaPlayer[GiPLAYER_1].SetIfShipShouldFire(true);
	}

	// If user is pressing Next Weapon
	if (cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardNextWeapon) || cpApp->mcMouse.GetButtonState(cpApp->mucMouseNextWeapon))
	{
		if (cpApp->mcaPlayer[GiPLAYER_1].mcChangeWeaponDelayTimer.XTimeHasPassedUsingSavedTime(GiCHANGE_WEAPON_DELAY))
		{
			// Change to Next Bullet in the list
			cpApp->mcaPlayer[GiPLAYER_1].NextWeapon();
		}
	}
	// If user is pressing Previous Weapon
	else if (cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardPreviousWeapon))
	{
		if (cpApp->mcaPlayer[GiPLAYER_1].mcChangeWeaponDelayTimer.XTimeHasPassedUsingSavedTime(GiCHANGE_WEAPON_DELAY))
		{
			// Change to Previous Bullet in the list
			cpApp->mcaPlayer[GiPLAYER_1].PreviousWeapon();
		}
	}

	// If user is pressing Detonate Bullet
	if (cpApp->mcKeyboard.GetKeyState(cpApp->mucKeyboardDetonateBullet) || cpApp->mcMouse.GetButtonState(cpApp->mucMouseDetonateBullet))
	{
		// If a detonatable weapon has been fired
		if (!cpApp->mcFiredBulletsList.mcBulletQueue.IsEmpty())
		{
			// If it has been long enough since the last Bullet was detonated manually
			if (cpApp->mcaPlayer[GiPLAYER_1].mcFireWeaponDelayTimer.XTimeHasPassedUsingSavedTime(GiDETONATE_WEAPON_DELAY))
			{
				// Detonate the Bullet fired the longest ago that hasn't yet detonated
				cpBullet = cpApp->mcFiredBulletsList.ReturnBullet(cpApp->mcFiredBulletsList.mcBulletQueue.RemoveFromBack());
				cpBullet->Detonate();
			}
		}
	}

// TODO: Remove these before final release, just for testing purposes
// Test Enemy Kill/Death feature
if (cpApp->mcKeyboard.GetKeyState(DIK_K))
{
	// Move to Start of Enemy List
	cpApp->mcActiveShipList.MoveToStartOfList();

	// Cycle through Enemy List
	while (cpApp->mcActiveShipList.CurrentShipExists())
	{
		// Get Current Ship
		cpShip = cpApp->mcActiveShipList.ReturnCurrentShip();

		// If this is an Enemy
		if (cpShip->ReturnGeneralShipType() != GiPLAYER && cpShip->ReturnStatus() != GiSHIP_STATUS_DEAD)
		{
			// Kill them
			cpShip->SetStatus(GiSHIP_STATUS_DYING);
		}

		// Move to Next Ship
		cpApp->mcActiveShipList.MoveToNextShip();
	}
}

// Test Player Death feature
if (cpApp->mcKeyboard.GetKeyState(DIK_L))
{
	cpApp->mcaPlayer[GiPLAYER_1].SetStatus(GiSHIP_STATUS_DYING);
}

// Test Clock speed up/slow down functions
if (cpApp->mcKeyboard.GetKeyState(DIK_Q))
{
	cpApp->mcpClock->SetScaleFactor(cpApp->mcpClock->ReturnScaleFactor() - 0.01);

	if (cpApp->mcpClock->ReturnScaleFactor() < 0.0)
	{
		cpApp->mcpClock->SetScaleFactor(0.0);
	}
}else if (cpApp->mcKeyboard.GetKeyState(DIK_E))
{
	cpApp->mcpClock->SetScaleFactor(mcpClock->ReturnScaleFactor() + 0.01);
}else if (cpApp->mcKeyboard.GetKeyState(DIK_R))
{
	cpApp->mcpClock->SetScaleFactor(1.0);
}

}


// Updates all Objects (Players, Enemies, items, etc)
void CApp::UpdateObjects(void* vpApp, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpApp;		// cpApp = our CApp class


	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		// Clear All Lists
		cpApp->mcFiredBulletsList.Purge();
		cpApp->mcActiveShipList.Purge();
		cpApp->mcActiveItemList.Purge();

		return;
	}
	// Else this state is just doing a normal run


	//********** Update All Players, Bullets, Enemies, and Items **********

	// Update All Bullets Pictures and Positions
	cpApp->mcFiredBulletsList.UpdateAllBulletsInList(cpApp->ReturnAmountOfTimeSinceLastFrame());

	// Update All Enemies and Players Pictures and Positions
	cpApp->mcActiveShipList.UpdateAllShipsInList(cpApp->ReturnAmountOfTimeSinceLastFrame());

	// Update All Items
	cpApp->mcActiveItemList.UpdateAllItemsInList(cpApp->ReturnAmountOfTimeSinceLastFrame());
}


// Tests for Collisions and Processes the results
void CApp::ProcessCollisionsAndObjects(void* vpApp, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpApp;		// cpApp = our CApp class

	CShip* cpShip;					// Temp pointer to Ships
	CBullet* cpBullet;				// Temp pointer to Bullets
	CItem* cpItem;					// Temp pointer to Items
	
	int iBulletID;					// Temp Bullet ID holder
	int iIndex;						// Temp counting variable
	int iWidth, iHeight;			// Temp holders of Ships Width and Height before exploding
	int iDamage;					// Used in calculating how much Damage a Ship should take

	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		return;
	}
	// Else this state is just doing a normal run

	
	//********** Process Fired Bullets and test for Collisions **********

	// Move to Start of Players Fired Bullets List
	cpApp->mcFiredBulletsList.MoveToStartOfList();
	
	// Cycle through Fired Bullets List
	while (cpApp->mcFiredBulletsList.CurrentBulletExists())
	{
		// Return the Bullet
		cpBullet = cpApp->mcFiredBulletsList.ReturnCurrentBullet();

		// If Bullet can still do damage
		if (cpBullet->IsLive())
		{
			//***** Test for collisions against Ships *****

			// Move to Start of Ship List
			cpApp->mcActiveShipList.MoveToStartOfList();

			// Cycle through Ship List
			while (cpApp->mcActiveShipList.CurrentShipExists())
			{
				// Get Current Ship
				cpShip = cpApp->mcActiveShipList.ReturnCurrentShip();

				// If this type of Ship did not fire the Bullet
				if ((cpBullet->ReturnOwner() == GiPLAYER && cpShip->ReturnGeneralShipType() != GiPLAYER) ||
					(cpBullet->ReturnOwner() != GiPLAYER && cpShip->ReturnGeneralShipType() == GiPLAYER))
				{
					// If Bullet is hitting Ship, Ship is not dead, and this Bullet has not hit this Ship yet
					if (cpApp->Collision(&cpBullet->mcPicture, &cpShip->mcPicture) && cpShip->ReturnHealth() > 0 &&
										!cpShip->mcBulletsThatHaveAlreadyHitShipList.IsInQueue(cpBullet->ReturnID()))
					{
						// If Bullet is supposed to Detonate On Contact and has not Detonated yet
						if (!cpBullet->IsDetonated() && (cpBullet->ReturnCauseOfDetonation() == GiDETONATE_ON_CONTACT || 
							cpBullet->ReturnCauseOfDetonation() == GiDETONATE_ON_KEYPRESS_OR_CONTACT))
						{
							// Make it Detonate
							cpBullet->Detonate();
						}

						// If this Ship is the Player and they are done the Level (Leaving the Screen)
						if (cpShip->ReturnGeneralShipType() == GiPLAYER && cpShip->ReturnStatus() == GiSHIP_STATUS_LEAVE_SCREEN)
						{
							// Do nothing since the Player cannot control their Ship anymore
						}
						// Else this is an Enemy or the Player and they are not done the Level yet
						else
						{
							// Calculate how much Damage Ship should take
							iDamage = ((cpBullet->ReturnBaseDamage() + (rand() % cpBullet->ReturnRandomDamage())) / cpShip->ReturnArmor());

							// TODO: If Ship has Shields on
							// iDamage /= cpShip->iShieldStrength()

							// Make Enemy take Damage from hitting the Bullet
							cpShip->SetHealthRelative(-iDamage);

							// If the Bullet does something other than deal damage
							if (cpBullet->ReturnIfBulletHasAlternateEffect())
							{
								// Check what Type of Bullet it is and process effects
								switch (cpBullet->ReturnSpecificType())
								{
									case GiBUBBLE_BULLET:
										// TODO: Do this effect
									break;

									case GiEMP_BULLET:
										// TODO: Do this effect
									break;

									default:

									break;
								};
							}

							// If Ship is Dead and it has not been marked Dying or Dead yet
							if (cpShip->ReturnHealth() < 1 && 
								cpShip->ReturnStatus() != GiSHIP_STATUS_DYING &&
								cpShip->ReturnStatus() != GiSHIP_STATUS_DEAD)
							{
								// Set the Ship is Dying
								cpShip->SetStatus(GiSHIP_STATUS_DYING);

								// Set the Ships Health to zero
								cpShip->SetHealth(0);
							}

							// Add Bullets to Ships Already Hit List so that it does not hit Ship twice
							cpShip->mcBulletsThatHaveAlreadyHitShipList.AddToFront(cpBullet->ReturnID());
						}

						// If Player or Enemy Boss was hit
						if (cpShip->ReturnGeneralShipType() == GiPLAYER ||
							cpShip->ReturnGeneralShipType() == GiENEMY_BOSS)
						{
							// Play sound of Bullet Hitting Player
							cpApp->PlaySoundEffect(GiBulletHitPlayerSound);
						}
					}
				}

				// Move to next Ship
				cpApp->mcActiveShipList.MoveToNextShip();
			}
	
			// If Bullet is off the top or bottom of the screen
			if ((cpBullet->mcPicture.ReturnYCoordinate() < -(cpBullet->mcPicture.ReturnHeight())) || (cpBullet->mcPicture.ReturnYCoordinate() > GiWINDOW_HEIGHT))
			{
				// Detonate and Kill Bullet
				cpBullet->Detonate();
				cpBullet->Kill();

				// Detonate the Bullet NOW
				cpBullet->mcPicture.SetCurrentState(GiBULLET_STATE_DETONATING);
				cpBullet->mcPicture.SetCurrentStatesPicture(GiBULLET_PICTURE_DETONATING_DONE);

				// Remove this Bullet from the Detonatable Bullets List if it is in it
				cpApp->mcFiredBulletsList.mcBulletQueue.RemoveFromQueue(cpBullet->ReturnID());
			
			}
			
			// If Bullet has been Detonated, make sure the Picture is Detonating
			if (cpBullet->IsDetonated())
			{
				// If Bullet is not in Detonate State
				if (cpBullet->mcPicture.ReturnCurrentState() != GiBULLET_STATE_DETONATING)
				{
					// Put Bullet in Detonate State
					cpBullet->mcPicture.SetCurrentState(GiBULLET_STATE_DETONATING);

					// Set the Bullets Velocity to it's Detonating Velocity
					cpBullet->SetVelocities(cpBullet->mcPicture.ReturnStatesXVelocity(cpBullet->mcPicture.ReturnCurrentState()), cpBullet->mcPicture.ReturnStatesYVelocity(cpBullet->mcPicture.ReturnCurrentState()));
				
					// Remove this Bullet from the Detonatable Bullets List if it is in it
					cpApp->mcFiredBulletsList.mcBulletQueue.RemoveFromQueue(cpBullet->ReturnID());

					// Play Detonation Sound based on the type of Bullet
					// Also take care of special detonation cases
					switch (cpBullet->ReturnSpecificType())
					{
						case GiNORMAL_WEAPON:

						break;

						case GiTHREE_NORMAL_WEAPON:

						break;

						case GiMACHINE_GUN_WEAPON:

						break;

						case GiTHREE_MACHINE_GUN_WEAPON:

						break;

						case GiFLAME_THROWER_WEAPON:
							
						break;

						case GiFIREBALL_WEAPON:

						break;

						case GiLASER_WEAPON:
								
						break;

						case GiBOMB_WEAPON:

							// Reposition bomb so it stays centered
							cpBullet->MoveRelative(-(cpBullet->mcPicture.ReturnWidth() / 2), -(cpBullet->mcPicture.ReturnHeight() / 3));

							cpApp->PlaySoundEffect(GiBombExplosionSound);

						break;

						case GiRAM_WEAPON:

						break;

						case GiBUBBLE_WEAPON:

						break;

						case GiHEAT_SEEK_WEAPON:

						break;

						case GiMISSILE_WEAPON:

						break;

						case GiSONIC_WAVE_WEAPON:

						break;

						case GiEMP_WEAPON:

						break;

						case GiCLUSTER_BOMB_WEAPON:

						break;

						default:
			
						break;
					};
				}
			}

			// Check the type of Bullet and Kill it if it is time (Some still do damage after detonating)
			switch (cpBullet->ReturnSpecificType())
			{
				case GiFLAME_THROWER_WEAPON:
					
					// If Flame has gone out
					if (cpBullet->ReturnDurationUntilDetonation() <= 0)
					{
						// Make Bullet harmless since it's already done damage
						cpBullet->Kill();

						// Set State to Extinguished so the Bullet can be removed
						cpBullet->mcPicture.SetCurrentState(GiBULLET_STATE_DETONATING);
					}

				break;

				case GiLASER_WEAPON:
					
					// If Laser is off the Screen
					if (cpBullet->mcPicture.ReturnYCoordinate() < -cpBullet->mcPicture.ReturnHeight() || cpBullet->mcPicture.ReturnYCoordinate() > GiWINDOW_HEIGHT)
					{
						// Make Bullet Harmless since it's off the screen
						cpBullet->Kill();

						// Set State to Detonated so the Bullet can be removed
						cpBullet->mcPicture.SetCurrentState(GiBULLET_STATE_DETONATING);
					}

				break;

				case GiBOMB_WEAPON:

					// If Bomb is done exploding
					if (cpBullet->mcPicture.ReturnCurrentPicture() == GiBULLET_PICTURE_DETONATING_DONE)
					{
						// Make Bullet harmless since it's already done damage from first explosion
						cpBullet->Kill();
					}

				break;

				case GiSONIC_WAVE_WEAPON:

					// If Sonic Wave is off the Screen

				break;

				case GiCLUSTER_BOMB_WEAPON:
					
					// If Bombs are done exploding

				break;

				case GiNORMAL_WEAPON:
				case GiTHREE_NORMAL_WEAPON:
				case GiMACHINE_GUN_WEAPON:
				case GiTHREE_MACHINE_GUN_WEAPON:
				case GiFIREBALL_WEAPON:
				case GiRAM_WEAPON:
				case GiBUBBLE_WEAPON:
				case GiHEAT_SEEK_WEAPON:
				case GiMISSILE_WEAPON:
				case GiEMP_WEAPON:
				default:
					// If Bullet has Detonated
					if (cpBullet->IsDetonated())
					{
						// Make Bullet harmless since it's already done damage
						cpBullet->Kill();
					}

				break;
			};
		}

		// If Bullet is done Detonating
		if (cpBullet->mcPicture.ReturnCurrentPicture() == GiBULLET_PICTURE_DETONATING_DONE)
		{
			// Put Bullet on the Delete Later List
			cpApp->mcFiredBulletsList.PutBulletOnDeleteLaterList(cpBullet->ReturnID());
		}

		//********** Display Bullet **********

		// Display Fired Bullet
		cpApp->mcTexture[GiBULLET_TEXTURE].Blit(cpBullet->mcPicture.ReturnXCoordinate(), cpBullet->mcPicture.ReturnYCoordinate(),
												cpBullet->mcPicture.ReturnLeft(), cpBullet->mcPicture.ReturnTop(), 
												cpBullet->mcPicture.ReturnWidth(), cpBullet->mcPicture.ReturnHeight());

		// Move to next Fired Bullet
		cpApp->mcFiredBulletsList.MoveToNextBullet();
	}


	//********** Process Ships **********

	// Move to Start of Ship List
	cpApp->mcActiveShipList.MoveToStartOfList();

	// Cycle through Ship List
	while (cpApp->mcActiveShipList.CurrentShipExists())
	{
		// Get Current Ship
		cpShip = cpApp->mcActiveShipList.ReturnCurrentShip();

		// If this Ship is an Enemy
		if (cpShip->ReturnGeneralShipType() != GiPLAYER)
		{
			//********** Check if Enemy is hitting Player **********
		
			// If Enemy is Colliding with Player AND Enemy is not Dying or Dead AND Level is not complete
			if (cpApp->Collision(&cpShip->mcPicture, &cpApp->mcaPlayer[GiPLAYER_1].mcPicture) &&
				cpShip->ReturnStatus() != GiSHIP_STATUS_DYING && cpShip->ReturnStatus() != GiSHIP_STATUS_DEAD &&
				!cpApp->mbLevelComplete)
			{
				// If they weren't already Colliding last frame
				if (!cpShip->ReturnIfShipIsColliding())
				{
					// Make Player take Damage


					// Make Enemey take Damage


					// Make Player bounce back at 1/4 velocity
					cpApp->mcaPlayer[GiPLAYER_1].SetVelocities(-(cpApp->mcaPlayer[GiPLAYER_1].ReturnXVelocity() / 4), -(cpApp->mcaPlayer[GiPLAYER_1].ReturnYVelocity() / 4));

					// Make Enemy bounce back at 1/4 velocity
					cpShip->SetVelocities(-(cpShip->ReturnXVelocity() / 4), -(cpShip->ReturnYVelocity() / 4));
					cpShip->SetAccelerations(0, 0);

					// If Enemy is not bouncing back fast enough in the X direction
					if (abs(cpShip->ReturnXVelocity()) < 25)
					{
						// If Ship should be going left
						if (cpShip->ReturnXVelocity() < 0)
						{
							// Make Enemy go left faster
							cpShip->SetXVelocity(-25);
						}
						// Else Ship should be going right
						else
						{
							// Make Enemy go right faster
							cpShip->SetXVelocity(25);
						}
					}
		
					// If Enemy is not bouncing back fast enough in the Y direction
					if (abs(cpShip->ReturnYVelocity()) < 25)
					{
						// If Enemy should be going left
						if (cpShip->ReturnYVelocity() < 0)
						{
							// Make Enemy go left faster
							cpShip->SetYVelocity(-25);
						}
						// Else Enemy should be going right
						else
						{
							// Make Enemy go right faster
							cpShip->SetYVelocity(25);
						}
					}


					// Mark that Player is Colliding with an Enemy
					cpApp->mcaPlayer[GiPLAYER_1].SetIfShipIsColliding(true);
					cpApp->mcaPlayer[GiPLAYER_1].SetAccelerations(0, 0);

					// If Player is not bouncing back fast enough in the X direction
					if (abs(cpApp->mcaPlayer[GiPLAYER_1].ReturnXVelocity()) < 25)
					{
						// If Ship should be going left
						if (cpApp->mcaPlayer[GiPLAYER_1].ReturnXVelocity() < 0)
						{
							// Make Player go left faster
							cpApp->mcaPlayer[GiPLAYER_1].SetXVelocity(-25);
						}
						// Else Ship should be going right
						else
						{
							// Make Player go right faster
							cpApp->mcaPlayer[GiPLAYER_1].SetXVelocity(25);
						}
					}
		
					// If Player is not bouncing back fast enough in the Y direction
					if (abs(cpApp->mcaPlayer[GiPLAYER_1].ReturnYVelocity()) < 25)
					{
						// If Player should be going left
						if (cpApp->mcaPlayer[GiPLAYER_1].ReturnYVelocity() < 0)
						{
							// Make Player go left faster
							cpApp->mcaPlayer[GiPLAYER_1].SetYVelocity(-25);
						}
						// Else Player should be going right
						else
						{
							// Make Player go right faster
							cpApp->mcaPlayer[GiPLAYER_1].SetYVelocity(25);
						}
					}


					// Mark that Enemy Ship is Colliding with the Player
					cpShip->SetIfShipIsColliding(true);
				}
/*
				// Turn off Ships acceleration since they are still colliding
				cpShip->SetAccelerations(0, 0);

				// Turn off Players accelertion since they are still colliding
				cpApp->mcaPlayer[GiPLAYER_1].SetAccelerations(0, 0);
*/
			}
			// Else if Enemy is not Colliding with Player but was last frame
			else if (!cpApp->Collision(&cpShip->mcPicture, &cpApp->mcaPlayer[GiPLAYER_1].mcPicture) && 
					 cpShip->ReturnIfShipIsColliding())
			{
				// Set that Player is not Colliding with Enemy anymore
				cpApp->mcaPlayer[GiPLAYER_1].SetIfShipIsColliding(false);

				// Set that Enemy is not Colliding with Player anymore
				cpShip->SetIfShipIsColliding(false);
			}
			// Else Enemy is not colliding with Player
			else
			{
				// Set that Enemy is not Colliding with Player
				cpShip->SetIfShipIsColliding(false);
			}

			//********** Do Enemy AI **********

			cpShip->PerformAI(&cpApp->mcaPlayer[GiPLAYER_1]);
		}


		// If Ship is not Dying or Dead
		if (cpShip->ReturnStatus() != GiSHIP_STATUS_DYING && cpShip->ReturnStatus() != GiSHIP_STATUS_DEAD)
		{
			// If Ship is trying to Fire
			if (cpShip->ReturnIfShipShouldFire())
			{
				// If Current Weapon still has Ammo left or has Unlimited Ammo
				if (cpShip->ReturnCurrentWeaponsAmmo() > 0 || cpShip->ReturnCurrentWeaponsAmmo() == GiUNLIMITED_AMMO)
				{
					// If enough time has elapsed since shooting last Bullet
					if (cpShip->mcFireWeaponDelayTimer.XTimeHasPassedUsingSavedTime(cpShip->ReturnCurrentWeaponsFireDelay()))
					{
						// If this Bullet does not have an Alternate Effect (is not special)
						if (!cpShip->ReturnIfCurrentWeaponHasAlternateEffect())
						{
							// Create Bullet with Ship Bullets properties
							cpBullet = new CBullet(*cpShip->ReturnCurrentWeaponsBullet());
							
							// Mark what Type of Ship Fired this Bullet (Player or Enemy)
							cpBullet->SetOwner(cpShip->ReturnGeneralShipType());

							// If an Enemy fired this Bullet
							if (cpShip->ReturnGeneralShipType() != GiPLAYER)
							{
								// Move the Bullet to the proper spot relative to the Enemy
								cpBullet->MoveTo(cpShip->ReturnCurrentWeaponsFireFromXPosition(), cpShip->ReturnCurrentWeaponsFireFromYPosition() + cpShip->mcPicture.ReturnHeight());

								// Reverse Bullets Y Velocity
								cpBullet->SetYVelocity(cpBullet->ReturnYVelocity() * -1);
							}
							// Else the Player fired this Bullet
							else
							{
								// Move the Bullet to the proper spot relative to the Player
								cpBullet->MoveTo(cpShip->ReturnCurrentWeaponsFireFromXPosition(), cpShip->ReturnCurrentWeaponsFireFromYPosition());
							}
							
							// Add the Bullet to the Players Fired Bullets List
							iBulletID = cpApp->mcFiredBulletsList.AddBullet(cpBullet);

							// Reduce amount of Ammo if not Unlimited
							if (cpShip->ReturnCurrentWeaponsAmmo() != GiUNLIMITED_AMMO)
							{
								cpShip->SetCurrentWeaponsAmmoRelative(-1);
							}

							// If Bullet is externally Detonatable and Player fired it
							if ((cpBullet->ReturnCauseOfDetonation() == GiDETONATE_ON_KEYPRESS_OR_CONTACT ||
								cpBullet->ReturnCauseOfDetonation() == GiDETONATE_ON_KEY_PRESS) &&
								cpBullet->ReturnOwner() == GiPLAYER)
							{
								cpApp->mcFiredBulletsList.mcBulletQueue.AddToFront(iBulletID);
							}
						}
						// Else Bullet Has an Alternate Effect (is special)
						else
						{
							// Check which Type of Bullet this is and process it accordingly
							switch (cpShip->ReturnCurrentWeaponsSpecificType())
							{
								case GiTHREE_NORMAL_WEAPON:
									
									// Loop through all 3 Bullets
									for (iIndex = 0; iIndex < 3; iIndex++)
									{
										// Create Bullet with Ship Bullets properties
										cpBullet = new CBullet(*cpShip->ReturnCurrentWeaponsBullet());
										
										// Mark that the Player fired this Buller
										cpBullet->SetOwner(cpShip->ReturnGeneralShipType());

										// Move the Bullet to the proper spot relative to the Player
										// If this is the first Bullet
										if (iIndex == 0)
										{
											// Put this Bullet on Left side
											cpBullet->MoveTo(cpShip->ReturnXCoordinate(), cpShip->ReturnCurrentWeaponsFireFromYPosition() + (cpShip->mcPicture.ReturnHeight() / 2));
										}
										// Else if this is the second Bullet
										else if (iIndex == 1)
										{
											// Put this Bullet in the middle
											cpBullet->MoveTo(cpShip->ReturnCurrentWeaponsFireFromXPosition(), cpShip->ReturnCurrentWeaponsFireFromYPosition());
										}
										// Else this is the third Bullet
										else
										{
											// Put this Bullet on the right side
											cpBullet->MoveTo((cpShip->ReturnXCoordinate() + cpShip->mcPicture.ReturnWidth() - cpBullet->mcPicture.ReturnWidth()), cpShip->ReturnCurrentWeaponsFireFromYPosition() + (cpShip->mcPicture.ReturnHeight() / 2));
										}
										
										// Add the Bullet to the Players Fired Bullets List
										iBulletID = cpApp->mcFiredBulletsList.AddBullet(cpBullet);
									}

									// Reduce amount of Ammo if not Unlimited
									if (cpShip->ReturnCurrentWeaponsAmmo() != GiUNLIMITED_AMMO)
									{
										// Subtract 3 from Ammo since firing 3 Bullets
										cpShip->SetCurrentWeaponsAmmoRelative(-3);
									}

								break;

								case GiTHREE_MACHINE_GUN_WEAPON:
				
									// Loop through all 3 Bullets
									for (iIndex = 0; iIndex < 3; iIndex++)
									{
										// Create Bullet with Ship Bullets properties
										cpBullet = new CBullet(*cpShip->ReturnCurrentWeaponsBullet());
										
										// Mark that the Player fired this Buller
										cpBullet->SetOwner(cpShip->ReturnGeneralShipType());

										// Move the Bullet to the proper spot relative to the Player
										// If this is the first Bullet
										if (iIndex == 0)
										{
											// Put this Bullet on Left side
											cpBullet->MoveTo(cpShip->ReturnXCoordinate(), cpShip->ReturnCurrentWeaponsFireFromYPosition() + (cpShip->mcPicture.ReturnHeight() / 2));
										}
										// Else if this is the second Bullet
										else if (iIndex == 1)
										{
											// Put this Bullet in the middle
											cpBullet->MoveTo(cpShip->ReturnCurrentWeaponsFireFromXPosition(), cpShip->ReturnCurrentWeaponsFireFromYPosition());
										}
										// Else this is the third Bullet
										else
										{
											// Put this Bullet on the right side
											cpBullet->MoveTo((cpShip->ReturnXCoordinate() + cpShip->mcPicture.ReturnWidth() - cpBullet->mcPicture.ReturnWidth()), cpShip->ReturnCurrentWeaponsFireFromYPosition() + (cpShip->mcPicture.ReturnHeight() / 2));
										}
										
										// Add the Bullet to the Players Fired Bullets List
										iBulletID = cpApp->mcFiredBulletsList.AddBullet(cpBullet);
									}

									// Reduce amount of Ammo if not Unlimited
									if (cpShip->ReturnCurrentWeaponsAmmo() != GiUNLIMITED_AMMO)
									{
										// Subtract 3 from Ammo since firing 3 Bullets
										cpShip->SetCurrentWeaponsAmmoRelative(-3);
									}

								break;

								default:

								break;
							};
						}
					}

					// If Weapon is out of Ammo
					if (cpShip->ReturnCurrentWeaponsAmmo() < 1 && cpShip->ReturnCurrentWeaponsAmmo() != GiUNLIMITED_AMMO)
					{
						// Remove this Weapon from the Ships Weapon List
						cpShip->DeleteWeapon(cpShip->ReturnCurrentWeaponsSpecificType());
					}
				}

				// Reset Ship to not Fire
				cpShip->SetIfShipShouldFire(false);
			}
		}
		

		//********** Check if Ship is Dead **********

		// If Ship is Dying
		if (cpShip->ReturnStatus() == GiSHIP_STATUS_DYING)
		{
			// If this is an Enemy Ship
			if (cpShip->ReturnGeneralShipType() != GiPLAYER)
			{
				// If Ship is not in Death State yet
				if (cpShip->mcPicture.ReturnCurrentState() != GiENEMY_STATE_DEATH)
				{
					// Give Player points for killing enemy
					cpApp->mcaPlayer[GiPLAYER_1].SetNumberOfPointsRelative(cpShip->ReturnNumberOfPoints());

					// Save Ships Width and Height before exploding
					iWidth = cpShip->mcPicture.ReturnWidth();
					iHeight = cpShip->mcPicture.ReturnHeight();

					// Put Ship in Death State
					cpShip->mcPicture.SetCurrentState(GiENEMY_STATE_DEATH);

					// Move explosion to where middle of Ships Picture was
					cpShip->MoveXRelative((iWidth / 2) - (cpShip->mcPicture.ReturnWidth() / 2)); 
					cpShip->MoveYRelative((iHeight / 2) - (cpShip->mcPicture.ReturnHeight() / 2));

					// If this Ship should drop an Item
					if (cpShip->ShipShouldDropItem())
					{
						// Create the new Item
						cpItem = new CItem(cpApp->mcaItemModel[cpShip->ReturnItemTypeToDrop()]);

						// Center it according to Ships position
						cpItem->MoveTo((cpShip->ReturnXCoordinate() + (cpShip->mcPicture.ReturnWidth() / 2)) - (cpItem->mcPicture.ReturnWidth() / 2),
									   (cpShip->ReturnYCoordinate() + (cpShip->mcPicture.ReturnHeight() / 2)) - (cpItem->mcPicture.ReturnHeight() / 2));

						// Add the new Item to the Active Item List
						cpApp->mcActiveItemList.AddItem(cpItem);
					}

					// Play Death Sound
					if (cpShip->ReturnGeneralShipType() == GiENEMY)
					{
						// Play regular Enemy Death Sound
						cpApp->PlaySoundEffect(GiEnemyDeathSound);
					}
					// Else this Enemy is a Boss
					else
					{
						// Play Enemy Boss Death Sound
						cpApp->PlaySoundEffect(GiEnemyBossDeathSound);
					}
				}
			}
			// Else it is a Players Ship
			else
			{
				// If Ship is not in Death State yet
				if (cpShip->mcPicture.ReturnCurrentState() != GiSHIP_STATE_DEATH)
				{
					// Save Ships Width and Height before exploding
					iWidth = cpShip->mcPicture.ReturnWidth();
					iHeight = cpShip->mcPicture.ReturnHeight();

					// Put Ship in Death State
					cpShip->mcPicture.SetCurrentState(GiSHIP_STATE_DEATH);

					// Move explosion to where middle of Picture was
					cpShip->MoveTo((cpShip->ReturnXCoordinate() + (iWidth / 2)) - (cpShip->mcPicture.ReturnWidth() / 2), 
								   (cpShip->ReturnYCoordinate() + (iHeight / 2)) - (cpShip->mcPicture.ReturnHeight() / 2));

					// Play player Death sound
					cpApp->PlaySoundEffect(GiPlayerDeathSound);
				}
			}

			// Move Ship down on the screen
			cpShip->SetAccelerations(0, 0);
			cpShip->SetVelocities(GiDEATH_X_VELOCITY, GiDEATH_Y_VELOCITY);

			// If Ship is done 'dying'
			if (cpShip->mcPicture.ReturnCurrentPicture() == GiSHIP_PICTURE_DEATH_DONE)
			{
				// Mark that Ship is Dead
				cpShip->SetStatus(GiSHIP_STATUS_DEAD);
			}
		}
		// Else if Ship is Dead
		else if (cpShip->ReturnStatus() == GiSHIP_STATUS_DEAD)
		{
			// If this is an Enemy Ship
			if (cpShip->ReturnGeneralShipType() != GiPLAYER)
			{
				// If this Enemy was a boss
				if (cpShip->ReturnSpecificShipType() >= GiENEMY_BOSSES_STARTING_ID)
				{
					// The Level is done so mark it as Completed
					cpApp->mbLevelComplete = true;
				}

				// Delete the Ship
				cpApp->mcActiveShipList.PutCurrentShipOnDeleteLaterList();
			}
			// Else it's a Players Ship
			else
			{
				// If Player has more men left
				if (cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfLives() > 0)
				{	
					// Set up Players new Ship

					// Decrement Players Number Of Lives
					cpApp->mcaPlayer[GiPLAYER_1].SetNumberOfLives(cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfLives() - 1);

					// Set Players Health
					cpApp->mcaPlayer[GiPLAYER_1].SetHealth(cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxHealth());

					// Reposition Players Ship
					cpApp->mcaPlayer[GiPLAYER_1].MoveTo(GiWINDOW_WIDTH / 2 - (cpApp->mcaPlayer[GiPLAYER_1].mcPicture.ReturnWidth() / 2), GiWINDOW_HEIGHT - cpApp->mcaPlayer[GiPLAYER_1].mcPicture.ReturnHeight());
					cpApp->mcaPlayer[GiPLAYER_1].SetVelocities(0, 0);
					cpApp->mcaPlayer[GiPLAYER_1].SetAccelerations(0, 0);

					// Set Ships State
					cpApp->mcaPlayer[GiPLAYER_1].mcPicture.SetCurrentState(GiSHIP_STATE_BACKWARDS);

					// Set Ships Status
					cpApp->mcaPlayer[GiPLAYER_1].SetStatus(GiSHIP_STATUS_NONE);

					// Remove all non Base Weapons
					cpApp->mcaPlayer[GiPLAYER_1].DeleteAllWeaponsOfGeneralType(GiFOREIGN_WEAPON);

					// Set Ships Current Weapon

					// If Ship has Normal Gun Weapon
					if (cpApp->mcaPlayer[GiPLAYER_1].WeaponIsInList(GiNORMAL_WEAPON))
					{
						cpApp->mcaPlayer[GiPLAYER_1].SetCurrentWeapon(GiNORMAL_WEAPON);
					}
					// Else the Ship should have the Three Normal Gun Weapon
					else
					{
						cpApp->mcaPlayer[GiPLAYER_1].SetCurrentWeapon(GiTHREE_NORMAL_WEAPON);
					}
				}
				// Else Game Over so return to Main Menu
				else
				{
					// Game Over
					cpApp->mcGameStateManager.ShutdownCurrentStateAndSetNextState(GameStateMainMenu, cpApp);
					return;
				}
			}
		}


		//********** Make sure Ships don't go off the screen **********
/*
		// If this is NOT an Enemy Boss
		if (cpShip->ReturnGeneralShipType() != GiENEMY_BOSS)
		{
			// Normal enemies/player hit screen edge once any part of their ship touches it

			// If Ship is trying to go too far Left
			if (cpShip->ReturnXCoordinate() < 0 && 
				cpShip->ReturnStatus() != GiSHIP_STATUS_DYING)
			{
				// Move Ship back to edge of screen
				cpShip->MoveXTo(0);
				cpShip->SetXVelocity(0);
				cpShip->SetXAcceleration(0);
			}

			// If Ship is trying to go too far Right
			if ((cpShip->ReturnXCoordinate() + cpShip->mcPicture.ReturnWidth()) > GiWINDOW_WIDTH &&
				cpShip->ReturnStatus() != GiSHIP_STATUS_DYING)
			{
				// Move Ship back to edge of screen
				cpShip->MoveXTo((GiWINDOW_WIDTH - cpShip->mcPicture.ReturnWidth()));
				cpShip->SetXVelocity(0);
				cpShip->SetXAcceleration(0);
			}
		}
		// Else this is an Enemy Boss
		else
		{
*/			// Enemy bosses hit screen edge with the center of their ship

			// If Ship is trying to go too far Left
			if ((cpShip->ReturnXCoordinate() + (cpShip->mcPicture.ReturnWidth() / 2)) < 0 && 
				cpShip->ReturnStatus() != GiSHIP_STATUS_DYING)
			{
				// Move Ship back to edge of screen
				cpShip->MoveXTo(-(cpShip->mcPicture.ReturnWidth() / 2));
				cpShip->SetXVelocity(0);
				cpShip->SetXAcceleration(0);
			}

			// If Ship is trying to go too far Right
			if ((cpShip->ReturnXCoordinate() + (cpShip->mcPicture.ReturnWidth() / 2)) > GiWINDOW_WIDTH &&
				cpShip->ReturnStatus() != GiSHIP_STATUS_DYING)
			{
				// Move Ship back to edge of screen
				cpShip->MoveXTo(GiWINDOW_WIDTH - (cpShip->mcPicture.ReturnWidth() / 2));
				cpShip->SetXVelocity(0);
				cpShip->SetXAcceleration(0);
			}
//		}

		// If Ship is trying to go too far Up
		if (cpShip->ReturnYCoordinate() < 0)
		{
			// If this is an Enemy Ship
			if (cpShip->ReturnGeneralShipType() != GiPLAYER)
			{
				// Make Enemy coast back onto the screen
				cpShip->SetYVelocity(25);
				cpShip->SetYAcceleration(0);
			}
			// Else, if the Level is not complete
			else if (!cpApp->mbLevelComplete)
			{
				// Move Player to top of screen
				cpShip->MoveYTo(0);
				cpShip->SetYVelocity(0);
				cpShip->SetYAcceleration(0);
			}
			// Else the Level is complete so allow Player to Leave the Screen
			else
			{
				// If Player has moved Off Screen
				if (cpShip->ReturnYCoordinate() < -cpShip->mcPicture.ReturnHeight())
				{
					// If Player was on the Last Level of the Game
					if (cpApp->miCurrentLevel == GiTOTAL_NUMBER_OF_LEVELS_IN_GAME)
					{
						// Player has beat the Game so show End Story
						cpApp->mcGameStateManager.ShutdownCurrentStateAndSetNextState(GameStatePlayerBeatGame, cpApp);
					}
					// Else go to the Space Station
					else
					{
						cpApp->mcGameStateManager.ShutdownCurrentStateAndSetNextState(GameStateSpaceStation, cpApp);
					}
				}
			}
		}

		// If Ship is trying to go too far Down
		if (cpShip->ReturnYCoordinate() > (GiWINDOW_HEIGHT - cpShip->mcPicture.ReturnHeight()))
		{
			// If they are not an Enemy Ship AND are not Leaving the Screen
			if ((cpShip->ReturnGeneralShipType() != GiENEMY) || 
				(cpShip->ReturnGeneralShipType() == GiENEMY && cpShip->ReturnStatus() != GiSHIP_STATUS_LEAVE_SCREEN))
			{
				// Move Ship to bottom of screen
				cpShip->MoveYTo(GiWINDOW_HEIGHT - cpShip->mcPicture.ReturnHeight());
				cpShip->SetYVelocity(0);
				cpShip->SetYAcceleration(0);
			}
			// Else it is an Enemy Ship leaving the screen
			else
			{
				// If Ship has left the screen
				if (cpShip->ReturnYCoordinate() > GiWINDOW_HEIGHT)
				{
					// Delete the Ship
					cpApp->mcActiveShipList.PutCurrentShipOnDeleteLaterList();
				}
			}
		}


		//********** Display Enemy **********

		// Display Enemy Picture
		cpApp->mcTexture[GiSHIP_TEXTURE].Blit(cpShip->ReturnXCoordinate(), cpShip->ReturnYCoordinate(),
											  cpShip->mcPicture.ReturnLeft(), cpShip->mcPicture.ReturnTop(),
										      cpShip->mcPicture.ReturnWidth(), cpShip->mcPicture.ReturnHeight());

		// Move to Next Enemy Ship
		cpApp->mcActiveShipList.MoveToNextShip();
	}


	//********** Process Items **********

	// Move to start of Item List
	cpApp->mcActiveItemList.MoveToStartOfList();

	// Loop through all Items in List
	while (cpApp->mcActiveItemList.CurrentItemExists())
	{
		// Get the Current Item
		cpItem = cpApp->mcActiveItemList.ReturnCurrentItem();

		// Check if Player is Colliding with Item (picking it up) and Player is not dead
		if (cpApp->Collision(&cpItem->mcPicture, &cpApp->mcaPlayer[GiPLAYER_1].mcPicture))
		{
			if (cpApp->mcaPlayer[GiPLAYER_1].ReturnStatus() != GiSHIP_STATUS_DYING && cpApp->mcaPlayer[GiPLAYER_1].ReturnStatus() != GiSHIP_STATUS_DEAD)
            {
				// Process Item Effect based on Items Type
				switch (cpItem->ReturnType())
				{
					case GiITEM_FREE_SHIP:

						// Increase Players Ships (men)
						cpApp->mcaPlayer[GiPLAYER_1].SetNumberOfLives(cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfLives() + 1);

					break;

					case GiITEM_RESTORE_HEALTH_100:

						// Restore Players Health by 100
						cpApp->mcaPlayer[GiPLAYER_1].SetHealthRelative(100);

					break;

					case GiITEM_RESTORE_HEALTH_500:

						// Restore Players Health by 500
						cpApp->mcaPlayer[GiPLAYER_1].SetHealthRelative(500);

					break;

					case GiITEM_RESTORE_HEALTH_1000:

						// Restore Players Health by 1000
						cpApp->mcaPlayer[GiPLAYER_1].SetHealthRelative(1000);

					break;

					case GiITEM_RESTORE_HEALTH_FULL:

						// Restore Players Health completely
						cpApp->mcaPlayer[GiPLAYER_1].SetHealth(cpApp->mcaPlayer[GiPLAYER_1].ReturnMaxHealth());

					break;

					case GiITEM_INCREASE_ARMOR:

					break;

					case GiITEM_INCREASE_MAX_VELOCITY:

					break;

					case GiITEM_INCREASE_ACCELERATION:

					break;

					case GiITEM_FIREBALL_WEAPON_50:

						// If this Weapon in not already in the List
						if (cpApp->mcaPlayer[GiPLAYER_1].WeaponIsInList(GiFIREBALL_WEAPON))
						{
							// Increase it's ammo
							cpApp->mcaPlayer[GiPLAYER_1].SetAmmoRelative(GiFIREBALL_WEAPON, 50);
						}
						// Else it's not in the List yet
						else
						{
							cpApp->mcaPlayer[GiPLAYER_1].AddWeapon(cpApp->mcaBulletModel[GiFIREBALL_WEAPON], 50);
						}

					break;

					case GiITEM_LASER_WEAPON_75:

						// If this Weapon in not already in the List
						if (cpApp->mcaPlayer[GiPLAYER_1].WeaponIsInList(GiLASER_WEAPON))
						{
							// Increase it's ammo
							cpApp->mcaPlayer[GiPLAYER_1].SetAmmoRelative(GiLASER_WEAPON, 75);
						}
						// Else it's not in the List yet
						else
						{
							cpApp->mcaPlayer[GiPLAYER_1].AddWeapon(cpApp->mcaBulletModel[GiLASER_WEAPON], 75);
						}

					break;

					case GiITEM_BOMB_WEAPON_15:

						// If this Weapon in not already in the List
						if (cpApp->mcaPlayer[GiPLAYER_1].WeaponIsInList(GiBOMB_WEAPON))
						{
							// Increase it's ammo
							cpApp->mcaPlayer[GiPLAYER_1].SetAmmoRelative(GiBOMB_WEAPON, 15);
						}
						// Else it's not in the List yet
						else
						{
							cpApp->mcaPlayer[GiPLAYER_1].AddWeapon(cpApp->mcaBulletModel[GiBOMB_WEAPON], 15);
						}

					break;

					// If there was a problem
					default:

						// Display an Error Message
						MessageBox(cpApp->GethWnd(), "Error processing Item", "Unrecognized Item Type", MB_OK);

					break;
				};

				// Delete the Item since Player picked it up
				cpApp->mcActiveItemList.PutCurrentItemOnDeleteLaterList();

				// Play Pickup Item Sound
				cpApp->PlaySoundEffect(GiPickupItemSound);
			}
		}

		// Display the Item
		cpApp->mcTexture[GiITEM_TEXTURE].Blit(cpItem->ReturnXCoordinate(), cpItem->ReturnYCoordinate(),
											  cpItem->mcPicture.ReturnLeft(), cpItem->mcPicture.ReturnTop(),
											  cpItem->mcPicture.ReturnWidth(), cpItem->mcPicture.ReturnHeight());

		// Move to next Item
		cpApp->mcActiveItemList.MoveToNextItem();
	}


	//********** Delete all Dead Objects **********

	// Delete all 'dead' Bullets
	cpApp->mcFiredBulletsList.DeleteBulletsInDeleteLaterList();

	// Delete any 'dead' Enemies
	cpApp->mcActiveShipList.DeleteAllShipsInDeleteLaterList();

	// Delete any 'dead' Items
	cpApp->mcActiveItemList.DeleteAllItemsInDeleteLaterList();


	//********** Check and Process if Level is Complete **********

	// If End Boss has been killed
	if (cpApp->mbLevelComplete)
	{
		// Loop through all Ships and have them Leave the Screen

		// Move to the Start of the Ship List
		cpApp->mcActiveShipList.MoveToStartOfList();

		// Loop through all Ships
		while (cpApp->mcActiveShipList.CurrentShipExists())
		{
			// Get the Current Ship
			cpShip = cpApp->mcActiveShipList.ReturnCurrentShip();

			// Mark this Ship to Leave the Screen if it's not Dying or Dead
			if (cpShip->ReturnStatus() != GiSHIP_STATUS_DYING && cpShip->ReturnStatus() != GiSHIP_STATUS_DEAD)
			{
				cpShip->SetStatus(GiSHIP_STATUS_LEAVE_SCREEN);
			}

			// Move to Next Ship In List
			cpApp->mcActiveShipList.MoveToNextShip();
		}
	}
}


// Displays Screen Information, such as players current weapon, score, lives left, etc
void CApp::DisplayScreenInfo(void* vpApp, EPurpose ePurpose)
{	
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpApp;		// cpApp = our CApp class

	char caString[20];				// Temp String holder
	CBullet* cpBullet;				// Temp pointer to a Bullet
	CCharacterPicture* cpPicture;	// Temp pointer to CharacterPictures


	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		return;
	}
	// Else this state is just doing a normal run

	
	// Start up font
	cpApp->mcFontRegular.Begin();

// TODO: Remove the following piece of code from final release. It is used in testing	
/*	
ltoa(cpApp->mcNewScriptedEnemyTimer.ReturnTimePassedSinceLastCheckUsingSavedTime(), caString, 10);
cpApp->mcFontRegular.Print(caString, 10, 30);
*/

	//***** Display amount of lives left *****
	
	// Display Ship Picture
	cpPicture = &cpApp->mcaPlayerShipModel[GiPLAYER_1].mcPicture;
	cpApp->mcTexture[GiSHIP_TEXTURE].Blit(5, 6, cpPicture->ReturnLeft(), cpPicture->ReturnTop(), 
										  cpPicture->ReturnWidth(), cpPicture->ReturnHeight());

	// Display X Picture
	cpApp->mcTexture[GiPLAY_TEXTURE].Blit(40, 9, 1, 1, 43, 32, 0.5, 0.5);

	// Display number of Lives left
	itoa(cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfLives(), caString, 10);
	cpApp->mcFontRegular.Print(caString, 67, 5, 0, 0, D3DCOLOR_RGBA(255, 0, 0, 255));


	//***** Display Current Health *****

	// Display heart
	cpApp->mcTexture[GiPLAY_TEXTURE].Blit(120, 6, 102, 41, 25, 19);

	// Display Health left
//	cpApp->mcTexture[GiPLAY_TEXTURE].Blit(150, 8, 1, 41, 100, 15, 1.0, 1.0);
	itoa(cpApp->mcaPlayer[GiPLAYER_1].ReturnHealth(), caString, 10);
	cpApp->mcFontRegular.Print(caString, 150, 5, 0, 0, D3DCOLOR_RGBA(180, 0, 0, 255));


	//***** Display Current Bullet *****

	// Display Bullet Holder
	cpApp->mcTexture[GiPLAY_TEXTURE].Blit(((GiWINDOW_WIDTH / 2) - 16), 4, 170, 2, 33, 33);

	// Get Current Bullet if Current Weapon is set
	if ((cpBullet = cpApp->mcaPlayer[GiPLAYER_1].ReturnCurrentWeaponsBullet()) != 0)
	{
		// If this Bullet does not have an Alternate Effect (is not special)
		if (!cpBullet->ReturnIfBulletHasAlternateEffect())
		{
			cpApp->mcTexture[GiBULLET_TEXTURE].Blit(((GiWINDOW_WIDTH / 2) - (cpBullet->mcPicture.ReturnWidth() / 2)), (20 - (cpBullet->mcPicture.ReturnHeight() / 2)), cpBullet->mcPicture.ReturnLeft(), cpBullet->mcPicture.ReturnTop(), cpBullet->mcPicture.ReturnWidth(), cpBullet->mcPicture.ReturnHeight());
		}
		// Else Bullet Has an Alternate Effect (is special)
		else
		{
			// Check which Type of Bullet this is and process it accordingly
			switch (cpBullet->ReturnSpecificType())
			{
				case GiTHREE_NORMAL_GUN_BULLET:
				case GiTHREE_MACHINE_GUN_BULLET:
					
					// Put this Bullet on Left side
					cpApp->mcTexture[GiBULLET_TEXTURE].Blit(((GiWINDOW_WIDTH / 2) - (cpBullet->mcPicture.ReturnWidth() / 2) - 10), (25 - (cpBullet->mcPicture.ReturnHeight() / 2)), cpBullet->mcPicture.ReturnLeft(), cpBullet->mcPicture.ReturnTop(), cpBullet->mcPicture.ReturnWidth(), cpBullet->mcPicture.ReturnHeight());
				
				
					// Put this Bullet in the middle
					cpApp->mcTexture[GiBULLET_TEXTURE].Blit(((GiWINDOW_WIDTH / 2) - (cpBullet->mcPicture.ReturnWidth() / 2)), (15 - (cpBullet->mcPicture.ReturnHeight() / 2)), cpBullet->mcPicture.ReturnLeft(), cpBullet->mcPicture.ReturnTop(), cpBullet->mcPicture.ReturnWidth(), cpBullet->mcPicture.ReturnHeight());
				
				
					// Put this Bullet on the right side
					cpApp->mcTexture[GiBULLET_TEXTURE].Blit(((GiWINDOW_WIDTH / 2) - (cpBullet->mcPicture.ReturnWidth() / 2) + 10), (25 - (cpBullet->mcPicture.ReturnHeight() / 2)), cpBullet->mcPicture.ReturnLeft(), cpBullet->mcPicture.ReturnTop(), cpBullet->mcPicture.ReturnWidth(), cpBullet->mcPicture.ReturnHeight());

				break;

				default:

				break;
			};
		}
	}
		
	// Display amount of Ammo left
	
	// If ammo is Unilimited
	if (cpApp->mcaPlayer[GiPLAYER_1].ReturnCurrentWeaponsAmmo() == GiUNLIMITED_AMMO)
	{
		// Fill the string with "Unlimited"
		strcpy(caString, "Unlimited");
	}
	// Else ammo is limited
	else
	{
		// Fill the string with the amount of Ammo left for the Current Weapon
		itoa(cpApp->mcaPlayer[GiPLAYER_1].ReturnCurrentWeaponsAmmo(), caString, 10);
	}

	// Display the amount of Ammo
	cpApp->mcFontRegular.Print(caString, 345, 5, 0, 0, D3DCOLOR_RGBA(0, 0, 255, 255));
	

	//***** Display Score *****
	
	// Display Score picture
	cpApp->mcTexture[GiPLAY_TEXTURE].Blit(480, 9, 47, 2, 121, 31, 0.5, 0.5); 

	// Display current Score
	itoa(cpApp->mcaPlayer[GiPLAYER_1].ReturnNumberOfPoints(), caString, 10);
	cpApp->mcFontRegular.Print(caString, 550, 5, 0, 0, D3DCOLOR_RGBA(0, 0, 255, 255));


	// Stop font
	cpApp->mcFontRegular.End();
}




//********** Do Level Stuff Functions **********
		
// Do Level 1 Stuff
void CApp::Level1(void* vpApp, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpApp;		// cpApp = our CApp class

	// Holds Background Stars
	static SVertex SsStar[GiNUM_OF_LEVEL_1_BACKGROUND_STARS];

	int iIndex = 0;					// Temp counter
	int iBrightness = 0;			// Temp variable for Star Brightness


	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		// Initialize background Stars
		for (iIndex = 0; iIndex < GiNUM_OF_LEVEL_1_BACKGROUND_STARS; iIndex++)
		{
			SsStar[iIndex].fX		= rand() % GiWINDOW_WIDTH;
			SsStar[iIndex].fY		= rand() % GiWINDOW_HEIGHT;
			SsStar[iIndex].fZ		= 0;
			SsStar[iIndex].fRHW		= 1;
			SsStar[iIndex].fYVelocity= rand() % GiMAX_VELOCITY_OF_LEVEL_1_BACKGROUND_STARS;

			// Slow down half of the Stars that are going fast
			if (SsStar[iIndex].fYVelocity > (GiMAX_VELOCITY_OF_LEVEL_1_BACKGROUND_STARS / 2))
			{
				// Only True 1/2 the time
				if (rand() % 2 == 0)
				{
					SsStar[iIndex].fYVelocity = rand() % (GiMAX_VELOCITY_OF_LEVEL_1_BACKGROUND_STARS / 2);
				}
				// If they were not slowed down, freeze some more of them
				else if (rand() % 4 == 0)
				{
					SsStar[iIndex].fYVelocity = 0;
				}
			}

			// Make slower stars dimmer so they appear farther away using their Velocity as Alpha value
			// Add extra value so they aren't too dim
			if ((iBrightness = (int)floorf(SsStar[iIndex].fYVelocity) + GiLEVEL_1_BASE_STAR_BRIGHTNESS) > 255)
			{
				iBrightness = 255;
			}
			SsStar[iIndex].diffuse = D3DCOLOR_RGBA(255, 255, 255, iBrightness);
		}	
		
		// Load this Levels Enemies and Objects
		cpApp->LoadEnemiesFromFile(cpApp, "Data\\Levels\\Level1.objects");

		// Set Friction for this level
		cpApp->miLevelXFriction = GiLEVEL_1_X_FRICTION;
		cpApp->miLevelYFriction = GiLEVEL_1_Y_FRICTION;

		// Start Level 1 Music
		cpApp->PlayMusic(GiLevel1Music);

		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		return;
	}
	// Else this state is just doing a normal run

	// Move background stars
	for (iIndex = 0; iIndex < GiNUM_OF_LEVEL_1_BACKGROUND_STARS; iIndex++)
	{
		// Move Star
		SsStar[iIndex].fY += cpApp->GetFloatDistanceTravelled(SsStar[iIndex].fYVelocity, cpApp->ReturnAmountOfTimeSinceLastFrame());

		// Make Star wrap around screen
		if (SsStar[iIndex].fY > GiWINDOW_HEIGHT)
		{
			SsStar[iIndex].fY = -(rand() % (GiWINDOW_HEIGHT / 2));	// Reposition Star vertically so they are not all beside each other again
			SsStar[iIndex].fX = rand() % GiWINDOW_WIDTH;			// Reposition Star horizontlly
		}
	}

	// Place Stars into Vertex Buffer and Draw them
	cpApp->mcVertexBuffer.Set(0, GiNUM_OF_LEVEL_1_BACKGROUND_STARS, SsStar);
	cpApp->mcVertexBuffer.Render(0, GiNUM_OF_LEVEL_1_BACKGROUND_STARS, D3DPT_POINTLIST);
}

// Do Level 2 Stuff
void CApp::Level2(void* vpApp, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpApp;		// cpApp = our CApp class

	// Holds Background Stars
	static SVertex SsStar[GiNUM_OF_LEVEL_2_BACKGROUND_STARS];

	int iIndex = 0;					// Temp counter
	int iBrightness = 0;			// Temp variable for Star Brightness


	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		// Initialize background Stars
		for (iIndex = 0; iIndex < GiNUM_OF_LEVEL_2_BACKGROUND_STARS; iIndex++)
		{
			SsStar[iIndex].fX		= rand() % GiWINDOW_WIDTH;
			SsStar[iIndex].fY		= rand() % GiWINDOW_HEIGHT;
			SsStar[iIndex].fZ		= 0;
			SsStar[iIndex].fRHW		= 1;
			SsStar[iIndex].fYVelocity= rand() % GiMAX_VELOCITY_OF_LEVEL_2_BACKGROUND_STARS;

			// Slow down half of the Stars that are going fast
			if (SsStar[iIndex].fYVelocity > (GiMAX_VELOCITY_OF_LEVEL_2_BACKGROUND_STARS / 2))
			{
				// Only True 1/2 the time
				if (rand() % 2 == 0)
				{
					SsStar[iIndex].fYVelocity = rand() % (GiMAX_VELOCITY_OF_LEVEL_2_BACKGROUND_STARS / 2);
				}
				// If they were not slowed down, freeze some more of them
				else if (rand() % 4 == 0)
				{
					SsStar[iIndex].fYVelocity = 0;
				}
			}

			// Make slower stars dimmer so they appear farther away using their Velocity as Alpha value
			// Add extra value so they aren't too dim
			if ((iBrightness = (int)floorf(SsStar[iIndex].fYVelocity) + GiLEVEL_2_BASE_STAR_BRIGHTNESS) > 255)
			{
				iBrightness = 255;
			}
			SsStar[iIndex].diffuse = D3DCOLOR_RGBA(255, 255, 255, iBrightness);
		}	
		
		// Load this Levels Enemies and Objects
		cpApp->LoadEnemiesFromFile(cpApp, "Data\\Levels\\Level2.objects");

		// Set Friction for this level
		cpApp->miLevelXFriction = GiLEVEL_2_X_FRICTION;
		cpApp->miLevelYFriction = GiLEVEL_2_Y_FRICTION;

		// Start Level 2 Music
		cpApp->PlayMusic(GiLevel2Music);

		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		return;
	}
	// Else this state is just doing a normal run

	// Move background stars
	for (iIndex = 0; iIndex < GiNUM_OF_LEVEL_2_BACKGROUND_STARS; iIndex++)
	{
		// Move Star
		SsStar[iIndex].fY += cpApp->GetFloatDistanceTravelled(SsStar[iIndex].fYVelocity, cpApp->ReturnAmountOfTimeSinceLastFrame());

		// Make Star wrap around screen
		if (SsStar[iIndex].fY > GiWINDOW_HEIGHT)
		{
			SsStar[iIndex].fY = -(rand() % (GiWINDOW_HEIGHT / 2));	// Reposition Star vertically so they are not all beside each other again
			SsStar[iIndex].fX = rand() % GiWINDOW_WIDTH;			// Reposition Star horizontlly
		}
	}

	// Place Stars into Vertex Buffer and Draw them
	cpApp->mcVertexBuffer.Set(0, GiNUM_OF_LEVEL_2_BACKGROUND_STARS, SsStar);
	cpApp->mcVertexBuffer.Render(0, GiNUM_OF_LEVEL_2_BACKGROUND_STARS, D3DPT_POINTLIST);
}

// Do Level 3 Stuff
void CApp::Level3(void* vpApp, EPurpose ePurpose)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpApp;		// cpApp = our CApp class

	// Holds Background Stars
	static SVertex SsStar[GiNUM_OF_LEVEL_3_BACKGROUND_STARS];

	int iIndex = 0;					// Temp counter
	int iBrightness = 0;			// Temp variable for Star Brightness


	//********** Check State Purpose **********

	// If this state is being called for the first time
	if (ePurpose == eInitialize)
	{
		// Initialize background Stars
		for (iIndex = 0; iIndex < GiNUM_OF_LEVEL_3_BACKGROUND_STARS; iIndex++)
		{
			SsStar[iIndex].fX		= rand() % GiWINDOW_WIDTH;
			SsStar[iIndex].fY		= rand() % GiWINDOW_HEIGHT;
			SsStar[iIndex].fZ		= 0;
			SsStar[iIndex].fRHW		= 1;
			SsStar[iIndex].fYVelocity= rand() % GiMAX_VELOCITY_OF_LEVEL_3_BACKGROUND_STARS;

			// Slow down half of the Stars that are going fast
			if (SsStar[iIndex].fYVelocity > (GiMAX_VELOCITY_OF_LEVEL_3_BACKGROUND_STARS / 2))
			{
				// Only True 1/2 the time
				if (rand() % 2 == 0)
				{
					SsStar[iIndex].fYVelocity = rand() % (GiMAX_VELOCITY_OF_LEVEL_3_BACKGROUND_STARS / 2);
				}
				// If they were not slowed down, freeze some more of them
				else if (rand() % 4 == 0)
				{
					SsStar[iIndex].fYVelocity = 0;
				}
			}

			// Make slower stars dimmer so they appear farther away using their Velocity as Alpha value
			// Add extra value so they aren't too dim
			if ((iBrightness = (int)floorf(SsStar[iIndex].fYVelocity) + GiLEVEL_3_BASE_STAR_BRIGHTNESS) > 255)
			{
				iBrightness = 255;
			}
			SsStar[iIndex].diffuse = D3DCOLOR_RGBA(255, 255, 255, iBrightness);
		}	
		
		// Load this Levels Enemies and Objects
		cpApp->LoadEnemiesFromFile(cpApp, "Data\\Levels\\Level3.objects");

		// Set Friction for this level
		cpApp->miLevelXFriction = GiLEVEL_3_X_FRICTION;
		cpApp->miLevelYFriction = GiLEVEL_3_Y_FRICTION;

		// Start Level 3 Music
		cpApp->PlayMusic(GiLevel3Music);

		return;
	}
	// Else if this state is returning (Was initialized earlier, game state changed, and changed back to this state again)
	else if (ePurpose == eReturning)
	{
		return;
	}
	// Else if this state is being called for the last time
	else if (ePurpose == eShutdown)
	{
		return;
	}
	// Else this state is just doing a normal run

	// Move background stars
	for (iIndex = 0; iIndex < GiNUM_OF_LEVEL_3_BACKGROUND_STARS; iIndex++)
	{
		// Move Star
		SsStar[iIndex].fY += cpApp->GetFloatDistanceTravelled(SsStar[iIndex].fYVelocity, cpApp->ReturnAmountOfTimeSinceLastFrame());

		// Make Star wrap around screen
		if (SsStar[iIndex].fY > GiWINDOW_HEIGHT)
		{
			SsStar[iIndex].fY = -(rand() % (GiWINDOW_HEIGHT / 2));	// Reposition Star vertically so they are not all beside each other again
			SsStar[iIndex].fX = rand() % GiWINDOW_WIDTH;			// Reposition Star horizontlly
		}
	}

	// Place Stars into Vertex Buffer and Draw them
	cpApp->mcVertexBuffer.Set(0, GiNUM_OF_LEVEL_3_BACKGROUND_STARS, SsStar);
	cpApp->mcVertexBuffer.Render(0, GiNUM_OF_LEVEL_3_BACKGROUND_STARS, D3DPT_POINTLIST);
}

// Loads Enemies from File into mcInactiveEnemyList
void CApp::LoadEnemiesFromFile(void* vpApp, char* cpFileName)
{
	//********** Initialize Variables **********

	CApp* cpApp	= (CApp*)vpApp;		// cpApp = our CApp class
	ifstream InFile;				// File Object
	char caType[100];				// Holds Enemy Type
	char caX[20];					// Holds Enemies X position
	char caTime[20];				// Holds Time to Elapse before Enemy should be created
	char caBuffer[501];				// Temp Buffer to store garbage
	
	bool bHaveReadInRandomEnemyValues = false;	// Tells if the Random Enemy Values have been read in yet or not
	char caMaxRandomEnemyTime[10];		// Holds the Max time that a Random Enemy must be created in
	char caRandomEnemyLikeliness[10];	// Holds the Likeliness that a Random Enemy will be created once time has elapsed

	// Open File (do not create a new one)
	InFile.open(cpFileName, ios_base::in);
    
	// Read the whole file 
	while (InFile)
	{
		// Read in the Enemies Type
		InFile >> caType;
		
		// If this is the start of a multi-line comment
		if (!strcmp(caType, "/*"))
		{
			// Read data in until the end of the multi-line comment or EOF is found
			while (InFile && strcmp(caType, "*/"))
			{
				InFile >> caType;
			}
		}
		// If this is not a line comment or empty line
		else if (strcmp(caType, "//") && strcmp(caType, ""))
		{
			// If we have already read in the Random Enemy Values
			if (bHaveReadInRandomEnemyValues)
			{
				// Read in X and Y position, and Time until object should be created
				InFile >> caX >> caTime;

				// Add Enemy/Object to Inactive Enemy List
				cpApp->mcInactiveEnemyList.AddObjectPrototype(atoi(caType), atol(caTime), atoi(caX), -cpApp->mcaEnemyModel[atoi(caType)].mcPicture.ReturnHeight());
			}
			// Else we still need to read in the Random Enemy Values
			else
			{
				// Read in the Random Enemy Info
				InFile >> caMaxRandomEnemyTime >> caRandomEnemyLikeliness;

				// Set Random Enemy info
				cpApp->mulMinRandomEnemyTime	= atol(caType);
				cpApp->mulMaxRandomEnemyTime	= atol(caMaxRandomEnemyTime);
				cpApp->miRandomEnemyLikeliness	= atoi(caRandomEnemyLikeliness);

				bHaveReadInRandomEnemyValues = true;
			}
		}
	
		// Read in the rest of the line
		InFile.getline(caBuffer, 500, '\n');
	}

	// Close the File
	InFile.close();
}



//********** Other Function Declarations **********

// Returns True if there is a Collision between Object 1 and Object 2, False if not
bool CApp::Collision(CCharacterPicture* cpObject1, CCharacterPicture* cpObject2)
{
	// Get Object 1 Box
	int iObject1Left	= cpObject1->ReturnXCoordinate();
	int iObject1Top		= cpObject1->ReturnYCoordinate();
	int iObject1Right	= iObject1Left + cpObject1->ReturnWidth();
	int iObject1Bottom	= iObject1Top + cpObject1->ReturnHeight();

	// Get Object 2 Box
	int iObject2Left	= cpObject2->ReturnXCoordinate();
	int iObject2Top		= cpObject2->ReturnYCoordinate();
	int iObject2Right	= iObject2Left + cpObject2->ReturnWidth();
	int iObject2Bottom	= iObject2Top + cpObject2->ReturnHeight();

	// Test for No Collision between tops and bottoms
	if (iObject1Bottom < iObject2Top)
	{
		return false;
	}
	
	// Test for No Collision between tops and bottoms
	if (iObject1Top > iObject2Bottom)
	{
		return false;
	}

	// Test for No Collision between left and right sides
	if (iObject1Right < iObject2Left)
	{
		return false;
	}
	
	// Test for No Collision between left and right sides
	if (iObject1Left > iObject2Right)
	{
		return false;
	}

	// Return that there was a collision
	return(true);
}


// Create Object "Models" (Sets Pictures, States,and other info)
void CApp::CreateModels(void* vpApp)
{
	CApp* cpApp	= (CApp*)vpApp;

	int iIndex = 0;


	//********** Define Rotation Orders **********
	
	// Players' Ship Picture Rotations
	int iaShipForward[2]		= {GiSHIP_PICTURE_HALF_IMPULSE, GiSHIP_PICTURE_FULL_IMPULSE};
	int iaShipBackwards[1]		= {GiSHIP_PICTURE_ZERO_IMPULSE};
	int iaShipIdle[2]			= {GiSHIP_PICTURE_HALF_IMPULSE, GiSHIP_PICTURE_ZERO_IMPULSE};
	
	// Sequential Enemy Picture Rotations
	int iaEnemyPictureRotation1[1]	= {GiENEMY_PICTURE_1};
	int iaEnemyPictureRotation2[2]	= {GiENEMY_PICTURE_1, GiENEMY_PICTURE_2};
	int iaEnemyPictureRotation3[3]	= {GiENEMY_PICTURE_1, GiENEMY_PICTURE_2, GiENEMY_PICTURE_3}; 
	int iaEnemyPictureRotation4[4]	= {GiENEMY_PICTURE_1, GiENEMY_PICTURE_2, GiENEMY_PICTURE_3, GiENEMY_PICTURE_4}; 
	int iaEnemyPictureRotation7[7]	= {GiENEMY_PICTURE_1, GiENEMY_PICTURE_2, GiENEMY_PICTURE_3, GiENEMY_PICTURE_4, GiENEMY_PICTURE_5, GiENEMY_PICTURE_6, GiENEMY_PICTURE_7}; 
	int iaEnemyPictureRotation12[12]= {GiENEMY_PICTURE_1, GiENEMY_PICTURE_2, GiENEMY_PICTURE_3, GiENEMY_PICTURE_4, GiENEMY_PICTURE_5, GiENEMY_PICTURE_6, GiENEMY_PICTURE_7, GiENEMY_PICTURE_8, GiENEMY_PICTURE_9, GiENEMY_PICTURE_10, GiENEMY_PICTURE_11, GiENEMY_PICTURE_12}; 
	
	// Special Enemy Picture Rotations
	int iaEnemySlimyAlienPictureRotation6[6] = {GiENEMY_PICTURE_1, GiENEMY_PICTURE_2, GiENEMY_PICTURE_3, GiENEMY_PICTURE_4, GiENEMY_PICTURE_2, GiENEMY_PICTURE_3}; 

	// Player/Enemy Death 'Scene'
	int iaDeath[8]				= {GiSHIP_PICTURE_DEATH_1, GiSHIP_PICTURE_DEATH_2, GiSHIP_PICTURE_DEATH_3, GiSHIP_PICTURE_DEATH_4, GiSHIP_PICTURE_DEATH_5, GiSHIP_PICTURE_DEATH_6, GiSHIP_PICTURE_DEATH_7, GiSHIP_PICTURE_DEATH_DONE};
	
	// Bullet Travel/Detonation 'Scenes'
	int iaBulletTraveling1[1]	= {GiBULLET_PICTURE_TRAVELING_1};
	int iaBulletTraveling3[3]	= {GiBULLET_PICTURE_TRAVELING_1, GiBULLET_PICTURE_TRAVELING_2, GiBULLET_PICTURE_TRAVELING_3};
	int iaBulletDetonating1[2]	= {GiBULLET_PICTURE_DETONATING_1, GiBULLET_PICTURE_DETONATING_DONE};
	int iaBulletDetonating2[3]	= {GiBULLET_PICTURE_DETONATING_1, GiBULLET_PICTURE_DETONATING_2, GiBULLET_PICTURE_DETONATING_DONE};
	int iaBulletDetonating3[4]	= {GiBULLET_PICTURE_DETONATING_1, GiBULLET_PICTURE_DETONATING_2, GiBULLET_PICTURE_DETONATING_3, GiBULLET_PICTURE_DETONATING_DONE};
	int iaBulletDetonating4[5]	= {GiBULLET_PICTURE_DETONATING_1, GiBULLET_PICTURE_DETONATING_2, GiBULLET_PICTURE_DETONATING_3, GiBULLET_PICTURE_DETONATING_4, GiBULLET_PICTURE_DETONATING_DONE};

	// Item Moving 'Scene'
	int iaItemMoving[1]			= {GiITEM_PICTURE};

	// Space Station Revolution Order
	int iaSpaceStationRevolving[6] = {GiSPACE_STATION_PICTURE_REVOLUTION_1, GiSPACE_STATION_PICTURE_REVOLUTION_2, GiSPACE_STATION_PICTURE_REVOLUTION_3, GiSPACE_STATION_PICTURE_REVOLUTION_4, GiSPACE_STATION_PICTURE_REVOLUTION_5, GiSPACE_STATION_PICTURE_REVOLUTION_6};


	//***** Create Bullet Models First *****

	// Create Bullet Pictures

	// Create Normal Gun Bullet Pictures
	cpApp->mcaBulletModel[GiNORMAL_GUN_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_TRAVELING_1, 9, 25, 3, 6);
	cpApp->mcaBulletModel[GiNORMAL_GUN_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_1, 14, 25, 8, 8);
	cpApp->mcaBulletModel[GiNORMAL_GUN_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_2, 25, 25, 11, 10);
	cpApp->mcaBulletModel[GiNORMAL_GUN_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_DONE, 0, 0, 1, 1);

	// Create Machine Gun Bullet Pictures
	cpApp->mcaBulletModel[GiMACHINE_GUN_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_TRAVELING_1, 9, 38, 1, 3);
	cpApp->mcaBulletModel[GiMACHINE_GUN_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_1, 12, 38, 5, 5);
	cpApp->mcaBulletModel[GiMACHINE_GUN_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_2, 19, 38, 5, 5);
	cpApp->mcaBulletModel[GiMACHINE_GUN_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_DONE, 0, 0, 1, 1);

	//cpApp->mcFlameThrowerBulletModel.mcPicture.CreatePicture(....);

	// Create Fireball Bullet Pictures
	cpApp->mcaBulletModel[GiFIREBALL_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_TRAVELING_1, 0, 3, 14, 14);
	cpApp->mcaBulletModel[GiFIREBALL_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_TRAVELING_2, 18, 3, 17, 16);
	cpApp->mcaBulletModel[GiFIREBALL_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_TRAVELING_3, 41, 3, 17, 15);
	cpApp->mcaBulletModel[GiFIREBALL_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_1, 62, 3, 30, 19);
	cpApp->mcaBulletModel[GiFIREBALL_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_2, 97, 3, 29, 20);
	cpApp->mcaBulletModel[GiFIREBALL_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_3, 131, 3, 32, 22);
	cpApp->mcaBulletModel[GiFIREBALL_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_4, 168, 3, 38, 20);
	cpApp->mcaBulletModel[GiFIREBALL_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_DONE, 0, 0, 1, 1);

	// Create Laser Bullet Pictures
	cpApp->mcaBulletModel[GiLASER_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_TRAVELING_1, 0, 24, 4, 27);
	cpApp->mcaBulletModel[GiLASER_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_1, 0, 24, 4, 27);
	cpApp->mcaBulletModel[GiLASER_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_DONE, 0, 0, 1, 1);

	// Create Bomb Bullet Pictures
	cpApp->mcaBulletModel[GiBOMB_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_TRAVELING_1, 40, 25, 6, 16);  
	cpApp->mcaBulletModel[GiBOMB_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_1, 0, 361, 149,151);
	cpApp->mcaBulletModel[GiBOMB_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_2, 159, 361, 149, 151);
	cpApp->mcaBulletModel[GiBOMB_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_3, 321, 361, 149, 151);
	cpApp->mcaBulletModel[GiBOMB_BULLET].mcPicture.CreatePicture(GiBULLET_PICTURE_DETONATING_DONE, 0, 0, 1, 1);



	// Create Bullet States

	// Create Normal Gun Bullet States
	cpApp->mcaBulletModel[GiNORMAL_GUN_BULLET].mcPicture.CreateState(GiBULLET_STATE_TRAVELING, iaBulletTraveling1, 1);
	cpApp->mcaBulletModel[GiNORMAL_GUN_BULLET].mcPicture.CreateState(GiBULLET_STATE_DETONATING, iaBulletDetonating2, 3, 100, GiNORMAL_GUN_BULLET_DETONATION_X_VELOCITY, GiNORMAL_GUN_BULLET_DETONATION_Y_VELOCITY);

	// Create Machine Gun Bullet States
	cpApp->mcaBulletModel[GiMACHINE_GUN_BULLET].mcPicture.CreateState(GiBULLET_STATE_TRAVELING, iaBulletTraveling1, 1);
	cpApp->mcaBulletModel[GiMACHINE_GUN_BULLET].mcPicture.CreateState(GiBULLET_STATE_DETONATING, iaBulletDetonating2, 3, 100, GiMACHINE_GUN_BULLET_DETONATION_X_VELOCITY, GiMACHINE_GUN_BULLET_DETONATION_Y_VELOCITY);

	// cpApp->mcFlameThrowerBulletModel.mcPicture.CreateState(....);

	// Create Fireball Bullet States
	cpApp->mcaBulletModel[GiFIREBALL_BULLET].mcPicture.CreateState(GiBULLET_STATE_TRAVELING, iaBulletTraveling3, 3, 100);
	cpApp->mcaBulletModel[GiFIREBALL_BULLET].mcPicture.CreateState(GiBULLET_STATE_DETONATING, iaBulletDetonating4, 5, 100, GiFIREBALL_BULLET_DETONATION_X_VELOCITY, GiFIREBALL_BULLET_DETONATION_Y_VELOCITY);

	// Create Laser Bullet States
	cpApp->mcaBulletModel[GiLASER_BULLET].mcPicture.CreateState(GiBULLET_STATE_TRAVELING, iaBulletTraveling1, 1);
	cpApp->mcaBulletModel[GiLASER_BULLET].mcPicture.CreateState(GiBULLET_STATE_DETONATING, iaBulletDetonating1, 2, GiLASER_BULLET_DETONATION_X_VELOCITY, GiLASER_BULLET_DETONATION_Y_VELOCITY);

	// Create Bomb Bullet States
	cpApp->mcaBulletModel[GiBOMB_BULLET].mcPicture.CreateState(GiBULLET_STATE_TRAVELING, iaBulletTraveling1, 1);
	cpApp->mcaBulletModel[GiBOMB_BULLET].mcPicture.CreateState(GiBULLET_STATE_DETONATING, iaBulletDetonating3, 4, 150, GiBOMB_BULLET_DETONATION_X_VELOCITY, GiBOMB_BULLET_DETONATION_Y_VELOCITY);

	// Set Current Bullet States
	cpApp->mcaBulletModel[GiNORMAL_GUN_BULLET].mcPicture.SetCurrentState(GiBULLET_STATE_TRAVELING);
	cpApp->mcaBulletModel[GiTHREE_NORMAL_GUN_BULLET].mcPicture.CopyFrom(cpApp->mcaBulletModel[GiNORMAL_GUN_BULLET].mcPicture);
	cpApp->mcaBulletModel[GiMACHINE_GUN_BULLET].mcPicture.SetCurrentState(GiBULLET_STATE_TRAVELING);
	cpApp->mcaBulletModel[GiTHREE_MACHINE_GUN_BULLET].mcPicture.CopyFrom(cpApp->mcaBulletModel[GiMACHINE_GUN_BULLET].mcPicture);
	// cpApp->mcFlameThrowerBulletModel.mcPicture.SetCurrentState(GiBULLET_STATE_TRAVELING);
	cpApp->mcaBulletModel[GiFIREBALL_BULLET].mcPicture.SetCurrentState(GiBULLET_STATE_TRAVELING);
	cpApp->mcaBulletModel[GiLASER_BULLET].mcPicture.SetCurrentState(GiBULLET_STATE_TRAVELING);
	cpApp->mcaBulletModel[GiBOMB_BULLET].mcPicture.SetCurrentState(GiBULLET_STATE_TRAVELING);

	
	// Fill in Bullet info (Type, Damage amounts, Detonation Cause, Velocities, 
	// Has Alternate Effect, Detonation Duration, Fire Delay, etc)

	for (iIndex = 0; iIndex < GiTOTAL_NUMBER_OF_WEAPONS_IN_GAME; iIndex++)
	{
		// Fill in Weapon info
		cpApp->mcaBulletModel[iIndex].SetSpecificType(iIndex);
		cpApp->mcaBulletModel[iIndex].SetGeneralType(GiBULLET_GENERAL_TYPES[iIndex]);
		cpApp->mcaBulletModel[iIndex].SetBaseDamage(GiBULLET_BASE_DAMAGE[iIndex]);
		cpApp->mcaBulletModel[iIndex].SetRandomDamage(GiBULLET_RANDOM_DAMAGE[iIndex]);
		cpApp->mcaBulletModel[iIndex].SetCauseOfDetonation(GiBULLET_CAUSE_OF_DETONATION[iIndex]);
		cpApp->mcaBulletModel[iIndex].SetVelocities(GiBULLET_X_VELOCITY[iIndex], GiBULLET_Y_VELOCITY[iIndex]);
		cpApp->mcaBulletModel[iIndex].SetFireDelay(GiWEAPON_DELAY[iIndex]);
		cpApp->mcaBulletModel[iIndex].SetIfHasAlternateEffect(GiBULLET_HAS_ALTERNATE_EFFECT[iIndex]);
	}



	//***** Create Item Models *****

	// Create Item Pictures
	cpApp->mcaItemModel[GiITEM_FREE_SHIP].mcPicture.CreatePicture(GiITEM_PICTURE, 0, 0, 31, 31);
	cpApp->mcaItemModel[GiITEM_RESTORE_HEALTH_100].mcPicture.CreatePicture(GiITEM_PICTURE, 32, 0, 31, 31);
	cpApp->mcaItemModel[GiITEM_RESTORE_HEALTH_500].mcPicture.CreatePicture(GiITEM_PICTURE, 64, 0, 31, 31);
	cpApp->mcaItemModel[GiITEM_RESTORE_HEALTH_1000].mcPicture.CreatePicture(GiITEM_PICTURE, 96, 0, 31, 31);
	cpApp->mcaItemModel[GiITEM_RESTORE_HEALTH_FULL].mcPicture.CreatePicture(GiITEM_PICTURE, 128, 0, 31, 31);
	cpApp->mcaItemModel[GiITEM_INCREASE_ARMOR].mcPicture.CreatePicture(GiITEM_PICTURE, 160, 0, 31, 31);
	cpApp->mcaItemModel[GiITEM_INCREASE_MAX_VELOCITY].mcPicture.CreatePicture(GiITEM_PICTURE, 192, 0, 31, 31);
	cpApp->mcaItemModel[GiITEM_INCREASE_ACCELERATION].mcPicture.CreatePicture(GiITEM_PICTURE, 224, 0, 31, 31);
	cpApp->mcaItemModel[GiITEM_FIREBALL_WEAPON_50].mcPicture.CreatePicture(GiITEM_PICTURE, 0, 32, 31, 31);
	cpApp->mcaItemModel[GiITEM_LASER_WEAPON_75].mcPicture.CreatePicture(GiITEM_PICTURE, 32, 32, 31, 31);
	cpApp->mcaItemModel[GiITEM_BOMB_WEAPON_15].mcPicture.CreatePicture(GiITEM_PICTURE, 64, 32, 31, 31);

	// Create and Set States, and set all other info of all Items
	for (iIndex = 0; iIndex < GiTOTAL_NUMBER_OF_TYPES_OF_ITEMS_IN_GAME; iIndex++)
	{
		cpApp->mcaItemModel[iIndex].mcPicture.CreateState(0, iaItemMoving, 1, 0);
		cpApp->mcaItemModel[iIndex].mcPicture.SetCurrentState(0);
		cpApp->mcaItemModel[iIndex].SetType(iIndex);
		cpApp->mcaItemModel[iIndex].SetVelocities(0, GiITEM_Y_VELOCITY);
	}

	
	//***** Create Ship Models *****

	// Create Ship 1 Pictures
	cpApp->mcaPlayerShipModel[GiPLAYER_1].mcPicture.CreatePicture(GiSHIP_PICTURE_ZERO_IMPULSE, 0, 0, 29, 22);
	cpApp->mcaPlayerShipModel[GiPLAYER_1].mcPicture.CreatePicture(GiSHIP_PICTURE_HALF_IMPULSE, 34, 0, 29, 23);
	cpApp->mcaPlayerShipModel[GiPLAYER_1].mcPicture.CreatePicture(GiSHIP_PICTURE_FULL_IMPULSE, 68, 0, 29, 24);

	// Create Ship 2 Pictures
	cpApp->mcaPlayerShipModel[GiPLAYER_2].mcPicture.CreatePicture(GiSHIP_PICTURE_ZERO_IMPULSE, 0, 32, 28, 22);
	cpApp->mcaPlayerShipModel[GiPLAYER_2].mcPicture.CreatePicture(GiSHIP_PICTURE_HALF_IMPULSE, 34, 32, 28, 23);
	cpApp->mcaPlayerShipModel[GiPLAYER_2].mcPicture.CreatePicture(GiSHIP_PICTURE_FULL_IMPULSE, 68, 32, 28, 24);

	// Create and Set Player Info
	for (iIndex = 0; iIndex < GiMAX_NUMBER_OF_PLAYERS; iIndex++)
	{
		// Set Ship Info
		cpApp->mcaPlayerShipModel[iIndex].SetGeneralShipType(GiPLAYER);
		cpApp->mcaPlayerShipModel[iIndex].SetHealth(GiPLAYER_LEVEL_1_MAX_HEALTH);
		cpApp->mcaPlayerShipModel[iIndex].SetMaxHealth(GiPLAYER_LEVEL_1_MAX_HEALTH);
		cpApp->mcaPlayerShipModel[iIndex].SetShieldStrength(GfPLAYER_LEVEL_1_SHIELD_STRENGTH);
		cpApp->mcaPlayerShipModel[iIndex].SetMaxShieldStrength(GfPLAYER_MAX_SHIELD_STRENGTH);
		cpApp->mcaPlayerShipModel[iIndex].SetArmor(GfPLAYER_LEVEL_1_ARMOR);
		cpApp->mcaPlayerShipModel[iIndex].SetMaxArmor(GfPLAYER_MAX_ARMOR);
		cpApp->mcaPlayerShipModel[iIndex].SetNumberOfLives(GiPLAYER_DEFAULT_NUMBER_OF_LIVES);
		cpApp->mcaPlayerShipModel[iIndex].SetMaxNumberOfLives(GiPLAYER_MAX_NUMBER_OF_LIVES);
		cpApp->mcaPlayerShipModel[iIndex].AddWeapon(cpApp->mcaBulletModel[GiPLAYER_DEFAULT_WEAPON_TYPE]);
		cpApp->mcaPlayerShipModel[iIndex].SetNumberOfPoints(GiPLAYER_DEFAULT_POINTS);
		cpApp->mcaPlayerShipModel[iIndex].SetMaxVelocities(GiPLAYER_LEVEL_1_MAX_X_VELOCITY, GiPLAYER_LEVEL_1_MAX_Y_VELOCITY);
		cpApp->mcaPlayerShipModel[iIndex].SetMinVelocities(-GiPLAYER_LEVEL_1_MAX_X_VELOCITY, -GiPLAYER_LEVEL_1_MAX_Y_VELOCITY);
		cpApp->mcaPlayerShipModel[iIndex].SetMaxAccelerations(GiPLAYER_LEVEL_1_MAX_X_ACCELERATION, GiPLAYER_LEVEL_1_MAX_Y_ACCELERATION);
		cpApp->mcaPlayerShipModel[iIndex].SetMinAccelerations(-GiPLAYER_LEVEL_1_MAX_X_ACCELERATION, -GiPLAYER_LEVEL_1_MAX_Y_ACCELERATION);
		cpApp->mcaPlayerShipModel[iIndex].SetNormalAccelerations(GiPLAYER_LEVEL_1_MAX_X_ACCELERATION, GiPLAYER_LEVEL_1_MAX_Y_ACCELERATION);

		// Create Death Pictures
		cpApp->mcaPlayerShipModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_1, 668, 9, 31, 33);
		cpApp->mcaPlayerShipModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_2, 709, 9, 38, 34);
		cpApp->mcaPlayerShipModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_3, 750, 3, 46, 45);
		cpApp->mcaPlayerShipModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_4, 803, 0, 48, 48);
		cpApp->mcaPlayerShipModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_5, 859, 0, 48, 48);
		cpApp->mcaPlayerShipModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_6, 916, 0, 48, 48);
		cpApp->mcaPlayerShipModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_7, 977, 0, 46, 48);
		cpApp->mcaPlayerShipModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_DONE, 0, 0, 1, 1);

		// Create Player Ship States
		cpApp->mcaPlayerShipModel[iIndex].mcPicture.CreateState(GiSHIP_STATE_IDLE, iaShipIdle, 1, 500);
		cpApp->mcaPlayerShipModel[iIndex].mcPicture.CreateState(GiSHIP_STATE_FORWARD, iaShipForward, 2, 150);
		cpApp->mcaPlayerShipModel[iIndex].mcPicture.CreateState(GiSHIP_STATE_BACKWARDS, iaShipBackwards, 1);
		cpApp->mcaPlayerShipModel[iIndex].mcPicture.CreateState(GiSHIP_STATE_DEATH, iaDeath, 8, 100);
		
		// Set Ships State
		cpApp->mcaPlayerShipModel[iIndex].mcPicture.SetCurrentState(GiSHIP_STATE_BACKWARDS);

		// Set Timers Clock
		cpApp->mcaPlayerShipModel[iIndex].mcChangeWeaponDelayTimer.TimerIsAffectedByClockScaleFactor(true);
		cpApp->mcaPlayerShipModel[iIndex].mcDetonateBulletDelayTimer.TimerIsAffectedByClockScaleFactor(true);
		cpApp->mcaPlayerShipModel[iIndex].mcFireWeaponDelayTimer.TimerIsAffectedByClockScaleFactor(true);
	}

	// Set Specific Player Ship Info
	cpApp->mcaPlayerShipModel[GiPLAYER_1].SetSpecificShipType(GiPLAYER_1);
	cpApp->mcaPlayerShipModel[GiPLAYER_2].SetSpecificShipType(GiPLAYER_2);
		


	//***** Create Enemy Models *****

	// Create Enemy Pictures
	cpApp->mcaEnemyModel[GiENEMY_UFO_OPEN_TOP].mcPicture.CreatePicture(GiENEMY_PICTURE_1, 119, 17, 30, 20);
	cpApp->mcaEnemyModel[GiENEMY_UFO_OPEN_TOP].mcPicture.CreatePicture(GiENEMY_PICTURE_2, 151, 17, 30, 20);

	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].mcPicture.CreatePicture(GiENEMY_PICTURE_1, 215, 0, 28, 15);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].mcPicture.CreatePicture(GiENEMY_PICTURE_2, 119, 0, 28, 15);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].mcPicture.CreatePicture(GiENEMY_PICTURE_3, 151, 0, 28, 15);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].mcPicture.CreatePicture(GiENEMY_PICTURE_4, 183, 0, 28, 15);

	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].mcPicture.CreatePicture(GiENEMY_PICTURE_1, 376, 0, 32, 18);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].mcPicture.CreatePicture(GiENEMY_PICTURE_2, 409, 0, 32, 18);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].mcPicture.CreatePicture(GiENEMY_PICTURE_3, 442, 0, 32, 18);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].mcPicture.CreatePicture(GiENEMY_PICTURE_4, 475, 0, 32, 18);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].mcPicture.CreatePicture(GiENEMY_PICTURE_5, 508, 0, 32, 18);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].mcPicture.CreatePicture(GiENEMY_PICTURE_6, 541, 0, 32, 18);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].mcPicture.CreatePicture(GiENEMY_PICTURE_7, 574, 0, 32, 18);

	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].mcPicture.CreatePicture(GiENEMY_PICTURE_1, 182, 17, 63, 32);

	cpApp->mcaEnemyModel[GiENEMY_BATTLE_SHIP].mcPicture.CreatePicture(GiENEMY_PICTURE_1, 246, 0, 124, 80);

	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].mcPicture.CreatePicture(GiENEMY_PICTURE_1, 166, 91, 100, 51);
	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].mcPicture.CreatePicture(GiENEMY_PICTURE_2, 268, 91, 105, 55);
	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].mcPicture.CreatePicture(GiENEMY_PICTURE_3, 376, 90, 112, 52);
	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].mcPicture.CreatePicture(GiENEMY_PICTURE_4, 491, 91, 100, 51);

	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].mcPicture.CreatePicture(GiENEMY_PICTURE_1, 725, 75, 139, 50);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].mcPicture.CreatePicture(GiENEMY_PICTURE_2, 880, 75, 137, 50);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].mcPicture.CreatePicture(GiENEMY_PICTURE_3, 725, 140, 139, 50);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].mcPicture.CreatePicture(GiENEMY_PICTURE_4, 880, 140, 141, 50);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].mcPicture.CreatePicture(GiENEMY_PICTURE_5, 725, 200, 142, 50);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].mcPicture.CreatePicture(GiENEMY_PICTURE_6, 880, 200, 141, 50);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].mcPicture.CreatePicture(GiENEMY_PICTURE_7, 725, 260, 135, 48);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].mcPicture.CreatePicture(GiENEMY_PICTURE_8, 880, 260, 133, 48);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].mcPicture.CreatePicture(GiENEMY_PICTURE_9, 725, 320, 134, 48);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].mcPicture.CreatePicture(GiENEMY_PICTURE_10, 880, 320, 137, 48);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].mcPicture.CreatePicture(GiENEMY_PICTURE_11, 725, 380, 137, 48);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].mcPicture.CreatePicture(GiENEMY_PICTURE_12, 880, 380, 137, 48);

	cpApp->mcaEnemyModel[GiENEMY_GHOUL].mcPicture.CreatePicture(GiENEMY_PICTURE_1, 165, 150, 121, 144);
	cpApp->mcaEnemyModel[GiENEMY_GHOUL].mcPicture.CreatePicture(GiENEMY_PICTURE_2, 290, 150, 121, 134);
	cpApp->mcaEnemyModel[GiENEMY_GHOUL].mcPicture.CreatePicture(GiENEMY_PICTURE_3, 418, 150, 121, 134);
	cpApp->mcaEnemyModel[GiENEMY_GHOUL].mcPicture.CreatePicture(GiENEMY_PICTURE_4, 546, 150, 121, 144);

	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].mcPicture.CreatePicture(GiENEMY_PICTURE_1, 1, 66, 157, 134);
	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].mcPicture.CreatePicture(GiENEMY_PICTURE_2, 1, 208, 157, 128);
	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].mcPicture.CreatePicture(GiENEMY_PICTURE_3, 1, 343, 157, 134);
	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].mcPicture.CreatePicture(GiENEMY_PICTURE_4, 1, 480, 157, 125);

	cpApp->mcaEnemyModel[GiENEMY_LIDDLE_BUNNY].mcPicture.CreatePicture(GiENEMY_PICTURE_1, 1, 840, 178, 163);
	cpApp->mcaEnemyModel[GiENEMY_LIDDLE_BUNNY].mcPicture.CreatePicture(GiENEMY_PICTURE_2, 181, 840, 178, 173);
	cpApp->mcaEnemyModel[GiENEMY_LIDDLE_BUNNY].mcPicture.CreatePicture(GiENEMY_PICTURE_3, 363, 840, 179, 177);


	// Create Enemy States
	cpApp->mcaEnemyModel[GiENEMY_UFO_OPEN_TOP].mcPicture.CreateState(GiENEMY_STATE_MOVING, iaEnemyPictureRotation2, 2, 150);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].mcPicture.CreateState(GiENEMY_STATE_MOVING, iaEnemyPictureRotation4, 4, 150);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].mcPicture.CreateState(GiENEMY_STATE_MOVING, iaEnemyPictureRotation7, 7, 150);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].mcPicture.CreateState(GiENEMY_STATE_MOVING, iaEnemyPictureRotation1, 1, 150);
	cpApp->mcaEnemyModel[GiENEMY_BATTLE_SHIP].mcPicture.CreateState(GiENEMY_STATE_MOVING, iaEnemyPictureRotation1, 1, 150);
	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].mcPicture.CreateState(GiENEMY_STATE_MOVING, iaEnemySlimyAlienPictureRotation6, 6, 150);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].mcPicture.CreateState(GiENEMY_STATE_MOVING, iaEnemyPictureRotation12, 12, 100);
	cpApp->mcaEnemyModel[GiENEMY_GHOUL].mcPicture.CreateState(GiENEMY_STATE_MOVING, iaEnemyPictureRotation4, 4, 150);
	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].mcPicture.CreateState(GiENEMY_STATE_MOVING, iaEnemyPictureRotation4, 4, 150);
	cpApp->mcaEnemyModel[GiENEMY_LIDDLE_BUNNY].mcPicture.CreateState(GiENEMY_STATE_MOVING, iaEnemyPictureRotation3, 3, 150);


	// Loop through all Enemies
	for (iIndex = 0; iIndex < GiTOTAL_NUMBER_OF_ENEMIES_IN_GAME; iIndex++)
	{
		// Set Enemy Info
		cpApp->mcaEnemyModel[iIndex].SetGeneralShipType(GiENEMY);
		cpApp->mcaEnemyModel[iIndex].SetSpecificShipType(iIndex);
		cpApp->mcaEnemyModel[iIndex].SetHealth(GiENEMY_HEALTH[iIndex]);
		cpApp->mcaEnemyModel[iIndex].SetMaxHealth(GiENEMY_HEALTH[iIndex]);
		cpApp->mcaEnemyModel[iIndex].SetShieldStrength(GfENEMY_SHIELD_STRENGTH[iIndex]);
		cpApp->mcaEnemyModel[iIndex].SetMaxShieldStrength(GfENEMY_SHIELD_STRENGTH[iIndex]);
		cpApp->mcaEnemyModel[iIndex].SetArmor(GfENEMY_ARMOR[iIndex]);
		cpApp->mcaEnemyModel[iIndex].SetMaxArmor(GfENEMY_ARMOR[iIndex]);
		cpApp->mcaEnemyModel[iIndex].SetNumberOfLives(GiENEMY_NUMBER_OF_LIVES[iIndex]);
		cpApp->mcaEnemyModel[iIndex].SetMaxNumberOfLives(GiENEMY_NUMBER_OF_LIVES[iIndex]);
		cpApp->mcaEnemyModel[iIndex].AddWeapon(cpApp->mcaBulletModel[GiENEMY_WEAPON_TYPE[iIndex]]);
		cpApp->mcaEnemyModel[iIndex].SetNumberOfPoints(GiENEMY_POINTS[iIndex]);
		cpApp->mcaEnemyModel[iIndex].SetMaxVelocities(GiENEMY_MAX_X_VELOCITY[iIndex], GiENEMY_MAX_Y_VELOCITY[iIndex]);
		cpApp->mcaEnemyModel[iIndex].SetMinVelocities(-GiENEMY_MAX_X_VELOCITY[iIndex], -GiENEMY_MAX_Y_VELOCITY[iIndex]);
		cpApp->mcaEnemyModel[iIndex].SetNormalAccelerations(GiENEMY_X_ACCELERATION[iIndex], GiENEMY_Y_ACCELERATION[iIndex]);

		// Set Timers Clock
		cpApp->mcaEnemyModel[iIndex].mcChangeWeaponDelayTimer.TimerIsAffectedByClockScaleFactor(true);
		cpApp->mcaEnemyModel[iIndex].mcDetonateBulletDelayTimer.TimerIsAffectedByClockScaleFactor(true);
		cpApp->mcaEnemyModel[iIndex].mcFireWeaponDelayTimer.TimerIsAffectedByClockScaleFactor(true);

		// Create Death Pictures
		cpApp->mcaEnemyModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_1, 668, 9, 31, 33);
		cpApp->mcaEnemyModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_2, 709, 9, 38, 34);
		cpApp->mcaEnemyModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_3, 750, 3, 46, 45);
		cpApp->mcaEnemyModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_4, 803, 0, 48, 48);
		cpApp->mcaEnemyModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_5, 859, 0, 48, 48);
		cpApp->mcaEnemyModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_6, 916, 0, 48, 48);
		cpApp->mcaEnemyModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_7, 977, 0, 46, 48);
		cpApp->mcaEnemyModel[iIndex].mcPicture.CreatePicture(GiSHIP_PICTURE_DEATH_DONE, 0, 0, 1, 1);

		// Create Death State
		cpApp->mcaEnemyModel[iIndex].mcPicture.CreateState(GiENEMY_STATE_DEATH, iaDeath, 8, 100);

		// Set Current State
		cpApp->mcaEnemyModel[iIndex].mcPicture.SetCurrentState(GiENEMY_STATE_MOVING);
	}


	// Set Enemy AI Patterns and give Enemies Items

	cpApp->mcaEnemyModel[GiENEMY_UFO_OPEN_TOP].AddPatternToList(eAvoidObject, eNeverFire);
	cpApp->mcaEnemyModel[GiENEMY_UFO_OPEN_TOP].AddPatternToList(eTrackObject, eFireWhenCloseToObject, 0, 50, 300, 50, 100, GiALWAYS_FIRE);
	cpApp->mcaEnemyModel[GiENEMY_UFO_OPEN_TOP].AddPatternToList(eMoveUp, eFireAsOftenAsPossible);
	cpApp->mcaEnemyModel[GiENEMY_UFO_OPEN_TOP].SetRandomFireTimes(10, 3000);
	cpApp->mcaEnemyModel[GiENEMY_UFO_OPEN_TOP].SetRandomMovementTimes(250, 3000);
	cpApp->mcaEnemyModel[GiENEMY_UFO_OPEN_TOP].SetRandomSwitchPatternTimes(500, 2500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_OPEN_TOP].ChoosePatternsRandomly(false);
	cpApp->mcaEnemyModel[GiENEMY_UFO_OPEN_TOP].StayOnScreenForXMilliseconds(20000);

	cpApp->mcaEnemyModel[GiENEMY_UFO_OPEN_TOP].AddItem(GiITEM_RESTORE_HEALTH_500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_OPEN_TOP].SetChanceOfDroppingItem(10);


	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].AddPatternToList(eMoveDown, eFireAsOftenAsPossible, 500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].AddPatternToList(eMoveDownRight, eFireAsOftenAsPossible, 500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].AddPatternToList(eMoveRight, eNeverFire, 500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].AddPatternToList(eMoveUpRight, eNeverFire, 500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].AddPatternToList(eMoveUp, eNeverFire, 500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].AddPatternToList(eMoveUpLeft, eNeverFire, 500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].AddPatternToList(eMoveLeft, eNeverFire, 500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].AddPatternToList(eMoveDownLeft, eFireAsOftenAsPossible, 500);
//	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].AddPatternToList(eRandomMovement, eRandomFiring);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].SetRandomFireTimes(1000, 10000);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].SetRandomMovementTimes(1000, 5000);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].SetRandomSwitchPatternTimes(500, 3000);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].ChoosePatternsRandomly(false);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].StayOnScreenForXMilliseconds(20000);

	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].AddItem(GiITEM_LASER_WEAPON_75);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CLOSED_TOP].SetChanceOfDroppingItem(3);


	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].AddPatternToList(eMoveDownRight, eRandomFiring, 750);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].AddPatternToList(eMoveLeft, eFireAsOftenAsPossible, 750);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].AddPatternToList(eMoveUpRight, eRandomFiring, 750);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].AddPatternToList(eMoveLeft, eRandomFiring, 750);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].AddPatternToList(eMoveDownLeft, eRandomFiring, 750);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].AddPatternToList(eMoveRight, eFireAsOftenAsPossible, 750);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].AddPatternToList(eMoveUpLeft, eRandomFiring, 750);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].AddPatternToList(eMoveRight, eRandomFiring, 750);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].AddPatternToList(eRandomMovement, eFireAsOftenAsPossible);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].AddPatternToList(eMoveUp, eFireWhenCloseToObject, 1000, GiALWAYS_MOVE, GiALWAYS_MOVE, 0, 100, GiALWAYS_FIRE);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].SetRandomFireTimes(100, 2000);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].SetRandomMovementTimes(250, 3000);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].SetRandomSwitchPatternTimes(500, 2500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].ChoosePatternsRandomly(false);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].StayOnScreenForXMilliseconds(12500);

	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].AddItem(GiITEM_RESTORE_HEALTH_100);
	cpApp->mcaEnemyModel[GiENEMY_UFO_SMALL].SetChanceOfDroppingItem(4);


	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].AddPatternToList(eMoveDown, eFireAsOftenAsPossible);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].AddPatternToList(eMoveUpRight, eFireAsOftenAsPossible);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].AddPatternToList(eMoveDown, eFireAsOftenAsPossible);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].AddPatternToList(eMoveUpLeft, eFireAsOftenAsPossible);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].AddPatternToList(eMoveDown, eFireAsOftenAsPossible);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].AddPatternToList(eRandomMovement, eFireAsOftenAsPossible);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].AddPatternToList(eMoveUp, eFireAsOftenAsPossible, 300);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].AddPatternToList(eRandomMovement, eFireAsOftenAsPossible);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].AddPatternToList(eRandomMovement, eFireAsOftenAsPossible);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].AddPatternToList(eMoveUp, eFireAsOftenAsPossible, 500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].SetRandomFireTimes(100, 2000);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].SetRandomMovementTimes(250, 2000);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].SetRandomSwitchPatternTimes(500, 1500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].ChoosePatternsRandomly(false);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].StayOnScreenForXMilliseconds(25000);

	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].AddItem(GiITEM_RESTORE_HEALTH_1000);
	cpApp->mcaEnemyModel[GiENEMY_UFO_CURVED_WINGS].SetChanceOfDroppingItem(10);


	cpApp->mcaEnemyModel[GiENEMY_BATTLE_SHIP].AddPatternToList(eRandomMovement, eRandomFiring);
	cpApp->mcaEnemyModel[GiENEMY_BATTLE_SHIP].AddPatternToList(eRandomMovement, eRandomFiring, 200);
	cpApp->mcaEnemyModel[GiENEMY_BATTLE_SHIP].AddPatternToList(eStopAccelerating, eRandomFiring, 1000);
	cpApp->mcaEnemyModel[GiENEMY_BATTLE_SHIP].AddPatternToList(eMoveUp, eRandomFiring, 750);
	cpApp->mcaEnemyModel[GiENEMY_BATTLE_SHIP].AddPatternToList(eTrackObjectHorizontally, eFireAsOftenAsPossible, 2000, GiALWAYS_MOVE, 400, 100);
	cpApp->mcaEnemyModel[GiENEMY_BATTLE_SHIP].SetRandomFireTimes(100, 2000);
	cpApp->mcaEnemyModel[GiENEMY_BATTLE_SHIP].SetRandomMovementTimes(250, 1500);
	cpApp->mcaEnemyModel[GiENEMY_BATTLE_SHIP].SetRandomSwitchPatternTimes(500, 1500);
	cpApp->mcaEnemyModel[GiENEMY_BATTLE_SHIP].ChoosePatternsRandomly(false);
	cpApp->mcaEnemyModel[GiENEMY_BATTLE_SHIP].StayOnScreenForXMilliseconds(20000);

	cpApp->mcaEnemyModel[GiENEMY_BATTLE_SHIP].AddItem(GiITEM_FIREBALL_WEAPON_50);
	cpApp->mcaEnemyModel[GiENEMY_BATTLE_SHIP].SetChanceOfDroppingItem(4);


	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].AddPatternToList(eStopMovement, eNeverFire);
	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].AddPatternToList(eAvoidObject, eRandomFiring);
	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].AddPatternToList(eTrackObject, eRandomFiring, 500);
	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].AddPatternToList(eMoveUpRight, eFireAsOftenAsPossible);
	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].AddPatternToList(eMoveLeft, eRandomFiring, 750);
	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].SetRandomFireTimes(100, 1500);
	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].SetRandomMovementTimes(250, 1500);
	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].SetRandomSwitchPatternTimes(500, 1500);
	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].ChoosePatternsRandomly(false);
	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].StayOnScreenForXMilliseconds(20000);

	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].AddItem(GiITEM_BOMB_WEAPON_15);
	cpApp->mcaEnemyModel[GiENEMY_SLIMY_ALIEN].SetChanceOfDroppingItem(5);


	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].AddPatternToList(eRandomMovement, eRandomFiring);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].AddPatternToList(eRandomMovement, eRandomFiring);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].AddPatternToList(eRandomMovement, eRandomFiring);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].AddPatternToList(eMoveUp, eRandomFiring, 500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].SetRandomFireTimes(100, 1500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].SetRandomMovementTimes(250, 1500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].SetRandomSwitchPatternTimes(500, 1500);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].ChoosePatternsRandomly(false);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].StayOnScreenForXMilliseconds(20000);

	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].AddItem(GiITEM_FIREBALL_WEAPON_50);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].AddItem(GiITEM_RESTORE_HEALTH_FULL);
	cpApp->mcaEnemyModel[GiENEMY_UFO_LARGE_SAUCER].SetChanceOfDroppingItem(7);


	cpApp->mcaEnemyModel[GiENEMY_GHOUL].AddPatternToList(eStopMovement, eNeverFire, 1000);
	cpApp->mcaEnemyModel[GiENEMY_GHOUL].AddPatternToList(eTrackObjectHorizontally, eFireAsOftenAsPossible, 3000, GiALWAYS_MOVE, GiALWAYS_MOVE, 50);
	cpApp->mcaEnemyModel[GiENEMY_GHOUL].AddPatternToList(eRandomMovement, eRandomFiring, 1000);
	cpApp->mcaEnemyModel[GiENEMY_GHOUL].SetRandomFireTimes(100, 1500);
	cpApp->mcaEnemyModel[GiENEMY_GHOUL].SetRandomMovementTimes(250, 1500);
	cpApp->mcaEnemyModel[GiENEMY_GHOUL].SetRandomSwitchPatternTimes(500, 1500);
	cpApp->mcaEnemyModel[GiENEMY_GHOUL].ChoosePatternsRandomly(false);
	cpApp->mcaEnemyModel[GiENEMY_GHOUL].StayOnScreenForXMilliseconds(0);

	cpApp->mcaEnemyModel[GiENEMY_GHOUL].AddItem(GiITEM_FREE_SHIP);
	cpApp->mcaEnemyModel[GiENEMY_GHOUL].SetChanceOfDroppingItem(1);


	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].AddPatternToList(eRandomMovement, eRandomFiring, 800);
	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].AddPatternToList(eMoveUp, eRandomFiring, 500);
	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].AddPatternToList(eRandomMovement, eRandomFiring);
	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].AddPatternToList(eRandomMovement, eRandomFiring);
	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].SetRandomFireTimes(100, 1000);
	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].SetRandomMovementTimes(500, 2500);
	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].SetRandomSwitchPatternTimes(500, 3000);
	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].ChoosePatternsRandomly(false);
	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].StayOnScreenForXMilliseconds(0);
	
	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].AddItem(GiITEM_FREE_SHIP);
	cpApp->mcaEnemyModel[GiENEMY_BLUE_HEAD].SetChanceOfDroppingItem(1);


	cpApp->mcaEnemyModel[GiENEMY_LIDDLE_BUNNY].AddPatternToList(eTrackObjectHorizontally, eFireAsOftenAsPossible, 2000, GiALWAYS_MOVE, GiALWAYS_MOVE, 25);
	cpApp->mcaEnemyModel[GiENEMY_LIDDLE_BUNNY].AddPatternToList(eRandomMovement, eFireAsOftenAsPossible, 1000);
	cpApp->mcaEnemyModel[GiENEMY_LIDDLE_BUNNY].AddPatternToList(eMoveUp, eFireAsOftenAsPossible, 750);
	cpApp->mcaEnemyModel[GiENEMY_LIDDLE_BUNNY].SetRandomFireTimes(200, 2000);
	cpApp->mcaEnemyModel[GiENEMY_LIDDLE_BUNNY].SetRandomMovementTimes(500, 2500);
	cpApp->mcaEnemyModel[GiENEMY_LIDDLE_BUNNY].SetRandomSwitchPatternTimes(500, 3000);
	cpApp->mcaEnemyModel[GiENEMY_LIDDLE_BUNNY].ChoosePatternsRandomly(false);
	cpApp->mcaEnemyModel[GiENEMY_LIDDLE_BUNNY].StayOnScreenForXMilliseconds(0);
	


	//***** Create Space Station Model *****

	// Create Space Station Pictures
	cpApp->mcSpaceStationModel.CreatePicture(GiSPACE_STATION_PICTURE_REVOLUTION_1, 1, 1, 181, 167);
	cpApp->mcSpaceStationModel.CreatePicture(GiSPACE_STATION_PICTURE_REVOLUTION_2, 186, 1, 181, 167);
	cpApp->mcSpaceStationModel.CreatePicture(GiSPACE_STATION_PICTURE_REVOLUTION_3, 373, 1, 181, 167);
	cpApp->mcSpaceStationModel.CreatePicture(GiSPACE_STATION_PICTURE_REVOLUTION_4, 561, 1, 181, 167);
	cpApp->mcSpaceStationModel.CreatePicture(GiSPACE_STATION_PICTURE_REVOLUTION_5, 750, 1, 181, 167);
	cpApp->mcSpaceStationModel.CreatePicture(GiSPACE_STATION_PICTURE_REVOLUTION_6, 1, 182, 181, 167);

	// Create Space Station States
	cpApp->mcSpaceStationModel.CreateState(GiSPACE_STATION_STATE_REVOLVING, iaSpaceStationRevolving, 6, 85);

	// Set Current Space Station State
	cpApp->mcSpaceStationModel.SetCurrentState(GiSPACE_STATION_STATE_REVOLVING);
}

// Returns float value of how far an object should travel, given it's Velocity (pixels per second) and Time (milliseconds)
float CApp::GetFloatDistanceTravelled(float fVelocity, unsigned long ulTime)
{
	// Convert Time from Milliseconds to Seconds
	float fTimeInSeconds = ulTime / 1000.0;

	// Return the distance travelled
	// Distance = Velocity * Time
	return (fVelocity * fTimeInSeconds);
}

// Returns int value of how far an object should travel, given it's Velocity (pixels per second) and Time (milliseconds)
int CApp::GetIntDistanceTravelled(float fVelocity, unsigned long ulTime)
{
	// Convert Time from Milliseconds to Seconds
	float fTimeInSeconds = ulTime / 1000.0;

	// Return the distance travelled
	return (int)floorf((fVelocity * fTimeInSeconds));
}

// Plays a Sound
// Loop = -1 means loop forever, 0 means just play once, 1 means loop once, etc
void CApp::PlaySoundEffect(int iSoundToPlay, float fVolumePercent, int iLoopCount)
{
/*	static int SiSoundChannelToUse	= 0;	// Holds which Sound Channel To Use to Play the Sound
	int iStartingSoundChannel = SiSoundChannelToUse;	// Used to tell if all Sound Channels have been checked

	// Do While current Sound Channel is in use and we have not checked all Sound Channels
	do
	{
		// Increment to next Sound Channel and check if it's valid
		if (++SiSoundChannelToUse == GiNUMBER_OF_SOUND_CHANNELS)
		{
			// Wrap around to first Sound Channel
			SiSoundChannelToUse = 0;
		}
	} while (mcSoundChannel[SiSoundChannelToUse].IsPlaying() && 
			 iStartingSoundChannel != SiSoundChannelToUse);

	// If all Sound Channels are not already in use
	if (!mcSoundChannel[SiSoundChannelToUse].IsPlaying())
	{
		// Play the Sound
		mcSoundChannel[SiSoundChannelToUse].Play(cpSoundToPlay, lVolumePercent, lLoop);

		// Return which Sound Channel was used
		return SiSoundChannelToUse;
	}
*/
	FMOD_RESULT Result;

	// Load the sound, but Paused
	Result = mcpSoundSystem->playSound(FMOD_CHANNEL_FREE, mcpSound[iSoundToPlay], true, &mcpSoundChannel);
	ERRCHECK(Result);

	// Set the number of times to loop
	mcpSound[iSoundToPlay]->setLoopCount(iLoopCount);

	// Set the Volume
	mcpSoundChannel->setVolume(fVolumePercent / 100.0);

	// Unpause it
	mcpSoundChannel->setPaused(false);
}

// Stops current music and Plays specified Music
// Loop = -1 means loop forever, 0 means just play once, 1 means loop once, etc
void CApp::PlayMusic(int iMusicToPlay, float fVolumePercent, int iLoopCount)
{
/*	// Stop current Music if playing and Free the resource
	mcMusicChannel.Stop();

	// Load new Music
	mcMusicChannel.Load(cpMusicToPlay);

	// Start Playing Music
	return mcMusicChannel.Play(lVolumePercent, lLoop);
*/
	FMOD_RESULT Result;

	// Stop the currently playing Music
	mcpMusicChannel->stop();

	// Load the Music, but Paused so we can set Volume, etc
	Result = mcpSoundSystem->playSound(FMOD_CHANNEL_FREE, mcpMusic[iMusicToPlay], true, &mcpMusicChannel);
    ERRCHECK(Result);

	// Set the number of times to loop
	mcpMusic[iMusicToPlay]->setLoopCount(iLoopCount);

	// Set the volume
	mcpMusicChannel->setVolume(fVolumePercent / 100.0);

	// Unpause the Music
	mcpMusicChannel->setPaused(false);
}

// Stops currently playing Music
void CApp::StopMusic()
{
	// Stop the currently playing Music
	mcpMusicChannel->stop();
}



