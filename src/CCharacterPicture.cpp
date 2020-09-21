// CCharacterPicture.cpp

#include "CCharacterPicture.h"


// Constructor
CCharacterPicture::CCharacterPicture()
{
	// Initialize all private variables and start Timer

	miX = miY = miZ = -1;
	
	mspCurrentState		= 0;
	mspCurrentPicture	= 0;
	mspStateList		= 0;
	mspPictureList		= 0;

	mulRotationTimer	= 0;
}

// Copy Constructor
CCharacterPicture::CCharacterPicture(CCharacterPicture const& cObjectToCopyFrom)
{
	// Initialize Pointers
	mspCurrentState		= 0;
	mspCurrentPicture	= 0;
	mspStateList		= 0;
	mspPictureList		= 0;

	// Copy data from other object
	CopyFrom(cObjectToCopyFrom);
}

// Destructor
CCharacterPicture::~CCharacterPicture()
{
	Purge();
}

// Deep copy from another object
void CCharacterPicture::CopyFrom(CCharacterPicture const& cObjectToCopyFrom)
{
	int iIndex = 0;		// Counting variable

	// Erase all current data
	Purge();

	// Copy Coordinates
	miX = cObjectToCopyFrom.ReturnXCoordinate();
	miY = cObjectToCopyFrom.ReturnYCoordinate();
	miZ = cObjectToCopyFrom.ReturnZCoordinate();

	// Copy Rotation Timer
	mulRotationTimer = cObjectToCopyFrom.ReturnRotationTimer();

	// Create constant pointers to Current State/Picture and State/Picture List of ObjectToCopyFrom
	SState const* spOtherCurrentState		= cObjectToCopyFrom.ReturnPointerToCurrentState();
	SState const* spOtherStateList			= cObjectToCopyFrom.ReturnPointerToStateList();
	SPicture const* spOtherCurrentPicture	= cObjectToCopyFrom.ReturnPointerToCurrentPicture();
	SPicture const* spOtherPictureList		= cObjectToCopyFrom.ReturnPointerToPictureList();

	// Create pointers to this objects Lists to be used to fill in the Lists
	SState* spThisStateList		= mspStateList;
	SState* spPreviousState		= 0;
	SPicture* spThisPictureList	= mspPictureList;
	SPicture* spPreviousPicture	= 0;

	// Copy Picture List to this object
	while (spOtherPictureList != 0)
	{
		// Create a New Picture
		spThisPictureList = new SPicture;

		// If this is not first Picture in the list
		if (spPreviousPicture != 0)
		{
			// Make last Picture point to this Picture
			spPreviousPicture->spNextPicture = spThisPictureList;
		}
		// Else if this is the first Picture in the list
		else
		{
			// Make the List head point to this Picture
			mspPictureList = spThisPictureList;
		}

		// Copy Other Pictures data into this Picture
		spThisPictureList->iHeight	= spOtherPictureList->iHeight;
		spThisPictureList->iLeft	= spOtherPictureList->iLeft;
		spThisPictureList->iPicture	= spOtherPictureList->iPicture;
		spThisPictureList->iTop		= spOtherPictureList->iTop;
		spThisPictureList->iWidth	= spOtherPictureList->iWidth;

		// If Other Object has a Current Picture
		if (spOtherCurrentPicture != 0)
		{
			// Check if this should be the Current Picture
			if (spOtherCurrentPicture->iPicture == spThisPictureList->iPicture)
			{
				mspCurrentPicture = spThisPictureList;
			}
		}

		// Make this Pictures Next Picture NULL
		spThisPictureList->spNextPicture = 0;

		// Save this Picture using Prevoius Picture
		spPreviousPicture = spThisPictureList;

		// Move OtherPictureList to next Picture
		spOtherPictureList = spOtherPictureList->spNextPicture;
	};

	// Copy State List to this object
	while (spOtherStateList != 0)
	{
		// Create a New State
		spThisStateList = new SState;

		// If this is not first State in the list
		if (spPreviousState != 0)
		{
			// Make last State point to this State
			spPreviousState->spNextState = spThisStateList;
		}
		// Else if this is the first State in the list
		else
		{
			// Make the List head point to this State
			mspStateList = spThisStateList;
		}
		
		// Copy Other States data into this State
		spThisStateList->iState							= spOtherStateList->iState;
		spThisStateList->iCurrentPositionInRotation		= spOtherStateList->iCurrentPositionInRotation;
		spThisStateList->iNumOfPicturesInRotation		= spOtherStateList->iNumOfPicturesInRotation;
		spThisStateList->ulDefaultPictureRotationTime	= spOtherStateList->ulDefaultPictureRotationTime;
		spThisStateList->ulPictureRotationTime			= spOtherStateList->ulPictureRotationTime;
		spThisStateList->iXVelocity						= spOtherStateList->iXVelocity;
		spThisStateList->iYVelocity						= spOtherStateList->iYVelocity;
		spThisStateList->iZVelocity						= spOtherStateList->iZVelocity;

		spThisStateList->ipPictureRotationOrder = new int[spOtherStateList->iNumOfPicturesInRotation];

		// Deep Copy Rotation Order
		for (iIndex = 0; iIndex < spOtherStateList->iNumOfPicturesInRotation; iIndex++)
		{
			spThisStateList->ipPictureRotationOrder[iIndex] = spOtherStateList->ipPictureRotationOrder[iIndex];
		}

		// If Other Object has a Current State
		if (spOtherCurrentState != 0)
		{
			// Check if this should be the Current State
			if (spOtherCurrentState->iState == spThisStateList->iState)
			{
				mspCurrentState = spThisStateList;
			}
		}

		// Make this States Next State NULL
		spThisStateList->spNextState = 0;

		// Save this State using Previous State
		spPreviousState = spThisStateList;

		// Move OtherStateList to next State
		spOtherStateList = spOtherStateList->spNextState;
	};
}

