/*****************************************************************************/
/* CShip.h (and CShip.cpp)                                                   */
/* Written by Daniel Schroeder                                               */
/* Created on July 22 2004                                                   */
/* Last Updated November 14, 2006                                            */
/*---------------------------------------------------------------------------*/
/*   The CShip class is used to hold all information of a Ship, including the*/
/* ships position, velocity, acceleration, weapons, health, armor, shields,  */
/* lives, points, AI, etc. This also holds the Ships Picture in mcPicture    */
/* (see CCharacterPicture.h). After setting all of the ships speeds (Velocity*/
/* or Acceleration), just call the Update() function to update the ships     */
/* position and Picture.                                                     */
/*****************************************************************************/

#ifndef CSHIP_H
#define CSHIP_H

#include "CCharacterPicture.h"
#include "CTimer.h"
#include "CBullet.h"
#include "CBulletList.h"


// Global variable signifying Unlimited Ammunition
const int GiUNLIMITED_AMMO	= -1;

// Holds a Weapon and it's amount of ammunition
struct SWeapon
{
	CBullet* cpBullet;
	int iAmmo;

	SWeapon* spNext;
	SWeapon* spPrevious;
};


// Holds an Items Type
struct SItem
{
	int iItemType;		// Holds the Type of Item

	SItem* spNext;		// Points to Next Item in the List
};


// Ship Status'
const int GiSHIP_STATUS_NONE			= 0;
const int GiSHIP_STATUS_DYING			= 1;
const int GiSHIP_STATUS_DEAD			= 2;
const int GiSHIP_STATUS_LEAVE_SCREEN	= 3;
const int GiSHIP_STATUS_DISABLED		= 4;


// ***** AI Pattern Data *****

const int GiALWAYS_FIRE		= -1;
const int GiALWAYS_MOVE		= -1;

const int iTOTAL_NUMBER_OF_MOVEMENT_PATTERNS = 19;

// Movement Patterns
enum EMovementPatterns
{
	eRandomMovement = 0,
	eMoveLeft,
	eMoveRight,
	eMoveUp,
	eMoveDown,
	eMoveUpLeft,
	eMoveUpRight,
	eMoveDownLeft,
	eMoveDownRight,
	eTrackObject,
	eTrackObjectVertically,
	eTrackObjectHorizontally,
	eAvoidObject,
	eAvoidObjectVertically,
	eAvoidObjectHorizontally,
	eStopAccelerating,			// Continues in whatever direction it happens to be going
	eStopHorizontalMovement,
	eStopVerticalMovement,
	eStopMovement				// Ship will come to a hault
};

// Fire (shoot) Patterns
enum EFirePatterns
{
	eRandomFiring = 0,
	eFireAsOftenAsPossible,
	eFireWhenCloseToObject,
	eFireWhenFarFromObject,
	eNeverFire
};

// Holds an AI Pattern
struct SPattern
{
	EMovementPatterns eMovementPattern;	// Holds what Movement to do
	unsigned long ulTimeToDoPatternFor;	// How long to do Movement for
	int iXTrackRange;					// If MovementPattern = TrackObject, this holds how close to objects X position to get
	int iYTrackRange;					// If MovementPattern = TrackObject, this holds how close to objects Y position to get
	int iXAvoidRange;					// If MovementPattern = AvoidObject, this holds how far from objects X position to get
	int iYAvoidRange;					// If MovementPattern = AvoidObject, this holds how far from objects Y position to get
	int iTrackingPrecision;				// How accurate the Track/AvoidObject Movement will be

	EFirePatterns eFirePattern;	// What Fire Pattern to follow
	int iXCloseFireRange;		// If FirePattern = FireWhenCloseToObject, this holds how close to objects X position Ship must be
	int iYCloseFireRange;		// If FirePattern = FireWhenCloseToObject, this holds how close to objects Y position Ship must be
	int iXFarFireRange;			// If FirePattern = FireWhenFarFromObject, this holds how far from objects X position Ship must be
	int iYFarFireRange;			// If FirePattern = FireWhenFarFromObject, this holds how far from objects Y position Ship must be

	int iPatternID;				// Unique ID of this Pattern
	SPattern* spNext;			// Points to Next Pattern
};


// Holds all information of a Ship
class CShip
{
	public:
		
		//********** Contructors and Copy Functions **********

