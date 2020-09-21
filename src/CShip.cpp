// CShip.cpp	Created July 15, 2004

#include "CShip.h"
#include <float.h>		// Needed for getting Max and Min float values
#include <cmath>		// Needed for floorf() function	
#include <stdlib.h>		// Needed for the random numbers
#include <time.h>		// Needed for the random number
using namespace std;


//********** Contructors and Copy Functions **********

// Default Constructor
CShip::CShip()
{
	// Initialize all private data
	mspWeaponListHead = mspWeaponListTail = mspCurrentWeapon = 0;
	mspItemListHead = mspCurrentItem = 0;
	mspPatternListHead = mspCurrentPattern = 0;
	Purge();
}

// Explicit Constructor
CShip::CShip(CClock* cpClock)
{
	// Initialize all private data
	mspWeaponListHead = mspWeaponListTail = mspCurrentWeapon = 0;
	mspItemListHead = mspCurrentItem = 0;
	mspPatternListHead = mspCurrentPattern = 0;
	Purge();
}

// Destructor
CShip::~CShip()
{
	Purge();
}

// Copy Constructor
CShip::CShip(CShip const& cShipToCopyFrom)
{
	// Initialize private data
	mspWeaponListHead = mspWeaponListTail = mspCurrentWeapon = 0;
	mspItemListHead = mspCurrentItem = 0;
	mspPatternListHead = mspCurrentPattern = 0;
	Purge();

	// Copy data over from other ship
	CopyFrom(cShipToCopyFrom);
}

// Copy from another object
void CShip::CopyFrom(CShip const& cShipToCopyFrom)
{
	bool bWeaponListIsEmpty = true;		// Used to tell if any Weapons were added to the List

	// Erase all info from this Ship
	Purge();

	// Get pointers to other Ships Weapon List and Current Weapon
	SWeapon const* spOtherShipWeaponList	= cShipToCopyFrom.ReturnPointerToWeaponList();
	SWeapon const* spOtherShipCurrentWeapon	= cShipToCopyFrom.ReturnPointerToCurrentWeapon();

	// Get pointers to other Ships Item List and Current Item
	SItem const* spOtherShipItemList		= cShipToCopyFrom.ReturnPointerToItemList();
	SItem const* spOtherShipCurrentItem		= cShipToCopyFrom.ReturnPointerToCurrentItem();

	// Get pointers to other Ships Pattern List and Current Pattern
	SPattern const* spOtherShipPatternList	= cShipToCopyFrom.ReturnPointerToPatternList();
	SPattern const* spOtherShipCurrentPattern = cShipToCopyFrom.ReturnPointerToCurrentPattern();

	// Create temp pointers to cycle through this Ships Weapon List
	SWeapon* spWeapon = mspWeaponListHead;
	SWeapon* spPreviousWeapon = 0;

	// Create temp pointers to cycle through this Ships Item List
	SItem* spItem = mspItemListHead;
	SItem* spPreviousItem = 0;

	// Create temp pointers to cycle through this Ships Pattern List
	SPattern* spPattern = mspPatternListHead;
	SPattern* spPatternListTail = 0;

	// Copy Pictures and States
	mcPicture.CopyFrom(cShipToCopyFrom.mcPicture);

	// Copy Ship data
	cShipToCopyFrom.ReturnCoordinates(mfX, mfY, mfZ);
	cShipToCopyFrom.ReturnVelocities(mfXVelocity, mfYVelocity, mfZVelocity);
	cShipToCopyFrom.ReturnMaxVelocities(mfMaxXVelocity, mfMaxYVelocity, mfMaxZVelocity);
	cShipToCopyFrom.ReturnMinVelocities(mfMinXVelocity, mfMinYVelocity, mfMinZVelocity);
	cShipToCopyFrom.ReturnAccelerations(mfXAcceleration, mfYAcceleration, mfZAcceleration);
	cShipToCopyFrom.ReturnMaxAccelerations(mfMaxXAcceleration, mfMaxYAcceleration, mfMaxZAcceleration);
	cShipToCopyFrom.ReturnMinAccelerations(mfMinXAcceleration, mfMinYAcceleration, mfMinZAcceleration);
	cShipToCopyFrom.ReturnNormalAccelerations(mfNormalXAcceleration, mfNormalYAcceleration, mfNormalZAcceleration);
	cShipToCopyFrom.ReturnFrictions(mfXFriction, mfYFriction, mfZFriction);

	miHealth = cShipToCopyFrom.ReturnHealth();
	miMaxHealth = cShipToCopyFrom.ReturnMaxHealth();
	mfArmor = cShipToCopyFrom.ReturnArmor();
	mfMaxArmor = cShipToCopyFrom.ReturnMaxArmor();
	mfShieldStrength = cShipToCopyFrom.ReturnShieldStrength();
	mfMaxShieldStrength = cShipToCopyFrom.ReturnMaxShieldStrength();
	miStatus = cShipToCopyFrom.ReturnStatus();
	miLives = cShipToCopyFrom.ReturnNumberOfLives();
	miMaxLives = cShipToCopyFrom.ReturnMaxNumberOfLives();
	miGeneralShipType = cShipToCopyFrom.ReturnGeneralShipType();
	miSpecificShipType = cShipToCopyFrom.ReturnSpecificShipType();
	muiPoints = cShipToCopyFrom.ReturnNumberOfPoints();
	mbCollision = cShipToCopyFrom.ReturnIfShipIsColliding();

	muiChanceOfDropping = cShipToCopyFrom.ReturnChanceOfDroppingItem();

	mulMinRandomSwitchPatternTime = cShipToCopyFrom.ReturnMinRandomSwitchPatternTime();
	mulMaxRandomSwitchPatternTime = cShipToCopyFrom.ReturnMaxRandomSwitchPatternTime();
	mulMinRandomMovementTime = cShipToCopyFrom.ReturnMinRandomMovementTime();
	mulMaxRandomMovementTime = cShipToCopyFrom.ReturnMaxRandomMovementTime();
	mulMinRandomFireTime = cShipToCopyFrom.ReturnMinRandomFireTime();
	mulMaxRandomFireTime = cShipToCopyFrom.ReturnMaxRandomFireTime();
	mulSwitchPatternTime = cShipToCopyFrom.ReturnSwitchPatternTime();
	mulRandomMovementTime = cShipToCopyFrom.ReturnRandomMovementTime();
	mulRandomFireTime = cShipToCopyFrom.ReturnRandomFireTime();
	miRandomXTrackingPrecision = cShipToCopyFrom.ReturnRandomXTrackingPrecision();
	miRandomYTrackingPrecision = cShipToCopyFrom.ReturnRandomYTrackingPrecision();
	mbChoosePatternsRandomly = cShipToCopyFrom.ReturnIfPatternsAreBeingChosenRandomly();
	mbAIHasRanOnce = cShipToCopyFrom.ReturnIfAIHasRanOnce();
	mulStayOnScreenTime = cShipToCopyFrom.ReturnHowLongShipShouldStayOnScreen();
	mbShipShouldFire = cShipToCopyFrom.ReturnIfShipShouldFire();

	// Deep copy Weapon List and Current Weapon
	while (spOtherShipWeaponList != 0)
	{	
		bWeaponListIsEmpty = false;		// Show that at least one Weapon was added to the list

		// Create new Weapon
		spWeapon = new SWeapon;

		// If this is not the first Weapon in the List
		if (spPreviousWeapon != 0)
		{
			spPreviousWeapon->spNext = spWeapon;
			spWeapon->spPrevious = spPreviousWeapon;
		}
		// Else make the Weapon List Head point to this Weapon
		else
		{
			mspWeaponListHead = spWeapon;
			mspWeaponListHead->spPrevious = 0;
		}

		// Copy Weapon info
		spWeapon->cpBullet	= new CBullet(*spOtherShipWeaponList->cpBullet);
		spWeapon->iAmmo		= spOtherShipWeaponList->iAmmo;
		spWeapon->spNext	= 0;

		// Save this Weapon
		spPreviousWeapon = spWeapon;

		// If other Ships Current Weapon is set
		if (spOtherShipCurrentWeapon != 0)
		{
			// If this should be the Current Weapon
			if (spWeapon->cpBullet->ReturnSpecificType() == spOtherShipCurrentWeapon->cpBullet->ReturnSpecificType())
			{
				// Make this the Current Weapon
				mspCurrentWeapon = spWeapon;
			}
		}

		// Move Other Ship Pointer to Next Weapon
		spOtherShipWeaponList = spOtherShipWeaponList->spNext;
	}
	
	// If Weapons were added to the list
	if (!bWeaponListIsEmpty)
	{
		mspWeaponListTail = spPreviousWeapon;	// Set the Tail to the last weapon in the list
	}

	
	// Deep Copy Item List
	while (spOtherShipItemList != 0)
	{
		// Create Item
		spItem = new SItem;

		// If this is not the first Item in the List
		if (mspItemListHead != 0)
		{
			// Make Previous Items Next point to this Item
			spPreviousItem->spNext = spItem;
		}
		// Else make the Item List Head point to this Item
		else
		{
			mspItemListHead = spItem;
		}

		// Copy Item info
		spItem->iItemType = spOtherShipItemList->iItemType;

		// Save this as the Previous Weapon for Next iteration
		spPreviousItem = spItem;

		// It other Ships Current Item is set
		if (spOtherShipCurrentItem != 0)
		{
			// If this should be the Current Item
			if (spItem->iItemType == spOtherShipCurrentItem->iItemType)
			{
				// Set the Current Item to this Item
				mspCurrentItem = spItem;
			}
		}

		// Move Other Ships Item pointer to the Next Item
		spOtherShipItemList = spOtherShipItemList->spNext;
	}

	// Save how many Items are in the List
	miItemCount = cShipToCopyFrom.ReturnNumberOfItemsInList();
	

	// Deep copy AI Patterns and Current Pattern
	while (spOtherShipPatternList != 0)
	{	
		// Create new Pattern
		spPattern = new SPattern;

		// If this is not the first Pattern in the List
		if (mspPatternListHead != 0)
		{
			spPatternListTail->spNext = spPattern;
		}
		// Else make the Weapon List Head point to this Weapon
		else
		{
			mspPatternListHead = spPattern;
		}

		// Copy Pattern info
		spPattern->eMovementPattern		= spOtherShipPatternList->eMovementPattern;
		spPattern->eFirePattern			= spOtherShipPatternList->eFirePattern;
		spPattern->ulTimeToDoPatternFor	= spOtherShipPatternList->ulTimeToDoPatternFor;
		spPattern->iXAvoidRange			= spOtherShipPatternList->iXAvoidRange;
		spPattern->iXTrackRange			= spOtherShipPatternList->iXTrackRange;
		spPattern->iXCloseFireRange		= spOtherShipPatternList->iXCloseFireRange;
		spPattern->iXFarFireRange		= spOtherShipPatternList->iXFarFireRange;
		spPattern->iYAvoidRange			= spOtherShipPatternList->iYAvoidRange;
		spPattern->iYTrackRange			= spOtherShipPatternList->iYTrackRange;
		spPattern->iYCloseFireRange		= spOtherShipPatternList->iYCloseFireRange;
		spPattern->iYFarFireRange		= spOtherShipPatternList->iYFarFireRange;
		spPattern->iTrackingPrecision	= spOtherShipPatternList->iTrackingPrecision;
		spPattern->iPatternID			= spOtherShipPatternList->iPatternID;
		spPattern->spNext				= 0;

		// If this Pattern ID is higher than our counter
		if (miPatternIDCounter < (spPattern->iPatternID + 1))
		{
			// Save this as the highest Pattern ID
			miPatternIDCounter = spPattern->iPatternID + 1;
		}

		// Save that this is the currently the Last Pattern in the List
		spPatternListTail = spPattern;

		// If other Ships Current Pattern is set
		if (spOtherShipCurrentPattern != 0)
		{
			// If this should be the Current Pattern
			if (spPattern->iPatternID == spOtherShipCurrentPattern->iPatternID)
			{
				// Make this the Current Pattern
				mspCurrentPattern = spPattern;
				eCurrentMovementPattern = spPattern->eMovementPattern;
			}
		}

		// Move Other Ship Pointer to Next Pattern
		spOtherShipPatternList = spOtherShipPatternList->spNext;
	}
}


