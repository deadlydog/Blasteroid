// CTimer.cpp

#include "CTimer.h"


//********** CClock Functions **********

// Initialize the Instance pointer to NULL
CClock* CClock::mScpInstance		= 0;
unsigned long CClock::mSulSavedTime	= 0;

// Constructor - Does nothing
CClock::CClock()
{}

// Destructor - Does nothing
CClock::~CClock()
{}

// Returns a pointer to the class's Instance
CClock* CClock::ReturnClassInstance()
{
	// If class has been Instanced already
	if (mScpInstance != 0)
	{
		// Return pointer to class Instance
		return mScpInstance;
	}
	
	// Else class has not been Instanced yet so Instance it and return it
	mScpInstance = new CClock();
	return mScpInstance;
}

// Stores the Current Time and returns the time elapsed between last Saved Time and new one
unsigned long CClock::SaveCurrentTimeAndReturnElapsedTime()
{
	// Get old Saved Time
	unsigned long ulOldSavedTime = ReturnSavedTime();

	// Get and Save new Saved Time
	mSulSavedTime = ReturnCurrentTime();

	// Return how much time had elapsed between old Saved Time and new one
	return (mSulSavedTime - ulOldSavedTime);
}

// Returns the Saved Time
unsigned long CClock::ReturnSavedTime() const
{
	return mSulSavedTime;
}

// Returns the Current system Time in milliseconds (1000ms = 1s)
unsigned long CClock::ReturnCurrentTime() const
{
	return timeGetTime();
}



//********** CTimer Functions **********

// Constructor
CTimer::CTimer()
{
	// Initialize private data to NULL
	mcpClock = 0;
	mulStartTime = 0;
}

// Specifies what CTime to use and starts Timer with Current Time
bool CTimer::SetClock(CClock* cpClock)
{
	// If Clock pointer is not valid
	if (cpClock == 0)
	{
		// Return failure
		return false;
	}

	// Save the Clock Instance Pointer
	mcpClock = cpClock;

	// Start the Timer
	StartTimerUsingCurrentTime();

	// Return success
	return true;
}

// Sets the Start Time to the Current Time
// Returns false if Clock not set yet
bool CTimer::StartTimerUsingCurrentTime()
{
	// If the Clock is not set yet
	if (mcpClock == 0)
	{
		return false;
	}

	// Else Clock is set so Start Timer
	mulStartTime = ReturnCurrentTime();
	return true;
}

// Sets the Start Time to the Clocks Saved Time
// Returns false if Clock not set yet
bool CTimer::StartTimerUsingSavedTime()
{
	// If the Clock is not set yet
	if (mcpClock == 0)
	{
		return false;
	}

	// Else Clock is set so Start Timer
	mulStartTime = ReturnSavedTime();
	return true;
}

// Returns True if ulTime has passed (Current Time - Start Time), False if not
// NOTE: Restarts Timer if True
bool CTimer::XTimeHasPassedUsingCurrentTime(unsigned long ulTimeInMilliseconds)
{
	// If the Clock is not set yet
	if (mcpClock == 0)
	{
		return false;
	}	

	// If the given amount of time has elapsed
	if (XTimeHasPassedUsingCurrentTimeWithoutRestartingTimer(ulTimeInMilliseconds))
	{
		StartTimerUsingCurrentTime();	// Restart the Timer

		// Return that ulTimeInMilliseconds has elapsed
		return true;
	}

	// Return that ulTimeInMilliseconds has not yet elapsed
	return false;
}

// Returns True if ulTime has passed (Clocks Saved Time - Start Time), False if not
// NOTE: Restarts Timer if True
bool CTimer::XTimeHasPassedUsingSavedTime(unsigned long ulTimeInMilliseconds)
{
	// If the Clock is not set yet
	if (mcpClock == 0)
	{
		return false;
	}

	// If the given amount of time has elapsed
	if (XTimeHasPassedUsingSavedTimeWithoutRestartingTimer(ulTimeInMilliseconds))
	{
		StartTimerUsingSavedTime();	// Restart the Timer

		// Return that ulTimeInMilliseconds has elapsed
		return true;
	}

	// Return that ulTimeInMilliseconds has not yet elapsed
	return false;
}