		CShip();										// Default Constructor
		CShip(CClock* cpClock);							// Explicit Constructor
		~CShip();										// Destructor

		CShip(CShip const& cShipToCopyFrom);			// Copy Constructor
		void CopyFrom(CShip const& cShipToCopyFrom);	// Copy from another object


		//********** Movement Functions **********
		
		// NOTE: All positions/movements are in terms of Pixels. ex Velocity = pixels/sec

		// Move Ship to specified position
		// NOTE: MoveShip() is not required when using this function
		void MoveTo(float fX, float fY, float fZ = 0);
		void MoveXTo(float fX);
		void MoveYTo(float fY);
		void MoveZTo(float fZ);

		// Move Ship relative to current position
		// NOTE: MoveShip() is not required when using this function
		void MoveToRelative(float fX, float fY, float fZ = 0);
		void MoveXRelative(float fX);
		void MoveYRelative(float fY);
		void MoveZRelative(float fZ);

		// Return Ships Coordinates
		void ReturnCoordinates(float& fX, float& fY, float& fZ) const;
		float ReturnXCoordinate() const;
		float ReturnYCoordinate() const;
		float ReturnZCoordinate() const;

		// Set Velocities
		// NOTE: Requires Update() to be called to actually move the ship by current Velocity
		void SetVelocities(float fXVelocity, float fYVelocity, float fZVelocity = 0);
		void SetXVelocity(float fXVelocity);
		void SetYVelocity(float fYVelocity);
		void SetZVelocity(float fZVelocity);

		// Return Velocities
		void ReturnVelocities(float& fXVelocity, float& fYVelocity, float& fZVelocity) const; 
		float ReturnXVelocity() const;
		float ReturnYVelocity() const;
		float ReturnZVelocity() const;

		// Set Max Velocities
		void SetMaxVelocities(float fMaxXVelocity, float fMaxYVelocity, float fMaxZVelocity = 0);
		void SetMaxXVelocity(float fMaxXVelocity);
		void SetMaxYVelocity(float fMaxYVelocity);
		void SetMaxZVelocity(float fMaxZVelocity);

		// Return Max Velocities
		void ReturnMaxVelocities(float& fMaxXVelocity, float& fMaxYVelocity, float& fMaxZVelocity) const;
		float ReturnMaxXVelocity() const;
		float ReturnMaxYVelocity() const;
		float ReturnMaxZVelocity() const;

		// Set Min Velocities
		void SetMinVelocities(float fMinXVelocity, float fMinYVelocity, float fMinZVelocity = 0);
		void SetMinXVelocity(float fMinXVelocity);
		void SetMinYVelocity(float fMinYVelocity);
		void SetMinZVelocity(float fMinZVelocity);

		// Return Min Velocities
		void ReturnMinVelocities(float& fMinXVelocity, float& fMinYVelocity, float& fMinZVelocity) const;
		float ReturnMinXVelocity() const;
		float ReturnMinYVelocity() const;
		float ReturnMinZVelocity() const;

		// Set Accelerations
		// Acceleration is used to increase/decrease the velocity every time MoveShip() is called
		// If you do not want the Ship to speed up/slow down, set Accelerations to zero
		// NOTE: Requires Update() to be called each frame to actually move the ship
		// NOTE: When UpdateShipsPosition() is called Velocity is automatically increased/decreased by current Acceleration
		void SetAccelerations(float fXAcceleration, float fYAcceleration, float fZAcceleration = 0);
		void SetXAcceleration(float fXAcceleration);
		void SetYAcceleration(float fYAcceleration);
		void SetZAcceleration(float fZAcceleration);
		
		// Return Accelerations
		void ReturnAccelerations(float& fXAcceleration, float& fYAcceleration, float& fZAcceleration) const; 
		float ReturnXAcceleration() const;
		float ReturnYAcceleration() const;
		float ReturnZAcceleration() const;

		// Set Max Accelerations
		void SetMaxAccelerations(float fMaxXAcceleration, float fMaxYAcceleration, float fMaxZAcceleration = 0);
		void SetMaxXAcceleration(float fMaxXAcceleration);
		void SetMaxYAcceleration(float fMaxYAcceleration);
		void SetMaxZAcceleration(float fMaxZAcceleration);

		// Return Max Accelerations
		void ReturnMaxAccelerations(float& fMaxXAcceleration, float& fMaxYAcceleration, float& fMaxZAcceleration) const;
		float ReturnMaxXAcceleration() const;
		float ReturnMaxYAcceleration() const;
		float ReturnMaxZAcceleration() const;

