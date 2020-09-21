// High Scores Implementaion File (CHighScores.cpp)

#include "CHighScores.h"
#include <fstream>	// Needed to save file
#include <string>	// Needed to copy Names

using namespace std;

/*	Private Data:
			sPlayer msHighScoresList[iMAX_NUMBER_OF_HIGH_SCORES];	// Holds all of our High Score info
*/

// Default Constructor
// Sets all private data to default values
CHighScores::CHighScores()
{
	int iHighScoreIndex		= 0;	// Looping variable
	int iNameLengthIndex	= 0;	// Looping variable

	// Cycle through private data and set it all to empty
	for (iHighScoreIndex = 0; iHighScoreIndex < iMAX_NUMBER_OF_HIGH_SCORES; iHighScoreIndex++)
	{
		for (iNameLengthIndex = 0; iNameLengthIndex < GiMAX_NAME_LENGTH; iNameLengthIndex++)
		{
			// Set name to nothing
			msHighScoresList[iHighScoreIndex].caName[iNameLengthIndex] = 0;
		}
		
		// Set the last character of the name to end line
		msHighScoresList[iHighScoreIndex].caName[GiMAX_NAME_LENGTH] = '\n';

		// Set Score and Other Data to zero
		msHighScoresList[iHighScoreIndex].uiScore = 0;
		msHighScoresList[iHighScoreIndex].iOtherData = 0;
	}
}

// Class Destructor
CHighScores::~CHighScores()
{}

// Inserts a High Score into the list
// Returns True if player was inserted into High Score list, False if not
// NOTE: Player will only be inserted if their Score is better than
//		 any of the other Scores in the High Score list, AND their
//		 Score is GREATER than zero
bool CHighScores::Insert(char* cpName, unsigned int uiScore, int iOtherData)
{
	int iSpotToInsertTo		= 0;	// Holds the High Score Position to Insert the new High Score in to
	int iHighScoreIndex		= 0;	// Looping variable

	// Find which position to Insert the new High Score in to
	while (uiScore <= msHighScoresList[iSpotToInsertTo].uiScore && iSpotToInsertTo < iMAX_NUMBER_OF_HIGH_SCORES)
	{
		iSpotToInsertTo++;
	}

	// If the Position To Insert To is valid
	if (iSpotToInsertTo >= iMAX_NUMBER_OF_HIGH_SCORES)
	{
		// Return that the Player was not insterted into the List
		return false;
	}

	// Move all High Scores under Position To Insert To down one
	for (iHighScoreIndex = (iMAX_NUMBER_OF_HIGH_SCORES - 1); 
		 iHighScoreIndex > iSpotToInsertTo; iHighScoreIndex--)
	{
		// Move Name, Score, and Other Data down one spot
		//msHighScoresList[iHighScoreIndex] = msHighScoresList[iHighScoreIndex - 1];
		strncpy(msHighScoresList[iHighScoreIndex].caName, msHighScoresList[iHighScoreIndex - 1].caName, GiMAX_NAME_LENGTH);
		msHighScoresList[iHighScoreIndex].uiScore = msHighScoresList[iHighScoreIndex - 1].uiScore;
		msHighScoresList[iHighScoreIndex].iOtherData = msHighScoresList[iHighScoreIndex - 1].iOtherData;
	}

	// Place new Name and Score into list
	strcpy(msHighScoresList[iSpotToInsertTo].caName, cpName);
	msHighScoresList[iSpotToInsertTo].uiScore = uiScore;
	msHighScoresList[iSpotToInsertTo].iOtherData = iOtherData;

	// Return success
	return true;
}

// Removes the specified High Score from the list using which position the High Score is currently in
// Returns True if successful, False if not
// NOTE: HighScorePositionToDelete is the position in the High Score List, so it's the High Score List array position - 1
bool CHighScores::Delete(int iHighScorePositionToDelete)
{
	int iHighScoreIndex	= 0;		// Looping variable

	// Make sure the specified position is valid
	if (iHighScorePositionToDelete < 1 || iHighScorePositionToDelete > iMAX_NUMBER_OF_HIGH_SCORES ||
		IsEmpty())
	{
		return false;
	}

	// Move all Scores below High Score Position To Delete, up one position
	for (iHighScoreIndex = (iHighScorePositionToDelete - 1);
		 iHighScoreIndex < (iMAX_NUMBER_OF_HIGH_SCORES - 1); iHighScoreIndex++)
	{
		// Move Name and Score up one spot
		strncpy(msHighScoresList[iHighScoreIndex].caName, msHighScoresList[iHighScoreIndex + 1].caName, GiMAX_NAME_LENGTH);
		msHighScoresList[iHighScoreIndex].uiScore = msHighScoresList[iHighScoreIndex + 1].uiScore;
		msHighScoresList[iHighScoreIndex].iOtherData = msHighScoresList[iHighScoreIndex + 1].iOtherData;
	}


	// Place a blank High Score in the end of the list
	strcpy(msHighScoresList[iMAX_NUMBER_OF_HIGH_SCORES - 1].caName, "");
	msHighScoresList[iMAX_NUMBER_OF_HIGH_SCORES - 1].uiScore = 0;
	msHighScoresList[iMAX_NUMBER_OF_HIGH_SCORES - 1].iOtherData = 0;

	// Return Success
	return true;
}

