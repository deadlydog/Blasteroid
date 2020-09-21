/*****************************************************************************/
/* CShipList.h (and CShipList.cpp)                                           */
/* Written by Daniel Schroeder                                               */
/* Created on July 24 2004                                                   */
/* Last Updated August 20, 2005                                              */
/*---------------------------------------------------------------------------*/
/*   The CShipList class is used to hold a list of CShips (see CShip.h). To  */
/* delete a ship from the list you must add it to the DeleteLaterList. Then  */
/* once done traversing the list, call DeleteAllShipsInDeleteLaterList() to  */
/* actually delete the ships.                                                */
/*****************************************************************************/

#ifndef CSHIP_LIST_H
#define CSHIP_LIST_H

#include "CShip.h"
#include "CTimer.h"


// Items to be held in the List
struct SShipListItem
{
	CShip* cpShip;
	bool bAlive;					// Used to tell if SShipListItem is in DeleteLaterList
	bool bDeleteShipWhenRemoved;	// Used to tell if the Ship should be Deleted once removed from the List

	SShipListItem* spNext;

	// Constructor
	SShipListItem()
	{
		cpShip = 0;
		spNext = 0;
		bAlive = true;
		bDeleteShipWhenRemoved = true;
	}

	// Destructor
	~SShipListItem()
	{
		// If the Ship should be Deleted
		if (bDeleteShipWhenRemoved)
		{
			delete cpShip;
		}
	}
};


// Holds a list of CShips
class CShipList
{
	public:

		CShipList();	// Constructor
		~CShipList();	// Destructor

		// Add a Ship to the List
		// NOTE: The List will use/modify the given Ship. It does not create a new one
		// NOTE: The given Ship is deleted when removed from the List if bDeleteShipWhenRemoved is true
		bool AddShip(CShip* cpShipToAdd, bool bDeleteShipWhenRemoved = true);

		// Makes Current Ship point to the first Ship in the List
		void MoveToStartOfList();

		// Move Current Ship to the next Ship in the List
		// Returns False if Current Ship is NULL
		bool MoveToNextShip();

		// Returns a Pointer to the Current Ship
		// Returns zero if Current Ship doesn't exist
		CShip* ReturnCurrentShip() const;

		// Tells if Current Ship is actually pointing to a Ship or is NULL
		bool CurrentShipExists() const;

		// Put Current Ship on the Delete Later List
		// Returns False if Current Ship is NULL
		bool PutCurrentShipOnDeleteLaterList();

		// Delete all Items in the Delete Later List
		// NOTE: The supplied Ships Pointer is also deleted
		void DeleteAllShipsInDeleteLaterList();

		
		// Updates all Ships in the list
		void UpdateAllShipsInList(unsigned long ulAmountOfTimeSinceLastUpdate);

		// Remove all items from both the Ship List and Delete Later List
		// NOTE: All supplied Ships Pointers are also deleted
		void Purge();
	

	private:

		SShipListItem* mspHead;							// Head of the list
		SShipListItem* mspCurrentShip;					// Points to the Current Ship in the list		
};

#endif

