#pragma once
#include <time.h>
#include "uthash.h"
#include "dString.h"

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

typedef struct message {
	tstamp_t * tstamp;
	dString * content;
	struct message * next;
}message;

typedef struct word_list {
	char * word;	// acts as the key
	message * head;	//points to head of linked list of messages
	UT_hash_handle hh;
}word_list;

/* Function to initialize a message node. 
*/
message * message_new();

/* Function to set the timestamp for a message. 
*/
void message_set_tstamp(message *,int hour, int min, int ampm, int wday, int month, int mday, int year);

/* Function to add a node (message) to the linked list for a specific word.
@param1: List to add the message to
@param2: Message node to add*/
void word_list_add_node(word_list *, message *);

/* Function that initializes a new word list based on a given word.
@param1: The word to initialize the list
*/
word_list * word_list_new(char *);

/* Function that prints time in the facebook timestamp format: (Sunday, April 7, 1999 at 1:27pm) 
@param1: Timestamp to print */
void print_time(tstamp_t *);

/* Function that compares two timestamps and returns -1 or 1 to order in the earliest to latest time stamp. 
@param1: First timestamp
@param2: Second timestamp
*/
int tstamp_comp_asc(tstamp_t *, tstamp_t *);

/* Function that compares two timestamps and returns -1 or 1 to order in the latest to earliest time stamp.
@param1: First timestamp
@param2: Second timestamp
*/
int tstamp_comp_desc(tstamp_t *, tstamp_t *);

/* Function to translate the facebook HTML file into a pretty XML file.  Assumes there are no < or > characters in file, which is fine because Facebook converts them to &lt and &gt.
@param1: Path to input file
@param2: Path to output file
Return Codes:
0: Success
1: Failure with opening files
2: Binary map error
*/
int fileToXML(char * input, char * output);