/*****************************************************************************/
/* CHighScores.h (and CHighScores.cpp)                                       */
/* Written by Daniel Schroeder                                               */
/* Created in 2004					                                         */
/* Last Updated August 30, 2005                                              */
/*---------------------------------------------------------------------------*/
/*   The CHighScores class is used to save and load high scrores from an     */
/* external file.                                                            */
/*****************************************************************************/
#ifndef CHIGH_SCORES_H
#define CHIGH_SCORES_H


const int iMAX_NUMBER_OF_HIGH_SCORES	= 3;	// Max number of High Scores to save per file
const int GiMAX_NAME_LENGTH				= 30;	// Max length a players name can be


// Holds a players info
struct SPlayer
{
	char caName[GiMAX_NAME_LENGTH + 1];	// Holds the Names of all the players in the High Score list (One extra spot for End Line character)
	unsigned int uiScore;				// Holds the Scores of all the players in the High Score list
	int iOtherData;						// Holds optional other data (such as difficulty game was on perhaps)
};


// Holds Names and Scores of players with a High Score
class CHighScores
{
	public:

		CHighScores();			// Default Constructor
		~CHighScores();			// Destructor

		// Inserts a High Score into the list
		// Returns true if Player was inserted into List, false if not
		// NOTE: High Score is only Inserted if it is greater than the other Scores in the List and the List is not full
		bool Insert(char* cpName, unsigned int uiScore, int iOtherData = 0);
	
		// Deletes a High Score from the list
		// Returns true if Player was deleted from List, false if there weren't found
		bool Delete(int iHighScorePositionToDelete);
		bool Delete(char* cpName, unsigned int uiScore);
		bool DeleteAllScores();

		// Stores the Name of the player in the specified High Score Position in cpName
		// Returns false if specified position is invalid, true if successful
		// NOTE: The supplied cpName must be allocated already as the Name is copied into it
		//       i.e. char* cpName = new char[GiMAX_NAME_LENGTH];
		bool ReturnName(int iHighScorePosition, char* cpName) const;
		
		// Returns the Score of the player in the specified High Score Position
		unsigned int ReturnScore(int iHighScorePosition) const;
		
		// Returns the Score of the player in last place
		unsigned int ReturnLastPlaceScore() const;

		// Returns true if the High Scores List is Empty/Full
		bool IsEmpty() const;
		bool IsFull() const;

		// Save/Load the High Scores to/from the specified File
		// Returns True if Successful, False if not
		// NOTE: Save automatically overwrites other files with the same File Name
		bool Save(char* cpFileName = "HighScores.dat");
		bool Load(char* cpFileName = "HighScores.dat");

	private:

		// Holds all of the High Score info
		SPlayer msHighScoresList[iMAX_NUMBER_OF_HIGH_SCORES];
};

#endif
