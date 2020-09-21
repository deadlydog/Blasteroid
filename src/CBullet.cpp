// CBullet.cpp

#include "CBullet.h"
#include <cmath>
using namespace std;


// Default Constructor
CBullet::CBullet()
{
	Purge();
}

// Default Destructor
CBullet::~CBullet()
{
	// Erase and initialize all data
	Purge();
}

// Explicit Constructor
// NOTE: Bullet Duration only has effect if Cause Of Detonation is iDETONATE_ON_TIMER
CBullet::CBullet(int iGeneralBulletType, int iSpecificBulletType, int iOwner, int iCauseOfDetonation, 
				 CCharacterPicture* cpBulletPicture, float fX, float fY, float fZ, float fXVelocity,
				 float fYVelocity, float fZVelocity, unsigned long ulDuration)
{
	Purge();

	miGeneralBulletType = iGeneralBulletType;
	miSpecificBulletType = iSpecificBulletType;
	miOwner = iOwner;
	miCauseOfDetonation = iCauseOfDetonation;
	SetDurationUntilDetonation(ulDuration);

	mcPicture.CopyFrom(*cpBulletPicture);
	MoveTo(fX, fY, fZ);
	SetVelocities(fXVelocity, fYVelocity, fZVelocity);
}

// Copy Constructor
CBullet::CBullet(CBullet const& cBulletToCopyFrom)
{
	CopyFrom(cBulletToCopyFrom);
}

// Copy from another object
void CBullet::CopyFrom(CBullet const& cBulletToCopyFrom)
{
	// Erase and initialize all data
	Purge();
	
	// Copy information from other Bullet to This Bullet
	SetDurationUntilDetonation(cBulletToCopyFrom.ReturnDurationUntilDetonation());
	mbBulletHasDetonated = cBulletToCopyFrom.IsDetonated();
	mbBulletIsLive = cBulletToCopyFrom.IsLive();
	miCauseOfDetonation = cBulletToCopyFrom.ReturnCauseOfDetonation(); 
	miGeneralBulletType = cBulletToCopyFrom.ReturnGeneralType();
	miSpecificBulletType = cBulletToCopyFrom.ReturnSpecificType();
	miOwner = cBulletToCopyFrom.ReturnOwner();
	miBulletID = cBulletToCopyFrom.ReturnID();
	miBaseDamage = cBulletToCopyFrom.ReturnBaseDamage();
	miRandomDamage = cBulletToCopyFrom.ReturnRandomDamage();
	mbHasAlternateEffect = cBulletToCopyFrom.ReturnIfBulletHasAlternateEffect();
	mulFireBulletDelay = cBulletToCopyFrom.ReturnFireDelay();
	MoveTo(cBulletToCopyFrom.ReturnXCoordinate(), cBulletToCopyFrom.ReturnYCoordinate(), cBulletToCopyFrom.ReturnZCoordinate());
	SetVelocities(cBulletToCopyFrom.ReturnXVelocity(), cBulletToCopyFrom.ReturnYVelocity(), cBulletToCopyFrom.ReturnZVelocity());
	mcPicture.CopyFrom(cBulletToCopyFrom.mcPicture);
		
	mbToBeDeleted = cBulletToCopyFrom.IsMarkedToBeDeleted();	
}

// Change a Bullets coordinates to specified coordinates
void CBullet::MoveTo(float fX, float fY, float fZ)
{
	mfX = fX;
	mfY = fY;
	mfZ = fZ;

	mcPicture.MoveCharacterTo((int)floorf(mfX), (int)floorf(mfY), (int)floorf(mfZ));
}

// Change Bullets X coordinate
void CBullet::MoveXTo(float fX)
{
	mfX = fX;
	mcPicture.MoveCharacterXTo((int)floorf(mfX));
}

// Change Bullets Y coordinate
void CBullet::MoveYTo(float fY)
{
	mfY = fY;
	mcPicture.MoveCharacterYTo((int)floorf(mfY));
}

// Change Bullets Z coordinate
void CBullet::MoveZTo(float fZ)
{
	mfZ = fZ;
	mcPicture.MoveCharacterZTo((int)floorf(mfZ));
}		

// Change a Bullets coordinates relative to current coordinates
void CBullet::MoveRelative(float fX, float fY, float fZ)
{
	mfX += fX;
	mfY += fY;
	mfZ += fZ;

	mcPicture.MoveCharacterTo((int)floorf(mfX), (int)floorf(mfY), (int)floorf(mfZ));
}

