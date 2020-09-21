// CEnemyList.cpp

#include "CItemList.h"

//*********** CItemList Functions **********

// Constructor
CItemList::CItemList()
{
	// Initialize pointers to NULL
	mspHead				= 0;
	mspCurrentItem		= 0;
}

// Destructor
CItemList::~CItemList()
{
	Purge();
}
		
// Add a Item to the list
// NOTE: The List will use/modify the given Item. It does not create a new one
bool CItemList::AddItem(CItem* cpItemToAdd, bool bDeleteItemWhenRemoved)
{
	SItemListItem* spNewItem;

	// Make sure Item To Add is valid
	if (cpItemToAdd == 0)
	{
		return false;
	}

	// Create the Item
	spNewItem = new SItemListItem;

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
	mspHead->bDeleteItemWhenRemoved = bDeleteItemWhenRemoved;
	mspHead->cpItem	= cpItemToAdd;

	return true;
}

// Makes Current Item point to the first Item in the List
void CItemList::MoveToStartOfList()
{
	mspCurrentItem = mspHead;
}

// Move Current Item to the next Item in the List
// Returns False if Current Item is NULL
bool CItemList::MoveToNextItem()
{
	// If Current Item is Null
	if (mspCurrentItem == 0)
	{
		return false;
	}

	mspCurrentItem = mspCurrentItem->spNext;

	return true;
}

// Returns a Pointer to the Current Item
// Returns zero if Current Item doesn't exist
CItem* CItemList::ReturnCurrentItem() const
{
	// If Current Item doesn't exist
	if (mspCurrentItem == 0)
	{
		return false;
	}

	return mspCurrentItem->cpItem;
}

// Tells if Current Item is actually pointing to a Item or is NULL
bool CItemList::CurrentItemExists() const
{
	// If Current Item doesn't exist
	if (mspCurrentItem == 0)
	{
		return false;
	}

	return true;
}

// Put Current Item on the Delete Later List
// Returns False if Current Item is NULL
bool CItemList::PutCurrentItemOnDeleteLaterList()
{
	// If Current Item does not exist
	if (mspCurrentItem == 0)
	{
		return false;
	}

	mspCurrentItem->bAlive = false;

	return true;
}

// Delete all Items in the Delete Later List
void CItemList::DeleteAllItemsInDeleteLaterList()
{
	SItemListItem* spCurrentItem = mspHead;
	SItemListItem* spPreviousItem = 0;

	// Cycle through List and Delete Dead Items
	while (spCurrentItem != 0)
	{
		// If this Item is supposed to be Deleted
		if (!spCurrentItem->bAlive)
		{
			// If we are deleting the first Item in the List
			if (spCurrentItem == mspHead)
			{
				mspHead = mspHead->spNext;	// Move Head to Next Item in List
				delete spCurrentItem;		// Delete first Item in List
				spCurrentItem = mspHead;	// Make Current Item point to the new Head
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


// Updates all Items in the list
void CItemList::UpdateAllItemsInList(unsigned long ulAmountOfTimeSinceLastUpdate)
{
	SItemListItem* spCurrentItem = mspHead;

	// Cycle through List and Update all Items
	while (spCurrentItem != 0)
	{
		// Update Current Item
		spCurrentItem->cpItem->Update(ulAmountOfTimeSinceLastUpdate);

		// Move to Next Item
		spCurrentItem = spCurrentItem->spNext;
	}
}


// Remove all items from both the Item List and Delete Later List
void CItemList::Purge()
{
	SItemListItem* spCurrentItem = mspHead;
	SItemListItem* spNextItem = 0;
	
	// Cycle through List and Delete all Items
	while (spCurrentItem != 0)
	{
		spNextItem = spCurrentItem->spNext;
		delete spCurrentItem;
		spCurrentItem = spNextItem;
	}

	// Initialize pointers to NULL
	mspHead				= 0;
	mspCurrentItem		= 0;
}


