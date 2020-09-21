/*****************************************************************************/
/* CBulletList.h (and CBulletList.cpp)                                       */
/* Written by Daniel Schroeder                                               */
/* Created on August 17, 2004                                                */
/* Last Updated July 18, 2005                                                */
/*---------------------------------------------------------------------------*/
/*   The CBulletList class is used to hold a list of CBullets. When a Bullet */
/* is added to the list it gets a new unique ID which is returned.           */
/*   The CBulletList class also contains a CBulletQueue object, which can be */
/* used as a separate list for special types of Bullets. CBulletQueue objects*/
/* should still also exist in the CBulletList list. (I am currently using the*/
/* mcBulletQueue object to keep track of Bullets that are externally         */
/* detonatable, such as bombs, so when someone hits the detonate button, it  */
/* detonates the Bullet that was fired first)                                */
/*****************************************************************************/

#ifndef CBULLET_LIST_H
#define CBULLET_LIST_H

#include "CBullet.h"

// Used to hold items in CBulletQueue class
struct SQueueBullet
{
	int iBulletID;
	SQueueBullet* spNext;
};

// Used in CBulletList class
class CBulletQueue
{
	public:

		CBulletQueue();			// Constructor
		~CBulletQueue();		// Destructor

		// Add and Remove Bullets in FIFO fashion
		void AddToFront(int iBulletID);
		int RemoveFromBack();			// Returns -1 if Queue is Empty
		
		// Remove a Bullet from the Queue. Returns false if BulletID not found
		bool RemoveFromQueue(int iBulletID);

		// Returns if the Bullet is in the Queue or not
		bool IsInQueue(int iBulletID);

		// Returns True if Queue is Empty, False if not
		bool IsEmpty();


		// Empties Queue
		void Purge();

	private:

		SQueueBullet* mspHead;					// Points to the Start of the Queue
};


// Structure used to hold Bullets Lists Items
struct SBulletListItem
{
	CBullet* cpBullet;
	bool bDeleteBulletWhenRemoved;	// Used to tell if Bullet should be deleted once removed from the List

	SBulletListItem* spNext;

	// Constructor
	SBulletListItem()
	{
		cpBullet = 0;
		bDeleteBulletWhenRemoved = true;
		spNext = 0;
	}

	// Destructor
	~SBulletListItem()
	{
		// If Bullet should be Deleted
		if (bDeleteBulletWhenRemoved)
		{
			delete cpBullet;
		}
	}
};

// Used to hold a list of Bullets
class CBulletList
{
	public:
		
		CBulletList();			// Constructor
		~CBulletList();			// Destructor

		// Add a Bullet to the List 
		// Returns zero if cBullet not valid
		// NOTE: Also Sets and Returns the Bullets new unique ID if needed to find it later
		// NOTE: Does not create a new Bullet, so the given Bullet may be modified
		// NOTE: The given Bullet is deleted when removed from the List if bDeleteBulletWhenRemoved is true
		int AddBullet(CBullet* cpBullet, bool bDeleteBulletWhenRemoved = true);

		// Put Bullet on the List to be Deleted Later
		// NOTE: Bullets are automatically Deleted after Detonation is complete
		bool PutCurrentBulletOnDeleteLaterList();
		bool PutBulletOnDeleteLaterList(int iBulletID);

		// Delete all Bullets in the Delete Later List
		void DeleteBulletsInDeleteLaterList();


		// Move Current Bullet to the First Bullet in the List
		void MoveToStartOfList();

		// Move Current Bullet to the Next Bullet in the List
		// Returns false if Current Bullet doesn't exist
		bool MoveToNextBullet();

		// Returns True if Current Ship is not NULL 
		bool CurrentBulletExists();
		
		// Returns True if Bullet is found in list
		bool BulletIsInList(int iBulletID);

		// Returns the Current Bullets ID
		int ReturnCurrentBulletsID() const;

		// Return pointer to Current Bullet
		CBullet* ReturnCurrentBullet() const;
		CBullet* ReturnBullet(int iBulletID) const;


		// Updates Bullets
		// NOTE: This should be called every frame
		void UpdateAllBulletsInList(unsigned long ulAmountOfTimeSinceLastUpdate);

		// Delete all Bullets from List
		void Purge();

		// Can be used to hold another list of weapons; perhaps ones with a special attribute
		CBulletQueue mcBulletQueue;

	private:

		// Retuns an Item if it's in the List
		// NOTE: Returns 0 if Item not found
		SBulletListItem* BulletIsInListAndReturnIt(int iBulletID) const;

		SBulletListItem* mspHead;			// Points to Start of List
		SBulletListItem* mspCurrentBullet;	// Points to Current Bullet
		int miIDCounter;					// Used to generate BulletID
};


#endif

