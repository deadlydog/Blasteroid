// CItem.cpp

#include "CItem.h"
#include <cmath>
using namespace std;


// Default Constructor
CItem::CItem()
{
	Purge();
}

// Default Destructor
CItem::~CItem()
{
	// Erase and initialize all data
	Purge();
}

// Explicit Constructor
// NOTE: Item Duration only has effect if Cause Of Detonation is iDETONATE_ON_TIMER
CItem::CItem(int iItemType, CCharacterPicture* cpItemPicture, float fX, float fY, float fZ, 
			 float fXVelocity, float fYVelocity, float fZVelocity)
{
	Purge();

	miType = iItemType;
	mcPicture.CopyFrom(*cpItemPicture);
	MoveTo(fX, fY, fZ);
	SetVelocities(fXVelocity, fYVelocity, fZVelocity);
}

// Copy Constructor
CItem::CItem(CItem const& cItemToCopyFrom)
{
	CopyFrom(cItemToCopyFrom);
}

// Copy from another object
void CItem::CopyFrom(CItem const& cItemToCopyFrom)
{
	// Erase and initialize all data
	Purge();
	
	// Copy information from other Item to This Item
	miType = cItemToCopyFrom.ReturnType();
	mcPicture.CopyFrom(cItemToCopyFrom.mcPicture);
	MoveTo(cItemToCopyFrom.ReturnXCoordinate(), cItemToCopyFrom.ReturnYCoordinate(), cItemToCopyFrom.ReturnZCoordinate());
	SetVelocities(cItemToCopyFrom.ReturnXVelocity(), cItemToCopyFrom.ReturnYVelocity(), cItemToCopyFrom.ReturnZVelocity());	
}

// Change a Items coordinates to specified coordinates
void CItem::MoveTo(float fX, float fY, float fZ)
{
	mfX = fX;
	mfY = fY;
	mfZ = fZ;

	mcPicture.MoveCharacterTo((int)floorf(mfX), (int)floorf(mfY), (int)floorf(mfZ));
}

// Change Items X coordinate
void CItem::MoveXTo(float fX)
{
	mfX = fX;
	mcPicture.MoveCharacterXTo((int)floorf(mfX));
}

// Change Items Y coordinate
void CItem::MoveYTo(float fY)
{
	mfY = fY;
	mcPicture.MoveCharacterYTo((int)floorf(mfY));
}

// Change Items Z coordinate
void CItem::MoveZTo(float fZ)
{
	mfZ = fZ;
	mcPicture.MoveCharacterZTo((int)floorf(mfZ));
}		

// Change a Items coordinates relative to current coordinates
void CItem::MoveRelative(float fX, float fY, float fZ)
{
	mfX += fX;
	mfY += fY;
	mfZ += fZ;

	mcPicture.MoveCharacterTo((int)floorf(mfX), (int)floorf(mfY), (int)floorf(mfZ));
}

// Move X coordinate relative to current coordinate
void CItem::MoveXRelative(float fX)
{
	mfX += fX;
	mcPicture.MoveCharacterXTo((int)floorf(mfX));
}

// Move Y coordinate relative to current coordinate
void CItem::MoveYRelative(float fY)
{
	mfY += fY;
	mcPicture.MoveCharacterYTo((int)floorf(mfY));
}

// Move Z coordinate relative to current coordinate
void CItem::MoveZRelative(float fZ)
{
	mfZ += fZ;
	mcPicture.MoveCharacterZTo((int)floorf(mfZ));
}

// Return a Items coordinates
void CItem::ReturnCoordinates(float& fX, float& fY, float& fZ) const
{
	fX = mfX;
	fY = mfY;
	fZ = mfZ;
}

// Return Items X coordinate
float CItem::ReturnXCoordinate() const
{
	return mfX;
}

// Return Items Y coordinate
float CItem::ReturnYCoordinate() const
{
	return mfY;
}

// Return Items Z coordinate
float CItem::ReturnZCoordinate() const
{
	return mfZ;
}

// Set Velocities
// NOTE: Requires Update() to be called each frame in order to move Item
void CItem::SetVelocities(float fXVelocity, float fYVelocity, float fZVelocity)
{
	mfXVelocity = fXVelocity;
	mfYVelocity = fYVelocity;
	mfZVelocity = fZVelocity;
}

// Set X Velocity
void CItem::SetXVelocity(float fXVelocity)
{
	mfXVelocity = fXVelocity;
}

// Set Y Velocity
void CItem::SetYVelocity(float fYVelocity)
{
	mfYVelocity = fYVelocity;
}

// Set Z Velocity
void CItem::SetZVelocity(float fZVelocity)
{
	mfZVelocity = fZVelocity;
}

// Return Velocities
void CItem::ReturnVelocities(float& fXVelocity, float& fYVelocity, float& fZVelocity)
{
	fXVelocity = mfXVelocity;
	fYVelocity = mfYVelocity;
	fZVelocity = mfZVelocity;
}

// Return X Velocity
float CItem::ReturnXVelocity() const
{
	return mfXVelocity;
}

// Return Y Velocity
float CItem::ReturnYVelocity() const
{
	return mfYVelocity;
}

// Return Z Velocity
float CItem::ReturnZVelocity() const
{
	return mfZVelocity;
}

// Set Items Type
void CItem::SetType(int iType)
{
	miType = iType;
}

// Returns the Type of Item
int CItem::ReturnType() const
{
	return miType;
}

// Erase all information as if Item was just created
void CItem::Purge()
{
	// Initialize private data
	miType			= 0;
	mfX = mfY = mfZ	= 0;
	mfXVelocity = mfYVelocity = mfZVelocity = 0;

	mcPicture.Purge();
}

// Updates Items Picture and Position and Detonate it if it's Detonation Timer has expired
// NOTE: This should be called every frame
// NOTE: ulAmountOfTimeSinceLastUpdate should be in milliseconds (1000ms = 1s)
void CItem::Update(unsigned long ulAmountOfTimeSinceLastUpdate)
{
	float fAmountOfTimeSinceLastUpdateInSeconds = ulAmountOfTimeSinceLastUpdate / 1000.0;

	// Update Items Picture
	mcPicture.UpdatePicture(ulAmountOfTimeSinceLastUpdate);

	// Update Items position
	// Distance = Time * Velocity
	MoveRelative(fAmountOfTimeSinceLastUpdateInSeconds * mfXVelocity,
				 fAmountOfTimeSinceLastUpdateInSeconds * mfYVelocity,
				 fAmountOfTimeSinceLastUpdateInSeconds * mfZVelocity);
}

