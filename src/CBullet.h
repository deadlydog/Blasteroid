/*****************************************************************************/
/* CBullet.h (and CBullet.cpp)                                               */
/* Written by Daniel Schroeder                                               */
/* Created on August 17, 2004                                                */
/* Last Updated July 18, 2005                                                */
/*---------------------------------------------------------------------------*/
/*   The CBullet class is used to hold all information of a Bullet, such as  */
/* it's position, velocity, type, owner (who fired it), if it's still live   */
/* (can do damage), it's detonation cause (explodes, disappears, etc),       */
/* duration until it detonates, etc.                                         */
/*****************************************************************************/

#ifndef CBULLET_H
#define CBULLET_H

#include "CCharacterPicture.h"
#include "GlobalWeaponAndBulletInfo.h"	// Needed to get types of Detonation Causes

// Holds the attributes of a Bullet
class CBullet
{
	public:

		CBullet();			// Default Constructor
		~CBullet();			// Default Destructor

		// Explicit Constructor
		// NOTE: Bullet Duration only has effect if Cause Of Detonation is iDETONATE_ON_TIMER
		CBullet(int iGeneralBulletType, int iSpecificBulletType, int iOwner, int iCauseOfDetonation, 
				CCharacterPicture* cpBulletPicture, float fX, float fY, float fZ, float fXVelocity,
				float fYVelocity, float fZVelocity = 0, unsigned long ulDuration = 0);

		CBullet(CBullet const& cBulletToCopyFrom);			// Copy Constructor
		void CopyFrom(CBullet const& cBulletToCopyFrom);	// Copy from another object
		
		// Change a Bullets coordinates to specified coordinates
		void MoveTo(float fX = 0, float fY = 0, float fZ = 0);
		void MoveXTo(float fX = 0);
		void MoveYTo(float fY = 0);
		void MoveZTo(float fZ = 0);

		// Change a Bullets coordinates relative to current coordinates
		void MoveRelative(float fX = 0, float fY = 0, float fZ = 0);
		void MoveXRelative(float fX);
		void MoveYRelative(float fY);
		void MoveZRelative(float fZ);

		// Return a Bullets coordinates
		void ReturnCoordinates(float& fX, float& fY, float& fZ) const;
		float ReturnXCoordinate() const;
		float ReturnYCoordinate() const;
		float ReturnZCoordinate() const;

		// Set Velocities
		// NOTE: Requires Update() to be called each frame in order to move Bullet
		// NOTE: Velocity is in terms of pixels per second
		void SetVelocities(float fXVelocity = 0, float fYVelocity = 0, float fZVelocity = 0);
		void SetXVelocity(float fXVelocity = 0);
		void SetYVelocity(float fYVelocity = 0);
		void SetZVelocity(float fZVelocity = 0);

		// Return Velocities
		void ReturnVelocities(float& fXVelocity, float& fYVelocity, float& fZVelocity);
		float ReturnXVelocity() const;
		float ReturnYVelocity() const;
		float ReturnZVelocity() const;

		// Detonate a Bullet so it explodes or disappears or whatever
		void Detonate();

		// Returns True if the Bullet has been detonated
		bool IsDetonated() const;

		// Sets a Bullet as dead so it does not do anymore damage
		void Kill();

		// Revive Bullet so it is live and can do damage again
		void Revive();

		// Returns True if Bullet is not dead (can still do damage)
		bool IsLive() const;

		// Set and Return the General Type of Bullet
		void SetGeneralType(int iGeneralType);
		int ReturnGeneralType() const;

		// Set and Return the Specific Type of Bullet
		void SetSpecificType(int iSpecificType);
		int ReturnSpecificType() const;

		// Set and Return Bullets Detonation Cause type
		void SetCauseOfDetonation(int iCauseOfDetonation);
		int ReturnCauseOfDetonation() const;

		// Set and Return how long until the Bullet Detonates on its own
		// NOTE: Bullet will only detonate on its own if 
		void SetDurationUntilDetonation(unsigned long ulDuration);
		unsigned long ReturnDurationUntilDetonation()const;

		// Set and Return who fired the Bullet
		void SetOwner(int iOwner);
		int ReturnOwner() const;

		// Set and Return this Bullets ID (used to track this specific Bullet later if needed)
		void SetID(int iBulletID);
		int ReturnID() const;

		// Set and Return Base amount of Damage Bullet can do
		void SetBaseDamage(int iBaseDamage);
		int ReturnBaseDamage() const;

		// Set the Random Damage which can be added to the Base Damage to give Total Damage
		// NOTE: This does not return a Random value, but can be used in main program to specify
		// the max Random Damage to be added on.
		void SetRandomDamage(int iRandomDamage);
		int ReturnRandomDamage() const;

		// Set and Return if Bullet has an Alternate effect other than doing Damage
		void SetIfHasAlternateEffect(bool bBulletHasAlternateEffect = true);
		bool ReturnIfBulletHasAlternateEffect() const;

		// Set and Return how much time must elapse before another Bullet of the same Type can be fired
		void SetFireDelay(unsigned long ulDelayInMilliseconds);
		unsigned long ReturnFireDelay() const;


		// Mark Bullet for Deletion - Used by CBulletList class
		void MarkToBeDeleted();
		bool IsMarkedToBeDeleted() const;


		// Erase all information as if Bullet was just created
		void Purge();

		// Updates Bullets Picture, Position, and Detonates it if it's Detonation Timer has expired
		// NOTE: This should be called every frame
		// NOTE: ulAmountOfTimeSinceLastUpdate should be in milliseconds (1000ms = 1s)
		void Update(unsigned long ulAmountOfTimeSinceLastUpdate);


		// Holds Bullets Picture
		CCharacterPicture mcPicture;

	private:

		unsigned long mulDetonationDuration;// Holds how long until the Bullet Detonates
		bool mbBulletHasDetonated;			// Tells if the Bullet has Detonated yet
		bool mbBulletIsLive;				// Tells if the Bullet can still cause damage
		int miCauseOfDetonation;			// Tells what causes the weapon to detonate 
		int miGeneralBulletType;			// Holds what General Type of Bullet this is
		int miSpecificBulletType;			// Holds what Specific Type of Bullet this is
		int miOwner;						// Tells who fired the Bullet
		int miBulletID;						// This Bullets ID
		
		int miBaseDamage;					// The Base (minimum) Damage this Bullet should do
		int miRandomDamage;					// The extra Damage on top of the Base Damage the Bullet can do
		bool mbHasAlternateEffect;			// Tells if the Bullet does something else beside deal Damage
		unsigned long mulFireBulletDelay;	// Holds how much time must elapse before another Bullet of the same Type can be fired

		float mfX, mfY, mfZ;				// Keeps track of Bullets position
		float mfXVelocity, mfYVelocity, mfZVelocity;	// Keeps track of Bullets Velocities

		bool mbToBeDeleted;					// Tells if the Bullet should be Deleted (Used by CBulletList)
};

#endif