// Move Character to specified world Position
void CCharacterPicture::MoveCharacterTo(int iX, int iY, int iZ)
{
	miX = iX;
	miY = iY;
	miZ = iZ;
}

// Move Characters X Picture to specifed location
void CCharacterPicture::MoveCharacterXTo(int iX)
{
	miX = iX;
}

// Move Characters Y Picture to specifed location
void CCharacterPicture::MoveCharacterYTo(int iY)
{
	miY = iY;
}

// Move Characters Z Picture to specifed location
void CCharacterPicture::MoveCharacterZTo(int iZ)
{
	miZ = iZ;
}


// Move Character relative to current Position
void CCharacterPicture::MoveCharacterRelative(int iX, int iY, int iZ)
{
	miX += iX;
	miY += iY;
	miZ += iZ;
}

// Move Characters X Picture by specified amount
void CCharacterPicture::MoveCharacterXRelative(int iX)
{
	miX += iX;
}

// Move Characters Y Picture by specified amount
void CCharacterPicture::MoveCharacterYRelative(int iY)
{
	miY += iY;
}

// Move Characters Z Picture by specified amount
void CCharacterPicture::MoveCharacterZRelative(int iZ)
{
	miZ += iZ;
}


// Set Coordinates of picture on .bmp
void CCharacterPicture::CreatePicture(int iPicture, int iLeft, int iTop,
									  int iWidth, int iHeight)
{
	SPicture* spPicture = mspPictureList;
	SPicture* spPreviousPicture = 0;
	
	// Loop through Picture List until we find desired Picture or end of List
	while (spPicture != 0 && spPicture->iPicture != iPicture)
	{
		spPreviousPicture = spPicture;
		spPicture = spPicture->spNextPicture;
	};

	// If Picture was not found in List, Create it and make spPicture point to it
	if (spPicture == 0)
	{
		// If this is the very first Picture to be created
		if (spPreviousPicture == 0)
		{
			mspPictureList = new SPicture;
			spPicture = mspPictureList;
		}else
		{
			spPreviousPicture->spNextPicture = new SPicture;
			spPicture = spPreviousPicture->spNextPicture;
		}
	}

	// Set Picture Coordinates to specified Coordinates
	spPicture->iPicture			= iPicture;
	spPicture->iLeft			= iLeft;
	spPicture->iTop				= iTop;
	spPicture->iWidth			= iWidth;
	spPicture->iHeight			= iHeight;
	spPicture->spNextPicture	= 0;
}