		// Set Min Accelerations
		void SetMinAccelerations(float fMinXAcceleration, float fMinYAcceleration, float fMinZAcceleration = 0);
		void SetMinXAcceleration(float fMinXAcceleration);
		void SetMinYAcceleration(float fMinYAcceleration);
		void SetMinZAcceleration(float fMinZAcceleration);

		// Return Min Accelerations
		void ReturnMinAccelerations(float& fMinXAcceleration, float& fMinYAcceleration, float& fMinZAcceleration) const;
		float ReturnMinXAcceleration() const;
		float ReturnMinYAcceleration() const;
		float ReturnMinZAcceleration() const;

		// Set ships Normal Accelerations (Are not used in UpdateShipsPosition())
		// These are just used to hold the Ships usual Accelerations
		void SetNormalAccelerations(float fNormalXAcceleration, float fNormalYAcceleration, float fNormalZAcceleration = 0);
		void SetNormalXAcceleration(float fNormalXAcceleration);
		void SetNormalYAcceleration(float fNormalYAcceleration);
		void SetNormalZAcceleration(float fNormalZAcceleration);

		// Return ships Normal Accelerations
		void ReturnNormalAccelerations(float& fNormalXAcceleration, float& fNormalYAcceleration, float& fNormalZAcceleration) const;
		float ReturnNormalXAcceleration() const;
		float ReturnNormalYAcceleration() const;
		float ReturnNormalZAcceleration() const;

		// Set Frictions
		// Frictions will slow the ship down no matter what direction it is going
		// Zero friction means the ship will not slow down on its own
		// NOTE: Requires Update() to be called each frame to have an effect
		// NOTE: Specified friction X will slow ship down by X pixels per second
		void SetFrictions(float fXFriction, float fYFriction, float fZFriction = 0);
		void SetXFriction(float fXFriction);
		void SetYFriction(float fYFriction);
		void SetZFriction(float fZFriction);

		// Return Frictions
		void ReturnFrictions(float& fXFriction, float& fYFriction, float& fZFriction) const;
		float ReturnXFriction() const;
		float ReturnYFriction() const;
		float ReturnZFriction() const;

		// Updates Ships Position and Picture
		// Moves Ship by specified Velocity amounts, which is calculated using the Acceleration values
		// NOTE: This should be called every frame
		// NOTE: ulAmountOfTimeSinceLastUpdate should be in milliseconds (1000ms = 1s)
		void Update(unsigned long ulAmountOfTimeSinceLastUpdate);



		//********** Ship Attributes (Heath/Shield/Status/etc) Functions **********

		// Set/Return Ships General Type (ie Player or Enemy)
		void SetGeneralShipType(int iType);
		int ReturnGeneralShipType() const;

		// Set/Return Ships Specific Type (ie Which Player or Enemy)
		void SetSpecificShipType(int iType);
		int ReturnSpecificShipType() const;

		// Set/Increase or Decrease/Return Health
		void SetHealth(int iHealth);
		void SetHealthRelative(int iHealthToAdd);
		int ReturnHealth() const;

		// Set/Return Max Health
		// Will only limit SetHealth() if MaxHealth is not -1
		void SetMaxHealth(int iMaxHealth);
		int ReturnMaxHealth() const;

		// Set/Return Armor
		// Armor should reduce the amount of damage taken by the Ship
		// NOTE: User will have to take Armor into account themselves when determining damage taken
		void SetArmor(float fArmor);
		float ReturnArmor() const;

		// Set/Return Max Armor
		// Will only limit SetArmor() if MaxArmor is not -1
		void SetMaxArmor(float fMaxArmor);
		float ReturnMaxArmor() const;

		// Set Shield Strength
		// Shields should take damage instead of Ship
		// NOTE: User will have to take Shield Strength into account themselves when determining damage taken
		void SetShieldStrength(float fShieldStrength);
		float ReturnShieldStrength() const;
	
		// Set/Return Max Shield Strength
		// Will only limit SetShieldStrength() if MaxShieldStrength is not -1
		void SetMaxShieldStrength(float fMaxShieldStrength);
		float ReturnMaxShieldStrength() const;

		// Set/Return Ships Status
		void SetStatus(int iStatus = GiSHIP_STATUS_NONE);
		int ReturnStatus() const;

