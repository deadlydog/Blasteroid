/*****************************************************************************/
/* CItem.h (and CItem.cpp)                                                   */
/* Written by Daniel Schroeder                                               */
/* Created on July 26, 2005                                                  */
/* Last Updated July 26, 2005                                                */
/*---------------------------------------------------------------------------*/
/*   These CItem class is used to hold all information of an Item, such as   */
/* it's position, velocity, and type. It does not keep track of the effects  */
/* the Item has though. This must be done in the main program.               */
/*****************************************************************************/

#ifndef CITEM_H
#define CITEM_H

#include "CCharacterPicture.h"
#include "CTimer.h"

// Holds the attributes of a Item
class CItem
{
	public:

		CItem();			// Default Constructor
		~CItem();			// Default Destructor

		// Explicit Constructor
		CItem(int iItemType, CCharacterPicture* cpItemPicture, float fX, float fY, float fZ, 
			  float fXVelocity, float fYVelocity, float fZVelocity = 0);

		CItem(CItem const& cItemToCopyFrom);			// Copy Constructor
		void CopyFrom(CItem const& cItemToCopyFrom);	// Copy from another object
		
		// Change a Items coordinates to specified coordinates
		void MoveTo(float fX = 0, float fY = 0, float fZ = 0);
		void MoveXTo(float fX = 0);
		void MoveYTo(float fY = 0);
		void MoveZTo(float fZ = 0);

		// Change a Items coordinates relative to current coordinates
		void MoveRelative(float fX = 0, float fY = 0, float fZ = 0);
		void MoveXRelative(float fX);
		void MoveYRelative(float fY);
		void MoveZRelative(float fZ);

		// Return a Items coordinates
		void ReturnCoordinates(float& fX, float& fY, float& fZ) const;
		float ReturnXCoordinate() const;
		float ReturnYCoordinate() const;
		float ReturnZCoordinate() const;

		// Set Velocities
		// NOTE: Requires Update() to be called each frame in order to move Item
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

		// Set and Return the Type of Item
		void SetType(int iType);
		int ReturnType() const;

		// Erase all information as if Item was just created
		void Purge();

		// Updates Items Picture, Position, and Detonates it if it's Detonation Timer has expired
		// NOTE: This should be called every frame
		// NOTE: ulAmountOfTimeSinceLastUpdate should be in milliseconds (1000ms = 1s)
		void Update(unsigned long ulAmountOfTimeSinceLastUpdate);


		// Holds Items Picture
		CCharacterPicture mcPicture;

	private:
		
        int miType;										// Type of Item
		float mfX, mfY, mfZ;							// Keeps track of Items position
		float mfXVelocity, mfYVelocity, mfZVelocity;	// Keeps track of Items Velocities
};


#endif