// Create Character State
void CCharacterPicture::CreateState(int iState, int* iPictureRotationOrder, 
									int iNumOfPicturesInRotation, unsigned long ulPictureRotationTime,
									int iXVelocity, int iYVelocity, int iZVelocity)
{
	SState* spNext;
	int iIndex = 0;

	// If this is the first State to be allocated
	if (mspStateList == 0)
	{
		// Allocate a new SState to mspState
		mspStateList = new SState;
		spNext = mspStateList;
	}else
	{
		spNext = mspStateList;

		// Find the last State in the linked list
		while (spNext->spNextState != 0)
		{
			spNext = spNext->spNextState;	
		};

		// Allocate a new state and make last State in list point to it
		spNext->spNextState = new SState;
		spNext = spNext->spNextState;
	}

	// Enter specfied data into the State
	spNext->iState						= iState;
	spNext->ulPictureRotationTime		= ulPictureRotationTime;
	spNext->ulDefaultPictureRotationTime= ulPictureRotationTime;
	spNext->iNumOfPicturesInRotation	= iNumOfPicturesInRotation;
	spNext->iCurrentPositionInRotation	= 0;
	spNext->iXVelocity					= iXVelocity;
	spNext->iYVelocity					= iYVelocity;
	spNext->iZVelocity					= iZVelocity;
	spNext->spNextState					= 0;
	
	// Set the rotation order
	spNext->ipPictureRotationOrder = new int[iNumOfPicturesInRotation];

	// Deep Copy Rotation Order
	for (iIndex = 0; iIndex < iNumOfPicturesInRotation; iIndex++)
	{
		spNext->ipPictureRotationOrder[iIndex] = iPictureRotationOrder[iIndex];
	}	
}

// Set the Characters Current State to the State specified
bool CCharacterPicture::SetCurrentState(int iState, int iStartingRotationPosition)
{
	SState* spNextState = mspStateList;
	SPicture* spNextPicture = mspPictureList;
	int iPicture;
	int iIndex = 0;

	// Loop through State list until we find specified State or end of list
	while (spNextState != 0 && spNextState->iState != iState)
	{
		spNextState = spNextState->spNextState;	// Move to next State in list
	};

	// If specified State was not found
	if (spNextState == 0)
	{
		return false;
	}
	// Else the State was found

	// Make sure RotationPosition is valid
	if (iStartingRotationPosition < 0 || iStartingRotationPosition >= spNextState->iNumOfPicturesInRotation)
	{
		iStartingRotationPosition = 0;
	}

	// Make sure all needed Pictures exist
	for (iIndex = 0; iIndex < spNextState->iNumOfPicturesInRotation; iIndex++)
	{
		iPicture = spNextState->ipPictureRotationOrder[iIndex];

		// If Picture is not found in Picture List
		if (!DoesPictureExist(iPicture))
		{
			return false;
		}
		// Else the Picture was found
	}

	// Find the desired Position in the Rotation
	iPicture = spNextState->ipPictureRotationOrder[iStartingRotationPosition];
	while (spNextPicture != 0 && spNextPicture->iPicture != iPicture)
	{
		spNextPicture = spNextPicture->spNextPicture;
	};

	// If could not find desired Position
	if (spNextPicture == 0)
	{
		return false;
	}

	// Save this as the Current Picture and State
	mspCurrentPicture = spNextPicture;
	mspCurrentState = spNextState;
	mspCurrentState->iCurrentPositionInRotation = iStartingRotationPosition;

	return true;
}

// Change a States Picture Rotation Time
bool CCharacterPicture::ChangeStatesRotationTime(int iState, unsigned long ulPictureRotationTime)
{
	SState* spNextState = mspStateList;

	// Loop through State list until we find specified State or end of list
	while (spNextState != 0 && spNextState->iState != iState)
	{
		spNextState = spNextState->spNextState;	// Move to next State in list
	};

	// If specified State was not found or there is only 1 picture so no rotation is needed
	if (spNextState == 0 || spNextState->iNumOfPicturesInRotation == 1)
	{
		return false;
	}
	// Else the State was found

	// Change the States Rotation Time
	spNextState->ulPictureRotationTime = ulPictureRotationTime;

	return true;
}