		// Set/Return Number Of Lives
		void SetNumberOfLives(int iLives);
		int ReturnNumberOfLives() const;

		// Set/Return Max Number Of Lives
		// Will only limit SetNumberOfLives() if MaxLives is not -1
		void SetMaxNumberOfLives(int iMaxLives);
		int ReturnMaxNumberOfLives() const;

		// Set/Increase or Decrease/Return number of Points ship is worth
		// NOTE: Makes sure Points do not go below zero
		void SetNumberOfPoints(unsigned int uiPoints);
		void SetNumberOfPointsRelative(int iPointsToAdd);
		unsigned int ReturnNumberOfPoints() const;

		// Set/Return if Ship is colliding with another Ship or not
		void SetIfShipIsColliding(bool bIsCollision = true);
		bool ReturnIfShipIsColliding() const;



		//********** Weapon Functions **********

		// Add/Delete/Find a Weapon in the Weapon List
		// NOTE: Add Weapon() returns False if Weapon is already in List, other functions
		// return false if WeaponType is not found
		// NOTE: Weapons Types are equal to the given Bullets Types
		// NOTE: A new Bullet is created from the given Bullet Model, so the given Bullet 
		// Model will not be modified in any way
		bool AddWeapon(CBullet cpBulletModel, int iAmountOfAmmo = GiUNLIMITED_AMMO);
		bool DeleteWeapon(int iSpecificWeaponType);
		bool DeleteAllWeaponsOfGeneralType(int iGeneralType);
		bool WeaponIsInList(int iSpecificWeaponType) const;

		// Set the Current Weapon
		// NOTE: Returns false if WeaponType not found or List empty
		bool SetCurrentWeapon(int iSpecificWeaponType);
		bool SetCurrentWeaponToFirstWeaponInList();

		// Return a Weapon in the Weapon List
		// NOTE: Returns 0 if Weapon not found
		CBullet* ReturnWeaponsBullet(int iSpecificWeaponType) const;
		CBullet* ReturnCurrentWeaponsBullet() const;
		
		// Return the Current Weapon Type
		// NOTE: Returns -1 if Current Weapon not currently set
		int ReturnCurrentWeaponsSpecificType() const;
		int ReturnCurrentWeaponsGeneralType() const;

		// Returns if the Current Weapon has an Alternate Effect than doing damage
		// NOTE: Returns false if Current Weapon doesn't exist
		bool ReturnIfCurrentWeaponHasAlternateEffect() const;

		// Returns time Delay until Weapon can be fired again
		// Returns zero if Current Weapon does not exist
		unsigned long ReturnCurrentWeaponsFireDelay() const;

		// Move the Current Weapon to the next Weapon in the List (wraps around end of list)
		bool NextWeapon();
		bool PreviousWeapon();

		// Set a Weapons amount of Ammo
		// NOTE: Returns false if Weapon not found
		bool SetAmmo(int iWeaponType, int iAmountOfAmmo = GiUNLIMITED_AMMO);
		bool SetCurrentWeaponsAmmo(int iAmountOfAmmo = GiUNLIMITED_AMMO);

		// Add to a Weapons current amount of Ammo
		// NOTE: If subtracting Ammo, total will not go below zero
		// NOTE: Returns false if Weapon not found
		bool SetAmmoRelative(int iSpecificWeaponType, int iAmountOfAmmoToAdd);
		bool SetCurrentWeaponsAmmoRelative(int iAmountOfAmmoToAdd);

		// Return a Weapons ammount of Ammo
		// NOTE: Returns 0 if Weapon not found
		int ReturnAmmo(int iSpecificWeaponType) const;		
		int ReturnCurrentWeaponsAmmo() const;

		// Return Current Weapons Bullets XYZ coordinates to be fired from
		// X coordinate is centered to ship
		// Y coordinate is same as top of Ship
		// NOTE: Returns false/-1 if there is no Current Weapon
		bool ReturnCurrentWeaponsFireFromPosition(float& fX, float& fY, float& fZ);
		float ReturnCurrentWeaponsFireFromXPosition() const;
		float ReturnCurrentWeaponsFireFromYPosition() const;
		float ReturnCurrentWeaponsFireFromZPosition() const;


		//********** Item Functions **********

