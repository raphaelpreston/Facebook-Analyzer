#pragma once
#include <time.h>

//structs

/* structs and functions for utlist.h - needed for speakers and words */

typedef struct tstamp {
	int min;	// seconds
	int hour;	// hour
	int ampm;	// am or pm (0 for am, 1 for pm)
	int mday;	// day of month
	int month;	// month (0 is January)
	int year;	// year
	int wday;	// day of week (0 is sunday)
}tstamp_t;

typedef struct word_node {
	char * word;
	tstamp_t tstamp;
	struct word_node * next;
}word_node;








//struct Thread {
//	UT_array * speakers;		//array of pointers to participants in conversation
//	UT_array * messages;		//array of pointers to each message
//	UT_hash_handle hh;
//} thread_t;
//									//WORKING HERE, HAVE TO MAKE ALL OF THESE UNIQUE
//struct Speaker {
//	int id;					//facebook id of user
//	UT_string * name;		//name of user
//	UT_hash_handle hh;
//} speaker_t;
//
//struct Message {
//	UT_string * speaker;	//who sent the message
//	struct tm * meta;		//pointer to tm struct translated from 'Sunday, January 5, 2014 at 12:17am MST' using strptime
//	UT_string * content;	//content of the message
//	UT_hash_handle hh;		
//} message_t;

/* Function to translate the facebook HTML file into a pretty XML file.  Assumes there are no < or > characters in file, which is fine because Facebook converts them to &lt and &gt.
@param1: Path to input file
@param2: Path to output file
Return Codes:
0: Success
1: Failure with opening files
2: Binary map error
*/
int fileToXML(char * input, char * output);

/* Function that loads all of the message data into memory into a giant chain of hashmaps.  
@param1: Key to index Thread structs by
@param2: Key to index Speaker structs by
@param3: Key to index Message structs by
*/
// int loadAllMessages(char * threadKey, char * speakerKey, char * messageKey);

/* Helper function that returns a new string with the character appended to it.  The string "str" is freed, thus must have been malloced before.
@param1: String to append char onto
@param2: Character to append to string
*/
char * cAppend(char * str, char c);