// Change the Current States Rotation Time
bool CCharacterPicture::ChangeCurrentStatesRotationTime(unsigned long ulPictureRotationTime)
{
	// If no State has been Set yet or there is only 1 picture so no rotation is needed
	if (mspCurrentState == 0 || mspCurrentState->iNumOfPicturesInRotation == 1)
	{
		return false;
	}

	// Change the States Rotation Time
	mspCurrentState->ulPictureRotationTime = ulPictureRotationTime;

	return true;
}

// Change a States Default Picture Rotation Time
bool CCharacterPicture::ChangeStatesDefaultRotationTime(int iState, unsigned long ulNewDefaultRotationTime)
{
	SState* spNextState = mspStateList;

	// Loop through State list until we find specified State or end of list
	while (spNextState != 0 && spNextState->iState != iState)
	{
		spNextState = spNextState->spNextState;	// Move to next State in list
	};

	// If specified State was not found or there is only 1 picture so no rotation is needed
	if (spNextState == 0 || spNextState->iNumOfPicturesInRotation == 1)
	{
		return false;
	}
	// Else the State was found

	// Change the States Default Rotation Time
	spNextState->ulDefaultPictureRotationTime = ulNewDefaultRotationTime;

	return true;
}

// Change Current States Default Picture Rotation Time
bool CCharacterPicture::ChangeCurrentStatesDefaultRotationTime(unsigned long ulNewDefaultRotationTime)
{
	// If no State has been Set yet or there is only 1 picture so no rotation is needed
	if (mspCurrentState == 0 || mspCurrentState->iNumOfPicturesInRotation == 1)
	{
		return false;
	}

	// Change the States Default Rotation Time
	mspCurrentState->ulDefaultPictureRotationTime = ulNewDefaultRotationTime;

	return true;
}

// Restore a States Picture Rotation Time back to the default time
bool CCharacterPicture::RestoreStatesDefaultRotationTime(int iState)
{
	SState* spNextState = mspStateList;

	// Loop through State list until we find specified State or end of list
	while (spNextState != 0 && spNextState->iState != iState)
	{
		spNextState = spNextState->spNextState;	// Move to next State in list
	};

	// If specified State was not found
	if (spNextState == 0)
	{
		return false;
	}
	// Else the State was found

	// Restore the States Rotation Time back to the default
	spNextState->ulPictureRotationTime = spNextState->ulDefaultPictureRotationTime;

	return true;
}

// Restore the Current States Picture Rotation Time back to the default time
bool CCharacterPicture::RestoreCurrentStatesDefaultRotationTime()
{
	// If no State has been Set yet
	if (mspCurrentState == 0)
	{
		return false;
	}

	// Restore the States Rotation Time back to the default
	mspCurrentState->ulPictureRotationTime = mspCurrentState->ulDefaultPictureRotationTime;

	return true;
}

// Set a States XYZ Velocities
bool CCharacterPicture::SetStatesVelocity(int iState, int iXVelocity, int iYVelocity, int iZVelocity)
{
	SState* spNextState = mspStateList;

	// Loop through State list until we find specified State or end of list
	while (spNextState != 0 && spNextState->iState != iState)
	{
		spNextState = spNextState->spNextState;	// Move to next State in list
	};

	// If specified State was not found
	if (spNextState == 0)
	{
		return false;
	}
	// Else the State was found

	spNextState->iXVelocity = iXVelocity;
	spNextState->iYVelocity = iYVelocity;
	spNextState->iZVelocity = iZVelocity;

	return true;
}
	
