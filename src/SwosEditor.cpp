/* Swoseditor 2018 - (c) 1996-2018 Mark Green (BippyM)

This is a C++ interpretation of SwosEditor. It will read and manipulate Amiga and PC save game files. This will be a long process to learn C++
including development of a working GUI, File I/O, file manipulation. Steps to create this will be as follows:

1). Read the Swos save file into memory
2). Each team will be an object/ class
3). Each player will be a class for each team, and associated to the team they belong to. This will allow players to be moved about
4).

Functions: (Edit as required)
Load team
Save Team 

*/


#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>

#include "Swos_offsets.h"		// Our offsets - Stored as data structures.

//string getVersion(char * buffer, char * CJOFFSET, int* swosOffsets, string* cjvers);

int main()
{
	const int VERSIONS = 8;							// Currently we support 8 versions
	const int short TEAMLENGTH = 684;				// Each team is 684 bytes
	int short numteams;								// Number of teams within the savefile
	
	// Rename some common types
	typedef const unsigned char ucchar;				// Unsigned Const Char - Shorten

	// Variables for the actual program
//	char buffer[200000];							// A save file will never be more than 200kb; Well i have never seen one!
	int filelen;
	const int BUFLEN = 4-1;							// Length of our buffer.
	
// Swos specific offset information. This will be copied into a string.
	// cjvers[i][j] [i]= This is the actual version ; [j] = This is the CJ version number to check against

	std::string cjvers[VERSIONS][2] = {  {"CJ281112", "Sensible World of Soccer v0.09 - 28/11/1994 11.00am"},
									{"CJ0612235288", "Sensible World of Soccer v1.0 - 06/12/1994 10.50pm"},
									{"CJ230511", "Sensible World of Soccer v1.1 - 23 / 05 / 1995 23.30pm"},
									{"CJ100117", "Sensible World of Soccer German Release - 10/01/1995"},
									{"CJ011211458A", "Sensible World of Soccer 95/96 - 01/12/1995 11.35"},
									{"CJ180421", "Sensible World of Soccer 95 / 96 Euro Update - 18 / 04 / 1996 21.50"},
									{"CJ061114", "Sensible World of Soccer 96/97 - 06/11/1996 13.55"},
									{"CJ061114", "Sensible World of Soccer PC Version - 06/11/1996 13.55"} 
	
	};



	enum offsets {
		// Constants so we can easily identify offsets later in the code . They are related to the array swosOffsets 

		NUM_PLAYERS_POS,	    // Number of players in the team
		TEAM_NAME_POS,			// Team Name
		MGR_FORENAME_POS,		// #   - Managers forename
		MGR_SURNAME_POS,		// #   - Managers Surname
		MGR_FULLNAME_POS,		// #  - Managers Full Name(Yes it is stored twice!!)
		PLR1_POSITION_POS,		// #  - Position of the first player in the file.This is then 38 bytes of data
		MONEY_POS,				// Money
		TACT_POS,				// First Tactic
		FIRST_TEAM,				// First team in the file
		CJOFFSET,				// File position of the version number
		MAX_OFFSETS
	};


	int rowNumber				= 0;					// This will hold the row number from the returned version. Row 0 is Swos Beta, Row 1 Swos v1.0 etc. This will then be used to populte the struct


	int swosOffsets[VERSIONS][MAX_OFFSETS] = { { 56717, 55429, 54987, 54996, 55460, 55500, 54742, 88374, 90635, 90614},  // Swos Beta [0][1][2][3][4][5][6][7][8][9] - Row 0
									 { 56719, 55431, 54989, 54998, 55462, 55502, 54744, 88378, 90637, 90618},	// Swos v1.0
									 { 56723, 55435, 54990, 54999, 55462, 55506, 54744, 88424, 90637, 90664},   // Swos 1.1
									 { 56719, 55431, 54990, 54999, 55462, 55502, 54744, 88378, 90637, 90618},	// Swos German Ver
									 { 57485, 56197, 55134, 55143, 56228, 56268, 54748, 96222, 98485, 98462},	// Swos 95-96
									 { 57483, 56195, 55132, 55141, 56226, 56266, 54746, 96220, 98483, 98460},	// Swos 95-96 Euro
									 { 57485, 56197, 55134, 55143, 56228, 56268, 54748, 96222, 98485, 98462},	// Swos 96-97
									 { 0,0,0,0,0,0,0,0,0,0 }	// Swos PC - Still need to get these offsets
	};

//	struct Swos_Main_Offsets swosVers[VERSIONS];

	// Version identifiers and memory locations
	
	// File path & name

	std::string file = "E://9596.CAR";
	
	// Get filesize for buffer creation
	// NEED TO MAKE THIS A LOOP WHEN A GUI IS CREATED.

	filelen = getFileSize(file);
	std::cout << std::endl << "Filesize is :" << filelen << std::endl;

	if (filelen == 0) {

		std::cout << "not a valid Swos Save File" << std::endl;
		return 0;
	}
	else if (filelen == -1) {

		std::cout << "File does not exist" << std::endl;
		return 0;
	}
	
	char *buffer;
	buffer = new char[filelen];							// Save file buffer


	// Load the file to our buffer
	loadfile(file, buffer, &filelen);					// Load the actual variable with the memlocation of the buffer
	
	
	// Here is where we will check the version numbers and return which version we are using

	std::string CJVer = "";
	int veroffset;

	Swos_Main_Offsets swosoffsets;		// The offsets

	
	// Check which version of a Swos save file we have: CJVer will hold the version string

	for (int i = 0; i < 8; i++) {
		CJVer = charToBuffer(buffer, swosOffsets[i][CJOFFSET], 0, 0x00);     // Offsets are located at array [x][9] (no buffer lenght, $0x00 terminating character)
		if (getversion(CJVer, cjvers, 2, i)) {
			std::cout << " matches " << cjvers[i][0] << " and is the " << cjvers[i][1] << std::endl;
			int swos_version = i;

			// Allocate the offsets to the main_offsets_structure
			swosoffsets.num_players_pos = swosOffsets[swos_version][NUM_PLAYERS_POS];
			swosoffsets.team_name_pos = swosOffsets[swos_version][TEAM_NAME_POS];
			swosoffsets.mgr_forename_pos = swosOffsets[swos_version][MGR_FORENAME_POS];
			swosoffsets.mgr_surname_pos = swosOffsets[swos_version][MGR_SURNAME_POS];
			swosoffsets.mgr_fullname_pos = swosOffsets[swos_version][MGR_FULLNAME_POS];
			swosoffsets.plr1_Position_pos = swosOffsets[swos_version][PLR1_POSITION_POS];
			swosoffsets.money_pos = swosOffsets[swos_version][MONEY_POS];
			swosoffsets.tact_pos = swosOffsets[swos_version][TACT_POS];
			swosoffsets.first_team = swosOffsets[swos_version][FIRST_TEAM];
			swosoffsets.CJOffset = swosOffsets[swos_version][CJOFFSET];
			
			unsigned int money = buffToInteger(buffer, swosoffsets.money_pos, 3);
			unsigned int numplrs = buffToInteger(buffer, swosoffsets.num_players_pos, 0);
			std::string teamname = charToBuffer(buffer, swosoffsets.first_team, 0, 0x00);
			std::cout << "number of players in team position is: " << swosoffsets.num_players_pos << std::endl;
			std::cout << "First team in the file is: " << teamname << std::endl;
			std::cout << "There are: " << numplrs << " players in the team" << std::endl;
			std::cout << "There are a total of " << (filelen - swosoffsets.first_team) / 684 << " teams in this save file" << std::endl;


			std::cout << "Team money is: " << money << std::endl;
			break;
		}
		else {
			//std::cout << "Does not match " << cjvers[i][0] << std::endl;
		}


	}

	
	// How much money does our team currently have
	unsigned int money = buffToInteger(buffer, 54744, BUFLEN);
	
	// How many teams are there

	//cout << std::endl << hex << "money = "<< money << std::endl;
	

	// Pause the output window
	std::cout << std::endl << "Press any key to continue/end.." << std::endl;
	int tmp;
	std::cin >> tmp;

	// Cleanup

	delete[] buffer;

	return 0;
}