//********** Movement Functions **********

// Move Ship to specified position
void CShip::MoveTo(float fX, float fY, float fZ)
{
	mfX = fX;
	mfY = fY;
	mfZ = fZ;

	mcPicture.MoveCharacterTo((int)floorf(mfX), (int)floorf(mfY), (int)floorf(mfZ));
}

// Move Ship X to specified position
void CShip::MoveXTo(float fX)
{
	mfX = fX;
	mcPicture.MoveCharacterXTo((int)floorf(mfX));
}

// Move Ship Y to specified position
void CShip::MoveYTo(float fY)
{
	mfY = fY;
	mcPicture.MoveCharacterYTo((int)floorf(mfY));
}

// Move Ship Z to specified position
void CShip::MoveZTo(float fZ)
{
	mfZ = fZ;
	mcPicture.MoveCharacterZTo((int)floorf(mfZ));
}

// Move Ship relative to current position
void CShip::MoveToRelative(float fX, float fY, float fZ)
{
	mfX += fX;
	mfY += fY;
	mfZ += fZ;
	mcPicture.MoveCharacterTo((int)floorf(mfX), (int)floorf(mfY), (int)floorf(mfZ));
}

// Move Ship X relative to current position
void CShip::MoveXRelative(float fX)
{
	mfX += fX;
	mcPicture.MoveCharacterXTo((int)floorf(mfX));
}

// Move Ship Y relative to current position
void CShip::MoveYRelative(float fY)
{
	mfY += fY;
	mcPicture.MoveCharacterYTo((int)floorf(mfY));
}

// Move Ship Z relative to current position
void CShip::MoveZRelative(float fZ)
{
	mfZ += fZ;
	mcPicture.MoveCharacterZTo((int)floorf(mfZ));
}

// Return Ships Coordinates
void CShip::ReturnCoordinates(float& fX, float& fY, float& fZ) const
{
	fX = mfX;
	fY = mfY;
	fZ = mfZ;
}

// Return Ships X Coordinate
float CShip::ReturnXCoordinate() const
{
	return mfX;
}

// Return Ships Y Coordinate
float CShip::ReturnYCoordinate() const
{
	return mfY;
}

// Return Ships Z Coordinate
float CShip::ReturnZCoordinate() const
{
	return mfZ;
}

// Set Velocity
void CShip::SetVelocities(float fXVelocity, float fYVelocity, float fZVelocity)
{
	mfXVelocity = fXVelocity;
	mfYVelocity = fYVelocity;
	mfZVelocity = fZVelocity;
}

// Set X Velocity
void CShip::SetXVelocity(float fXVelocity)
{
	mfXVelocity = fXVelocity;
}

// Set Y Velocity
void CShip::SetYVelocity(float fYVelocity)
{
	mfYVelocity = fYVelocity;
}

// Set Z Velocity
void CShip::SetZVelocity(float fZVelocity)
{
	mfZVelocity = fZVelocity;
}

// Return Velocities
void CShip::ReturnVelocities(float& fXVelocity, float& fYVelocity, float& fZVelocity) const
{
	fXVelocity = mfXVelocity;
	fYVelocity = mfYVelocity;
	fZVelocity = mfZVelocity;
}

// Return X Velocity
float CShip::ReturnXVelocity() const
{
	return mfXVelocity;
}

// Return Y Velocity
float CShip::ReturnYVelocity() const
{
	return mfYVelocity;
}

// Return Z Velocity
float CShip::ReturnZVelocity() const
{
	return mfZVelocity;
}

// Set Max Velocities
void CShip::SetMaxVelocities(float fMaxXVelocity, float fMaxYVelocity, float fMaxZVelocity)
{
	mfMaxXVelocity = fMaxXVelocity;
	mfMaxYVelocity = fMaxYVelocity;
	mfMaxZVelocity = fMaxZVelocity;
}

// Set Max X Velocity
void CShip::SetMaxXVelocity(float fMaxXVelocity)
{
	mfMaxXVelocity = fMaxXVelocity;
}

// Set Max Y Velocity
void CShip::SetMaxYVelocity(float fMaxYVelocity)
{
	mfMaxYVelocity = fMaxYVelocity;
}

// Set Max Z Velocity
void CShip::SetMaxZVelocity(float fMaxZVelocity)
{
	mfMaxZVelocity = fMaxZVelocity;
}

// Return Max Velocities
void CShip::ReturnMaxVelocities(float& fMaxXVelocity, float& fMaxYVelocity, float& fMaxZVelocity) const
{
	fMaxXVelocity = mfMaxXVelocity;
	fMaxYVelocity = mfMaxYVelocity;
	fMaxZVelocity = mfMaxZVelocity;
}

// Return Max X Velocity
float CShip::ReturnMaxXVelocity() const
{
	return mfMaxXVelocity;
}

// Return Max Y Velocity
float CShip::ReturnMaxYVelocity() const
{
	return mfMaxYVelocity;
}

// Return Max Z Velocity
float CShip::ReturnMaxZVelocity() const
{
	return mfMaxZVelocity;
}

// Set Min Velocities
void CShip::SetMinVelocities(float fMinXVelocity, float fMinYVelocity, float fMinZVelocity)
{
	mfMinXVelocity = fMinXVelocity;
	mfMinYVelocity = fMinYVelocity;
	mfMinZVelocity = fMinZVelocity;
}

// Set Min X Velocity
void CShip::SetMinXVelocity(float fMinXVelocity)
{
	mfMinXVelocity = fMinXVelocity;
}

// Set Min Y Velocity
void CShip::SetMinYVelocity(float fMinYVelocity)
{
	mfMinYVelocity = fMinYVelocity;
}

// Set Min Z Velocity
void CShip::SetMinZVelocity(float fMinZVelocity)
{
	mfMinZVelocity = fMinZVelocity;
}

// Return Min Velocities
void CShip::ReturnMinVelocities(float& fMinXVelocity, float& fMinYVelocity, float& fMinZVelocity) const
{
	fMinXVelocity = mfMinXVelocity;
	fMinYVelocity = mfMinYVelocity;
	fMinZVelocity = mfMinZVelocity;
}

// Return Min X Velocity
float CShip::ReturnMinXVelocity() const
{
	return mfMinXVelocity;
}

// Return Min Y Velocity
float CShip::ReturnMinYVelocity() const
{
	return mfMinYVelocity;
}

// Return Min Z Velocity
float CShip::ReturnMinZVelocity() const
{
	return mfMinZVelocity;
}

// Set Accelerations
// Acceleration is used to increase/decrease the velocity every time MoveShip() is called
// If you do not want the Ship to speed up/slow down, set Accelerations to zero
// NOTE: Requires UpdateShipsPosition() to be called each frame to actually move the ship
// NOTE: When UpdateShipsPosition() is called Velocity is automatically increased/decreased by current Acceleration
void CShip::SetAccelerations(float fXAcceleration, float fYAcceleration, float fZAcceleration)
{
	mfXAcceleration = fXAcceleration;
	mfYAcceleration = fYAcceleration;
	mfZAcceleration = fZAcceleration;
}

// Set X Acceleration
void CShip::SetXAcceleration(float fXAcceleration)
{
	mfXAcceleration = fXAcceleration;
}

// Set Y Acceleration
void CShip::SetYAcceleration(float fYAcceleration)
{
	mfYAcceleration = fYAcceleration;
}

// Set Z Acceleration
void CShip::SetZAcceleration(float fZAcceleration)
{
	mfZAcceleration = fZAcceleration;
}
		
// Return Accelerations
void CShip::ReturnAccelerations(float& fXAcceleration, float& fYAcceleration, float& fZAcceleration) const
{
	fXAcceleration = mfXAcceleration;
	fYAcceleration = mfYAcceleration;
	fZAcceleration = mfZAcceleration;
}

// Return X Acceleration
float CShip::ReturnXAcceleration() const
{
	return mfXAcceleration;
}

// Return Y Acceleration
float CShip::ReturnYAcceleration() const
{
	return mfYAcceleration;
}

// Return Z Acceleration
float CShip::ReturnZAcceleration() const
{
	return mfZAcceleration;
}

// Set Max Accelerations
void CShip::SetMaxAccelerations(float fMaxXAcceleration, float fMaxYAcceleration, float fMaxZAcceleration)
{
	mfMaxXAcceleration = fMaxXAcceleration;
	mfMaxYAcceleration = fMaxYAcceleration;
	mfMaxZAcceleration = fMaxZAcceleration;
}

// Set Max X Acceleration
void CShip::SetMaxXAcceleration(float fMaxXAcceleration)
{
	mfMaxXAcceleration = fMaxXAcceleration;
}

// Set Max Y Acceleration
void CShip::SetMaxYAcceleration(float fMaxYAcceleration)
{
	mfMaxYAcceleration = fMaxYAcceleration;
}

// Set Max Z Acceleration
void CShip::SetMaxZAcceleration(float fMaxZAcceleration)
{
	mfMaxZAcceleration = fMaxZAcceleration;
}

// Return Max Accelerations
void CShip::ReturnMaxAccelerations(float& fMaxXAcceleration, float& fMaxYAcceleration, float& fMaxZAcceleration) const
{
	fMaxXAcceleration = mfMaxXAcceleration;
	fMaxYAcceleration = mfMaxYAcceleration;
	fMaxZAcceleration = mfMaxZAcceleration;
}

// Return Max X Acceleration
float CShip::ReturnMaxXAcceleration() const
{
	return mfMaxXAcceleration;
}

// Return Max Y Acceleration
float CShip::ReturnMaxYAcceleration() const
{
	return mfMaxYAcceleration;
}

// Return Max Z Acceleration
float CShip::ReturnMaxZAcceleration() const
{
	return mfMaxZAcceleration;
}

// Set Min Accelerations
void CShip::SetMinAccelerations(float fMinXAcceleration, float fMinYAcceleration, float fMinZAcceleration)
{
	mfMinXAcceleration = fMinXAcceleration;
	mfMinYAcceleration = fMinYAcceleration;
	mfMinZAcceleration = fMinZAcceleration;
}

// Set Min X Acceleration
void CShip::SetMinXAcceleration(float fMinXAcceleration)
{
	mfMinXAcceleration = fMinXAcceleration;
}

// Set Min Y Acceleration
void CShip::SetMinYAcceleration(float fMinYAcceleration)
{
	mfMinYAcceleration = fMinYAcceleration;
}

// Set Min Z Acceleration
void CShip::SetMinZAcceleration(float fMinZAcceleration)
{
	mfMinZAcceleration = fMinZAcceleration;
}

// Return Min Accelerations
void CShip::ReturnMinAccelerations(float& fMinXAcceleration, float& fMinYAcceleration, float& fMinZAcceleration) const
{
	fMinXAcceleration = mfMinXAcceleration;
	fMinYAcceleration = mfMinYAcceleration;
	fMinZAcceleration = mfMinZAcceleration;
}

// Return Min X Acceleration
float CShip::ReturnMinXAcceleration() const
{
	return mfMinXAcceleration;
}

// Return Min Y Acceleration
float CShip::ReturnMinYAcceleration() const
{
	return mfMinYAcceleration;
}

// Return Min Z Acceleration
float CShip::ReturnMinZAcceleration() const
{
	return mfMinZAcceleration;
}

