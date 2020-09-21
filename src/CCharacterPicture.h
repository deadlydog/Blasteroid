/*****************************************************************************/
/* CCharacterPicture.h (and CCharacterPicture.cpp)                           */
/* Written by Daniel Schroeder                                               */
/* Created in June 2004                                                      */
/* Last Updated November 12, 2006                                            */
/*---------------------------------------------------------------------------*/
/*   The CCharacterPicture class can be used to create Pictures which hold   */
/* the position of an image within a file (as one file may contain many      */
/* images). You do not specify the file name here though; you just use this  */
/* to specify the image position and/or height/width when blitting a texture.*/
/*   You can also create States, which are a collection of Pictures. You     */
/* specify the order in which they should appear and how long to wait before */
/* changing Pictures.  So in order to change the animation all you have to do*/
/* is change the current State.                                              */
/*****************************************************************************/

#ifndef CCHARACTER_PICTURE_H
#define CCHARACTER_PICTURE_H

// Holds State info. Example of a State would be Walking, Running, Jumping, etc.
struct SState
{
	int iState;
	int* ipPictureRotationOrder;
	int iCurrentPositionInRotation;
	int iNumOfPicturesInRotation;
	unsigned long ulPictureRotationTime;
	unsigned long ulDefaultPictureRotationTime;

	int iXVelocity;
	int iYVelocity;
	int iZVelocity;

	SState* spNextState;
};

// Holds the Picture of the image on the .bmp
struct SPicture
{
	int iPicture;
	int iLeft;
	int iTop;
	int iWidth;
	int iHeight;

	SPicture* spNextPicture;
};

// Controls the created Pictures and States
class CCharacterPicture
{
	public:

		CCharacterPicture();											// Constructor
		CCharacterPicture(CCharacterPicture const& cObjectToCopyFrom);	// Copy Constructor
		~CCharacterPicture();											// Destructor

		// Deep copy from another object
		void CopyFrom(CCharacterPicture const& cObjectToCopyFrom);

		// Move Character to specified world Position
		void MoveCharacterTo(int iX, int iY, int iZ = 0);
		void MoveCharacterXTo(int iX);
		void MoveCharacterYTo(int iY);
		void MoveCharacterZTo(int iZ);

		// Move Character relative to current Position
		void MoveCharacterRelative(int iX, int iY, int iZ = 0);
		void MoveCharacterXRelative(int iX);
		void MoveCharacterYRelative(int iY);
		void MoveCharacterZRelative(int iZ);

		// Specify Character picture coordinates on the .bmp
		void CreatePicture(int iPicture, int iLeft, int iTop, int iWidth, int iHeight);

		// Create a Character State. 0 Rotation Time = Do not Rotate Picture
		void CreateState(int iState, int* ipPictureRotationOrder, 
						 int iNumOfPicturesInRotation, unsigned long ulPictureRotationTime = 0,
						 int iXVelocity = 0, int iYVelocity = 0, int iZVelocity = 0);
		
		// Set the Current State. False means State does not exist
		// NOTE: Can set the Current Picture by specifying the StartingRotationPosition
		bool SetCurrentState(int iState, int iStartingRotationPosition = 0);

		// Change a States Picture Rotation Time
		bool ChangeStatesRotationTime(int iState, unsigned long ulPictureRotationTime);
		bool ChangeCurrentStatesRotationTime(unsigned long ulPictureRotationTime);

		// Change a States Default Picture Rotation Time
		bool ChangeStatesDefaultRotationTime(int iState, unsigned long ulNewDefaultRotationTime);
		bool ChangeCurrentStatesDefaultRotationTime(unsigned long ulNewDefaultRotationTime);

		// Restore a States Picture Rotation Time back to the default time
		bool RestoreStatesDefaultRotationTime(int iState);
		bool RestoreCurrentStatesDefaultRotationTime();

		// Set a States XYZ Velocities
		bool SetStatesVelocity(int iState, int iXVelocity, int iYVelocity, int iZVelocity = 0);
		bool SetStatesXVelocity(int iState, int iXVelocity);
		bool SetStatesYVelocity(int iState, int iYVelocity);
		bool SetStatesZVelocity(int iState, int iZVelocity);

		// Return XYZ Coordinates
		void ReturnCoordinates(int& iX, int& iY, int& iZ) const;
		int ReturnXCoordinate() const;
		int ReturnYCoordinate() const;
		int ReturnZCoordinate() const;

		// Return a States XYZ Velocities
		bool ReturnStatesVelocity(int iState, int& iXVelocity, int& iYVelocity, int& iZVelocity) const;
		int ReturnStatesXVelocity(int iState) const;
		int ReturnStatesYVelocity(int iState) const;
		int ReturnStatesZVelocity(int iState) const;

		// Return Character Picture Coordinates
		bool ReturnPictureCoordinates(int& iLeft, int& iTop, int& iWidth, int& iHeight) const;
		int ReturnLeft() const;
		int ReturnTop() const;
		int ReturnRight() const;
		int ReturnBottom() const;
		int ReturnWidth() const;
		int ReturnHeight() const;

		// Return Current Character State
		int ReturnCurrentState() const;

		// Return Current Character Picture
		int ReturnCurrentPicture() const;

		// Return Current Picture Rotation Position
		int ReturnCurrentPictureRotationPosition() const;

		// Return the Total Number of Pictures in a State
		int ReturnCurrentStatesTotalNumberOfPictures() const;
		int ReturnStatesTotalNumberOfPictures(int iStateID) const;

		// Set Current States Current Picture
		// NOTE: Moves Current Picture to Pictures first appearance in the Rotation Position, since the same Picture may appear in it several times
		bool SetCurrentStatesPicture(int iPicture);

		// Set Current States Current Picture to the Picture at the specified Rotation Position
		// NOTE: First RotationPosition starts at 1
		bool SetCurrentStatesPictureByRotationPosition(int iRotationPosition);

		// Return length of time before switching to next picture
		unsigned long ReturnPictureRotationTime() const;
		unsigned long ReturnDefaultPictureRotationTime() const;

		// Return if Picture exists
		bool DoesPictureExist(int iPicture) const;

		// Return if State exists
		bool DoesStateExist(int iStateID) const;

		// Returns if Current Picture is set yet
		bool CurrentPictureIsSet() const;

		// Returns if Current State is set yet
		bool CurrentStateIsSet() const;

		// Rotate the Picture if necessary (Should be called every frame)
		// Returns false if a needed Picture was not found
		bool UpdatePicture(unsigned long ulAmountOfTimeSinceLastUpdate);

		// Erase all info from this object
		void Purge();

		// The below functions should NOT be used by the user...they are for the Deep Copy functions
		// Return constant pointers to Current State/Picture and State/Picture List
		SState const* ReturnPointerToCurrentState() const;
		SState const* ReturnPointerToState(int iStateID) const;
		SState const* ReturnPointerToStateList() const;
		SPicture const* ReturnPointerToCurrentPicture() const;
		SPicture const* ReturnPointerToPicture(int iPicture) const;
		SPicture const* ReturnPointerToPictureList() const;
		
		// Return the Rotation Timers current time
		unsigned long ReturnRotationTimer() const;

	private:

		// XYZ Coordinates
		int miX, miY, miZ;
	
		// Pointer to Current State
		SState* mspCurrentState;

		// Pointer to Current Picture
		SPicture* mspCurrentPicture;

		// Pointer to State list
		SState* mspStateList;

		// Pointer to Picture list
		SPicture* mspPictureList;

		// Time used to tell if Pictures should be rotated or not yet
		unsigned long mulRotationTimer;
};

#endif

