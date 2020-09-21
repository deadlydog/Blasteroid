/*****************************************************************************/
/* CObjectPrototypeList.h (and CObjectPrototypeList.cpp)                     */
/* Written by Daniel Schroeder                                               */
/* Created on July 22 2005                                                   */
/* Last Updated August 2, 2005                                               */
/*---------------------------------------------------------------------------*/
/*   The CObjectPrototypeList class can be used to hold a list of CShips or  */
/* other Objects that haven't actually needed to be created yet (called      */
/* prototypes). (I created this prototype structure because it's much smaller*/
/* than a CShip structure, and will use much less space for ships that are   */
/* not active yet). It keeps track of the prototypes position, velocity, and */
/* the type of Object it will be, as well as a Time. The Time can be used    */
/* however you like, but I am currently using it to keep track of when the   */
/* Ship should appear on screen, at which point the prototype is deleted and */
/* a real ship is added to my ship list. You can also use this to hold a list*/
/* of other Items or whatever, but be sure the Ships, Items, etc all have    */
/* unique Types.                                                             */
/*****************************************************************************/

#ifndef COBJECT_PROTOTYPE_LIST_H
#define COBJECT_PROTOTYPE_LIST_H

// Holds basic information of a Object as well as a Time that the ship should appear
// Used to hold off-screen Object types, since this structure is much smaller than a CObject
// ulTime can be used to tell when the ship should appear on-screen, at which time the Prototype can be deleted and a real CObject created
struct SObjectPrototype
{
	int iType;									// Type of Object (Ship, Weapon, Item, etc)
	float fX, fY, fZ;							// XYZ Coordinates
	float fXVelocity, fYVelocity, fZVelocity;	// Object Velocities
	unsigned long ulTimeDurationUntilAppearing;	// Time duration until Object should appear
	
	int iPrototypeID;							// This Prototypes ID
	SObjectPrototype* spNext;					// Points to Next Object Prototype in List
};

// Holds a list of SObjectPrototypes
// NOTE: Orders Prototypes from smallest ulTime to largest
class CObjectPrototypeList
{
	public:

		CObjectPrototypeList();		// Constructor
		~CObjectPrototypeList();	// Destructor

		// Add a Object Prototype to the List
		// Returns Object Prototypes unique ID
		// Returns zero if a problem occured
		// If using Velocities you can test Objects XYZ coordinates to see if they should be onscreen yet
		// If not, you can test to see if TimeDurationUntilAppearing is zero yet for Object to appear onscreen
		int AddObjectPrototype(int iObjectType, unsigned long ulTimeDurationUntilAppearing = 0, 
							   float fX = 0, float fY = 0, float fZ = 0, float fXVelocity = 0, 
							   float fYVelocity = 0, float fZVelocity = 0);

		// Makes Current Object Prototype point to the first Object in the List
		void MoveToStartOfList();

		// Move Current Object Prototype to the Next Object in the List or specified Prototype
		// Returns False if Current Object is NULL or specified Prototype not found
		// NOTE: MoveToNextObjectPrototypeWithWrap() will only return false if List is empty, 
		// since it wraps from the end to the start of the List
		bool MoveToNextObjectPrototype();
		bool MoveToNextObjectPrototypeWithWrap();
		bool MoveToObjectPrototype(int iObjectPrototypeID);

		// Returns a Pointer to the Current Object Prototype
		// Returns zero if Current Object Prototype doesn't exist
		SObjectPrototype* ReturnCurrentObjectPrototype() const;

		// Tells if Current Object Prototype is actually pointing to a Object Prototype or is NULL
		bool CurrentObjectPrototypeExists() const;

		// Returns true if Object Prototype was found in List, false if not
		bool IsInList(int iObjectPrototypeID) const;

		// Returns true if a Prototype of the specified Type was found
		bool IsTypeInList(int iType) const;

		// Returns true if the Object Prototype List is Empty
		bool IsEmpty() const;

		// Remove a Object Prototype from the List
		// Returns false if Current or specified Object Prototype is not in List or doesn't exist
		bool RemoveCurrentObjectPrototype();
		bool RemoveObjectPrototype(int iObjectPrototypeID);

		// Returns the Prototype with the lowest Time
		// Returns zero if Prototype List is Empty
		SObjectPrototype* ReturnObjectPrototypeWithLowestTime() const;

		// Returns the Prototypes with the lowest Times Time
		// Returns zero if Prototype List is Empty
		unsigned long ReturnLowestTimeInList() const;

		// Update all Object Prototypes Positions according to their Velocity and Update their
		// TimeDurationUntilAppearing times
		void UpdateAllPrototypes(unsigned long ulAmountOfTimeSinceLastUpdate);

		// Remove all items from the List
		void Purge();
	
	private:

		SObjectPrototype* mspHead;					// Points to Head of the List
		SObjectPrototype* mspTail;					// Points to End of the List
		SObjectPrototype* mspCurrentObjectPrototype;// Points to the Current Object Prototype in the list

		int miIDCounter;							// Determines Object Prototypes ID
};

#endif