// Set ships Normal Accelerations (Are not used in UpdateShipsPosition())
// These are just used to hold the ships usual Accelerations
void CShip::SetNormalAccelerations(float fNormalXAcceleration, float fNormalYAcceleration, float fNormalZAcceleration)
{
	mfNormalXAcceleration = fNormalXAcceleration;
	mfNormalYAcceleration = fNormalYAcceleration;
	mfNormalZAcceleration = fNormalZAcceleration;
}

// Set ships Normal X Acceleration
void CShip::SetNormalXAcceleration(float fNormalXAcceleration)
{
	mfNormalXAcceleration = fNormalXAcceleration;
}

// Set ships Normal Y Acceleration
void CShip::SetNormalYAcceleration(float fNormalYAcceleration)
{
	mfNormalYAcceleration = fNormalYAcceleration;
}

// Set ships Normal Z Acceleration
void CShip::SetNormalZAcceleration(float fNormalZAcceleration)
{
	mfNormalZAcceleration = fNormalZAcceleration;
}

// Return ships Normal Accelerations
void CShip::ReturnNormalAccelerations(float& fNormalXAcceleration, float& fNormalYAcceleration, float& fNormalZAcceleration) const
{
	fNormalXAcceleration = mfNormalXAcceleration;
	fNormalYAcceleration = mfNormalYAcceleration;
	fNormalZAcceleration = mfNormalZAcceleration;
}

// Return ships Normal X Acceleration
float CShip::ReturnNormalXAcceleration() const
{
	return mfNormalXAcceleration;
}

// Return ships Normal Y Acceleration
float CShip::ReturnNormalYAcceleration() const
{
	return mfNormalYAcceleration;
}

// Return ships Normal Z Acceleration
float CShip::ReturnNormalZAcceleration() const
{
	return mfNormalZAcceleration;
}

// Set Frictions
// Frictions will slow the ship down no matter what direction it is going
// Zero friction means the ship will not slow down on its own
// NOTE: Requires UpdateShipsPosition() to be called each frame to have an effect
// NOTE: Specified friction X will slow ship down by X pixels per second
void CShip::SetFrictions(float fXFriction, float fYFriction, float fZFriction)
{
	mfXFriction = fXFriction;
	mfYFriction = fYFriction;
	mfZFriction = fZFriction;
}

// Set X Friction
void CShip::SetXFriction(float fXFriction)
{
	mfXFriction = fXFriction;
}

// Set Y Friction
void CShip::SetYFriction(float fYFriction)
{
	mfYFriction = fYFriction;
}

// Set Z Friction
void CShip::SetZFriction(float fZFriction)
{
	mfZFriction = fZFriction;
}

// Return Frictions
void CShip::ReturnFrictions(float& fXFriction, float& fYFriction, float& fZFriction) const
{
	fXFriction = mfXFriction;
	fYFriction = mfYFriction;
	fZFriction = mfZFriction;
}

// Return X Friction
float CShip::ReturnXFriction() const
{
	return mfXFriction;
}

// Return Y Friction
float CShip::ReturnYFriction() const
{
	return mfYFriction;
}

// Return Z Friction
float CShip::ReturnZFriction() const
{
	return mfZFriction;
}


// Updates Ships Position and Picture
// Moves Ship by specified Velocity amounts, which is calculated using the Acceleration values
// NOTE: This should be called every frame
// NOTE: ulAmountOfTimeSinceLastUpdate should be in milliseconds (1000ms = 1s)
void CShip::Update(unsigned long ulAmountOfTimeSinceLastUpdate)
{
	float fNewXVelocity = 0, fNewYVelocity = 0, fNewZVelocity = 0;
	float fXFrictionImpact = 0, fYFrictionImpact = 0, fZFrictionImpact = 0;
	float fAmountOfTimeSinceLastUpdateInSeconds = 0;

	// Update Ships Picture
	mcPicture.UpdatePicture(ulAmountOfTimeSinceLastUpdate);

	// Put TimeSinceLastUpdate in terms of seconds instead of milliseconds
	fAmountOfTimeSinceLastUpdateInSeconds = ulAmountOfTimeSinceLastUpdate / 1000.0;

	// Calculate new Velocities from Accelerations
	// Acceleration = (Vfinal - Vinitial) / Time   =>  Vfinal = (A * T) + Vinitial 
	fNewXVelocity = (mfXAcceleration * fAmountOfTimeSinceLastUpdateInSeconds) + mfXVelocity;
	fNewYVelocity = (mfYAcceleration * fAmountOfTimeSinceLastUpdateInSeconds) + mfYVelocity;
	fNewZVelocity = (mfZAcceleration * fAmountOfTimeSinceLastUpdateInSeconds) + mfZVelocity;

	// Calculate how much impact Friction should have
	fXFrictionImpact = mfXFriction * fAmountOfTimeSinceLastUpdateInSeconds;
	fYFrictionImpact = mfYFriction * fAmountOfTimeSinceLastUpdateInSeconds;
	fZFrictionImpact = mfZFriction * fAmountOfTimeSinceLastUpdateInSeconds;


	// Process Friction impact (Do nothing if Velocity is zero)
	
	// Slow down X Velocity by Friction amount
	if (fNewXVelocity > 0)
	{
		fNewXVelocity -= fXFrictionImpact;

		// If we changed Velocity to opposite direction
		if (fNewXVelocity < 0)
		{
			fNewXVelocity = 0;
		}
	}else if (fNewXVelocity < 0)
	{
		fNewXVelocity += fXFrictionImpact;

		// If we changed Velocity to opposite direction
		if (fNewXVelocity > 0)
		{
			fNewXVelocity = 0;
		}
	}

	// Slow down Y Velocity by Friction amount
	if (fNewYVelocity > 0)
	{
		fNewYVelocity -= fYFrictionImpact;

		// If we changed Velocity to opposite direction
		if (fNewYVelocity < 0)
		{
			fNewYVelocity = 0;
		}
	}else if (fNewYVelocity < 0)
	{
		fNewYVelocity += fYFrictionImpact;

		// If we changed Velocity to opposite direction
		if (fNewYVelocity > 0)
		{
			fNewYVelocity = 0;
		}
	}

	// Slow down Z Velocity by Friction amount
	if (fNewZVelocity > 0)
	{
		fNewZVelocity -= fZFrictionImpact;

		// If we changed Velocity to opposite direction
		if (fNewZVelocity < 0)
		{
			fNewZVelocity = 0;
		}
	}else if (fNewZVelocity < 0)
	{
		fNewZVelocity += fZFrictionImpact;

		// If we changed Velocity to opposite direction
		if (fNewZVelocity > 0)
		{
			fNewZVelocity = 0;
		}
	}


	// Make sure Velocities are not out of range

	// Make sure X Velocity is not out of range
	if (fNewXVelocity > mfMaxXVelocity)
	{
		fNewXVelocity = mfMaxXVelocity;
	}else if (fNewXVelocity < mfMinXVelocity)
	{
		fNewXVelocity = mfMinXVelocity;
	}

	// Make sure Y Velocity is not out of range
	if (fNewYVelocity > mfMaxYVelocity)
	{
		fNewYVelocity = mfMaxYVelocity;
	}else if (fNewYVelocity < mfMinYVelocity)
	{
		fNewYVelocity = mfMinYVelocity;
	}

	// Make sure Z Velocity is not out of range
	if (fNewZVelocity > mfMaxZVelocity)
	{
		fNewZVelocity = mfMaxZVelocity;
	}else if (fNewZVelocity < mfMinZVelocity)
	{
		fNewZVelocity = mfMinZVelocity;
	}

	// Set New Velocities
	mfXVelocity = fNewXVelocity;
	mfYVelocity = fNewYVelocity;
	mfZVelocity = fNewZVelocity;


	// Calculate new Relative Position from Velocities
	// Velocity = Distance / Time  =>  D = V * T
	MoveXRelative((mfXVelocity * fAmountOfTimeSinceLastUpdateInSeconds));
	MoveYRelative((mfYVelocity * fAmountOfTimeSinceLastUpdateInSeconds));
	MoveZRelative((mfZVelocity * fAmountOfTimeSinceLastUpdateInSeconds));
}



//********** Ship Attributes (Heath/Shield/Status/etc) Functions **********

// Set Ships General Type (ie Player or Enemy)
void CShip::SetGeneralShipType(int iShipType)
{
	miGeneralShipType = iShipType;
}

// Return Ships General Type (ie Player or Enemy)
int CShip::ReturnGeneralShipType() const
{
	return miGeneralShipType;
}

// Set Ships Specific Type (ie Which Player or Enemy)
void CShip::SetSpecificShipType(int iType)
{
	miSpecificShipType = iType;
}

// Return Ships Specific Type (ie Which Player or Enemy)
int CShip::ReturnSpecificShipType() const
{
	return miSpecificShipType;
}

// Set Health
void CShip::SetHealth(int iHealth)
{
	// Set Health
	miHealth = iHealth;

	// If Health is too high
	if (miHealth > miMaxHealth && miMaxHealth != -1)
	{
		// Set Health to the Max Health it can be
		miHealth = miMaxHealth;
	}
}

// Add HealthToAdd to Ships Health
void CShip::SetHealthRelative(int iHealthToAdd)
{
	// Set Health
	miHealth += iHealthToAdd;

	// If Health is too high
	if (miHealth > miMaxHealth && miMaxHealth != -1)
	{
		// Set Health to the Max Health it can be
		miHealth = miMaxHealth;
	}
}

// Return Health
int CShip::ReturnHealth() const
{
	return miHealth;
}

// Set Max Health
// Will only limit SetHealth() if MaxHealth is not -1
void CShip::SetMaxHealth(int iMaxHealth)
{
	// Set Max Health
	miMaxHealth = iMaxHealth;

	// If Health is too high
	if (miHealth > miMaxHealth && miMaxHealth != -1)
	{
		// Set Health to the Max Health it can be
		miHealth = miMaxHealth;
	}
}

// Return Max Health
int CShip::ReturnMaxHealth() const
{
	return miMaxHealth;
}

// Set Armor
void CShip::SetArmor(float fArmor)
{
	// Set Armor
	mfArmor = fArmor;

	// If Armor is too high
	if (mfArmor > mfMaxArmor && mfMaxArmor != -1)
	{
		// Set Armor to the Max it can be
		mfArmor = mfMaxArmor;
	}
}

// Return Armor
float CShip::ReturnArmor() const
{
	return mfArmor;
}

// Set Max Armor
// Will only limit SetArmor() if MaxArmor is not -1
void CShip::SetMaxArmor(float fMaxArmor)
{
	// Set Max Armor
	mfMaxArmor = fMaxArmor;

	// If Armor is too high
	if (mfArmor > mfMaxArmor && mfMaxArmor != -1)
	{
		// Set Armor to the Max it can be
		mfArmor = mfMaxArmor;
	}
}

// Return Max Armor
float CShip::ReturnMaxArmor() const
{
	return mfMaxArmor;
}

// Set Shield Strength
void CShip::SetShieldStrength(float fShieldStrength)
{
	// Set the Shield Strength
	mfShieldStrength = fShieldStrength;

	// If ShieldStrength is too high
	if (mfShieldStrength > mfMaxShieldStrength && mfMaxShieldStrength != -1)
	{
		mfShieldStrength = mfMaxShieldStrength;
	}
}

// Return Shield Strength
float CShip::ReturnShieldStrength() const
{
	return mfShieldStrength;
}

// Set Max Shield Strength
// Will only limit SetShieldStrength() if MaxShieldStrength is not -1
void CShip::SetMaxShieldStrength(float fMaxShieldStrength)
{
	// Set the Max Shield Strength
	mfMaxShieldStrength = fMaxShieldStrength;

	// If ShieldStrength is too high
	if (mfShieldStrength > mfMaxShieldStrength && mfMaxShieldStrength != -1)
	{
		mfShieldStrength = mfMaxShieldStrength;
	}
}

