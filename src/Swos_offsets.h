#ifndef Swos_offsets_H
#define Swos_offsets_H

#include <iostream>
#include <string>
#include <fstream>


// Data structures

struct Swos_Main_Offsets {
	int num_players_pos;				// Number of players in the team
	int team_name_pos;					// Team Name
	int mgr_forename_pos;				// , #   - Managers forename
	int mgr_surname_pos;				// , #   - Managers Surname
	int mgr_fullname_pos;				// #  - Managers Full Name(Yes it is stored twice!!)
	int plr1_Position_pos;				// #  - Position of the first player in the file.This is then 38 bytes of data
	int money_pos;						// Money
	int tact_pos;						// First Tactic
	int first_team;						// First team in the file
	int CJOffset;						// File position of the version number
	std::string CJ;						// This is the version we are expecting
	std::string version_pos;			//  #   - And this is the version
};

extern Swos_Main_Offsets swosoffsets;

// Function prototypes

int getFileSize(const std::string &fileName);
void *loadfile(std::string file, char *buf, int *fileleng);
int buffToInteger(char * buffer, int startbuf, int buflen);
bool getversion(std::string vers, std::string cjvers[][2], int length, int i);
std::string charToBuffer(char * inbuffer, int startbuf, int buflen, char termchar);


#endif