// Set a States X Velocity
bool CCharacterPicture::SetStatesXVelocity(int iState, int iXVelocity)
{
	SState* spNextState = mspStateList;

	// Loop through State list until we find specified State or end of list
	while (spNextState != 0 && spNextState->iState != iState)
	{
		spNextState = spNextState->spNextState;	// Move to next State in list
	};

	// If specified State was not found
	if (spNextState == 0)
	{
		return false;
	}
	// Else the State was found

	spNextState->iXVelocity = iXVelocity;

	return true;
}

// Set a States Y Velocity
bool CCharacterPicture::SetStatesYVelocity(int iState, int iYVelocity)
{
	SState* spNextState = mspStateList;

	// Loop through State list until we find specified State or end of list
	while (spNextState != 0 && spNextState->iState != iState)
	{
		spNextState = spNextState->spNextState;	// Move to next State in list
	};

	// If specified State was not found
	if (spNextState == 0)
	{
		return false;
	}
	// Else the State was found

	spNextState->iYVelocity = iYVelocity;

	return true;
}

// Set a States Z Velocity
bool CCharacterPicture::SetStatesZVelocity(int iState, int iZVelocity)
{
	SState* spNextState = mspStateList;

	// Loop through State list until we find specified State or end of list
	while (spNextState != 0 && spNextState->iState != iState)
	{
		spNextState = spNextState->spNextState;	// Move to next State in list
	};

	// If specified State was not found
	if (spNextState == 0)
	{
		return false;
	}
	// Else the State was found

	spNextState->iZVelocity = iZVelocity;

	return true;
}


// Return XYZ Coordinates
void CCharacterPicture::ReturnCoordinates(int& iX, int& iY, int& iZ) const
{
	iX = miX;
	iY = miY;
	iZ = miZ;
}

// Return X Coordinate
int CCharacterPicture::ReturnXCoordinate() const
{
	return miX;
}

// Return Y Coordinate
int CCharacterPicture::ReturnYCoordinate() const
{
	return miY;
}

// Return Z Coordinate
int CCharacterPicture::ReturnZCoordinate() const
{
	return miZ;
}

// Return a States XYZ Velocities
bool CCharacterPicture::ReturnStatesVelocity(int iState, int& iXVelocity, int& iYVelocity, int& iZVelocity) const
{
	SState* spNextState = mspStateList;

	// Loop through State list until we find specified State or end of list
	while (spNextState != 0 && spNextState->iState != iState)
	{
		spNextState = spNextState->spNextState;	// Move to next State in list
	};

	// If specified State was not found
	if (spNextState == 0)
	{
		return false;
	}
	// Else the State was found

	// Return velocities
	iXVelocity = spNextState->iXVelocity;
	iYVelocity = spNextState->iYVelocity;
	iZVelocity = spNextState->iZVelocity;

	return true;
}

// Return a States X Velocity
int CCharacterPicture::ReturnStatesXVelocity(int iState) const
{
	SState* spNextState = mspStateList;

	// Loop through State list until we find specified State or end of list
	while (spNextState != 0 && spNextState->iState != iState)
	{
		spNextState = spNextState->spNextState;	// Move to next State in list
	};

	// If specified State was not found
	if (spNextState == 0)
	{
		return -1;
	}
	// Else the State was found

	// Return velocities
	return spNextState->iXVelocity;
}

// Return a States Y Velocity
int CCharacterPicture::ReturnStatesYVelocity(int iState) const
{
	SState* spNextState = mspStateList;

	// Loop through State list until we find specified State or end of list
	while (spNextState != 0 && spNextState->iState != iState)
	{
		spNextState = spNextState->spNextState;	// Move to next State in list
	};

	// If specified State was not found
	if (spNextState == 0)
	{
		return -1;
	}
	// Else the State was found

	return spNextState->iYVelocity;
}

// Return a States Z Velocity
int CCharacterPicture::ReturnStatesZVelocity(int iState) const
{
	SState* spNextState = mspStateList;

	// Loop through State list until we find specified State or end of list
	while (spNextState != 0 && spNextState->iState != iState)
	{
		spNextState = spNextState->spNextState;	// Move to next State in list
	};

	// If specified State was not found
	if (spNextState == 0)
	{
		return -1;
	}
	// Else the State was found

	return spNextState->iZVelocity;
}

