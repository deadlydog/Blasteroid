// CBulletList.cpp				Copyright August 17, 2004  Daniel Schroeder

#include "CBulletList.h"


// Constructor
CBulletList::CBulletList()
{
	// Initialize private data as NULL
	mspHead				= 0;
	mspCurrentBullet	= 0;
	miIDCounter			= 0;
}

// Destructor
CBulletList::~CBulletList()
{
	Purge();
}

// Add a Bullet to the List 
// Returns zero if cBullet not valid
// NOTE: Also Sets and Returns the Bullets new unique ID if needed to find it later
// NOTE: Does not create a new Bullet, so the given Bullet may be modified
int CBulletList::AddBullet(CBullet* cpBullet, bool bDeleteBulletWhenRemoved)
{
	//********** Create Bullet and Fill in data **********

	SBulletListItem* spItem;	// Pointer to New Item

	// Make sure Bullet is valid
	if (cpBullet == 0)
	{
		return 0;
	}
	
	// Create the New Item
	spItem = new SBulletListItem;

	// Make sure Counter will not be zero
	if (++miIDCounter == 0)
	{
		// Increment it one more time
		++miIDCounter;
	}

	// Fill in Item data
	spItem->cpBullet = cpBullet;
	spItem->bDeleteBulletWhenRemoved = bDeleteBulletWhenRemoved;
	spItem->cpBullet->SetID(miIDCounter);

	// Add Item to the List
	spItem->spNext = mspHead;
	mspHead = spItem;

	// Return this Bullets new unique ID
	return spItem->cpBullet->ReturnID();
}

// Delete a Bullet
// NOTE: Bullets are automatically Deleted after Detonation is complete
bool CBulletList::PutCurrentBulletOnDeleteLaterList()
{
	// If Current Bullet is NULL
	if (mspCurrentBullet == 0)
	{
		return false;
	}

	mspCurrentBullet->cpBullet->MarkToBeDeleted();
	
	return true;
}

// Delete a Bullet
// NOTE: Bullets are automatically Deleted after Detonation is complete
bool CBulletList::PutBulletOnDeleteLaterList(int iBulletID)
{
	SBulletListItem* spBullet = 0;

	// If Item was not found
	if ((spBullet = BulletIsInListAndReturnIt(iBulletID)) == 0)
	{
		return false;
	}

	spBullet->cpBullet->MarkToBeDeleted();
	return true;
}

// Delete all Bullets in the Delete Later List
void CBulletList::DeleteBulletsInDeleteLaterList()
{
	SBulletListItem* spCurrent	= mspHead;
	SBulletListItem* spPrevious	= 0;
	SBulletListItem* spTemp		= 0;

	// Loop through entire List
	while (spCurrent != 0)
	{
		// If we need to Delete this Bullet
		if (spCurrent->cpBullet->IsMarkedToBeDeleted())
		{
			// If we are Deleting the First Item in the List
			if (spPrevious == 0)
			{
				// Move Current to next Item
				spTemp = spCurrent;
				spCurrent = spCurrent->spNext;

				// Delete Item
				delete spTemp;

				mspHead = spCurrent;	// Set Head to new start of list
			}else
			{
				// Fill in hole where Item to delete is
				spPrevious->spNext = spCurrent->spNext;
				spTemp = spCurrent;
				spCurrent = spCurrent->spNext;

				// Delete Item
				delete spTemp;
			}
		}
		// Else move to Next Item
		else
		{
			spPrevious = spCurrent;
			spCurrent = spCurrent->spNext;
		}
	}
}

// Move Current Bullet to the First Bullet in the List
void CBulletList::MoveToStartOfList()
{
	mspCurrentBullet = mspHead;
}

// Move Current Bullet to the Next Bullet in the List
// Returns false if Current Bullet doesn't exist
bool CBulletList::MoveToNextBullet()
{
	if (mspCurrentBullet == 0)
	{
		return false;
	}

	mspCurrentBullet = mspCurrentBullet->spNext;

	return true;
}

// Returns True if Current Ship is not NULL 
bool CBulletList::CurrentBulletExists()
{
	if (mspCurrentBullet == 0)
	{
		return false;
	}

	return true;
}

// Returns True if Bullet is found in list
bool CBulletList::BulletIsInList(int iBulletID)
{
	SBulletListItem* spCurrent	= mspHead;

	// Cycle through list until end of list or Bullet is found
	while (spCurrent != 0 && spCurrent->cpBullet->ReturnID() != iBulletID)
	{
		spCurrent = spCurrent->spNext;
	}

	// If Bullet was found
	if (spCurrent != 0)
	{
		return true;
	}
	// Else return Bullet was not found
	return false;
}

// Returns the Current Bullets ID
int CBulletList::ReturnCurrentBulletsID() const
{
	// If Current Bullet is NULL
	if (mspCurrentBullet == 0)
	{
		return -1;
	}

	return mspCurrentBullet->cpBullet->ReturnID();
}


// Returns a Bullets Picture
CBullet* CBulletList::ReturnCurrentBullet() const
{
	// If Current Bullet is NULL
	if (mspCurrentBullet == 0)
	{
		return 0;
	}

	return mspCurrentBullet->cpBullet;
}

// Returns a Bullets Picture
CBullet* CBulletList::ReturnBullet(int iBulletID) const
{
	SBulletListItem* spItem;

	// If Item was not found
	if ((spItem = BulletIsInListAndReturnIt(iBulletID)) == 0)
	{
		return 0;
	}

	return spItem->cpBullet;
}