// Move X coordinate relative to current coordinate
void CBullet::MoveXRelative(float fX)
{
	mfX += fX;
	mcPicture.MoveCharacterXTo((int)floorf(mfX));
}

// Move Y coordinate relative to current coordinate
void CBullet::MoveYRelative(float fY)
{
	mfY += fY;
	mcPicture.MoveCharacterYTo((int)floorf(mfY));
}

// Move Z coordinate relative to current coordinate
void CBullet::MoveZRelative(float fZ)
{
	mfZ += fZ;
	mcPicture.MoveCharacterZTo((int)floorf(mfZ));
}

// Return a Bullets coordinates
void CBullet::ReturnCoordinates(float& fX, float& fY, float& fZ) const
{
	fX = mfX;
	fY = mfY;
	fZ = mfZ;
}

// Return Bullets X coordinate
float CBullet::ReturnXCoordinate() const
{
	return mfX;
}

// Return Bullets Y coordinate
float CBullet::ReturnYCoordinate() const
{
	return mfY;
}

// Return Bullets Z coordinate
float CBullet::ReturnZCoordinate() const
{
	return mfZ;
}

// Set Velocities
// NOTE: Requires Update() to be called each frame in order to move Bullet
void CBullet::SetVelocities(float fXVelocity, float fYVelocity, float fZVelocity)
{
	mfXVelocity = fXVelocity;
	mfYVelocity = fYVelocity;
	mfZVelocity = fZVelocity;
}

// Set X Velocity
void CBullet::SetXVelocity(float fXVelocity)
{
	mfXVelocity = fXVelocity;
}

// Set Y Velocity
void CBullet::SetYVelocity(float fYVelocity)
{
	mfYVelocity = fYVelocity;
}

// Set Z Velocity
void CBullet::SetZVelocity(float fZVelocity)
{
	mfZVelocity = fZVelocity;
}

// Return Velocities
void CBullet::ReturnVelocities(float& fXVelocity, float& fYVelocity, float& fZVelocity)
{
	fXVelocity = mfXVelocity;
	fYVelocity = mfYVelocity;
	fZVelocity = mfZVelocity;
}

// Return X Velocity
float CBullet::ReturnXVelocity() const
{
	return mfXVelocity;
}

// Return Y Velocity
float CBullet::ReturnYVelocity() const
{
	return mfYVelocity;
}

// Return Z Velocity
float CBullet::ReturnZVelocity() const
{
	return mfZVelocity;
}

// Detonate a Bullet so it explodes or disappears or whatever
void CBullet::Detonate()
{
	mbBulletHasDetonated = true;
}

// Returns True if the Bullet has been detonated
bool CBullet::IsDetonated() const
{
	return mbBulletHasDetonated;
}

// Sets a Bullet as dead so it does not do anymore damage
void CBullet::Kill()
{
	mbBulletIsLive = false;
}

// Revive Bullet so it is live and can do damage again
void CBullet::Revive()
{
	mbBulletIsLive = true;
}

// Returns True if Bullet is not dead (can still do damage)
bool CBullet::IsLive() const
{
	return mbBulletIsLive;
}

// Set Bullets General Type
void CBullet::SetGeneralType(int iGeneralType)
{
	miGeneralBulletType = iGeneralType;
}

// Returns the General Type of Bullet
int CBullet::ReturnGeneralType() const
{
	return miGeneralBulletType;
}

// Set Bullets Specific Type
void CBullet::SetSpecificType(int iSpecificType)
{
	miSpecificBulletType = iSpecificType;
}

// Returns the Specific Type of Bullet
int CBullet::ReturnSpecificType() const
{
	return miSpecificBulletType;
}

// Set Bullets Detonation Cause type
void CBullet::SetCauseOfDetonation(int iCauseOfDetonation)
{
	miCauseOfDetonation = iCauseOfDetonation;
}

// Returns Bullets Detonation Cause type
int CBullet::ReturnCauseOfDetonation() const
{
	return miCauseOfDetonation;
}

// Set how long until the Bullet Detonates
void CBullet::SetDurationUntilDetonation(unsigned long ulDuration)
{
	mulDetonationDuration = ulDuration;
}

// Return how long until Bullet Detonates
unsigned long CBullet::ReturnDurationUntilDetonation() const
{
	return mulDetonationDuration;
}


// Set who fired the Bullet
void CBullet::SetOwner(int iOwner)
{
	miOwner = iOwner;
}