// Return Max Shield Strength
float CShip::ReturnMaxShieldStrength() const
{
	return mfMaxShieldStrength;
}

// Set Ships Status
void CShip::SetStatus(int iStatus)
{
	miStatus = iStatus;
}

// Return Ships Status
int CShip::ReturnStatus() const
{
	return miStatus;
}

// Set number of Lives
void CShip::SetNumberOfLives(int iLives)
{
	// Set Number of Lives
	miLives = iLives;

	// If Lives is too high
	if (miLives > miMaxLives && miMaxLives != -1)
	{
		// Set Lives to the Max it can be
		miLives = miMaxLives;
	}
}

// Return number of Lives
int CShip::ReturnNumberOfLives() const
{
	return miLives;
}

// Set Max Number Of Lives
// Will only limit SetNumberOfLives() if MaxLives is not zero
void CShip::SetMaxNumberOfLives(int iMaxLives)
{
	// Set Max Lives
	miMaxLives = iMaxLives;

	// If Lives is too high
	if (miLives > miMaxLives && miMaxLives != -1)
	{
		// Set Lives to the Max it can be
		miLives = miMaxLives;
	}
}

// Return Max Number Of Lives
int CShip::ReturnMaxNumberOfLives() const
{
	return miMaxLives;
}

// Set number of Points ship is worth
void CShip::SetNumberOfPoints(unsigned int uiPoints)
{
	muiPoints = uiPoints;
}

// Add PointsToAdd to current number of Points
// NOTE: Makes sure Points do not go below zero
void CShip::SetNumberOfPointsRelative(int iPointsToAdd)
{
	// If this would caues Points to go below zero
	if ((muiPoints < abs(iPointsToAdd)) && (iPointsToAdd < 0))
	{
		// Set Points to zero
		muiPoints = 0;
	}
	// Else simply add the PointsToAdd to the current number of Points
	else
	{
		// Add PointsToAdd to current number of Points
		muiPoints += iPointsToAdd;
	}
}

// Return number of Points ship is worth
unsigned int CShip::ReturnNumberOfPoints() const
{
	return muiPoints;
}

// Set if Ship is colliding with another Ship or not
void CShip::SetIfShipIsColliding(bool bIsCollision)
{
	mbCollision = bIsCollision;
}

// Return if Ship is colliding with another Ship or not
bool CShip::ReturnIfShipIsColliding() const
{
	return mbCollision;
}



//********** Weapon Functions **********

// Add a Weapon in the end of the Weapon List (if not already in Weapon List)
// NOTE: Add Weapon returns False if Weapon is already in List
// NOTE: Weapons Type is equal to the given Bullets Type
// NOTE: A new Bullet is created from the given Bullet Model, so the given Bullet 
// Model will not be modified in any way
bool CShip::AddWeapon(CBullet cBulletModel, int iAmountOfAmmo)
{
	SWeapon* spWeapon			= mspWeaponListHead;
	SWeapon* spPreviousWeapon	= 0;

	// While we haven't reached the end of the Weapon List
	while (spWeapon != 0 && spWeapon->cpBullet->ReturnSpecificType() != cBulletModel.ReturnSpecificType())
	{
		spPreviousWeapon = spWeapon;
		spWeapon = spWeapon->spNext;
	}

	// If this Weapon is already in the list
	if (spWeapon != 0)
	{
		return false;		// Return false since Weapon is already in list
	}

	// Create the Weapon
	spWeapon = new SWeapon;

	// If this is the first Weapon to be added to the list
	if (spPreviousWeapon == 0)
	{
		// Put Weapon at front of Weapon List
		mspWeaponListHead = spWeapon;

		// Also make this the Current Weapon since it's the only one in the List
		mspCurrentWeapon = spWeapon;
	}else
	{
		// Put Weapon at end of Weapon List
		spPreviousWeapon->spNext = spWeapon;
	}
	
	// Set Weapons data
	spWeapon->cpBullet = new CBullet(cBulletModel);
	spWeapon->iAmmo		= iAmountOfAmmo;
	spWeapon->spNext	= 0;
	spWeapon->spPrevious= spPreviousWeapon;	// If this is the Head, it's Previous will be set to NULL

	// Set Tail to this Weapon
	mspWeaponListTail = spWeapon;

	return true;
}

// Delete a Weapon from the List
// Returns false if Weapon is not found in List
bool CShip::DeleteWeapon(int iSpecificWeaponType)
{
	SWeapon* spWeapon = mspWeaponListHead;
	SWeapon* spPreviousWeapon = 0;
	SWeapon* spWeapon2 = 0;

	// Search the Weapon List for specified Weapon
	while (spWeapon != 0 && spWeapon->cpBullet->ReturnSpecificType() != iSpecificWeaponType)
	{
		spPreviousWeapon = spWeapon;
		spWeapon = spWeapon->spNext;
	}

	// If the Weapon wasn't found
	if (spWeapon == 0)
	{
		return false;
	}

	// If the Weapon is not at the end of the list
	if (spWeapon->spNext != 0)
	{
		// If we are Deleting the first Weapon in the list
		if (spPreviousWeapon == 0)
		{
			// Make the Head of the list point to the next Weapon
			mspWeaponListHead = mspWeaponListHead->spNext;

			// If there is another Weapon after the new Head
			if (mspWeaponListHead->spNext != 0)
			{
				// Make it's Previous point to the new Head
				spWeapon2 = mspWeaponListHead->spNext;
				spWeapon2->spPrevious = mspWeaponListHead;
			}
		}else
		{
			// Make the Previous Weapon point to the Next Weapon
			spPreviousWeapon->spNext = spWeapon->spNext;

			// Make the Next Weapons Previous point to the Previous Weapon
			spWeapon2 = spWeapon->spNext;
			spWeapon2->spPrevious = spPreviousWeapon;
		}
	}
	// Else if Weapon is at the end of the list
	else
	{
		// If there was a Previous Weapon (ie We are not deleting the Head)
		if (spPreviousWeapon != 0)
		{
			spPreviousWeapon->spNext = 0;
		}

		mspWeaponListTail = spPreviousWeapon;
	}

	// If we are deleting the Current Weapon
	if (mspCurrentWeapon == spWeapon)
	{
		// Set it to the first Weapon in the List
		mspCurrentWeapon = mspWeaponListHead;
	}

	// Delete the Weapon
	delete spWeapon->cpBullet;
	delete spWeapon;

	// Return that Weapon was Deleted successfully
	return true;
}

// Delete All Weapons that have the specified General Type
// Returns false if no Weapons of the specifiec Type are found
bool CShip::DeleteAllWeaponsOfGeneralType(int iGeneralType)
{
	SWeapon* spWeapon = mspWeaponListHead;	// Used to traverse Weapon List
	int iSpecificType;						// Holds Weapon to deletes Specific Type
	bool bOneWeaponFound = false;			// Holds whether a Weapon of the General Type was found or not

	// Cycle through all Weapons in the List
	while (spWeapon != 0)
	{
		// If this Weapon should be deleted
		if (spWeapon->cpBullet->ReturnGeneralType() == iGeneralType)
		{
			// Save it's Specific Type
			iSpecificType = spWeapon->cpBullet->ReturnSpecificType();

			// Move to Next Weapon
			spWeapon = spWeapon->spNext;

			// Delete the Weapon
			DeleteWeapon(iSpecificType);

			// Mark that we found one Weapon
			bOneWeaponFound = true;
		}
		// Else just move to the Next Weapon
		else
		{
			spWeapon = spWeapon->spNext;
		}
	}

	// Return if any Weapons were Deleted
	return bOneWeaponFound;
}

// Find if the specified Weapon is in the Weapon List or not
bool CShip::WeaponIsInList(int iSpecificWeaponType) const
{
	SWeapon* spWeapon = mspWeaponListHead;

	// Search for Weapon in the List
	while (spWeapon != 0 && spWeapon->cpBullet->ReturnSpecificType() != iSpecificWeaponType)
	{
		spWeapon = spWeapon->spNext;
	}

	// If the Weapon is not in the List
	if (spWeapon == 0)
	{
		return false;
	}

	// Else it was found
	return true;
}

// Set the Current Weapon to a specific Weapon
// NOTE: Returns false if WeaponType not found
bool CShip::SetCurrentWeapon(int iSpecificWeaponType)
{
	SWeapon* spWeapon = mspWeaponListHead;

	// Search for Weapon in the List
	while (spWeapon != 0 && spWeapon->cpBullet->ReturnSpecificType() != iSpecificWeaponType)
	{
		spWeapon = spWeapon->spNext;
	}

	// If the Weapon is not in the List
	if (spWeapon == 0)
	{
		return false;
	}

	// Else Weapon was found so set Current Weapon to this Weapon
	mspCurrentWeapon = spWeapon;

	// Return success
	return true;
}

// Set the Current Weapon to a the first Weapon in the List
// NOTE: Returns false if Weapon List is empty
bool CShip::SetCurrentWeaponToFirstWeaponInList()
{
	// If Weapon List is empty
	if (mspWeaponListHead == 0)
	{
		return false;
	}

	// Else set the Current Weapon to the first Weapon in the List
	mspCurrentWeapon = mspWeaponListHead;

	// Return success
	return true;
}
	
// Return a Weapon in the Weapon List
// NOTE: Returns 0 if Weapon not found
CBullet* CShip::ReturnWeaponsBullet(int iSpecificWeaponType) const
{
	SWeapon* spWeapon = mspWeaponListHead;

	// Search for Weapon in the List
	while (spWeapon != 0 && spWeapon->cpBullet->ReturnSpecificType() != iSpecificWeaponType)
	{
		spWeapon = spWeapon->spNext;
	}

	// If the Weapon is not in the List
	if (spWeapon == 0)
	{
		return 0;
	}

	// Else Weapon was found so return it
	return spWeapon->cpBullet;
}

// Return the Current Weapon
// NOTE: Returns 0 if Weapon not found
CBullet* CShip::ReturnCurrentWeaponsBullet() const
{
	// If Current Weapon is NULL
	if (mspCurrentWeapon == 0)
	{
		return 0;
	}

	// Else Current Weapon was found so return it
	return mspCurrentWeapon->cpBullet;
}

// Return the Current Weapons Specific Type
// NOTE: Returns -1 if Current Weapon not currently set
int CShip::ReturnCurrentWeaponsSpecificType() const
{
	// If Current Weapon isn't set yet
	if (mspCurrentWeapon == 0)
	{
		return -1;
	}

	return mspCurrentWeapon->cpBullet->ReturnSpecificType();
}

// Return the Current Weapons General Type
// NOTE: Returns -1 if Current Weapon not currently set
int CShip::ReturnCurrentWeaponsGeneralType() const
{
	// If Current Weapon isn't set yet
	if (mspCurrentWeapon == 0)
	{
		return -1;
	}

	return mspCurrentWeapon->cpBullet->ReturnGeneralType();
}

// Returns if the Current Weapon has an Alternate Effect than doing damage
// NOTE: Returns false if Current Weapon doesn't exist
bool CShip::ReturnIfCurrentWeaponHasAlternateEffect() const
{
	// If Current Weapon does not exist
	if (mspCurrentWeapon == 0)
	{
		return false;
	}

	// Else since Current Weapon exists return if it has an Alternate Effect
	return mspCurrentWeapon->cpBullet->ReturnIfBulletHasAlternateEffect();
}

// Returns time Delay until Weapon can be fired again
// Returns zero if Current Weapon does not exist
unsigned long CShip::ReturnCurrentWeaponsFireDelay() const
{
	// If Current Weapon does not exist
	if (mspCurrentWeapon == 0)
	{
		return 0;
	}

	// Else the Weapon exists so return its Fire Delay Time
	return mspCurrentWeapon->cpBullet->ReturnFireDelay();
}

