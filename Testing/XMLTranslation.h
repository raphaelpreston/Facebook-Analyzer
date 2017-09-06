#pragma once
#include <time.h>
#include "uthash.h"
#include "dString.h"
#include <stdbool.h>

//structs

/* structs and functions for utlist.h - needed for speakers and words */

typedef struct {
	void * key;
	UT_hash_handle hh;
}phash_t;

typedef struct {
	phash_t * head;
}ptr_hash;

/* hashtable that will keep track of which pointers have already been freed */


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

/* Struct that keeps track of word -> message */
typedef struct word_hash {
	word_list * head;
	ptr_hash * ptrhash;	//keeps track of whats been deleted so it doesn't try to free the same message again (if the same message is referenced twice in the hash this will happen)
}word_hash;

word_hash * word_hash_init();

void word_hash_add_list(word_hash * hash, word_list * list);

/* Find a list for a word given a word.  Must set search to NULL.
*/
word_list * word_hash_find_list(word_hash * hash, char * word, word_list * search);

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

/* Function that initializes a new word list based on a given word.  Zeroes struct before filling with attributes for hashing.
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

/*Deletes a message node.  Note: Doesn't actually free the message node.  Must do that after.  A true return signals that the message had already been deleted.
*/
bool message_mark_deletion(word_hash * hash, message * m);

/*Deletes a word list.  Note: Doesn't actually free the list.  Must do that after.
*/
void word_list_delete(word_hash * hash, word_list *);

/*Deletes a word hash.  Note: Doesn't actually free the hash.  Must do that after. 
*/
void word_hash_delete(word_hash *);

/* Adds a word to a word_hash, given a message.  Returns 1 if the word_list existed and was modified, 0 if it was a new word, and -1 if it failed.  
*/
int word_hash_add_word(word_hash * hash, char * word, message * message);

ptr_hash * ptr_hash_new();

void ptr_hash_add_ptr(ptr_hash * ptrhash, void * ptr);

bool ptr_hash_exists_ptr(ptr_hash * ptrhash, void * ptr);

void ptr_hash_delete(ptr_hash * ptrhash);

void word_hash_print(word_hash *);

/* Determine if a character is punctuation or not.*/
bool ispunc(char c);