// Return Character Picture Coordinates
bool CCharacterPicture::ReturnPictureCoordinates(int& iLeft, int& iTop, int& iWidth, int& iHeight) const
{
	// If Current Picture is not set
	if (mspCurrentPicture == 0)
	{
		return false;
	}

	iLeft = mspCurrentPicture->iLeft;
	iTop = mspCurrentPicture->iTop;
	iWidth = mspCurrentPicture->iWidth;
	iHeight = mspCurrentPicture->iHeight;

	return true;
}

// Return Pictures Left Coordinate
int CCharacterPicture::ReturnLeft() const
{
	// If Current Picture is not set
	if (mspCurrentPicture == 0)
	{
		return -1;
	}

	return mspCurrentPicture->iLeft;
}

// Return Pictures Top Coordinate
int CCharacterPicture::ReturnTop() const
{
	// If Current Picture is not set
	if (mspCurrentPicture == 0)
	{
		return -1;
	}

	return mspCurrentPicture->iTop;
}

// Return Pictures Rightmost Coordinate
int CCharacterPicture::ReturnRight() const
{
	// If Current Picture is not set
	if (mspCurrentPicture == 0)
	{
		return -1;
	}

	return (mspCurrentPicture->iLeft + mspCurrentPicture->iWidth);
}

// Return Pictures Bottommost Coordinate
int CCharacterPicture::ReturnBottom() const
{
	// If Current Picture is not set
	if (mspCurrentPicture == 0)
	{
		return -1;
	}

	return (mspCurrentPicture->iTop + mspCurrentPicture->iHeight);
}

// Return Pictures Width
int CCharacterPicture::ReturnWidth() const
{
	// If Current Picture is not set
	if (mspCurrentPicture == 0)
	{
		return -1;
	}

	return mspCurrentPicture->iWidth;
}

// Return Pictures Height
int CCharacterPicture::ReturnHeight() const
{
	// If Current Picture is not set
	if (mspCurrentPicture == 0)
	{
		return -1;
	}

	return mspCurrentPicture->iHeight;
}


// Return Current Character State
int CCharacterPicture::ReturnCurrentState() const
{
	// If Current State is not set
	if (mspCurrentState == 0)
	{
		return -1;
	}

	return mspCurrentState->iState;
}

// Return Current Character Picture
int CCharacterPicture::ReturnCurrentPicture() const
{
	// If Current State is not set
	if (mspCurrentState == 0)
	{
		return -1;
	}

	return mspCurrentState->ipPictureRotationOrder[mspCurrentState->iCurrentPositionInRotation];
}

// Return Current Picture Rotation Position
int CCharacterPicture::ReturnCurrentPictureRotationPosition() const
{
	// If Current State is not set
	if (mspCurrentState == 0)
	{
		return -1;
	}

	return mspCurrentState->iCurrentPositionInRotation;
}

// Return the Total Number of Pictures in the Current State
int CCharacterPicture::ReturnCurrentStatesTotalNumberOfPictures() const
{
	// If Current State is not set
	if (mspCurrentState == 0)
	{
		return -1;
	}

	return mspCurrentState->iNumOfPicturesInRotation;
}

// Return the Total Number of Pictures in a State
int CCharacterPicture::ReturnStatesTotalNumberOfPictures(int iStateID) const
{
	SState* spState = mspStateList;

	// Loop through State List until we find the State or reach the end of the List
	while (spState != 0 && spState->iState != iStateID)
	{
		spState = spState->spNextState;
	};

	// If State was not found
	if (spState == 0)
	{
		return -1;
	}

	return spState->iNumOfPicturesInRotation;
}

