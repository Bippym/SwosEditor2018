/* Sensible World of Soccer: Version offsets & Functions */


#include <iostream>
#include <string>
#include <fstream>
#include "Swos_offsets.h"

class Team {

	struct Player {
		// Each player is 38 bytes long
		// To get attributes we will need to bit shift.

	private:
		char nationality;
		char unknown_1;
		char shirt_number;
		char name[23];
		char pos_ethnic;
		char book_inj;
		char passing;					//  
		char shooting;					// "shooting = shooting >> 4 & 0xff" 
		char heading;					// "Heading = shooting & 0xf"  DO THIS BEFORE THE ABOVE
		char tackling;
		char ballcont;
		char speed;
		char finishing;
		char value;
		char lge_goals;
		char cup1_goals;
		char cup2_goals;
		char euro_goals;
		char unknown_2;

	};


private:



};


int getFileSize(const std::string &fileName)
{
	std::ifstream file(fileName.c_str(), std::ifstream::in | std::ifstream::binary);

	if (!file.is_open())
	{
		return -1;
	}

	file.seekg(0, std::ios::end);
	int fileSize = file.tellg();
	file.close();

	return fileSize;
}

void *loadfile(std::string file, char *buf, int *fileleng) {

	// Loads a file, copies it into a buffer and returns its start address for editing
	//cout << buf;
	std::ifstream input(file, std::ios::in | std::ios::binary | std::ios::ate);

	if (input.is_open()) {

//		*fileleng = input.tellg();		// length of file
		input.seekg(0, std::ios::beg);		// go to beginning of file
		input.read(buf, *fileleng);		// read file into the buffer
		input.close();					// close file

										// Determine the size of the required buffer
		std::cout << "File size in loadfunction is: " << *fileleng << " bytes";
		std::cout << " and the buffer location in memory is: $" << fileleng << std::endl;
		return 0;
	}
	else {
		std::cout << "File doesn't exist" << std::endl;
		return 0;
	}

}

int buffToInteger(char * buffer, int startbuf, int buflen) {
	// Takes 4 byte char and converts to an unsigned int

	unsigned int a = 0;

	//a = static_cast <int> (static_cast <unsigned char> (buffer[startbuf]) << (8 * (buflen)));

	for (int i = 0; i <= buflen; i++) {
		//	std::cout << hex<< (unsigned short int)buffer[startbuf + i]<< dec << std::endl << std::endl;

		a |= static_cast <int> (static_cast <unsigned char> (buffer[startbuf + i]) << (8 * (buflen - i)));
		//	std::cout << "i=" << i << " buflen = " << buflen << " buflen-i *8 =" << 8 * (buflen - i) << std::endl;
	}

	return a;
}

std::string charToBuffer(char * inbuffer, int startbuf, int buflen, char termchar) {
	/* Takes a char buffer of either a pre-determined length in chars or a terminating character.
	If both a length and a terminating character are given then it will give the length paramenter precedence

	*/

	std::string str = "";

	char a;
	int k = 0;
	//std::cout << "Starting offset: "<< startbuf << std::endl;

	// Test if a bufferlength is entered, this takes precedence over a terminating character
	if (buflen != 0) {

		//	std::cout << "A buffer length of: " << buflen << " is entered. This will take precedence over the terminating character" << std::endl << std::endl;

		for (int i = 0; i <= buflen; i++) {
			a = inbuffer[startbuf + i];
			str = str + inbuffer[startbuf + i];
		}
		return str;
	}
	else {
		//	std::cout << "No buffer length has been entered. We will keep going until we reach the character " << hex << (int) termchar << dec << std::endl;
		do {
			a = inbuffer[startbuf + k];
			// check if we have the terminating char, if we do we need to strip it
			if (a == termchar) {
				break;
			}
			str = str + inbuffer[startbuf + k];
			k++;
		} while (a != 0);

	}
	//CJVer.erase(CJVer.begin() + CJVer.length() - 1);
	return str;
}