// Deletes the specified High Score from the list
// Returns True if Successful, False if not
// NOTE: If there is more than one match, only Deletes the lowest positioned one
bool CHighScores::Delete(char* cpName, unsigned int uiScore)
{
	bool bNamesMatch	= false;	// Holds if the Names match or not
	bool bScoresMatch	= false;	// Holds if the Scores match or not
	int iPosition		= 0;		// Holds the Position the High Score to Delete is in
	int iHighScoreIndex	= 0;		// Looping variable

	// If the list is not empty
	if (IsEmpty())
	{
		// Return that there's nothing to delete
		return false;	
	}

	// Cycle through High Score list
	for (iHighScoreIndex = 0; iHighScoreIndex < iMAX_NUMBER_OF_HIGH_SCORES; iHighScoreIndex++)
	{
		// Compare the Names and Scores and hold if they are a match or not
		bNamesMatch = !(strcmp(cpName, msHighScoresList[iHighScoreIndex].caName));
		bScoresMatch = (msHighScoresList[iHighScoreIndex].uiScore == uiScore);

		// If both the Names and the Scores Match
		if (bNamesMatch && bScoresMatch)
		{
			// Hold the Position of the High Score to Delete
			iPosition = iHighScoreIndex + 1;
		}
	}

	// If we did not find a match
	if (iPosition == 0)
	{
		// Return that no match was found
		return false;
	}

	// Return if the Delete was Successful or not
	return Delete(iPosition);
}

// Deletes all High Scores from the list
// Returns True if Successful, False if not
bool CHighScores::DeleteAllScores()
{
	// While the High Score list is not empty
	while (!IsEmpty())
	{
		// Delete the First Place High Score
		Delete(1);
	}

	// Return if all the High Scores were Deleted from the list or not
	return (IsEmpty());
}

// Returns the Name of the player in the specified High Score Position
// Returns 0 if specified position is invalid
bool CHighScores::ReturnName(int iHighScorePosition, char* cpName) const
{
	// Make sure the specified position is valid
	if (iHighScorePosition < 1 || iHighScorePosition > iMAX_NUMBER_OF_HIGH_SCORES)
	{
		return false;
	}

	// Else return the Name in the position
	strcpy(cpName, msHighScoresList[iHighScorePosition - 1].caName);

	// Return success
	return true;
}

// Returns the specified Positions Score
// NOTE: Returns zero if the specified Position is not valid, or there
//		 isn't a High Score in the Position yet
unsigned int CHighScores::ReturnScore(int iHighScorePosition) const
{
	// Make sure the specified position is valid
	if (iHighScorePosition < 1 || iHighScorePosition > iMAX_NUMBER_OF_HIGH_SCORES)
	{
		return 0;
	}

	// Return the Positions Score
	return msHighScoresList[iHighScorePosition - 1].uiScore;
}

// Returns the Score of the Last Place Position
// NOTE: Returns zero if there is no High Score in the Last Place Position
unsigned int CHighScores::ReturnLastPlaceScore() const
{
	// Return the Last Places Score
	return msHighScoresList[iMAX_NUMBER_OF_HIGH_SCORES - 1].uiScore;
}

// Checks if the High Score List is Emtpy
// Returns True if it is Emtpy, False if it's not
bool CHighScores::IsEmpty() const
{
	// Returns True if there is no Score in the First Place Position
	return (msHighScoresList[0].uiScore == 0);
}

// Checks if the High Score List if Full
// Returns True if List is Full, False if not
bool CHighScores::IsFull() const
{
	// Returns True if there is a Score in the Last Place Position
	return (msHighScoresList[iMAX_NUMBER_OF_HIGH_SCORES - 1].uiScore > 0);
}

// Saves the High Scores to the specified File Name
// Returns True if Successful, False if not
// NOTE: Automatically overwrites other files with the same File Name
bool CHighScores::Save(char* cpFileName)
{
	bool bSaveSuccessful = false;	// Holds if the Save was Successful or not
	
	// Opens the specified File to write to
	ofstream ofHighScoreSaveFile;
	ofHighScoreSaveFile.open(cpFileName);

	// If the File was opened Successfully
	if (ofHighScoreSaveFile)
	{
		// Write this class info out to the File
		ofHighScoreSaveFile.write((char*)this, sizeof(CHighScores));

		// Store that the Save was Successful
		bSaveSuccessful = true;
	}

	ofHighScoreSaveFile.close();	// Close the File

	return bSaveSuccessful;			// Return if the Save was Successful or not
}

// Loads a High Score File from the specified File Name
// Returns True if the Load was Successful, False if not
bool CHighScores::Load(char* cpFileName)
{
	bool bLoadSuccessful = false;	// Holds if the Load was Successful or not

	// Opens the specified File to read from
	ifstream ifHighScoreLoadFile;
	ifHighScoreLoadFile.open(cpFileName, ios_base::in);

	// If the File was opened Successfully
	if (ifHighScoreLoadFile)
	{
		// Read in the class info from the File
		ifHighScoreLoadFile.read((char*) this, sizeof(CHighScores));

		// Store that the Load was Successful
		bLoadSuccessful = true;
	}

	ifHighScoreLoadFile.close();	// Close the File

	return bLoadSuccessful;			// Return if the Load was Successful or not
}

