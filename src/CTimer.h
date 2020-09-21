/*****************************************************************************/
/* CTimer.h (and CTimer.cpp)                                                 */
/* Written by Daniel Schroeder                                               */
/* Created in 2004                                                           */
/* Last Updated November 11, 2006                                            */
/*---------------------------------------------------------------------------*/
/*   The CClock class is a singleton class so only one object of it can be   */
/* created, and this is done auotmatically. It is used to get the current    */
/* system time for the Timers. It can also Save and Return a specific Time   */
/* (I will use this feature to save what Time the current frame is occuring  */
/* at).                                                                      */
/*   Example of instancing the CClock class if needed:                       */
/*     CClock* cpClock = CClock::ReturnClassInstance();                      */
/*     cpClock->ReturnCurrentTime();                                         */
/*                                                                           */
/*   The CTimer class is used as a Timer to keep track of how much time in   */
/* milliseconds has passed. The Timers automatically reference the CClock    */
/* instance, so the CClock class can be completely ignored if wanted. Also,  */
/* can use either Current Time or Saved Time when doing calculations.		 */
/* NOTE: Can only use Saved Time functions if Clock is being Reset           */
/*		 periodically, such as every loop of a game							 */
/* Saved Time is recommended for many cases so that all Timers in the game   */
/* loop will calculate the same elapsed time between frames (i.e. so that    */
/* Timers at the start of the game loop will return the same amount of time  */
/* as the ones at the end of the game loop). This is because the Timers will */
/* calculate their elapsed time from their Start Time to the Clocks Saved    */
/* Time (which will be the same for all Timers until the Clock is reset      */
/* again), instead of from their Start Time to the Current Time, which will  */
/* be different for every timer.											 */
/*   NOTE: 1000 milliseconds = 1 second                                      */
/*   -NOTE: Must have winmm.lib included in project to work-                 */
/*****************************************************************************/

#ifndef CTIMER_H
#define CTIMER_H

#include <windows.h>	// Needed for timeGetTime() function
// NOTE: Must also include winmm.lib in project for timeGetTime() to work


// Singleton class (only one object can be created) to get the system time
// All Timers will go by this one single Clock
class CClock
{
	public:
	
		~CClock();		// Destructor

		// Returns a pointer to the class's Instance
		static CClock* ReturnClassInstance();

		// Returns the actual amount of Time Elapsed since the last Reset, ignoring the Scale Factor
		unsigned long ReturnActualElapsedTimeAndResetClock();
		unsigned long ReturnActualElapsedTimeWithoutResettingClock();

		// Returns the Time Elapsed * Scale Factor
		unsigned long ReturnElapsedTimeAndResetClock();
		unsigned long ReturnElapsedTimeWithoutResettingClock();

		// Set and Return Scale Factor for Clocks Modified Elapsed Time
		// NOTE: ScaleFactor = 0.5 returns half of elapsed time, ScaleFactor = 2 returns 2 times elapsed time
		//		 ScaleFactor = 1.0 returns actual amount of elapsed time, ScaleFactor = 0 pauses Timer
		// NOTE: ulDuration = 0 means to use the given Scale Factor until explicitly changed again later
		// NOTE: After ulDuration expires, the Scale Factor will be set back to 1.0 automatically
		void SetScaleFactor(float fScaleFactor, unsigned long ulDuration = 0);
		float ReturnScaleFactor() const;

		unsigned long ReturnSavedTime();	// Returns the Saved Time
		unsigned long ReturnCurrentTime();	// Returns the Current Time

	private:

		CClock();			// Constructor (Private because this is a Singleton class)
		void ResetClock();	// Resets the Clock

		static CClock* mcpInstance;				// Holds pointer to Class Instance
		static unsigned long mulSavedTime;		// Holds what Time it was last time we checked

		static float mfScaleFactor;				// Holds how much to Scale the Clocks returned elapsed time
		static unsigned long mulScaleFactorDuration;			// Holds how long to use the Scale Factor
		static unsigned long mulScaleFactorDurationStartTime;	// Holds what time the Scale Factor started

		void RestoreScaleFactorIfExpired();		// Restores the Scale Factor to 1.0 if it should no longer be used
};

// Class to be used as a Timer to keep track of how much Time has elapsed
class CTimer
{
	public:

		CTimer(bool bEffectedByClockScaleFactor = true);	// Constructor

		void RestartTimerUsingCurrentTime();	// Resets the Start Time to the Current Time
		void RestartTimerUsingSavedTime();		// Resets the Start Time to the Clocks Saved Time

		// Returns True if ulTime has passed (Saved/Current Time - Start Time), False if not
		// NOTE: Restarts Timer if True
		bool XTimeHasPassedUsingCurrentTime(unsigned long ulTimeInMilliseconds);
		bool XTimeHasPassedUsingSavedTime(unsigned long ulTimeInMilliseconds);

		// Returns True if ulTime has passed (Saved/Current Time - Start Time), False if not
		// NOTE: Does not restart Timer
		bool XTimeHasPassedUsingCurrentTimeWithoutRestartingTimer(unsigned long ulTimeInMilliseconds);
		bool XTimeHasPassedUsingSavedTimeWithoutRestartingTimer(unsigned long ulTimeInMilliseconds);
		
		// Returns how much time has passed since last check
		unsigned long ReturnTimePassedSinceLastCheckUsingCurrentTime();
		unsigned long ReturnTimePassedSinceLastCheckUsingSavedTime();

		// Returns how much time has passed since last check
		// NOTE: Also restarts the Timer
		unsigned long ReturnTimePassedSinceLastCheckUsingCurrentTimeAndRestartTimer();
		unsigned long ReturnTimePassedSinceLastCheckUsingSavedTimeAndRestartTimer();

		// Set and Return if this Timer is effected by the Clocks Scale Factor or not
		void TimerIsAffectedByClockScaleFactor(bool bTrueOrFalse = true);
		bool ReturnIfTimerIsAffectedByClockScaleFactor() const;

		// Set and Return this Timers individual Scale Factor
		// NOTE: ScaleFactor = 0.5 returns half of elapsed time, ScaleFactor = 2 returns 2 times elapsed time
		//		 ScaleFactor = 1.0 returns actual amount of elapsed time, ScaleFactor = 0 pauses Timer
		// NOTE: ulDuration = 0 means to use the given Scale Factor until explicitly changed again later
		// NOTE: After ulDuration expires, the Scale Factor will be set back to 1.0 automatically
		void SetTimerScaleFactor(float fScaleFactor, unsigned long ulDuration = 0);
		float ReturnTimerScaleFactor() const;

		unsigned long ReturnStartTime() const;	// Returns the time the Timer was started
		unsigned long ReturnSavedTime();		// Returns the Clocks Saved Time
		unsigned long ReturnCurrentTime();		// Returns the Current Time

	private:

		CClock* mcpClock;					// Points to the Time object used to actually get the system time
		unsigned long mulStartTime;			// Holds the time the Timer was started
		
		bool mbEffectedByClockScaleFactor;	// Holds if this Timer is effected by Clock slow downs or speed ups
		float mfTimerScaleFactor;			// Holds this Timers Scale Factor

		unsigned long mulTimerScaleDuration;// Holds how long to use unusual Scale Factor for
		unsigned long mulTimerScaleDurationStartTime;	// Holds when the Timer Scale Factor was set

		void RestoreTimerScaleFactorIfExpired(); // Restores the Scale Factor to 1.0 if it should no longer be used
};


#endif

