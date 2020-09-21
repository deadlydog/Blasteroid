// CEnemyList.cpp

#include "CShipList.h"

//*********** CShipList Functions **********

// Constructor
CShipList::CShipList()
{
	// Initialize pointers to NULL
	mspHead				= 0;
	mspCurrentShip		= 0;
}

// Destructor
CShipList::~CShipList()
{
	Purge();
}
		
// Add a Ship to the list
// NOTE: The List will use/modify the given Ship. It does not create a new one
bool CShipList::AddShip(CShip* cpShipToAdd, bool bDeleteShipWhenRemoved)
{
	SShipListItem* spNewItem;

	// Make sure Ship To Add is valid
	if (cpShipToAdd == 0)
	{
		return false;
	}

	// Create the Item
	spNewItem = new SShipListItem;

	// If there are no Items in the List
	if (mspHead == 0)
	{
		mspHead = spNewItem;	// Make the Head point to this item
		mspHead->spNext	= 0;	// Since this is the first item, make it's Next NULL	
	}
	// Else add this Item to the List
	else
	{
		spNewItem->spNext = mspHead;
		mspHead = spNewItem;
	}

	// Set Items data
	mspHead->bAlive	= true;
	mspHead->bDeleteShipWhenRemoved = bDeleteShipWhenRemoved;
	mspHead->cpShip	= cpShipToAdd;

	return true;
}

// Makes Current Ship point to the first Ship in the List
void CShipList::MoveToStartOfList()
{
	mspCurrentShip = mspHead;
}

// Move Current Ship to the next Ship in the List
// Returns False if Current Ship is NULL
bool CShipList::MoveToNextShip()
{
	// If Current Ship is Null
	if (mspCurrentShip == 0)
	{
		return false;
	}

	mspCurrentShip = mspCurrentShip->spNext;

	return true;
}

// Returns a Pointer to the Current Ship
// Returns zero if Current Ship doesn't exist
CShip* CShipList::ReturnCurrentShip() const
{
	// If Current Ship doesn't exist
	if (mspCurrentShip == 0)
	{
		return false;
	}

	return mspCurrentShip->cpShip;
}

// Tells if Current Ship is actually pointing to a Ship or is NULL
bool CShipList::CurrentShipExists() const
{
	// If Current Ship doesn't exist
	if (mspCurrentShip == 0)
	{
		return false;
	}

	return true;
}

// Put Current Ship on the Delete Later List
// Returns False if Current Ship is NULL
bool CShipList::PutCurrentShipOnDeleteLaterList()
{
	// If Current Ship does not exist
	if (mspCurrentShip == 0)
	{
		return false;
	}

	mspCurrentShip->bAlive = false;

	return true;
}

// Delete all Items in the Delete Later List
void CShipList::DeleteAllShipsInDeleteLaterList()
{
	SShipListItem* spCurrentItem = mspHead;
	SShipListItem* spPreviousItem = 0;

	// Cycle through List and Delete Dead Items
	while (spCurrentItem != 0)
	{
		// If this Item is supposed to be Deleted
		if (!spCurrentItem->bAlive)
		{
			// If we are deleting the first Item in the List
			if (spCurrentItem == mspHead)
			{
				mspHead = mspHead->spNext;			// Move Head to Next Item in List
				delete spCurrentItem;				// Delete first Item in List
				spCurrentItem = mspHead;			// Make Current Item point to the new Head
			}else
			{	
				spPreviousItem->spNext = spCurrentItem->spNext;	// Make Previous Item point to Next Item
				delete spCurrentItem;							// Delete Current Item
				spCurrentItem = spPreviousItem->spNext;			// Move Current Item to Next Item
			}
		}
		// Else skip to Next Item
		else
		{
			spPreviousItem = spCurrentItem;
			spCurrentItem = spCurrentItem->spNext;
		}
	}
}


// Updates all Ships in the list
void CShipList::UpdateAllShipsInList(unsigned long ulAmountOfTimeSinceLastUpdate)
{
	SShipListItem* spCurrentItem = mspHead;

	// Cycle through List and Update all Ships
	while (spCurrentItem != 0)
	{
		// Update Current Ship
		spCurrentItem->cpShip->Update(ulAmountOfTimeSinceLastUpdate);

		// Move to Next Item
		spCurrentItem = spCurrentItem->spNext;
	}
}


// Remove all items from both the Ship List and Delete Later List
void CShipList::Purge()
{
	SShipListItem* spCurrentItem = mspHead;
	SShipListItem* spNextItem = 0;
	
	// Cycle through List and Delete all Items
	while (spCurrentItem != 0)
	{
		spNextItem = spCurrentItem->spNext;	// Save Next Item
		delete spCurrentItem;				// Delete the Ship from the List
		spCurrentItem = spNextItem;			// Move to Next Item
	}

	// Initialize pointers to NULL
	mspHead				= 0;
	mspCurrentShip		= 0;
}