		// Add/Remove Item Type to list
		// NOTE: AddItem() returns false if Item Type is already in List
		// NOTE: RemoveItem() returns false if Item Type is not found in the List
		bool AddItem(int iItemType);
		bool RemoveItem(int iItemType);
		
		// Returns Current Items Type
		// Returns 0 if Current Item doesn't exist
		int ReturnCurrentItemsType() const;

		// Set Current Item to the specified Type if it's in the List
		// Returns false if specified Type is not in the List
		bool SetCurrentItemToType(int iItemType);

		// Return if Current Item exists or not
		bool CurrentItemExists() const;

		// Moves Current Item to start of Item List
		void MoveCurrentItemToStartOfItemList();

		// Moves Current Item to next Item in List (With Wrap() wraps around to beginning of Item List)
		// Returns false if Current Item doesn't exist or Item List is empty
		bool MoveCurrentItemToNextItem();
		bool MoveCurrentItemToNextItemWithWrap();

		// Returns true if Item Type is in List, false if not
		bool ItemIsInList(int iItemType) const;

		// Returns how many Items are in the Item List
		int ReturnNumberOfItemsInList() const;

		// Set/Return Chance of Ship dropping an Item
		// NOTE: Chance = 1 means a 1 in 1 (100%) chance of dropping, 2 is a 1 in 2 (50%) chance of dropping, etc
		void SetChanceOfDroppingItem(unsigned int uiOneInXChanceOfDropping);
		unsigned int ReturnChanceOfDroppingItem() const;

		// Returns true if Ship Should Drop Item, false if not
		// Returns false if ChanceOfDropping has not been set or is zero
		bool ShipShouldDropItem() const;

		// Returns which Item Type should be dropped (random)
		// Returns -1 if Item List is empty
		// NOTE: Resets Current Item
		int ReturnItemTypeToDrop();
// TODO: Give each Item it's own individual ChanceOfDropping() and drop first one 


		//********** AI Functions **********
	
		// Add a Pattern to this AIs Pattern List
		// Returns Patterns unique ID for finding it later if needed
		// NOTE: If TimeToDoPatternFor is zero it will use the values supplied to the SetRandomSwitchPatternTimer() function to randomly decide when to switch Patterns
		//		 If TimeToDoPatternFor is greater than zero, it will do Pattern for that long before switching Patterns
		// NOTE: X/YMovementRange and TrackingPrecision will only be used if eMovementPattern = eTrack/AvoidObject
		// NOTE: X/YFireRange will only be used if eFirePattern = eFireWhenClose/FarToObject
		// NOTE: TrackingPrecision detmines how well this Ship will track/avoid the Object. A lower TrackingPrecision means better accuracy
		int AddPatternToList(EMovementPatterns eMovementPattern = eRandomMovement, EFirePatterns eFirePattern = eRandomFiring,
							 unsigned long ulTimeToDoPatternFor = 0, int iXMovementRange = GiALWAYS_MOVE, int iYMovementRange = GiALWAYS_MOVE,
							 int iTrackingPrecision = 0, int iXFireRange = GiALWAYS_FIRE, int iYFireRange = GiALWAYS_FIRE);

		// Remove a Pattern from this AIs Pattern List
		// Returns false if Pattern to remove is not in List
		bool DeletePatternFromList(int iPatternID);


		// Use the specified Pattern
		// Returns false if Pattern is not in this AIs Pattern List
		bool UsePattern(int iPatternID);

		// Choose a new Pattern (wraps around)
		// Returns false if this AI does not have any Patterns in it's Pattern List
		bool ChangePattern();


		// Set how often the AI should Randomly Switch Patterns
		// If no TimeToDoPatternFor was specified when adding the Pattern, the AI Pattern will not 
		// switch until MinSwitchTime has elapsed, and will switch before MaxSwitchTime has elapsed
		// Also, if ChoosePatternsRandomly has been set, these values will also be used to determine when to switch Patterns all the time
		void SetRandomSwitchPatternTimes(unsigned long ulMinSwitchTime, unsigned long ulMaxSwitchTime);

		// Return Min/Max/Current Random Switch Pattern Times
		unsigned long ReturnMinRandomSwitchPatternTime() const;
		unsigned long ReturnMaxRandomSwitchPatternTime() const;
		unsigned long ReturnSwitchPatternTime() const;

		// Set how often the Ship should change directions if set to Move Randomly
		// The Ship will wait for MinMovementTime to elapse before changing directions again, but no longer than MaxMovementTime
		void SetRandomMovementTimes(unsigned long ulMinMovementTime, unsigned long ulMaxMovementTime);

