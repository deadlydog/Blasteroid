/*****************************************************************************/
/* CItemList.h (and CItemList.cpp)                                           */
/* Written by Daniel Schroeder                                               */
/* Created on July 26 2005                                                   */
/* Last Updated July 26, 2005                                                */
/*---------------------------------------------------------------------------*/
/*   The CItemList class is used to hold a list of CItems (see CItem.h). To  */
/* delete an item from the list you must add it to the DeleteLaterList. Then */
/* once done traversing the list, call DeleteAllItemsInDeleteLaterList() to  */
/* actually delete the items.                                                */
/*****************************************************************************/

#ifndef CITEM_LIST_H
#define CITEM_LIST_H

#include "CItem.h"
#include "CTimer.h"


// Items to be held in the List
struct SItemListItem
{
	CItem* cpItem;
	bool bAlive;					// Used to tell if SItemListItem is in DeleteLaterList
	bool bDeleteItemWhenRemoved;	// Used to tell if Item should be deletedo once removed from the List

	SItemListItem* spNext;

	// Constructor
	SItemListItem()
	{
		cpItem = 0;
		spNext = 0;
		bAlive = true;
		bDeleteItemWhenRemoved = true;
	}

	// Destructor
	~SItemListItem()
	{
		// If Item should be Deleted when removed from the List
		if (bDeleteItemWhenRemoved)
		{
			delete cpItem;
		}
	}
};


// Holds a list of CItems
class CItemList
{
	public:

		CItemList();	// Constructor
		~CItemList();	// Destructor

		// Add a Item to the List
		// NOTE: The List will use/modify the given Item. It does not create a new one
		// NOTE: The given Item is deleted when removed from the List if bDeleteItemWhenRemoved is true
		bool AddItem(CItem* cpItemToAdd, bool bDeleteItemWhenRemoved = true);

		// Makes Current Item point to the first Item in the List
		void MoveToStartOfList();

		// Move Current Item to the next Item in the List
		// Returns False if Current Item is NULL
		bool MoveToNextItem();

		// Returns a Pointer to the Current Item
		// Returns zero if Current Item doesn't exist
		CItem* ReturnCurrentItem() const;

		// Tells if Current Item is actually pointing to a Item or is NULL
		bool CurrentItemExists() const;

		// Put Current Item on the Delete Later List
		// Returns False if Current Item is NULL
		bool PutCurrentItemOnDeleteLaterList();

		// Delete all Items in the Delete Later List
		void DeleteAllItemsInDeleteLaterList();

		
		// Updates all Items in the list
		void UpdateAllItemsInList(unsigned long ulAmountOfTimeSinceLastUpdate);

		// Remove all items from both the Item List and Delete Later List
		void Purge();
	

	private:

		SItemListItem* mspHead;				// Head of the list
		SItemListItem* mspCurrentItem;		// Points to the Current Item in the list		
};

#endif