// Returns True if ulTime has passed (Current Time - Start Time), False if not
// NOTE: Does not restart Timer
bool CTimer::XTimeHasPassedUsingCurrentTimeWithoutRestartingTimer(unsigned long ulTimeInMilliseconds) const
{
	// If the Clock is not set yet
	if (mcpClock == 0)
	{
		return false;
	}

	// If the given amount of time has not elapsed yet
	if ((ReturnCurrentTime() - ReturnStartTime()) < ulTimeInMilliseconds)
	{
		return false;	// Return that ulTimeInMilliseconds has not yet elapsed
	}

	return true;		// Return that ulTimeInMilliseconds has elapsed
}

// Returns True if ulTime has passed (Saved Time - Start Time), False if not
// NOTE: Does not restart Timer
bool CTimer::XTimeHasPassedUsingSavedTimeWithoutRestartingTimer(unsigned long ulTimeInMilliseconds) const
{
	// If the Clock is not set yet
	if (mcpClock == 0)
	{
		return false;
	}

	// If the given amount of time has not elapsed yet
	if ((ReturnSavedTime() - ReturnStartTime()) < ulTimeInMilliseconds)
	{
		return false;	// Return that ulTimeInMilliseconds has not yet elapsed
	}

	return true;		// Return that ulTimeInMilliseconds has elapsed
}

// Returns how much time has passed since last check using the Current Time
// Returns 0 if Clock not set yet
unsigned long CTimer::ReturnTimePassedSinceLastCheckUsingCurrentTime() const
{
	// If the Clock is not set yet
	if (mcpClock == 0)
	{
		return 0;
	}

	// Return the Time Passed
	return (ReturnCurrentTime() - ReturnStartTime());
}

// Returns how much time has passed since last check using the Clocks Saved Time
// Returns 0 if Clock not set yet
unsigned long CTimer::ReturnTimePassedSinceLastCheckUsingSavedTime() const
{
	// If the Clock is not set yet
	if (mcpClock == 0)
	{
		return 0;
	}

	// Return the Time Passed
	return (ReturnSavedTime() - ReturnStartTime());
}

// Returns how much time has passed since last check using the Current Time
// Returns 0 if Clock not set yet
// NOTE: Also restarts the Timer
unsigned long CTimer::ReturnTimePassedSinceLastCheckUsingCurrentTimeAndRestartTimer()
{
	unsigned long ulTimePassed = 0;

	// If the Clock is not set yet
	if (mcpClock == 0)
	{
		return 0;
	}

	// Get the Time Passed
	ulTimePassed = ReturnTimePassedSinceLastCheckUsingCurrentTime();

	// Restart the Timer
	StartTimerUsingCurrentTime();

	// Return how much Time has Passed since last check
	return ulTimePassed;
}

// Returns how much time has passed since last check using the Clocks Saved Time
// Returns 0 if Clock not set yet
// NOTE: Also restarts the Timer
unsigned long CTimer::ReturnTimePassedSinceLastCheckUsingSavedTimeAndRestartTimer()
{
	unsigned long ulTimePassed = 0;

	// If the Clock is not set yet
	if (mcpClock == 0)
	{
		return 0;
	}

	// Get the Time Passed
	ulTimePassed = ReturnTimePassedSinceLastCheckUsingSavedTime();

	// Restart the Timer
	StartTimerUsingSavedTime();

	// Return how much Time has Passed since last check
	return ulTimePassed;
}

// Returns the time the Timer was started
// Returns 0 if Clock not set yet
unsigned long CTimer::ReturnStartTime() const
{
	// If the Clock is not set yet
	if (mcpClock == 0)
	{
		return 0;
	}

	return mulStartTime;
}

// Returns the Clocks Saved Time
// Returns 0 if Clock not set yet
unsigned long CTimer::ReturnSavedTime() const
{
	// If the Clock is not set yet
	if (mcpClock == 0)
	{
		return 0;
	}

	return mcpClock->ReturnSavedTime();
}

// Returns the Current Time
// Returns 0 if Clock not set yet
unsigned long CTimer::ReturnCurrentTime() const
{
	// If the Clock is not set yet
	if (mcpClock == 0)
	{
		return 0;
	}

	return mcpClock->ReturnCurrentTime();
}

// Returns a pointer to the Clock
CClock* CTimer::ReturnClock() const
{
	return mcpClock;
}