// Move the Current Weapon to the next Weapon in the List (wraps around end of list)
bool CShip::NextWeapon()
{
	// If the Current Weapon isn't set yet
	if (mspCurrentWeapon == 0)
	{
		return false;
	}

	// If the Current Weapon is last Weapon in the list
	if (mspCurrentWeapon->spNext == 0)
	{
		// Wrap Current Weapon around to the first Weapon in the list
		mspCurrentWeapon = mspWeaponListHead;
	}else
	{
		mspCurrentWeapon = mspCurrentWeapon->spNext;
	}

	return true;
}

// Move the Current Weapon to the previous Weapon in the List (wraps around the list)
bool CShip::PreviousWeapon()
{
	// If the Current Weapon isn't set yet
	if (mspCurrentWeapon == 0)
	{
		return false;
	}

	// If the Current Weapon is first Weapon in the list
	if (mspCurrentWeapon == mspWeaponListHead)
	{
		// Wrap Current Weapon around to the last Weapon in the list
		mspCurrentWeapon = mspWeaponListTail;
	}else
	{
		mspCurrentWeapon = mspCurrentWeapon->spPrevious;
	}

	return true;
}

// Set a Weapons amount of Ammo
// NOTE: Returns false if Weapon not found
bool CShip::SetAmmo(int iSpecificWeaponType, int iAmountOfAmmo)
{
	SWeapon* spWeapon = mspWeaponListHead;	// Set Weapon Pointer to the front of the List

	// Search Weapon List for the specified Weapon
	while (spWeapon != 0 && spWeapon->cpBullet->ReturnSpecificType() != iSpecificWeaponType)
	{
		spWeapon = spWeapon->spNext;
	}

	// If specified Weapon is not in the List
	if (spWeapon == 0)
	{
		return false;
	}
	// Else the Weapon was found

	// Set the Weapons amount of Ammo
	spWeapon->iAmmo = iAmountOfAmmo;

	// Return success
	return true;
}

// Set Current Weapons amount of Ammo
// NOTE: Returns false if Current Weapon does not exist
bool CShip::SetCurrentWeaponsAmmo(int iAmountOfAmmo)
{
	// If Current Weapon doesn't exist yet
	if (mspCurrentWeapon == 0)
	{
		return false;
	}

	// Else Current Weapon exists so set its Amount of Ammo
	mspCurrentWeapon->iAmmo = iAmountOfAmmo;

	// Return success
	return true;
}

// Add to a Weapons current amount of Ammo
// NOTE: If subtracting Ammo, total will not go below zero
// NOTE: Returns false if Weapon not found
bool CShip::SetAmmoRelative(int iSpecificWeaponType, int iAmountOfAmmoToAdd)
{
	SWeapon* spWeapon = mspWeaponListHead;	// Set Weapon Pointer to the front of the List

	// Search Weapon List for the specified Weapon
	while (spWeapon != 0 && spWeapon->cpBullet->ReturnSpecificType() != iSpecificWeaponType)
	{
		spWeapon = spWeapon->spNext;
	}

	// If specified Weapon is not in the List
	if (spWeapon == 0)
	{
		return false;
	}
	// Else the Weapon was found

	// Add to the Weapons amount of Ammo
	spWeapon->iAmmo += iAmountOfAmmoToAdd;

	// Make sure Ammo does not go below zero
	if (spWeapon->iAmmo < 0)
	{
		spWeapon->iAmmo = 0;
	}

	// Return success
	return true;
}

// Add to Current Weapons current amount of Ammo
// NOTE: If subtracting Ammo, total will not go below zero
// NOTE: Returns false if Weapon not found
bool CShip::SetCurrentWeaponsAmmoRelative(int iAmountOfAmmoToAdd)
{
	// If Current Weapon doesn't exist yet
	if (mspCurrentWeapon == 0)
	{
		return false;
	}

	// Else add to Current Weapons amount of Ammo
	mspCurrentWeapon->iAmmo += iAmountOfAmmoToAdd;

	// Make sure Ammo does not go below zero
	if (mspCurrentWeapon->iAmmo < 0)
	{
		mspCurrentWeapon->iAmmo = 0;
	}

	// Return success
	return true;
}

// Return a Weapons amount of Ammo
// NOTE: Returns 0 if Weapon not found
int CShip::ReturnAmmo(int iSpecificWeaponType) const
{
	SWeapon* spWeapon = mspWeaponListHead;

	// Search for the specified Weapon
	while (spWeapon != 0 && spWeapon->cpBullet->ReturnSpecificType() != iSpecificWeaponType)
	{
		spWeapon = spWeapon->spNext;
	}

	// If the specified Weapon was not found
	if (spWeapon == 0)
	{
		return 0;
	}

	// Else it was found so return the amount of Ammo
	return spWeapon->iAmmo;
}

// Return Current Weapons amount of Ammo
// NOTE: Returns 0 if Current Weapon doesn't exist
int CShip::ReturnCurrentWeaponsAmmo() const
{
	// If Current Weapon does not exist
	if (mspCurrentWeapon == 0)
	{
		return 0;
	}

	// Else Current Weapon exists so return its amount of Ammo
	return mspCurrentWeapon->iAmmo;
}

// Return Current Weapons Bullets XYZ coordinates to be fired from
// X coordinate is centered to ship, Y coordinate is front of Ship
// NOTE: Returns false if there is no Current Weapon
bool CShip::ReturnCurrentWeaponsFireFromPosition(float& fX, float& fY, float& fZ)
{
	// If Current Weapon doesn't exist
	if (mspCurrentWeapon == 0)
	{
		return false;
	}

	// Return XYZ values
	fX = (ReturnXCoordinate() + (mcPicture.ReturnWidth() / 2) - (mspCurrentWeapon->cpBullet->mcPicture.ReturnWidth() / 2));
	fY = ReturnYCoordinate();
	fZ = 0;

	// Return success
	return true;
}

// Return Current Weapons Bullets X coordinate to be fired from
// X coordinate is centered to ship
// NOTE: Returns -1 if there is no Current Weapon
float CShip::ReturnCurrentWeaponsFireFromXPosition() const
{
	// If Current Weapon doesn't exist
	if (mspCurrentWeapon == 0)
	{
		return -1;
	}

	// Return X Coordinate
	return (ReturnXCoordinate() + (mcPicture.ReturnWidth() / 2) - (mspCurrentWeapon->cpBullet->mcPicture.ReturnWidth() / 2));
}

// Return Current Weapons Bullets Y coordinate to be fired from
// Y coordinate is same as top of Ship
// NOTE: Returns -1 if there is no Current Weapon
float CShip::ReturnCurrentWeaponsFireFromYPosition() const
{
	// If Current Weapon doesn't exist
	if (mspCurrentWeapon == 0)
	{
		return -1;
	}

	// Return Y Coordinate
	return ReturnYCoordinate();
}

// Return Current Weapons Bullets Z coordinate to be fired from
// NOTE: Returns -1 if there is no Current Weapon
float CShip::ReturnCurrentWeaponsFireFromZPosition() const
{
	// If Current Weapon doesn't exist
	if (mspCurrentWeapon == 0)
	{
		return -1;
	}

	// Return Z Coordinate
	return 0;
}



//********** Item Functions **********

// Add Item Type to List
// Returns false if Item Type is already in List
bool CShip::AddItem(int iItemType)
{
	SItem* spNewItem;			// Points to the new Item to create

	// If Item Type is already in List
	if (ItemIsInList(iItemType))
	{
		// Return that it is already in List
		return false;
	}
	// Else Item Type is not in List yet

	// Create new Item
	spNewItem = new SItem;
	spNewItem->iItemType = iItemType;

	// Place new Item in List

	// If Item List is empty
	if (mspItemListHead == 0)
	{
		// Place new Item in List
		mspItemListHead = spNewItem;
		spNewItem->spNext = 0;
	}
	// Else Item List is not Empty
	else
	{
		// Place new Item at start of List
		spNewItem->spNext = mspItemListHead;
		mspItemListHead = spNewItem;
	}

	// Increment number of Items in List
	miItemCount++;

	// Return success
	return true;
}

// Remove Item Type from List
// Returns false if Item Type is not found in the List
bool CShip::RemoveItem(int iItemType)
{
	SItem* spItem = mspItemListHead;	// Temp pointer to Items
	SItem* spPreviousItem = 0;			// Temp pointer to Previous Item

	// Traverse Item List until Item Type is found or end of Item List is reached
	while (spItem != 0 && spItem->iItemType != iItemType)
	{
		// Move to Next Item in List, saving the Previous one
		spPreviousItem = spItem;
		spItem = spItem->spNext;
	}

	// If Item Type was not found
	if (spItem == 0)
	{
		return false;
	}

	// Else remove the Item Type from the List

	// If Item to remove is the first in the list
	if (spItem == mspItemListHead)
	{
		// Move Head of List to next Item
		mspItemListHead = mspItemListHead->spNext;

		// Delete the Item
		delete spItem;
	}
	// Else Item is somewhere else in the List
	else
	{
		// Close hole where Item to Delete is
		spPreviousItem->spNext = spItem->spNext;

		// Dete the Item
		delete spItem;
	}

	// Decrement number of Items in List
	miItemCount--;

	// Return success
	return true;
}

// Returns Current Items Type
// Returns 0 if Current Item doesn't exist
int CShip::ReturnCurrentItemsType() const
{
	// If Current Item Exists
	if (CurrentItemExists())
	{
		return mspCurrentItem->iItemType;
	}
	
	// Else Current Item doesn't exist so return 0
	return 0;
}

// Set Current Item to the specified Type if it's in the List
// Returns false if specified Type is not in the List
bool CShip::SetCurrentItemToType(int iItemType)
{
	SItem* spItem = mspItemListHead;	// Temp pointer to Items

	// Traverse List looking for Item Type
	while (spItem != 0 && spItem->iItemType != iItemType)
	{
		spItem = spItem->spNext;
	}

	// If Item Type was not found
	if (spItem == 0)
	{
		return false;
	}

	// Else set this Item as the Current Item
	mspCurrentItem = spItem;

	// Return success
	return true;
}

// Return if Current Item exists or not
bool CShip::CurrentItemExists() const
{
	// If Current Item is not NULL
	if (mspCurrentItem != 0)
	{
		return true;
	}
	
	// Else Current Item doesn't exist
	return false;
}

// Moves Current Item to start of Item List
void CShip::MoveCurrentItemToStartOfItemList()
{
	mspCurrentItem = mspItemListHead;
}

// Moves Current Item to next Item in List
// Returns false if Current Item doesn't exist or Item List is empty
bool CShip::MoveCurrentItemToNextItem()
{
	// If Current Item doesn't exist or Item List is empty
	if (mspCurrentItem == 0 || mspItemListHead == 0)
	{
		return false;
	}

	// Else move Current Item to the next Item
	mspCurrentItem = mspCurrentItem->spNext;

	// Return success
	return true;
}

// Moves Current Item to next Item in List and wraps around to start of List if end is reached
// Returns false if Current Item doesn't exist or Item List is empty
bool CShip::MoveCurrentItemToNextItemWithWrap()
{
	// If Current Item doesn't exist or Item List is empty
	if (mspCurrentItem == 0 || mspItemListHead == 0)
	{
		return false;
	}

	// Else move Current Item to the next Item
	mspCurrentItem = mspCurrentItem->spNext;

	// If Current Item was already at the Last Item in the List
	if (mspCurrentItem == 0)
	{
		mspCurrentItem = mspItemListHead;
	}

	// Return success
	return true;
}

// Returns true if Item Type is in List, false if not
bool CShip::ItemIsInList(int iItemType) const
{
	SItem* spItem = mspItemListHead;	// Temp pointer to Items

	// Traverse Item List looking for Item Type
	while (spItem != 0 && spItem->iItemType != iItemType)
	{
		// Move to Next Item in List
		spItem = spItem->spNext;
	}

	// If Item Type was not found
	if (spItem == 0)
	{
		return false;
	}

	// Else Item Type was found so return success
	return true;
}

