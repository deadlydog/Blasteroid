// CObjectPrototypeList.cpp

#include "CObjectPrototypeList.h"

// Constructor
CObjectPrototypeList::CObjectPrototypeList()
{
	// Initialize private data
	mspHead = 0;
	mspTail = 0;
	mspCurrentObjectPrototype = 0;
	miIDCounter = 0;
}

// Destructor
CObjectPrototypeList::~CObjectPrototypeList()
{
	Purge();
}

// Add a Object Prototype to the List
// Returns Object Prototypes unique ID
// Returns zero if a problem occured
// If using Velocities you can test Objects XYZ coordinates to see if they should be onscreen yet
// If not, you can test to see if TimeDurationUntilAppearing is zero yet for Object to appear onscreen
int CObjectPrototypeList::AddObjectPrototype(int iObjectType, unsigned long ulTimeDurationUntilAppearing, 
										 float fX, float fY, float fZ, float fXVelocity, 
										 float fYVelocity, float fZVelocity)
{
	SObjectPrototype* spPrototype = 0;						// Used to traverse List
	SObjectPrototype* spPreviousPrototype = 0;				// Used when traversing List
	SObjectPrototype* spNewPrototype = new SObjectPrototype;	// Pointer to new Object Prototype

	// Make sure ID Counter is not zero
	if (++miIDCounter == 0)
	{
		// Increment it one more time
		++miIDCounter;
	}

	// Fill in Object Prototypes Data
	spNewPrototype->iType							= iObjectType;
	spNewPrototype->fX								= fX;
	spNewPrototype->fY								= fY;
	spNewPrototype->fZ								= fZ;
	spNewPrototype->fXVelocity						= fXVelocity;
	spNewPrototype->fYVelocity						= fYVelocity;
	spNewPrototype->fZVelocity						= fZVelocity;
	spNewPrototype->ulTimeDurationUntilAppearing	= ulTimeDurationUntilAppearing;
	spNewPrototype->iPrototypeID					= miIDCounter;

	// Add Prototype to List in order of Time (smallest Time at start, largest Time at end)
	
	// If this is the first Prototype to add to the List
	if (mspHead == 0)
	{
		mspHead = spNewPrototype;
		mspTail = spNewPrototype;
		spNewPrototype->spNext = 0;
	}
	// Else this is being added to the current List
	else
	{
		// If the New Prototype should be added at the front of the List
		if (spNewPrototype->ulTimeDurationUntilAppearing <= mspHead->ulTimeDurationUntilAppearing)
		{
			spNewPrototype->spNext = mspHead;
			mspHead = spNewPrototype;
		}
		// Else If the New Prototype should be added to the end of the List
		else if (spNewPrototype->ulTimeDurationUntilAppearing >= mspTail->ulTimeDurationUntilAppearing)
		{
			mspTail->spNext = spNewPrototype;
			mspTail = spNewPrototype;
			spNewPrototype->spNext = 0;
		}
		// Else it needs to be added somewhere in the middle
		else
		{
			spPrototype = mspHead;		// Point to start of List
			spPreviousPrototype = 0;

			// Loop through list to find where New Prototype should go
			while (spPrototype != 0 && (spPrototype->ulTimeDurationUntilAppearing < spNewPrototype->ulTimeDurationUntilAppearing))
			{
				spPreviousPrototype = spPrototype;
				spPrototype = spPrototype->spNext;
			}

			// Make sure nothing went wrong (since New Prototype should fit in the list somewhere in the middle)
			if (spPrototype == 0 || spPreviousPrototype == 0)
			{
				delete spNewPrototype;
				return 0;
			}

			// Add the New Prototype into the List
			spNewPrototype->spNext = spPrototype;
			spPreviousPrototype->spNext = spNewPrototype;
		}
	}
	
	// Return the New Prototypes ID
	return spNewPrototype->iPrototypeID;
}

// Makes Current Object Prototype point to the first Object in the List
void CObjectPrototypeList::MoveToStartOfList()
{
	mspCurrentObjectPrototype = mspHead;
}

// Move Current Object Prototype to the next Object in the List
// Returns False if Current Object is NULL
bool CObjectPrototypeList::MoveToNextObjectPrototype()
{
	// If Current Object Prototype doesn't exist
	if (mspCurrentObjectPrototype == 0)
	{
		return false;
	}

	// Else move to Next Object Prototype
	mspCurrentObjectPrototype = mspCurrentObjectPrototype->spNext;

	// Return that everything went fine
	return true;
}