// Set Current States Current Picture
// NOTE: Moves Current Picture to Pictures first appearance in the Rotation Position, since the same Picture may appear in it several times
bool CCharacterPicture::SetCurrentStatesPicture(int iPicture)
{
	SPicture* spPicture = mspPictureList;
	int iIndex = 0;

	// Loop through Picture List until we find the Picture or reach the end of the List
	while (spPicture != 0 && spPicture->iPicture != iPicture && spPicture->iPicture != iPicture)
	{
		spPicture = spPicture->spNextPicture;
	};

	// If Picture was not found
	if (spPicture == 0)
	{
		return false;
	}

	iIndex = 0;
	// Find Pictures Position in the Rotation
	while (iIndex < mspCurrentState->iNumOfPicturesInRotation && 
		   mspCurrentState->ipPictureRotationOrder[iIndex] != iPicture)
	{
		iIndex++;
	};

	// Move the Current Picture to the Picture at the specified Rotation Position
	mspCurrentState->iCurrentPositionInRotation = iIndex;
	mspCurrentPicture = spPicture;

	return true;
}

// Set Current States Current Picture to the Picture at the specified Rotation Position
// NOTE: First Rotation Position starts at 1
bool CCharacterPicture::SetCurrentStatesPictureByRotationPosition(int iRotationPosition)
{
	SPicture* spPicture = mspPictureList;
	int iPicture = 0;

	iRotationPosition--;		// Decrement supplied Rotation Position since it starts at 0, not 1

	// Make sure RotationPosition is valid
	if (iRotationPosition < 0 || iRotationPosition >= mspCurrentState->iNumOfPicturesInRotation)
	{
		return false;
	}

	// Store which Picture we should switch to
	iPicture = mspCurrentState->ipPictureRotationOrder[iRotationPosition];

	// Loop through Picture List until we find the Picture or reach the end of the List
	while (spPicture != 0 && spPicture->iPicture != iPicture)
	{
		spPicture = spPicture->spNextPicture;
	};

	// If Picture was not found
	if (spPicture == 0)
	{
		return false;
	}

	// Move the Current Picture to the Picture at the specified Rotation Position
	mspCurrentState->iCurrentPositionInRotation = iRotationPosition;
	mspCurrentPicture = spPicture;

	return true;
}

// Return current Picture Rotation Time
unsigned long CCharacterPicture::ReturnPictureRotationTime() const
{
	// If Current State is not set
	if (mspCurrentState == 0)
	{
		return -1;
	}

	return mspCurrentState->ulPictureRotationTime;
}

// Return current Default Picture Rotation Time
unsigned long CCharacterPicture::ReturnDefaultPictureRotationTime() const
{
	// If Current State is not set
	if (mspCurrentState == 0)
	{
		return -1;
	}

	return mspCurrentState->ulDefaultPictureRotationTime;
}

// Return if Picture exists
bool CCharacterPicture::DoesPictureExist(int iPicture) const
{
	SPicture* spPicture = mspPictureList;

	// Loop through Picture List until we find the Picture or reach the end of the List
	while (spPicture != 0 && spPicture->iPicture != iPicture)
	{
		spPicture = spPicture->spNextPicture;
	};

	// If Picture was not found
	if (spPicture == 0)
	{
		return false;
	}

	return true;
}

// Return if State exists
bool CCharacterPicture::DoesStateExist(int iState) const
{
	SState* spState = mspStateList;

	// Loop through State List until we find the State or reach the end of the List
	while (spState != 0 && spState->iState != iState)
	{
		spState = spState->spNextState;
	};

	// If State was not found
	if (spState == 0)
	{
		return false;
	}

	return true;
}

// Returns if Current Picture is set yet
bool CCharacterPicture::CurrentPictureIsSet() const
{
	// If Current Picture is not set yet return false
	if (mspCurrentPicture == 0)
	{
		return false;
	}

	// Return true since it is set
	return true;
}

// Returns if Current State is set yet
bool CCharacterPicture::CurrentStateIsSet() const
{
	// If Current State is not set yet return false
	if (mspCurrentState == 0)
	{
		return false;
	}

	// Return true since it is set
	return true;
}