// Returns how many Items are in the Item List
int CShip::ReturnNumberOfItemsInList() const
{
	return miItemCount;
}

// Set Chance of Ship dropping an Item
// NOTE: Chance = 1 means a 1 in 1 (100%) chance of dropping, 2 is a 1 in 2 (50%) chance of dropping, etc
void CShip::SetChanceOfDroppingItem(unsigned int uiOneInXChanceOfDropping)
{
	muiChanceOfDropping = uiOneInXChanceOfDropping;
}

// Return Chance of Ship dropping an Item
unsigned int CShip::ReturnChanceOfDroppingItem() const
{
	return muiChanceOfDropping;
}

// Returns true if Ship Should Drop Item, false if not
// Returns false if ChanceOfDropping has not been set or is zero
bool CShip::ShipShouldDropItem() const
{
	// Make sure we are not dividing by zero OR Item List is Empty
	if (muiChanceOfDropping == 0 || mspItemListHead == 0 || miItemCount < 1)
	{
		return false;
	}

	// Returns true if remainder is zero, false if not
	return (!(rand() % muiChanceOfDropping));
}

// Returns which Item Type should be dropped (random)
// Returns -1 if Item List is empty
// NOTE: Resets Current Item
int CShip::ReturnItemTypeToDrop()
{
	int iIndex = 0;			// Temp counting variable
	int iRandomNumber = 0;	// Temp variable to hold random number

	// If Item List is empty or there is a problem
	if (mspItemListHead == 0 || miItemCount < 1)
	{
		return -1;
	}

	// Else pick a random Item Type to drop

	// Move Current Item to start of the List
	MoveCurrentItemToStartOfItemList();

	// Choose a random number (0 - number of Items in List)
	iRandomNumber = (rand() % miItemCount);

	// Else choose a random Item to return
	for (iIndex = 0; iIndex < iRandomNumber; iIndex++)
	{
		MoveCurrentItemToNextItemWithWrap();
	}

	// Return random Item Type
	return mspCurrentItem->iItemType;
}



//********** AI Functions **********

// Add a Pattern to this AIs Pattern List
// Returns Patterns unique ID for finding it later if needed
// NOTE: If TimeToDoPatternFor is zero it will use the values supplied to the SetRandomSwitchPatternTimer() function to randomly decide when to switch Patterns
//		 If TimeToDoPatternFor is greater than zero, it will do Pattern for that long before switching Patterns
// NOTE: X/YMovementRange and TrackingPrecision will only be used if eMovementPattern = eTrack/AvoidObject
// NOTE: X/YFireRange will only be set if eFirePattern = eFireWhenClose/FarToObject
// NOTE: TrackingPrecision detmines how well this Ship will track/avoid the Object. A lower TrackingPrecision means better accuracy
int CShip::AddPatternToList(EMovementPatterns eMovementPattern, EFirePatterns eFirePattern, 
							unsigned long ulTimeToDoPatternFor, int iXMovementRange, int iYMovementRange,
                            int iTrackingPrecision, int iXFireRange, int iYFireRange)
{
	SPattern* spPattern		= 0;	// Temp pointer to Patterns
	SPattern* spPrevious	= 0;	// Temp pointer to Previous Pattern

	// Create the new Pattern
	SPattern* spNewPattern = new SPattern;

	// Fill in it's info
	spNewPattern->eMovementPattern		= eMovementPattern;
	spNewPattern->eFirePattern			= eFirePattern;
	spNewPattern->ulTimeToDoPatternFor	= ulTimeToDoPatternFor;
	spNewPattern->iXAvoidRange			= iXMovementRange;
	spNewPattern->iXTrackRange			= iXMovementRange;
	spNewPattern->iXCloseFireRange		= iXFireRange;
	spNewPattern->iXFarFireRange		= iXFireRange;
	spNewPattern->iYAvoidRange			= iYMovementRange;
	spNewPattern->iYTrackRange			= iYMovementRange;
	spNewPattern->iYCloseFireRange		= iYFireRange;
	spNewPattern->iYFarFireRange		= iYFireRange;
	spNewPattern->iPatternID			= miPatternIDCounter++;	// Counter will start from zero
	
	// If iTrackingPrecision is zero
	if (iTrackingPrecision == 0)
	{
		// Set it equal to 1 to make sure we don't divide by zero later
		iTrackingPrecision = 1;
	}

	spNewPattern->iTrackingPrecision	= iTrackingPrecision;


	// Put Pattern into List

	// If this is the first Pattern to go in the List
	if (mspPatternListHead == 0)
	{
		spNewPattern->spNext = 0;
		mspPatternListHead = spNewPattern;

		// Set this as the Curernt Pattern
		mspCurrentPattern = spNewPattern;
		eCurrentMovementPattern = mspCurrentPattern->eMovementPattern;;
	}
	// Else this is not the first so put it at the back of the List
	else
	{
		spPattern = mspPatternListHead;	// Point to Start of List

		// While not at the end of the List
		while (spPattern != 0)
		{
			// Move to Next Pattern
			spPrevious = spPattern;
			spPattern = spPattern->spNext;
		}

		// Add New Pattern to end of List
		spPrevious->spNext = spNewPattern;
		spNewPattern->spNext = 0;
	}

	// Return this Patterns ID
	return spNewPattern->iPatternID;
}

// Remove a Pattern from this AIs Pattern List
// Returns false if Pattern to remove is not in List
bool CShip::DeletePatternFromList(int iPatternID)
{
	SPattern* spPattern		= mspPatternListHead;	// Point to Start of List
	SPattern* spPrevious	= 0;					// Temp pointer to Previous Pattern

	// While not at the end of the List and PatternID not found
	while (spPattern != 0 && spPattern->iPatternID != iPatternID)
	{
		// Move to Next Pattern
		spPrevious = spPattern;
		spPattern = spPattern->spNext;
	}

	// If Pattern was not found
	if (spPattern == 0)
	{
		return false;
	}

	// Else Pattern was found so Delete it

	// If we are Deleting Current Pattern
	if (spPattern == mspCurrentPattern)
	{
		// Change Current Pattern to another Pattern
		ChangePattern();
	}

	// If Pattern is at Start of List
	if (spPrevious == 0)
	{
		mspPatternListHead = mspPatternListHead->spNext;
		delete spPattern;
	}
	// Else it is somewhere deeper in the List
	else
	{
		spPrevious->spNext = spPattern->spNext;
		delete spPattern;
	}

	// Return success
	return true;
}


// Use the specified Pattern
// Returns false if Pattern is not in this AIs Pattern List
bool CShip::UsePattern(int iPatternID)
{
	SPattern* spPattern		= mspPatternListHead;	// Point to Start of List

	// While not at the end of the List and PatternID not found
	while (spPattern != 0 && spPattern->iPatternID != iPatternID)
	{
		// Move to Next Pattern
		spPattern = spPattern->spNext;
	}

	// If Pattern was not found
	if (spPattern == 0)
	{
		return false;
	}

	// Else Pattern was found so use it
	mspCurrentPattern = spPattern;
	eCurrentMovementPattern = mspCurrentPattern->eMovementPattern;

	// Return success
	return true;
}

// Choose a new Pattern (wraps around)
// Returns false if this AI does not have any Patterns in it's Pattern List
bool CShip::ChangePattern()
{
	SPattern* spPattern	= 0;	// Temp pointer to Patterns
	int iRandomNumber	= 0;	// Used to hold which Random Pattern is coming Next

	// If Current Pattern is not set yet or List is Empty
	if (mspCurrentPattern == 0 || mspPatternListHead == 0)
	{
		return false;
	}

	// If new Pattern should not be picked Randomly
	if (!mbChoosePatternsRandomly)
	{
		// Move to the Next Pattern in the List

		// If the Next Pattern exists
		if (mspCurrentPattern->spNext != 0)
		{
			mspCurrentPattern = mspCurrentPattern->spNext;
		}
		// Else wrap around to start of List
		else
		{
			mspCurrentPattern = mspPatternListHead;
		}
	}
	// Else new Pattern should be chosen Randomly
	else
	{
		// While a new Pattern hasn't been selected (since the chosen one may have been removed)
		while (spPattern == 0)
		{
			// Randomly choose which Pattern should come Next
			if (miPatternIDCounter == 0)
			{
				iRandomNumber = ((rand() % (miPatternIDCounter + 1)) - 1);
			}else
			{
				iRandomNumber = (rand() % miPatternIDCounter);
			}

			// Set spPattern to start of Pattern List
			spPattern = mspPatternListHead;

			// Look for Pattern in List
			while (spPattern != 0 && spPattern->iPatternID != iRandomNumber)
			{
				spPattern = spPattern->spNext;
			}
		}

		// Set the new chosen Pattern as the Current Pattern
		mspCurrentPattern = spPattern;
	}

	// Set the Current Movement Pattern
	eCurrentMovementPattern = mspCurrentPattern->eMovementPattern;

	// Set Random Values and return if successful or not
	return SetRandomValues();
}


// Set the AI Pattern Switch Timer
// If no TimeToDoPatternFor was specified when adding the Pattern, the AI Pattern will not 
// switch until MinSwitchTime has elapsed, and will switch before MaxSwitchTime has elapsed
// Also, if ChoosePatternsRandomly has been set, these values will also be used to determine when to switch Patterns all the time
void CShip::SetRandomSwitchPatternTimes(unsigned long ulMinSwitchTime, unsigned long ulMaxSwitchTime)
{
	// If MaxSwitchTime is 0
	if (ulMaxSwitchTime == 0)
	{
		// Set it equal to 1 to make sure we don't divide by zero later
		ulMaxSwitchTime = 1;
	}

	mulMinRandomSwitchPatternTime = ulMinSwitchTime;
	mulMaxRandomSwitchPatternTime = ulMaxSwitchTime;
}

// Return Min Random Switch Pattern Time
unsigned long CShip::ReturnMinRandomSwitchPatternTime() const
{
	return mulMinRandomSwitchPatternTime;
}

// Return Max Random Switch Pattern Time
unsigned long CShip::ReturnMaxRandomSwitchPatternTime() const
{
	return mulMaxRandomSwitchPatternTime;
}

// Return Switch Pattern Time
unsigned long CShip::ReturnSwitchPatternTime() const
{
	return mulSwitchPatternTime;
}

// Set how often the Ship should change directions if set to Move Randomly
// The Ship will wait for MinMovementTime to elapse before changing directions again, but no longer than MaxMovementTime
void CShip::SetRandomMovementTimes(unsigned long ulMinMovementTime, unsigned long ulMaxMovementTime)
{
	// If MaxMovementTime is 0
	if (ulMaxMovementTime == 0)
	{
		// Set it equal to 1 to make sure we don't divide by zero later
		ulMaxMovementTime = 1;
	}

	mulMinRandomMovementTime = ulMinMovementTime;
	mulMaxRandomMovementTime = ulMaxMovementTime;
}

// Return Min Random Movement Time
unsigned long CShip::ReturnMinRandomMovementTime() const
{
	return mulMinRandomMovementTime;
}

// Return Max Random Movement Time
unsigned long CShip::ReturnMaxRandomMovementTime() const
{
	return mulMaxRandomMovementTime;
}

// Return Random Movement Time
unsigned long CShip::ReturnRandomMovementTime() const
{
	return mulRandomMovementTime;
}

// Set how often the Ship should Fire if set to Randomly Fire
// The Ship will wait for MinFireTime to elapse before firing again, but no longer than MaxFireTime
void CShip::SetRandomFireTimes(unsigned long ulMinFireTime, unsigned long ulMaxFireTime)
{
	// If MaxFireTime is 0
	if (ulMaxFireTime == 0)
	{
		// Set it equal to 1 to make sure we don't divide by zero later
		ulMaxFireTime = 1;
	}

	mulMinRandomFireTime = ulMinFireTime;
	mulMaxRandomFireTime = ulMaxFireTime;
}