// Updates Bullets Picture/Position and Detonates Bullet Automatically if it's Timer hits zero
// NOTE: Should be called every frame
void CBulletList::UpdateAllBulletsInList(unsigned long ulAmountOfTimeSinceLastUpdate)
{
	SBulletListItem* spItem = mspHead;

	// Cycle through entire List
	while (spItem != 0)
	{
		// Update Bullets Picture and Position
		spItem->cpBullet->Update(ulAmountOfTimeSinceLastUpdate);

		// Move to Next Item
		spItem = spItem->spNext;
	}
}

// Delete all Bullets from List
void CBulletList::Purge()
{
	SBulletListItem* spItem = mspHead;
	SBulletListItem* spTemp = 0;

	// Cycle through entire List
	while (spItem != 0)
	{
		// Delete Bullet and move to Next one
		spTemp = spItem;
		spItem = spItem->spNext;
		delete spTemp;
	}

	// Reset private data
	mspHead				= 0;
	mspCurrentBullet	= 0;
	miIDCounter			= 0;
}

// Retuns an Item if it's in the List
// NOTE: Returns 0 if Item not found
SBulletListItem* CBulletList::BulletIsInListAndReturnIt(int iBulletID) const
{
	SBulletListItem* spCurrent	= mspHead;

	while (spCurrent != 0 && spCurrent->cpBullet->ReturnID() != iBulletID)
	{
		spCurrent = spCurrent->spNext;
	}

	return spCurrent;
}



// Constructor
CBulletQueue::CBulletQueue()
{
	mspHead	= 0;			// Initialize as NULL
}

// Destructor
CBulletQueue::~CBulletQueue()
{
	Purge();
}

// Add a Bullet to Queue in FIFO fashion
void CBulletQueue::AddToFront(int iBulletID)
{
	// Initialize and Set the new Bullet
	SQueueBullet* spBullet	= new SQueueBullet;
	spBullet->iBulletID		= iBulletID;
	spBullet->spNext		= 0;

	// If there are other Bullets in the list
	if (mspHead != 0)
	{
		// Make this new Bullets Next point to the current head of the list
		spBullet->spNext = mspHead;
	}
	
	// Put this Bullet at the start of the list
	mspHead = spBullet;
}

// Remove a Bullet from Queue in FIFO fashion
// Returns -1 if Queue is Empty
int CBulletQueue::RemoveFromBack()
{
	SQueueBullet* spBullet			= mspHead;
	SQueueBullet* spPrevious		= mspHead;

	int iID = 0;

	if (mspHead == 0)
	{
		return -1;
	}

	// Loop until at end of queue
	while (spBullet->spNext != 0)
	{
		spPrevious = spBullet;
		spBullet = spBullet->spNext;
	}

	// Save the Last Bullet in the lists ID
	iID = spBullet->iBulletID;

	// Make 2nd last item in list point to nothing
	spPrevious->spNext = 0;
	
	// If we are removing the only Bullet in the Queue
	if (spBullet == mspHead)
	{
		mspHead = 0;
	}

	// Delete last Bullet
	delete spBullet;

	// Return the old Last Bullets ID
	return iID;
}
		
// Remove a Bullet from the Queue. Returns false if BulletID not found
bool CBulletQueue::RemoveFromQueue(int iBulletID)
{
	SQueueBullet* spBullet	= mspHead;
	SQueueBullet* spPrevious= mspHead;

	if (mspHead == 0)
	{
		return false;
	}

	// Loop until end of Queue or Bullet is found
	while (spBullet != 0 && spBullet->iBulletID != iBulletID)
	{
		spPrevious = spBullet;
		spBullet = spBullet->spNext;
	}

	// If Bullet was not found
	if (spBullet == 0)
	{
		return false;
	}

	// Bridge the gap from deleting the Bullet
	spPrevious->spNext = spBullet->spNext;

	// If we are removing the first Bullet in the Queue
	if (spBullet == mspHead)
	{
		// If this is the only Bullet in the Queue
		if (spBullet->spNext == 0)
		{
			mspHead = 0;
		}
		// Make Head point to second Bullet in Queue
		else
		{
			mspHead = spBullet->spNext;
		}
	}

	delete spBullet;

	return true;
}

// Returns if the Bullet is in the Queue or not
bool CBulletQueue::IsInQueue(int iBulletID)
{
	SQueueBullet* spBullet	= mspHead;

	// Loop until end of Queue or Bullet is found
	while (spBullet != 0 && spBullet->iBulletID != iBulletID)
	{
		spBullet = spBullet->spNext;
	}

	// If the Bullet was not found
	if (spBullet == 0)
	{
		return false;
	}

	// Else the Bullet was found so return True
	return true;
}

// Returns True if Queue is Empty, False if not
bool CBulletQueue::IsEmpty()
{
	// If Queue is Empty
	if (mspHead == 0)
	{
		return true;
	}

	// Else Queue is not Empty
	return false;
}

// Empties Queue
void CBulletQueue::Purge()
{
	SQueueBullet* spBullet		= mspHead;
	SQueueBullet* spPrevious	= mspHead;

	// Loop through the Queue and delete all the Bullets
	while (spBullet != 0)
	{
		spPrevious = spBullet;
		spBullet = spBullet->spNext;
		delete spPrevious;
	}
	
	// Reset private data
	mspHead = 0;
}