bool getversion(std::string vers, std::string cjvers[][2], int length, int i) {

//	for (int i = 0; i < 8; i++) {

		// Comparison
		if ((vers.compare(cjvers[i][0])) == 0) {
			//std::cout << vers << " matches " << cjvers[i][0] << " and is the " << cjvers[i][1] << std::endl;
			return true;
		}
		else {
			// Now we need to assign the offsets to the struct

			//std::cout << vers << " Does not match " << cjvers[i][0] << std::endl;
			return false;
		}

		//cout << cjvers[i][0] << std::endl;

	//}
}

// This function get's all the data from the save file and loads it into the various objects
void get_team_data() {


}

/*

struct CJ_Offsets {

	//string cjvers[8] = { "CJ281112", "CJ031223", "CJ23051128", "CJ100117", "CJ011211", "CJ180421", "CJ061114", "CJ061114" };
	//unsigned int cjlocation[8] = { 90614, 90618, 90664, 90618, 98462, 98460, 98462, 98462 };

	// Holds the different offset locations
	std::string Swos_Beta			= "CJ281112";
	std::string Swos_Release_v10	= "CJ031223";
	std::string SwosRelease_v11		= "CJ23051128";
	std::string SwosGerman			= "CJ100117";
	std::string Swos9596			= "CJ011211";
	std::string Swos9596_Euro		= "CJ180421";
	std::string Swos9697			= "CJ061114";
	std::string SwosPC				= "CJ061114";  // This is not the PC version. Needs to be investigated
};



struct Swos_Beta_09 {
	int num_players_pos = 56717;				// Number of players in the team
	int team_name_pos = 55429;					// Team Name
	int mgr_forename_pos = 54987;				// , #   - Managers forename
	int mgr_surname_pos = 54996;				// , #   - Managers Surname
	int mgr_fullname_pos = 55460;				// #  - Managers Full Name(Yes it is stored twice!!)
	int plr1_Position_pos = 55500;				// #  - Position of the first player in the file.This is then 38 bytes of data
	int money_pos = 54742;						// Money
	int tact_pos = 88374;						// First Tactic
	int first_team = 90635;						// First team in the file
	int CJOffset = 90614;						// File position of the version number
	std::string CJ = "CJ281112";						// This is the version we are expecting
	std::string version_pos = "Sensible World of Soccer v0.09 - 28/11/1994 11.00am"; //  #   - And this is the version
};


struct Swos_Release_v10 {              // _update= 95/96
	int num_players_pos = 56719;       // - Number of players in the team
	int team_name_pos = 55431;         // - Team Name
	int mgr_forename_pos = 54989;      // - Managers forename
	int mgr_surname_pos = 54998;       // - Managers Surname
	int mgr_fullname_pos = 55462;      // - Managers Full Name (Yes it is stored twice!!)
	int plr1_Posiiton_pos = 55502;     // - Position of the first player in the file. This is then 38 bytes of data
	int money_pos = 54744;             // - Money
	int tact_pos = 88378;              // - First Tactic
	int first_team = 90637;            // - Position of first team within the save file
	int CJOffset = 90618;              // - File position of the version number
	std::string CJ = "CJ031223";               // - This is the version we are expecting
	std::string version_pos = "Sensible World of Soccer v1.0 - 06/12/1994 10.50pm";  // - And this is the version
};

struct Swos_Release_v11 {              // _update= 95/96
	int num_players_pos = 56723;       // - Number of players in the team
	int team_name_pos = 55435;         // - Team Name
	int mgr_forename_pos = 54990;      // - Managers forename
	int mgr_surname_pos = 54999;       // - Managers Surname
	int mgr_fullname_pos = 55462;      // - Managers Full Name (Yes it is stored twice!!)
	int plr1_Posiiton_pos = 55506;     // - Position of the first player in the file. This is then 38 bytes of data
	int money_pos = 54744;             // - Money
	int tact_pos = 88424;              // - First Tactic
	int first_team;
	int CJOffset = 90664;              // - File position of the version number
	std::string CJ = "CJ23051128";             // - This is the version we are expecting
	std::string version_pos = "Sensible World of Soccer v1.1 - 23/05/1995 23.30pm";  // - And this is the version
};

struct Swos_Ger {                        // _update= 95/96
	int num_players_pos = 56719;       // - Number of players in the team
	int team_name_pos = 55431;         // - Team Name
	int mgr_forename_pos = 54990;      // - Managers forename
	int mgr_surname_pos = 54999;       // - Managers Surname
	int mgr_fullname_pos = 55462;      // - Managers Full Name (Yes it is stored twice!!)
	int plr1_Posiiton_pos = 55502;     // - Position of the first player in the file. This is then 38 bytes of data
	int money_pos = 54744;             // - Money
	int tact_pos = 88378;              // - First Tactic
	int CJOffset = 90618;              // - File position of the version number
	std::string CJ = "CJ100117";               // - This is the version we are expecting
	std::string version_pos = "Sensible World of Soccer German Release - 10/01/1995";  // - And this is the version
};

struct Swos_9596 {                       // _update= 95/96
	int num_players_pos = 57485;       // - Number of players in the team
	int team_name_pos = 56197;         // - Team Name
	int mgr_forename_pos = 55134;      // - Managers forename
	int mgr_surname_pos = 55143;       // - Managers Surname
	int mgr_fullname_pos = 56228;      // - Managers Full Name (Yes it is stored twice!!)
	int plr1_Position_pos = 56268;     // - Position of the first player in the file. This is then 38 bytes of data
	int money_pos = 54748;             // - Money
	int tact_pos = 96222;              // - First Tactic
	int first_team = 98485;            // - Position of first team within the save file
	int CJOffset = 98462;              // - File position of the version number
	std::string CJ = "CJ011211";               // - This is the version we are expecting
	std::string version_pos = "Sensible World of Soccer 95/96 - 01/12/1995 11.35";  // - And this is the version
};

struct Swos_9596_Euro {                  // _update= 95/96
	int num_players_pos = 57483;       // - Number of players in the team
	int team_name_pos = 56195;         // - Team Name
	int mgr_forename_pos = 55132;      // - Managers forename
	int mgr_surname_pos = 55141;       // - Managers Surname
	int mgr_fullname_pos = 56226;      // - Managers Full Name (Yes it is stored twice!!)
	int plr1_Posiiton_pos = 56266;     // - Position of the first player in the file. This is then 38 bytes of data
	int money_pos = 54746;             // - Money
	int tact_pos = 96220;              // - First Tactic
	int first_team = 98483;            // - Position of first team within the save file 
	int CJOffset = 98460;              // - File position of the version number
	std::string CJ = "CJ180421";               // - This is the version we are expecting
	std::string version_pos = "Sensible World of Soccer 95/96 Euro Update- 18/04/1996 21.50"; // - And this is the version
};

struct Swos_9697 {                       // _update= 95/96
	int num_players_pos = 57485;       // - Number of players in the team
	int team_name_pos = 56197;         // - Team Name
	int mgr_forename_pos = 55134;      // - Managers forename
	int mgr_surname_pos = 55143;       // - Managers Surname
	int mgr_fullname_pos = 56228;      // - Managers Full Name (Yes it is stored twice!!)
	int plr1_Posiiton_pos = 56268;     // - Position of the first player in the file. This is then 38 bytes of data
	int money_pos = 54748;             // - Money
	int tact_pos = 96222;              // - First Tactic
	int first_team = 98485;            // - Position of first team within the save file 
	int CJOffset = 98462;              // - File position of the version number
	std::string CJ = "CJ061114";               // - This is the version we are expecting
	std::string version_pos = "Sensible World of Soccer 96/97 - 06/11/1996 13.55";  // - And this is the version
};

struct Swos_PC {                         // _update= 95/96
	int num_players_pos = 57485;       // - Number of players in the team
	int team_name_pos = 56197;         // - Team Name
	int mgr_forename_pos = 55134;      // - Managers forename
	int mgr_surname_pos = 55143;       // - Managers Surname
	int mgr_fullname_pos = 56228;      // - Managers Full Name (Yes it is stored twice!!)
	int plr1_Posiiton_pos = 56268;     // - Position of the first player in the file. This is then 38 bytes of data
	int money_pos = 54748;             // - Money
	int tact_pos = 96222;              // - First Tactic
	int first_team = 96222;            // - Position of first team within the save file 
	int CJOffset = 98462;              // - File position of the version number
	std::string CJ = "CJ061114";               // - This is the version we are expecting
	std::string version_pos = "Sensible World of Soccer 96/97 - 06/11/1996 13.55";  // - And this is the version
};
*/