// Return Min Random Fire Time
unsigned long CShip::ReturnMinRandomFireTime() const
{
	return mulMinRandomFireTime;
}

// Return Max Random Fire Time
unsigned long CShip::ReturnMaxRandomFireTime() const
{
	return mulMaxRandomFireTime;
}

// Return Random Fire Time
unsigned long CShip::ReturnRandomFireTime() const
{
	return mulRandomFireTime;
}

// Return Current Random X Tracking Precision
int CShip::ReturnRandomXTrackingPrecision() const
{
	return miRandomXTrackingPrecision;
}

// Return Current Random Y Tracking Precision
int CShip::ReturnRandomYTrackingPrecision() const
{
	return miRandomYTrackingPrecision;
}

// Choose Patterns from the Pattern List Randomly or in Order
// If provided value is true, Patterns will be chosen Randomly from the List
// If provided value is false, Patterns will follow the order in which they were added (will wrap around)
void CShip::ChoosePatternsRandomly(bool bUseRandomPatterns)
{
	mbChoosePatternsRandomly = bUseRandomPatterns;
}

// Return if Patterns are chosen Randomly
// If false it means they are chosen in the order they were added
bool CShip::ReturnIfPatternsAreBeingChosenRandomly() const
{
	return mbChoosePatternsRandomly;
}

// Return if AI Has Ran at least Once yet or not
bool CShip::ReturnIfAIHasRanOnce() const
{
	return mbAIHasRanOnce;
}

// Set how long the Ship should stay on the screen before leaving
// NOTE: TimeToStayOnScreen = 0 means do not leave the screen
void CShip::StayOnScreenForXMilliseconds(unsigned long ulTimeToStayOnScreen)
{
	mulStayOnScreenTime = ulTimeToStayOnScreen;
}

// Return how long Ship should stay on the Screen for
unsigned long CShip::ReturnHowLongShipShouldStayOnScreen() const
{
	return mulStayOnScreenTime;
}


// Set if the Ship should be firing or not
void CShip::SetIfShipShouldFire(bool bShipShouldFire)
{
	mbShipShouldFire = bShipShouldFire;
}

// Return if the Ship should be firing or not
bool CShip::ReturnIfShipShouldFire() const
{
	return mbShipShouldFire;
}


// Move Ship based on AI Patterns. Supply Players Ship in case we need to track it
// NOTE: This should be called every frame
// NOTE: Once Ship Status = GiSHIP_STATUS_LEAVE_SCREEN the Ship will accelerate towards bottom of screen
// Returns false if List is Empty
bool CShip::PerformAI(CShip const* cpPlayer)
{
	// If List is Empty or no Current Pattern is set
	if (mspPatternListHead == 0 || mspCurrentPattern == 0)
	{
		// Return that there was a problem
		return false;
	}

	// If this is the first time running the AI
	if (!mbAIHasRanOnce)
	{
		SetRandomValues();

		// Set that AI has ran once
		mbAIHasRanOnce = true;
	}

	// If it's time to Change Patterns
	if (mcSwitchPatternTimer.XTimeHasPassedUsingSavedTime(mulSwitchPatternTime))
	{
		// Change to another Pattern
		ChangePattern();
	}


	// Check if it's time to Fire

	// Reset Fire variable
	mbShipShouldFire = false;

	// Check which EFirePatterns is being used
	switch (mspCurrentPattern->eFirePattern)
	{
		// If Firing Randomly
		case eRandomFiring:

			// If Random Fire time has passed
			if (mcRandomFireTimer.XTimeHasPassedUsingSavedTime(mulRandomFireTime))
			{
				// Fire
				mbShipShouldFire = true;

				// Reset the Random Fire Time
				if ((mulRandomFireTime = rand() % mulMaxRandomFireTime) < mulMinRandomFireTime)
				{
					mulRandomFireTime = mulMinRandomFireTime;
				}
			}

		break;

		// If Firing as Often As Possible
		case eFireAsOftenAsPossible:
			
			// Fire
			mbShipShouldFire = true;
		
		break;

		// If Firing When Close To the Object
		case eFireWhenCloseToObject:

			// If Ship X position is within 'Close' distance to Object
			if (((abs(cpPlayer->ReturnXCoordinate() - ReturnXCoordinate())) < mspCurrentPattern->iXCloseFireRange) ||
				mspCurrentPattern->iXCloseFireRange == GiALWAYS_FIRE)
			{
				// If Ship Y position is within 'Close' distance to Object
				if (((abs(cpPlayer->ReturnYCoordinate() - ReturnYCoordinate())) < mspCurrentPattern->iYCloseFireRange) ||
					mspCurrentPattern->iYCloseFireRange == GiALWAYS_FIRE)
				{
					mbShipShouldFire = true;
				}
			}

		break;

		// If Firing WhenFar From the Object
		case eFireWhenFarFromObject:

			// If Ship X position is 'Far' enough from Object
			if (((abs(cpPlayer->ReturnXCoordinate() - ReturnXCoordinate())) > mspCurrentPattern->iXCloseFireRange) ||
				mspCurrentPattern->iXCloseFireRange == GiALWAYS_FIRE)
			{
				// If Ship Y position is 'Far' enough from Object
				if (((abs(cpPlayer->ReturnYCoordinate() - ReturnYCoordinate())) > mspCurrentPattern->iYCloseFireRange) ||
					mspCurrentPattern->iYCloseFireRange == GiALWAYS_FIRE)
				{
					mbShipShouldFire = true;
				}
			}

		break;

		// If not Firing at all
		case eNeverFire:

			// Don't fire
			mbShipShouldFire = false;

		break;

		// If something went wrong
		default:
			MessageBox(NULL, "AI Fire Weapon Error", "Invalid AI Fire Pattern", MB_OK);
		break;
	};


	// Do Movement

	// Check which Movement Pattern is being used
	switch (eCurrentMovementPattern)
	{
		case eRandomMovement:

			// Change to another Random Movement Pattern
			eCurrentMovementPattern = EMovementPatterns(eRandomMovement + (rand() % iTOTAL_NUMBER_OF_MOVEMENT_PATTERNS));

		break;

		case eMoveLeft:

			// Move Ship Left
			SetXAcceleration(-ReturnNormalXAcceleration());

		break;

		case eMoveRight:

			// Move Ship Right
			SetXAcceleration(ReturnNormalXAcceleration());

		break;

		case eMoveUp:

			// Move Ship Up
			SetYAcceleration(-ReturnNormalYAcceleration());

		break;

		case eMoveDown:

			// Move Ship Down
			SetYAcceleration(ReturnNormalYAcceleration());

		break;

		case eMoveUpLeft:

			// Move Ship Up and Left
			SetAccelerations(-ReturnNormalXAcceleration(), -ReturnNormalYAcceleration());

		break;

		case eMoveUpRight:

			// Move Ship Up and Right
			SetAccelerations(ReturnNormalXAcceleration(), -ReturnNormalYAcceleration());

		break;

		case eMoveDownLeft:

			// Move Ship Down and Left
			SetAccelerations(-ReturnNormalXAcceleration(), ReturnNormalYAcceleration());

		break;

		case eMoveDownRight:

			// Move Ship Down and Right
			SetAccelerations(ReturnNormalXAcceleration(), ReturnNormalYAcceleration());

		break;

		case eTrackObject:

			// If Object appears to be to the Left of this Ship
			if ((cpPlayer->ReturnXCoordinate() + miRandomXTrackingPrecision) < ReturnXCoordinate())
			{
				// Move Left
				SetXAcceleration(-ReturnNormalXAcceleration());
			}
			// Else if Object appears to be to the Right of this Ship
			else if ((cpPlayer->ReturnXCoordinate() + miRandomXTrackingPrecision) > ReturnXCoordinate())
			{
				// Move Right
				SetXAcceleration(ReturnNormalXAcceleration());
			}
			// Else stop Accelerating
			else
			{
				SetXAcceleration(0);
			}

			// If Object appears to be Upwards of this Ship
			if ((cpPlayer->ReturnYCoordinate() + miRandomYTrackingPrecision) < ReturnYCoordinate())
			{
				// Move Up
				SetYAcceleration(-ReturnNormalYAcceleration());
			}
			// Else if Object appears to be Downwards of this Ship
			else if ((cpPlayer->ReturnYCoordinate() + miRandomYTrackingPrecision) > ReturnYCoordinate())
			{
				// Move Down
				SetYAcceleration(ReturnNormalYAcceleration());
			}
			// Else stop Accelerating
			else
			{
				SetYAcceleration(0);
			}

		break;

		case eTrackObjectVertically:

			// If Object appears to be Upwards of this Ship
			if ((cpPlayer->ReturnYCoordinate() + miRandomYTrackingPrecision) < ReturnYCoordinate())
			{
				// Move Up
				SetYAcceleration(-ReturnNormalYAcceleration());
			}
			// Else if Object appears to be Downwards of this Ship
			else if ((cpPlayer->ReturnYCoordinate() + miRandomYTrackingPrecision) > ReturnYCoordinate())
			{
				// Move Down
				SetYAcceleration(ReturnNormalYAcceleration());
			}
			// Else stop Accelerating
			else
			{
				SetYAcceleration(0);
			}

			// Stop Horizontal Movement

			// If Ship is moving Left
			if (ReturnXVelocity() < 0)
			{
				// Accelerate Right
				SetXAcceleration(ReturnNormalXAcceleration());
			}
			// Else if Ship is moving Right
			else if (ReturnXVelocity() > 0)
			{
				// Accelerate Left
				SetXAcceleration(-ReturnNormalXAcceleration());
			}
			// Else Ship is not moving Horizontal so stop Accelerating
			else
			{
				SetXAcceleration(0);
			}

		break;

		case eTrackObjectHorizontally:

			// If Object appears to be to the Left of this Ship
			if ((cpPlayer->ReturnXCoordinate() + miRandomXTrackingPrecision) < ReturnXCoordinate())
			{
				// Move Left
				SetXAcceleration(-ReturnNormalXAcceleration());
			}
			// Else if Object appears to be to the Right of this Ship
			else if ((cpPlayer->ReturnXCoordinate() + miRandomXTrackingPrecision) > ReturnXCoordinate())
			{
				// Move Right
				SetXAcceleration(ReturnNormalXAcceleration());
			}
			// Else stop Accelerating
			else
			{
				SetXAcceleration(0);
			}

			// Stop Vertical Movement

			// If Ship is moving Up
			if (ReturnYVelocity() < 0)
			{
				// Accelerate Down
				SetYAcceleration(ReturnNormalYAcceleration());
			}
			// Else if Ship is moving Down
			else if (ReturnYVelocity() > 0)
			{
				// Accelerate Up
				SetYAcceleration(-ReturnNormalYAcceleration());
			}
			// Else Ship is not moving Vertical so stop Accelerating
			else
			{
				SetYAcceleration(0);
			}

		break;

		case eAvoidObject:

			// Avoid Object Vertically

			// If Object appears to be Upwards of this Ship
			if ((cpPlayer->ReturnYCoordinate() + miRandomYTrackingPrecision) < ReturnYCoordinate())
			{
				// Move Down
				SetYAcceleration(ReturnNormalYAcceleration());
			}
			// Else if Object appears to be Downwards of this Ship, or directly to the Left/Right
			else
			{
				// Move Up
				SetYAcceleration(-ReturnNormalYAcceleration());
			}


			// Avoid Object Horizontally

			// If Object appears to be to the Left of this Ship
			if ((cpPlayer->ReturnXCoordinate() + miRandomXTrackingPrecision) < ReturnXCoordinate())
			{
				// Move Right
				SetXAcceleration(ReturnNormalXAcceleration());
			}
			// Else if Object appears to be to the Right of this Ship, or directly above/below this Ship
			else
			{
				// Move Left
				SetXAcceleration(-ReturnNormalXAcceleration());
			}

		break;

		case eAvoidObjectVertically:

			// If Object appears to be Upwards of this Ship
			if ((cpPlayer->ReturnYCoordinate() + miRandomYTrackingPrecision) < ReturnYCoordinate())
			{
				// Move Down
				SetYAcceleration(ReturnNormalYAcceleration());
			}
			// Else if Object appears to be Downwards of this Ship, or directly to the Left/Right
			else
			{
				// Move Up
				SetYAcceleration(-ReturnNormalYAcceleration());
			}

			// Stop Horizontal Movement

			// If Ship is moving Left
			if (ReturnXVelocity() < 0)
			{
				// Accelerate Right
				SetXAcceleration(ReturnNormalXAcceleration());
			}
			// Else if Ship is moving Right
			else if (ReturnXVelocity() > 0)
			{
				// Accelerate Left
				SetXAcceleration(-ReturnNormalXAcceleration());
			}
			// Else Ship is not moving Horizontal so stop Accelerating
			else
			{
				SetXAcceleration(0);
			}

		break;

		case eAvoidObjectHorizontally:

			// If Object appears to be to the Left of this Ship
			if ((cpPlayer->ReturnXCoordinate() + miRandomXTrackingPrecision) < ReturnXCoordinate())
			{
				// Move Right
				SetXAcceleration(ReturnNormalXAcceleration());
			}
			// Else if Object appears to be to the Right of this Ship, or directly above/below this Ship
			else
			{
				// Move Left
				SetXAcceleration(-ReturnNormalXAcceleration());
			}

			// Stop Vertical Movement

			// If Ship is moving Up
			if (ReturnYVelocity() < 0)
			{
				// Accelerate Down
				SetYAcceleration(ReturnNormalYAcceleration());
			}
			// Else if Ship is moving Down
			else if (ReturnYVelocity() > 0)
			{
				// Accelerate Up
				SetYAcceleration(-ReturnNormalYAcceleration());
			}
			// Else Ship is not moving Vertical so stop Accelerating
			else
			{
				SetYAcceleration(0);
			}

		break;

		case eStopAccelerating:

			// Stop all Acceleration
			SetAccelerations(0, 0);
			
		break;

		case eStopHorizontalMovement:

			// Stop Horizontal Movement

			// If Ship is moving Left
			if (ReturnXVelocity() < 0)
			{
				// Accelerate Right
				SetXAcceleration(ReturnNormalXAcceleration());
			}
			// Else if Ship is moving Right
			else if (ReturnXVelocity() > 0)
			{
				// Accelerate Left
				SetXAcceleration(-ReturnNormalXAcceleration());
			}
			// Else Ship is not moving Horizontal so stop Accelerating
			else
			{
				SetXAcceleration(0);
			}

		break;

		case eStopVerticalMovement:

			// Stop Vertical Movement

			// If Ship is moving Up
			if (ReturnYVelocity() < 0)
			{
				// Accelerate Right
				SetYAcceleration(ReturnNormalYAcceleration());
			}
			// Else if Ship is moving Down
			else if (ReturnYVelocity() > 0)
			{
				// Accelerate Left
				SetYAcceleration(-ReturnNormalYAcceleration());
			}
			// Else Ship is not moving Vertical so stop Accelerating
			else
			{
				SetYAcceleration(0);
			}

		break;

		case eStopMovement:

			// Stop Horizontal Movement

			// If Ship is moving Left
			if (ReturnXVelocity() < 0)
			{
				// Accelerate Right
				SetXAcceleration(ReturnNormalXAcceleration());
			}
			// Else if Ship is moving Right
			else if (ReturnXVelocity() > 0)
			{
				// Accelerate Left
				SetXAcceleration(-ReturnNormalXAcceleration());
			}
			// Else Ship is not moving Horizontal so stop Accelerating
			else
			{
				SetXAcceleration(0);
			}


			// Stop Vertical Movement

			// If Ship is moving Up
			if (ReturnYVelocity() < 0)
			{
				// Accelerate Right
				SetYAcceleration(ReturnNormalYAcceleration());
			}
			// Else if Ship is moving Down
			else if (ReturnYVelocity() > 0)
			{
				// Accelerate Left
				SetYAcceleration(-ReturnNormalYAcceleration());
			}
			// Else Ship is not moving Vertical so stop Accelerating
			else
			{
				SetYAcceleration(0);
			}

		break;
		
		default:
			MessageBox(NULL, "AI Movement Error", "Invalid Movement Pattern", MB_OK);
		break;
	};

	// If it's Time for the Ship to head Off Screen and it's not Dying, Dead, or Disabled
	if (mcOnScreenTimer.XTimeHasPassedUsingSavedTimeWithoutRestartingTimer(mulStayOnScreenTime) &&
		mulStayOnScreenTime != 0 && ReturnStatus() != GiSHIP_STATUS_DYING && 
		ReturnStatus() != GiSHIP_STATUS_DEAD &&	ReturnStatus() != GiSHIP_STATUS_DISABLED)
	{
		SetStatus(GiSHIP_STATUS_LEAVE_SCREEN);
	}

	// If Ship should be heading Off Screen
	if (ReturnStatus() == GiSHIP_STATUS_LEAVE_SCREEN)
	{
		// Set Y Acceleration downwards
		SetYAcceleration(ReturnNormalYAcceleration());
	}

	// Return success
	return true;
}