		// Return Min/Max/Current Random Movement Times
		unsigned long ReturnMinRandomMovementTime() const;
		unsigned long ReturnMaxRandomMovementTime() const;
		unsigned long ReturnRandomMovementTime() const;

		// Set how often the Ship should Fire if set to Fire Randomly
		// The Ship will wait for MinFireTime to elapse before firing again, but no longer than MaxFireTime
		void SetRandomFireTimes(unsigned long ulMinFireTime, unsigned long ulMaxFireTime);

		// Return Min/Max/Current Random Fire Times
		unsigned long ReturnMinRandomFireTime() const;
		unsigned long ReturnMaxRandomFireTime() const;
		unsigned long ReturnRandomFireTime() const;

		// Return Current Random X/Y Tracking Precisions
		int ReturnRandomXTrackingPrecision() const;
		int ReturnRandomYTrackingPrecision() const;


		// Set/Return if Ship should Choose Patterns from the Pattern List Randomly or in Order
		// If provided value is true, Patterns will be chosen Randomly from the List
		// If provided value is false, Patterns will follow the order in which they were added (will wrap around)
		// Returns false if Pattern List is empty
		void ChoosePatternsRandomly(bool bUseRandomPatterns = true);
		bool ReturnIfPatternsAreBeingChosenRandomly() const;


		// Return if AI Has Ran at least Once yet or not
		bool ReturnIfAIHasRanOnce() const;

		// Set/Return how long the Ship should stay on the screen before leaving
		// NOTE: TimeToStayOnScreen = 0 means do not leave the screen
		// NOTE: Timer is started as soon as object (Ship) is created
		void StayOnScreenForXMilliseconds(unsigned long ulTimeToStayOnScreen = 0);
		unsigned long ReturnHowLongShipShouldStayOnScreen() const;

			
		// Set/Return if the Ship should be firing or not
		void SetIfShipShouldFire(bool bShipShouldFire);
		bool ReturnIfShipShouldFire() const;


		// Move Ship based on AI Patterns. Supply Players Ship in case we need to track it
		// NOTE: This should be called every frame
		// NOTE: Once Ship Status = GiSHIP_STATUS_LEAVE_SCREEN the Ship will accelerate towards bottom of screen
		// Returns false if List is Empty
		bool PerformAI(CShip const* cpPlayer);



		//********** Other Functions and Objects **********

		// Erase all information as if object was just created
		void Purge();

		// Holds Ships Pictures
		CCharacterPicture	mcPicture;

		// Holds a List of Bullets that have already hit the Ship (so they don't hit them twice)
		CBulletQueue		mcBulletsThatHaveAlreadyHitShipList;

		// Weapon Timers
		CTimer	mcFireWeaponDelayTimer;		// Use to make sure Weapon is not fired too often
		CTimer	mcChangeWeaponDelayTimer;	// Use to add a delay when switching Weapons
		CTimer	mcDetonateBulletDelayTimer;	// Use to add a delay when detonating Bullets


		// These next functions should not be used by the user - they are only used for the Copy function
		
		// Return pointer to Weapon List
		SWeapon const* ReturnPointerToWeaponList() const;

		// Return pointer to Current Weapon
		SWeapon const* ReturnPointerToCurrentWeapon() const;

		// Return pointer to Item List
		SItem const* ReturnPointerToItemList() const;

		// Return pointer to Current Item
		SItem const* ReturnPointerToCurrentItem() const;

		// Return pointer to AI Pattern List
		SPattern const* ReturnPointerToPatternList() const;

		// Return pointer to Current Pattern
		SPattern const* ReturnPointerToCurrentPattern() const;

	private:

		//***** Movement Private Data *****

		float mfX, mfY, mfZ;									// XYZ Coordinates
		float mfXVelocity, mfYVelocity, mfZVelocity;			// Ship Velocities
		float mfMaxXVelocity, mfMaxYVelocity, mfMaxZVelocity;	// Max Ship Velocities
		float mfMinXVelocity, mfMinYVelocity, mfMinZVelocity;	// Min Ship Velocities
		float mfXAcceleration, mfYAcceleration, mfZAcceleration;// Ship Accelerations
		float mfMaxXAcceleration, mfMaxYAcceleration, mfMaxZAcceleration;	// Max Ship Accelerations
		float mfMinXAcceleration, mfMinYAcceleration, mfMinZAcceleration;	// Min Ship Accelerations
		float mfNormalXAcceleration, mfNormalYAcceleration, mfNormalZAcceleration;	// Stores Ships Normal Acceleration Rates
		float mfXFriction, mfYFriction, mfZFriction;			// Friction upon ship (slows ship down)