// Move Current Object Prototype to the next Object in the List, and wrap to start of List if end is reached
// Returns False if List is Empty
bool CObjectPrototypeList::MoveToNextObjectPrototypeWithWrap()
{
	// If List is Empty
	if (mspHead == 0)
	{
		// Return that List is Empty
		return false;
	}

	// If Current Object Prototype doesn't exist
	if (mspCurrentObjectPrototype == 0)
	{
		// Point it to the start of the List
		mspCurrentObjectPrototype = mspHead;
	}

	// If Next Prototype exists
	if (mspCurrentObjectPrototype->spNext != 0)
	{
		// Move to Next Object Prototype
		mspCurrentObjectPrototype = mspCurrentObjectPrototype->spNext;
	}
	// Else wrap to the start of the List
	else
	{
		mspCurrentObjectPrototype = mspHead;
	}

	// Return that everything went fine
	return true;
}

// Move Current Object Prototype to specified Prototype
// Returns false if specified Prototype not found
bool CObjectPrototypeList::MoveToObjectPrototype(int iObjectPrototypeID)
{
	SObjectPrototype* spPrototype = mspHead;	// Temp pointer to traverse List

	// Traverse List for specified Prototype
	while (spPrototype != 0 && (spPrototype->iPrototypeID != iObjectPrototypeID))
	{
		// Move to Next Prototype in the List
		spPrototype = spPrototype->spNext;
	}

	// If Prototype was not found
	if (spPrototype == 0)
	{
		return false;
	}

	// Else move Current Object Prototype to specified Prototype
	mspCurrentObjectPrototype = spPrototype;

	// Return success
	return true;
}

// Returns a Pointer to the Current Object Prototype
// Returns zero if Current Object Prototype doesn't exist
SObjectPrototype* CObjectPrototypeList::ReturnCurrentObjectPrototype() const
{
	// If Current Object Prototype doesn't exist
	if (mspCurrentObjectPrototype == 0)
	{
		return 0;
	}

	// Else return pointer to the Current Object Prototype
	return mspCurrentObjectPrototype;
}

// Tells if Current Object Prototype is actually pointing to a Object Prototype or is NULL
bool CObjectPrototypeList::CurrentObjectPrototypeExists() const
{
	// If Current Object Prototype exists
	if (mspCurrentObjectPrototype != 0)
	{
		return true;
	}

	// Else return that it doesn't exist
	return false;
}

// Returns true if Object Prototype was found in List, false if not
bool CObjectPrototypeList::IsInList(int iObjectPrototypeID) const
{
	SObjectPrototype* spPrototype = mspHead;	// Used to traverse List

	// Loop through List until Object Prototype found or end of List reached
	while (spPrototype != 0 && (spPrototype->iPrototypeID != iObjectPrototypeID))
	{
		spPrototype = spPrototype->spNext;
	}

	// If Prototype was found
	if (spPrototype != 0)
	{
		return true;
	}

	// Else return that the Object Prototype is not in the List
	return false;
}

// Returns true if a Prototype of the specified Type was found
bool CObjectPrototypeList::IsTypeInList(int iType) const
{
	SObjectPrototype* spPrototype = mspHead;	// Used to traverse List

	// Loop through List until Object Prototype found or end of List reached
	while (spPrototype != 0 && (spPrototype->iType != iType))
	{
		spPrototype = spPrototype->spNext;
	}

	// If Prototype with Type was found
	if (spPrototype != 0)
	{
		return true;
	}

	// Else return that a Prototype of the specified Type is not in the List
	return false;
}

// Returns true if the Object Prototype List is Empty
bool CObjectPrototypeList::IsEmpty() const
{
	// If List is Empty
	if (mspHead == 0)
	{
		return true;
	}

	// Else List is not Empty to return false
	return false;
}

// Remove Current Object Prototype from the List
// Returns false if Current Object Prototype doesn't exist
bool CObjectPrototypeList::RemoveCurrentObjectPrototype()
{
	SObjectPrototype* spPrototype = 0;	// Used to traverse the List
	SObjectPrototype* spPrevious	= 0;	// Used when traversing the List

	// Make sure Current Object Prototype exists
	if (mspCurrentObjectPrototype == 0)
	{
		return false;
	}

	// If Current Object Prototype is pointing to start of the List
	if (mspCurrentObjectPrototype == mspHead)
	{
		mspHead = mspHead->spNext;
		delete mspCurrentObjectPrototype;
	}
	// Else if Current Object Prototype is pointing somewhere else in the List
	else
	{
		spPrototype = mspHead;
		spPrevious = 0;

		// Traverse List looking for Current Object Prototype
		while (spPrototype != 0 && spPrototype != mspCurrentObjectPrototype)
		{
			spPrevious = spPrototype;
			spPrototype = spPrototype->spNext;
		}

		// If Current Object Prototype was not found or a problem occured
		if (spPrototype == 0 || spPrevious == 0)
		{
			return false;
		}

		// If Current Object Prototype is pointing to the Tail
		if (spPrototype == mspTail)
		{
			// Update the Tail and delete the Current Object Prototype
			mspTail = spPrevious;
			spPrevious->spNext = 0;
			delete spPrototype;
		}
		// Else it was somewhere in the middle of the List
		else
		{
			// Delete the Current Object Prototype
			spPrevious->spNext = spPrototype->spNext;
			delete spPrototype;
		}
	}

	// Return that Current Object Prototype was successfully deleted
	return true;
}