//********** Other Functions **********

// Erase all info as if Ship was just created
void CShip::Purge()
{
	// Erase all Pictures and States
	mcPicture.Purge();

	SWeapon* spWeapon = mspWeaponListHead;
	SWeapon* spPreviousWeapon = 0;

	// Delete all Weapons
	while (spWeapon != 0)
	{
		spPreviousWeapon = spWeapon;
		spWeapon = spWeapon->spNext;
		
		delete spPreviousWeapon->cpBullet;
		delete spPreviousWeapon;
	}

	// Delete all AI Patterns
	while (mspPatternListHead != 0)
	{
		DeletePatternFromList(mspPatternListHead->iPatternID);
	}

	// Reset all private data as NULL
	mfX	= mfY = mfZ = 0;
	mfXVelocity = mfYVelocity = mfZVelocity = 0;
	mfMaxXVelocity = mfMaxYVelocity = mfMaxZVelocity = FLT_MAX;	// Set to Max float value
	mfMinXVelocity = mfMinYVelocity = mfMinZVelocity = FLT_MIN;	// Set to Min float value
	mfXAcceleration = mfYAcceleration = mfZAcceleration = 0;
	mfMaxXAcceleration = mfMaxYAcceleration = mfMaxZAcceleration = FLT_MAX;	// Set to Max float value
	mfMinXAcceleration = mfMinYAcceleration = mfMinZAcceleration = FLT_MIN;	// Set to Min float value
	mfNormalXAcceleration = mfNormalYAcceleration = mfNormalZAcceleration = 0;
	mfXFriction = mfYFriction = mfZFriction = 0;
	miHealth = miStatus = miLives = muiPoints = miGeneralShipType = miSpecificShipType = 0;
	mfArmor = mfShieldStrength = 0.0;
	miMaxHealth = miMaxLives = -1;
	mfMaxArmor = mfMaxShieldStrength = -1.0;
	mbCollision = false;

	mspWeaponListHead = mspWeaponListTail = mspCurrentWeapon = 0;
	mspItemListHead = mspCurrentItem = 0;

	muiChanceOfDropping = 0;
	miItemCount = 0;

	mulMinRandomSwitchPatternTime = mulMinRandomMovementTime = mulMinRandomFireTime = 0;
	mulMaxRandomSwitchPatternTime = mulMaxRandomMovementTime = mulMaxRandomFireTime = 1; // Set Max Random values to 1 to ensure no divide by zero errors
	mulSwitchPatternTime = mulRandomMovementTime = mulRandomFireTime = 0;
	mulStayOnScreenTime = 0;

	miRandomXTrackingPrecision = miRandomYTrackingPrecision = 0;
	mbChoosePatternsRandomly = mbShipShouldFire = mbAIHasRanOnce = false;
		
	mspPatternListHead = mspCurrentPattern = 0;
	miPatternIDCounter = 0;
	eCurrentMovementPattern = eRandomMovement;
}


// Return pointer to Weapon List
SWeapon const* CShip::ReturnPointerToWeaponList() const
{
	return mspWeaponListHead;
}

// Return pointer to Current Weapon
SWeapon const* CShip::ReturnPointerToCurrentWeapon() const
{
	return mspCurrentWeapon;
}

// Return pointer to Item List
SItem const* CShip::ReturnPointerToItemList() const
{
	return mspItemListHead;
}

// Return pointer to Current Item
SItem const* CShip::ReturnPointerToCurrentItem() const
{
	return mspCurrentItem;
}

// Return pointer to AI Pattern List
SPattern const* CShip::ReturnPointerToPatternList() const
{
	return mspPatternListHead;
}

// Return pointer to Current Pattern
SPattern const* CShip::ReturnPointerToCurrentPattern() const
{
	return mspCurrentPattern;
}



//********** Private Functions **********

// Sets the Random Times and Tracking Precision
// Returns false if Pattern List is Empty
bool CShip::SetRandomValues()
{
	// If List is Empty or Current Pattern is not set
	if (mspPatternListHead == 0 || mspCurrentPattern == 0)
	{
		// Return that there was a problem
		return false;
	}

	// Set new Switch Time

	// If Current Patterns Switch Time is not Random
	if (mspCurrentPattern->ulTimeToDoPatternFor > 0)
	{
		mulSwitchPatternTime = mspCurrentPattern->ulTimeToDoPatternFor;
	}
	// Else the new Switch Time is Random
	else
	{
		// Set the new Switch Time to a Random value
		if ((mulSwitchPatternTime = rand() % ReturnMaxRandomSwitchPatternTime()) < ReturnMinRandomSwitchPatternTime())
		{
			mulSwitchPatternTime = ReturnMinRandomSwitchPatternTime();
		}
	}

	// If this Pattern Fires Randomly
	if (mspCurrentPattern->eFirePattern == eRandomFiring)
	{
		// Set the Random Fire Time
		if ((mulRandomFireTime = rand() % mulMaxRandomFireTime) < mulMinRandomFireTime)
		{
			mulRandomFireTime = mulMinRandomFireTime;
		}

		// Reset the Random Fire Timer
		mcRandomFireTimer.RestartTimerUsingSavedTime();
	}

	// If this Pattern Moves Randomly
	if (mspCurrentPattern->eMovementPattern == eRandomMovement)
	{
		// Set the Random Movement Time
		if ((mulRandomMovementTime = rand() % mulMaxRandomMovementTime) < mulMinRandomMovementTime)
		{
			mulRandomMovementTime = mulMinRandomMovementTime;
		}

		// Reset the Random Movement Timer
		mcRandomMovementTimer.RestartTimerUsingSavedTime();
	}

	// Set new Random Tracking Precision
	miRandomXTrackingPrecision = rand() % mspCurrentPattern->iTrackingPrecision;
	miRandomYTrackingPrecision = rand() % mspCurrentPattern->iTrackingPrecision;

	// 50/50 chance of Object 'appearing' to be more to the left
	if (rand() % 2 == 0)
	{
		miRandomXTrackingPrecision *= -1;
	}
	// Else it will 'appear' to be more to the right

	// 50/50 chance of Object 'appearing' to be more upwards
	if (rand() % 2 == 0)
	{
		miRandomXTrackingPrecision *= -1;
	}
	// Else it will 'appear' to be more downwards

	// Return success
	return true;
}