		//***** Ship Attributes Private Data *****

		int miHealth;								// Ships Hit Points (HP)
		int miMaxHealth;							// Max Hit Points Ship can have
		float mfArmor;								// How much Armor the Ship has
		float mfMaxArmor;							// Max Armor Ship can have
		float mfShieldStrength;						// Ships Shield Strength
		float mfMaxShieldStrength;					// Max Shield Strength Ship can have
		int miStatus;								// Ships current status
		int miLives;								// Number of Lives left
		int miMaxLives;								// Max Number of Lives Ship can have
		int miGeneralShipType;						// General Type of Ship
		int miSpecificShipType;						// Specific Type of Ship
		unsigned int muiPoints;						// Players Points / Points earned from killing this enemy ship
		bool mbCollision;							// Tells if Ship is colliding with other Ships

		
		//***** Weapons Private Data *****

		SWeapon* mspWeaponListHead;					// Points to head of Weapon List
		SWeapon* mspWeaponListTail;					// Points to tail of Weapon List
		SWeapon* mspCurrentWeapon;					// Points to Ships Current Weapon


		//***** Items Private Data *****

		SItem* mspItemListHead;						// Points to head of Item List
		SItem* mspCurrentItem;						// Points to the Current Item

		unsigned int muiChanceOfDropping;			// Holds the Chance of the Ship dropping an Item
		int miItemCount;							// Holds how many Items are in the Item List


		//***** AI Private Data *****

		bool SetRandomValues();						// Sets the Random Times and Tracking Precision

		CTimer mcSwitchPatternTimer;				// Timer used to determine when to switch Patterns
		CTimer mcRandomMovementTimer;				// Timer used to determine when to change Movement Patterns Randomly
		CTimer mcRandomFireTimer;					// Timer used to determine how often to Fire (shoot) Randomly

		unsigned long mulMinRandomSwitchPatternTime;// Minimum Time that must elapse before Randomly Switching Patterns again
		unsigned long mulMaxRandomSwitchPatternTime;// Maximum Time that can elapse before Randomly Switching Patterns again
		unsigned long mulMinRandomMovementTime;		// Minimum Time that must elapse before moving in another Random direction
		unsigned long mulMaxRandomMovementTime;		// Maximum Time that can elapse before moving in another Random direction
		unsigned long mulMinRandomFireTime;			// Minimum Time that must elapse before Randomly firing again
		unsigned long mulMaxRandomFireTime;			// Maximum Time that can elapse before Randomly firing again

		unsigned long mulSwitchPatternTime;			// Holds Time that must elapse before Switching Patterns
		unsigned long mulRandomMovementTime;		// Holds Time that must elapse before Randomly changing direction
		unsigned long mulRandomFireTime;			// Holds Time that must elapse before Randomly firing again

		int miRandomXTrackingPrecision;				// Holds a Random X value based on the Current Patterns Tracking Precision
		int miRandomYTrackingPrecision;				// Holds a Random Y value based on the Current Patterns Tracking Precision

		bool mbChoosePatternsRandomly;				// Tells if this Ship chooses Patterns Randomly or follows a set order
		
		CTimer mcOnScreenTimer;						// Used to tell how long Ship has been On Screen
		unsigned long mulStayOnScreenTime;			// How long this Ship should stay On Screen before leaving

		bool mbShipShouldFire;						// Used to tell if the Ship should fire (shoot)
		bool mbAIHasRanOnce;						// Used to tell if the AI Has Ran at least One time yet

		SPattern* mspPatternListHead;				// Points to start of Pattern List
		SPattern* mspCurrentPattern;				// Points the the Pattern Currently in use
		int miPatternIDCounter;						// Used to determine Patterns unique ID

		// TODO: Instead of just having a Current Movement Pattern, have a Current Pattern. Will need to overload the = operator too
		EMovementPatterns eCurrentMovementPattern;	// Used to hold which Movement Pattern to perform (since Random Movement changes the value of the Movement Pattern)
};


#endif