// Return who fired the Bullet
int CBullet::ReturnOwner() const
{
	return miOwner;
}

// Set this Bullets ID (used to track this specific Bullet later if needed)
void CBullet::SetID(int iBulletID)
{
	miBulletID = iBulletID;
}

// Return this Bullets ID (used to track this specific Bullet later if needed)
int CBullet::ReturnID() const
{
	return miBulletID;
}

// Set the Base amount of Damage Bullet can do
void CBullet::SetBaseDamage(int iBaseDamage)
{
	miBaseDamage = iBaseDamage;
}

// Return the Base amount of Damage Bullet can do
int CBullet::ReturnBaseDamage() const
{
	return miBaseDamage;
}

// Set the Random Damage which can be added to the Base Damage to give Total Damage
void CBullet::SetRandomDamage(int iRandomDamage)
{
	miRandomDamage = iRandomDamage;
}

// Return the Random Damage which can be added to the Base Damage to give Total Damage
// NOTE: This does not return a Random value, but can be used in main program to specify
// the max Random Damage to be added on.
int CBullet::ReturnRandomDamage() const
{
	return miRandomDamage;
}

// Set if Bullet has an Alternate effect other than doing Damage
void CBullet::SetIfHasAlternateEffect(bool bBulletHasAlternateEffect)
{
	mbHasAlternateEffect = bBulletHasAlternateEffect;
}

// Return if Bullet has an Alternate effect other than doing Damage
bool CBullet::ReturnIfBulletHasAlternateEffect() const
{
	return mbHasAlternateEffect;
}

// Set how much time must elapse before another Bullet of the same Type can be fired
void CBullet::SetFireDelay(unsigned long ulDelayInMilliseconds)
{
	mulFireBulletDelay = ulDelayInMilliseconds;
}

// Return how much time must elapse before another Bullet of the same Type can be fired
unsigned long CBullet::ReturnFireDelay() const
{
	return mulFireBulletDelay;
}


// Mark Bullet for Deletion
void CBullet::MarkToBeDeleted()
{
	mbToBeDeleted = true;
}

// Return if Bullet is Marked for Deletion
bool CBullet::IsMarkedToBeDeleted() const
{
	return mbToBeDeleted;
}

// Erase all information as if Bullet was just created
void CBullet::Purge()
{
	// Initialize private data
	mulDetonationDuration	= mulFireBulletDelay = 0;
	mbBulletHasDetonated	= false;
	mbBulletIsLive			= true;
	miCauseOfDetonation = miGeneralBulletType = miSpecificBulletType = miOwner = miBulletID = 0;
	miBaseDamage = miRandomDamage = 0;
	mbHasAlternateEffect	= false;
	mfX = mfY = mfZ			= 0;
	mfXVelocity = mfYVelocity = mfZVelocity = 0;

	mcPicture.Purge();

	mbToBeDeleted			= false;
}

// Updates Bullets Picture and Position and Detonate it if it's Detonation Timer has expired
// NOTE: This should be called every frame
// NOTE: ulAmountOfTimeSinceLastUpdate should be in milliseconds (1000ms = 1s)
void CBullet::Update(unsigned long ulAmountOfTimeSinceLastUpdate)
{
	float fAmountOfTimeSinceLastUpdateInSeconds = ulAmountOfTimeSinceLastUpdate / 1000.0;

	// Update Bullets Picture
	mcPicture.UpdatePicture(ulAmountOfTimeSinceLastUpdate);

	// Update Bullets position
	// Distance = Time * Velocity
	MoveRelative(fAmountOfTimeSinceLastUpdateInSeconds * mfXVelocity,
				 fAmountOfTimeSinceLastUpdateInSeconds * mfYVelocity,
				 fAmountOfTimeSinceLastUpdateInSeconds * mfZVelocity);

	// If Bullet is set to Detonate On Timer and it still hasn't Detonated
	if (miCauseOfDetonation == GiDETONATE_ON_TIMER && !IsDetonated())
	{
		// If Duration would be less than zero after updating it (ie Duration would expire)
		if (mulDetonationDuration < ulAmountOfTimeSinceLastUpdate)
		{
			// Detonate the Bullet
			Detonate();
		}
		// Else Duration has not expired yet
		else
		{
			// Subtract the Amount Of Time passed Since Last Update
			mulDetonationDuration -= ulAmountOfTimeSinceLastUpdate;
		}
	}
}

