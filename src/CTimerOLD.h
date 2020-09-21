/*****************************************************************************/
/* CTimer.h (and CTimer.cpp)                                                 */
/* Written by Daniel Schroeder                                               */
/* Created in 2004                                                           */
/* Last Updated August 19, 2005                                              */
/*---------------------------------------------------------------------------*/
/*   The CClock class is a singleton class so only one object of it can be   */
/* created, and this is done auotmatically. It is used to get the current    */
/* system time for the Timers. It can also Save and Return a specific Time   */
/* (I will use this feature to save what Time the current frame is occuring  */
/* at).                                                                      */
/*   Example of using the CClock class:                                      */
/*     CClock* cpClock = CClock::ReturnClassInstance();                      */
/*     cpClock->ReturnCurrentTime();                                         */
/*                                                                           */
/*   The CTimer class is used as a Timer to keep track of how much time in   */
/* milliseconds has passed. You must pass it the Instance to CClock before   */
/* using it.                                                                 */
/*   NOTE: 1000 milliseconds = 1 second                                      */
/*   NOTE: Must have winmm.lib included in project to work-                  */
/*****************************************************************************/

#ifndef CTIMER_H
#define CTIMER_H

#include <windows.h>	// Needed for timeGetTime() function


// Singleton class (only one object can be created) to get the system time
// All Timers will go by this one single Clock
class CClock
{
	public:
	
		~CClock();		// Destructor

		// Returns a pointer to the class's Instance
		static CClock* ReturnClassInstance();

		// Stores the Current Time and returns the time elapsed between last Saved Time and new one
		unsigned long SaveCurrentTimeAndReturnElapsedTime();
		unsigned long ReturnSavedTime() const;	// Returns the Saved Time
		
		unsigned long ReturnCurrentTime() const;// Returns the Current Time

	private:

		CClock();		// Constructor (Private because this is a Singleton class)
		
		static CClock* mScpInstance;		// Holds pointer to Class Instance
		static unsigned long mSulSavedTime;	// Holds what Time it was last time we checked
};


// Class to be used as a Timer to keep track of how much Time has elapsed
class CTimer
{
	public:

		CTimer();		// Constructor

		// Specifies what CClock to use and starts Timer with Current Time
		// NOTE: This function must be called first before any others
		bool SetClock(CClock* cpClock);

		// NOTE: All functions return false/0 if Clock has not been set yet

		bool StartTimerUsingCurrentTime();	// Sets the Start Time to the Current Time
		bool StartTimerUsingSavedTime();	// Sets the Start Time to the Clocks Saved Time

		// Returns True if ulTime has passed (Saved/Current Time - Start Time), False if not
		// NOTE: Restarts Timer if True
		bool XTimeHasPassedUsingCurrentTime(unsigned long ulTimeInMilliseconds);
		bool XTimeHasPassedUsingSavedTime(unsigned long ulTimeInMilliseconds);

		// Returns True if ulTime has passed (Saved/Current Time - Start Time), False if not
		// NOTE: Does not restart Timer
		bool XTimeHasPassedUsingCurrentTimeWithoutRestartingTimer(unsigned long ulTimeInMilliseconds) const;
		bool XTimeHasPassedUsingSavedTimeWithoutRestartingTimer(unsigned long ulTimeInMilliseconds) const;
		
		// Returns how much time has passed since last check
		unsigned long ReturnTimePassedSinceLastCheckUsingCurrentTime() const;
		unsigned long ReturnTimePassedSinceLastCheckUsingSavedTime() const;

		// Returns how much time has passed since last check
		// NOTE: Also restarts the Timer
		unsigned long ReturnTimePassedSinceLastCheckUsingCurrentTimeAndRestartTimer();
		unsigned long ReturnTimePassedSinceLastCheckUsingSavedTimeAndRestartTimer();

		unsigned long ReturnStartTime() const;		// Returns the time the Timer was started
		unsigned long ReturnSavedTime() const;		// Returns the Clocks Saved Time
		unsigned long ReturnCurrentTime() const;	// Returns the Current Time

		CClock* ReturnClock() const;	// Returns a pointer to the Clock

	private:

		unsigned long mulStartTime;		// Holds the time the Timer was started
		CClock* mcpClock;				// Points to the Time object used to actually get the system time
};


#endif