// Remove a Object Prototype from the List
// Returns false if specified Object Prototype is not in List
bool CObjectPrototypeList::RemoveObjectPrototype(int iObjectPrototypeID)
{
	SObjectPrototype* spPrototype = mspHead;	// Used to traverse the List
	SObjectPrototype* spPrevious	= 0;		// Used when traversing the List

	// Traverse List looking for Current Object Prototype
	while (spPrototype != 0 && spPrototype->iPrototypeID != iObjectPrototypeID)
	{
		spPrevious = spPrototype;
		spPrototype = spPrototype->spNext;
	}

	// If Current Object Prototype was not found
	if (spPrototype == 0)
	{
		return false;
	}

	// If specified Prototype is at the start of the List
	if (spPrototype == mspHead)
	{
		mspHead = mspHead->spNext;
		delete spPrototype;
	}
	// Else if specified Prototype is at the end of the List
	else if (spPrototype == mspTail)
	{
		// Update the Tail and delete the specified Object Prototype
		mspTail = spPrevious;
		spPrevious->spNext = 0;
		delete spPrototype;
	}
	// Else it was somewhere in the middle of the List
	else
	{
		// Delete the specified Object Prototype
		spPrevious->spNext = spPrototype->spNext;
		delete spPrototype;
	}

	// Return that Current Object Prototype was successfully deleted
	return true;
}

// Returns the Prototype with the lowest Time
// Returns zero if Prototype List is Empty
SObjectPrototype* CObjectPrototypeList::ReturnObjectPrototypeWithLowestTime() const
{
	// If List is Empty
	if (mspHead == 0)
	{
		return 0;
	}

	// Otherwise return Head (should have lowest time since sorted from lowest to highest time)
	return mspHead;
}

// Returns the Prototypes with the lowest Times Time
// Returns zero if Prototype List is Empty
unsigned long CObjectPrototypeList::ReturnLowestTimeInList() const
{
	// If List is Empty
	if (mspHead == 0)
	{
		return 0;
	}

	// Else return Heads Time (since it should be the lowest because sorted from lowest to highest time)
	return mspHead->ulTimeDurationUntilAppearing;
}

// Update all Object Prototypes Positions according to their Velocity and Update their
// TimeDurationUntilAppearing times
void CObjectPrototypeList::UpdateAllPrototypes(unsigned long ulAmountOfTimeSinceLastUpdate)
{
	SObjectPrototype* spPrototype = mspHead;	// Temp pointer used to traverse List

	// Get Time passed since last Update
	float fAmountOfTimeSinceLastUpdateInSeconds = (ulAmountOfTimeSinceLastUpdate / 1000.0);

	// Cycle through all Prototypes
	while (spPrototype != 0)
	{
		// Update Prototypes positions
		spPrototype->fX += (spPrototype->fXVelocity * fAmountOfTimeSinceLastUpdateInSeconds);
		spPrototype->fY += (spPrototype->fYVelocity * fAmountOfTimeSinceLastUpdateInSeconds);
		spPrototype->fZ += (spPrototype->fZVelocity * fAmountOfTimeSinceLastUpdateInSeconds);

		// If Time Duration Until Appearing has elapsed
		if (spPrototype->ulTimeDurationUntilAppearing < ulAmountOfTimeSinceLastUpdate)
		{
			// Set Time Duration Until Appearing to zero
			spPrototype->ulTimeDurationUntilAppearing = 0;
		}
		// Else Duration is not up so subtract Elapsed Time from the Duration
		else
		{
			spPrototype->ulTimeDurationUntilAppearing -= ulAmountOfTimeSinceLastUpdate;
		}

		// Move to Next Prototype
		spPrototype = spPrototype->spNext;
	}
}

// Remove all items from the List
void CObjectPrototypeList::Purge()
{
	SObjectPrototype* spPrototype = 0;

	// Delete all Prototypes from the List
	while (mspHead != 0)
	{
		spPrototype = mspHead;
		mspHead = mspHead->spNext;
		delete spPrototype;
	}

	// Reset private data
	mspHead = 0;
	mspTail = 0;
	mspCurrentObjectPrototype = 0;
	miIDCounter = 0;
}