// Rotate the Picture if necessary (Should be called every frame)
// Returns false if a needed Picture was not found
// Remember: 0 Rotation Time = Do not Rotate Picture
bool CCharacterPicture::UpdatePicture(unsigned long ulAmountOfTimeSinceLastUpdate)
{
	SPicture* spPicture = mspPictureList;
	int iPicture = 0;

	// If Current State is not set
	if (!CurrentStateIsSet())
	{
		return false;
	}

	// If Picture should not be Rotated
	if (mspCurrentState->ulPictureRotationTime == 0)
	{
		return true;
	}

	// Update the Rotation Timer
	mulRotationTimer += ulAmountOfTimeSinceLastUpdate;

	// If it's time to Rotate Picture
	if (mulRotationTimer > mspCurrentState->ulPictureRotationTime)
	{
		// Subtract the Rotation Time from our Rotation Timer
		mulRotationTimer -= mspCurrentState->ulPictureRotationTime;

		// Increment to next Position in Rotation
		mspCurrentState->iCurrentPositionInRotation++;

		// If currently at the last position in the Rotation, loop back to beginning of Rotation
		if (mspCurrentState->iCurrentPositionInRotation >= mspCurrentState->iNumOfPicturesInRotation)
		{
			mspCurrentState->iCurrentPositionInRotation = 0;
		}

		// Store which Picture we should switch to
		iPicture = mspCurrentState->ipPictureRotationOrder[mspCurrentState->iCurrentPositionInRotation];

		// Loop through Picture List until we find the Picture or reach the end of the List
		while (spPicture != 0 && spPicture->iPicture != iPicture)
		{
			spPicture = spPicture->spNextPicture;
		};

		// If Picture was not found
		if (spPicture == 0)
		{
			return false;
		}

		// Store this Picture as the Current Picture
		mspCurrentPicture = spPicture;
	}

	return true;
}

// Erase all info from this object
void CCharacterPicture::Purge()
{
	SState* spCurrentState = mspStateList;
	SState* spNextState;

	SPicture* spCurrentPicture = mspPictureList;
	SPicture* spNextPicture;

	// Loop through and delete all allocated States
	while (spCurrentState != 0)
	{
		spNextState = spCurrentState->spNextState;

		if (spCurrentState->ipPictureRotationOrder)
		{
			delete [] spCurrentState->ipPictureRotationOrder;
		}

		delete spCurrentState;
		spCurrentState = spNextState;
	};

	// Loop through and delete all allocated Pictures
	while (spCurrentPicture != 0)
	{
		spNextPicture = spCurrentPicture->spNextPicture;
		delete spCurrentPicture;
		spCurrentPicture = spNextPicture;
	};


	// Reset all private data
	miX = miY = miZ = -1;
	
	mspCurrentState		= 0;
	mspCurrentPicture	= 0;
	mspStateList		= 0;
	mspPictureList		= 0;

	mulRotationTimer	= 0;
}



// Return constant pointer to Current State
SState const* CCharacterPicture::ReturnPointerToCurrentState() const
{
	return mspCurrentState;
}

SState const* CCharacterPicture::ReturnPointerToState(int iStateID) const
{
	SState* spNextState = mspStateList;

	// Loop through State list until we find specified State or end of list
	while (spNextState != 0 && spNextState->iState != iStateID)
	{
		spNextState = spNextState->spNextState;	// Move to next State in list
	};

	// If specified State was not found
	if (spNextState == 0)
	{
		return 0;
	}
	// Else the State was found

	return spNextState;	
}

// Return constant pointer to State List
SState const* CCharacterPicture::ReturnPointerToStateList() const
{
	return mspStateList;
}

// Return constant pointer to Current Picture
SPicture const* CCharacterPicture::ReturnPointerToCurrentPicture() const
{
	return mspCurrentPicture;
}

SPicture const* CCharacterPicture::ReturnPointerToPicture(int iPicture) const
{
	SPicture* spPicture = mspPictureList;
	
	// Loop through Picture List until we find desired Picture or end of List
	while (spPicture != 0 && spPicture->iPicture != iPicture)
	{
		spPicture = spPicture->spNextPicture;
	};

	// If Picture was not found in List
	if (spPicture == 0)
	{
		return 0;
	}

	return spPicture;
}

// Return constant pointer to Picture List
SPicture const* CCharacterPicture::ReturnPointerToPictureList() const
{
	return mspPictureList;
}

// Return the Rotation Timers current time
unsigned long CCharacterPicture::ReturnRotationTimer() const
{
	return mulRotationTimer;
}