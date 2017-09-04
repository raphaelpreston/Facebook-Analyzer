#include <stdio.h>
#include "XMLTranslation.h"
#include "hashset.h"
#include <stdbool.h>
#include <string.h>
#include "utlist.h"

#define MAXSTATES 5
#define ENUM "C:/Users/IAMFRANK/Documents/FB Testing/ENUMERATE"
#define DSTRING_LENGTH 5
int fileToXML(char * input, char * output)
{
	/* open necessary files */
	FILE * inputP;
	FILE * outputP;
	inputP = fopen(input, "r");
	outputP = fopen(output, "w");

	/* error checking */
	if (inputP == NULL) {
		perror("Error opening input file!!");
		return 1;
	}
	if (outputP == NULL) {
		perror("Error opening output file!!");
		return 1;
	}

	/* start translation from stupid facebook HTML to XML*/

	const char h1[3] = "h1";	//state 0
	const char divClassThread[19] = "div class=\"thread\"";	//state 1
	const char spanClassUser[18] = "span class=\"user\"";	//state 2
	const char spanClassMeta[18] = "span class=\"meta\"";	//state 3
	const char p[2] = "p";		//state 4

	char * statesText[] = { h1, divClassThread, spanClassUser, spanClassMeta, p };
	int stateLengths[] = { 3, 19, 18, 18, 2 };

	int states[MAXSTATES] = { 0 };	// [0,1,1,1,0] binary map of possible states
	bool scanning = false;	//determine if we are in the process of matching or recording content
	char c;
	int idx = 0;
	int matched = -1;
	bool end;
	bool printedTag = false;
	bool foundTwo;
	int o;
	int l;
	bool foundOne;
	bool skip = false;
	bool goodSkip = false;
	bool recorded = false;

	//double charNum = 0;
	//double totalChars = 83327132;


	while ((c = fgetc(inputP)) != EOF) {
		//charNum++;
		//printf("\rProgress: %%%.4f", (charNum/totalChars)*100);
		if (c == '<' || c == '>') {	//start or finish scanning
			if (c == '>') {	//hit end of tag
				/* check to see if theres only one possibility yet*/
				if( !(skip || matched == -1) ){	//theres a match
					if (stateLengths[matched] - 1 != idx) {	//lengths aren't correct
					}
					else {	//theres only a single match
						scanning = false;	//toggle scanning - time to record
						skip = false;
						goodSkip = false;
						foundTwo = false;
					}
				}
			}
			else if (c == '<') {	//hit beginning of tag
				if (!scanning && !recorded && matched != -1) {	//supposed to be recording, means its a meaningful tag with empty content
					fprintf(outputP, matched == 0 ? "[[\n" : matched == 1 ? "]]\n" : matched == 2 ? "{{\n" : matched == 3 ? "}}\n" : matched == 4 ? "<<\n" : "<ERROR></ERROR>\n");
				}
				recorded = false;
				//printf("found <, scanning is");
				for (int i = 0; i < MAXSTATES; i++) { states[i] = 1; }	//all states are a possibility
				scanning = true;
				foundTwo = false;
				idx = 0;
				skip = false;
				goodSkip = false;
				if (printedTag) {
					fprintf(outputP, matched == 0 ? "[\n" : matched == 1 ? "]\n" : matched == 2 ? "{\n" : matched == 3 ? "}\n" : matched == 4 ? "<\n" : "</ERROR>\n");
					printedTag = false;
				}
				matched = -1;
			}
		}
		else {	//not a < or >
			if(!skip && !goodSkip) {	//not skipping
				if (scanning) {	//if scanning
					foundOne = false;	//use to determine if there is at least one 1 in the binary tree, so we know to continue scanning or just start skipping
					if (c == '/') {
						skip = true;
					}
					else {
						if (foundTwo == true) {	//either state 2 or state 3
							for (int k = 2; k < 4; k++) {
								if (statesText[k][idx] != c) {	//if the character being examined is the same as the character at index "idx" in the text pattern to match it to, set that state to one to continue searching, or 0 to disable searching for that specific text
									states[k] = 0;
								}
								else {
									if (!foundOne) {
										matched = k;
										foundOne = true;
									}
									else {
										//foundTwo is already true
									}
								}
							}
						}
						else if (matched != -1) {	//only compare the already matched value
							if (statesText[matched][idx] != c) {	//if the character being examined is the same as the character at index "idx" in the text pattern to match it to, set that state to one to continue searching, or 0 to disable searching for that specific text
								states[matched] = 0;
							}
							else {
								//matched is already set
								foundOne = true;
							}
						}
						else {
							for (int k = 0; k < MAXSTATES; k++) {	//for each possible state, compare the characters at the correct index ex. 'i' ?= "s(p)an class..." -> for each text comparison
								if (states[k] == 1) {	//if text string still matches and it's not overflowing, check this character
									if (statesText[k][idx] != c) {	//if the character being examined is the same as the character at index "idx" in the text pattern to match it to, set that state to one to continue searching, or 0 to disable searching for that specific text
										states[k] = 0;
									}
									else {
										if (!foundOne) {
											matched = k;
											foundOne = true;
										}
										else {
											foundTwo = true;
										}
									}
								}
								else {
								}
							}
						}

						/* determine if there are any 1s in binary map */
						if (!foundOne) {	//all zeroes
							skip = true;
						}

						/* determine if there is only one actual possibility left (no point in checking state 0 or 4)*/
						if (matched == 1 && c == 't' && idx == 11) {	//<div class="t
							goodSkip = true;
						}
						else if (matched == 2 && c == 'u' && idx == 12) {	//<span class="u
							goodSkip = true;
						}
						else if (matched == 3 && c == 'm' && idx == 12) {	//<span class="m
							goodSkip = true;
						}
					}
					idx++;
				}
				else {	//not scanning - match has been made
					if (!printedTag) {
						fprintf(outputP, matched == 0 ? "[" : matched == 1 ? "]" : matched == 2 ? "{" : matched == 3 ? "}" : matched == 4 ? "<" : "</ERROR>\n");
						printedTag = true;
					}
					fprintf(outputP, "%c", c);
					recorded = true;
				}
			}
			else if (goodSkip) {
				idx++;
			}
			//else do nothing
		}
	}

	/* close all files */
	fclose(inputP);
	fclose(outputP);
}

int loadXML(char * fileName) {
	//error checking for input file
	FILE * input;
	input = fopen(fileName, "r");
	if (input == NULL) {
		perror("Error opening input file!!");
		return 1;
	}

	/* create hashtables to keep track of words and speakers. */
	word_hash * w_hash = word_hash_init();

	/* create b+ tree for timestamps */

	
	
	 
	/* variables for keeping track of data */
	message * message;
	word_list * w_list;
	
	dString * name = dString_new(DSTRING_LENGTH);
	dString * thread = dString_new(DSTRING_LENGTH);
	dString * speaker = dString_new(DSTRING_LENGTH);
	dString * tstamp = dString_new(DSTRING_LENGTH);
	dString * content = dString_new(DSTRING_LENGTH);


	dString * word = dString_new(DSTRING_LENGTH);

	char reading = 'x';	//x for nothing, [ for name, ] for thread data, { for speaker, } for time stamp, < for content
	
	/* load in data */
	char c;
	while ((c = fgetc(input)) != EOF) {
		/* if reading */
		if (reading == '[' || reading == ']' || reading == '{' || reading == '}' || reading == '<') {
			/* stop reading if it's any of these and save to memory what it read */
			if (c == '[') {
				printf("Full name is %s...\n", name->buffer);
				dString_clear(name);
				reading = 'x';
			}
			else if (c == ']') {
				printf("Thread is %s...\n", thread->buffer);
				dString_clear(thread);
				reading = 'x';
			} 
			else if (c == '{') {
				printf("Speaker is %s...\n", speaker->buffer);
				dString_clear(speaker);
				reading = 'x';
			}
			else if (c == '}') {
				printf("Tstamp is %s...\n", tstamp->buffer);
				dString_clear(tstamp);
				reading = 'x';
			}
			else if (c == '<') {
				// printf("Content is %s...\n", content->buffer);
				//dString_clear(content);
				printf("Read in word: \"%s\"\n", word->buffer);
				// dString_clear(word);
				// free(word);
				word = dString_new(DSTRING_LENGTH);
				reading = 'x';
			}
			/* read the character in appropriately to the message buffer */
			else {
				if (reading == '[') {
					dString_append(name, c);
				}
				else if (reading == ']') {
					dString_append(thread, c);
				}
				else if (reading == '{') {
					dString_append(speaker, c);
				}
				else if (reading == '}') {
					dString_append(tstamp, c);
				}
				else if (reading == '<') {	//content
					if (c == ' ') {	//end of a word
						printf("Read in word: \"%s\"\n", word->buffer);
						// dString_clear(word);
						// free(word);
						word = dString_new(DSTRING_LENGTH);
					}
					else {	//reading in a word
						dString_append(word, c);
					}
					// dString_append(content, c);
				}
			}

		}

		/* not reading, but we hit an indicator */
		else if (c == '[' || c == ']' || c == '{' || c == '}' || c == '<') {
			reading = c;

		}
		else {
			//printf("Something else...\n");
		}


	}
	fclose(input);
}

void print_time(tstamp_t * tstmp) {
	printf("%s, %s %i, %i at %i:%s%i%s", tstmp->wday == 0 ? "Sunday" : tstmp->wday == 1 ? "Monday" : tstmp->wday == 2 ? "Tuesday" : tstmp->wday == 3 ? "Wednesday" : tstmp->wday == 4 ? "Thursday" : tstmp->wday == 5 ? "Friday" : tstmp->wday == 6 ? "Saturday" : "Unknown", tstmp->month == 0 ? "January" : tstmp->month == 1 ? "February" : tstmp->month == 2 ? "March" : tstmp->month == 3 ? "April" : tstmp->month == 4 ? "May" : tstmp->month == 5 ? "June" : tstmp->month == 6 ? "July" : tstmp->month == 7 ? "August" : tstmp->month == 8 ? "September" : tstmp->month == 9 ? "October" : tstmp->month == 10 ? "November" : tstmp->month == 11 ? "December" : "Unknown", tstmp->mday, tstmp->year, tstmp->hour, tstmp->min < 10 ? "0" : "", tstmp->min, tstmp->ampm == 0 ? "am" : tstmp->ampm == 1 ? "pm" : "unknown");
}

int tstamp_comp_asc(tstamp_t * ts1, tstamp_t * ts2) {
	//-1: ts1 < ts2
	//0: ts1 == ts2
	//1: ts1 > ts2
	//not dealing with time zones cus facebook only supplies with "MST" and whatnot and there are too many duplicates of the abbreviations to be viable

	/* get 24 hour time */
	int ts1_hour24 = ts1->hour == 12 ? ts1->ampm == 0 ? 0 : 12 : ts1->ampm == 0 ? ts1->hour : ts1->hour + 12;
	int ts2_hour24 = ts2->hour == 12 ? ts2->ampm == 0 ? 0 : 12 : ts2->ampm == 0 ? ts2->hour : ts2->hour + 12;

	/* compare year */
	if (ts1->year != ts2->year) return ts1->year > ts2->year ? -1 : 1;

	/* compare month */
	if (ts1->month != ts2->month) return ts1->month > ts2->month ? -1 : 1;

	/* compare day of the month */
	if (ts1->mday != ts2->mday) return ts1->mday > ts2->mday ? -1 : 1;

	/* compare hour */
	if (ts1_hour24 != ts2_hour24) return ts1_hour24 > ts2_hour24 ? -1 : 1;

	/* compare minutes */
	if (ts1->min != ts2->min) return ts1->min > ts2->min ? -1 : 1;
	else return 0;

	perror("Error comparing times.");
}

int tstamp_comp_desc(tstamp_t * ts1, tstamp_t * ts2) {
	//-1: ts1 < ts2
	//0: ts1 == ts2
	//1: ts1 > ts2
	//not dealing with time zones cus facebook only supplies with "MST" and whatnot and there are too many duplicates of the abbreviations to be viable

	/* get 24 hour time */
	int ts1_hour24 = ts1->hour == 12 ? ts1->ampm == 0 ? 0 : 12 : ts1->ampm == 0 ? ts1->hour : ts1->hour + 12;
	int ts2_hour24 = ts2->hour == 12 ? ts2->ampm == 0 ? 0 : 12 : ts2->ampm == 0 ? ts2->hour : ts2->hour + 12;

	/* compare year */
	if (ts1->year != ts2->year) return ts1->year > ts2->year ? 1 : -1;

	/* compare month */
	if (ts1->month != ts2->month) return ts1->month > ts2->month ? 1 : -1;

	/* compare day of the month */
	if (ts1->mday != ts2->mday) return ts1->mday > ts2->mday ? 1 : -1;

	/* compare hour */
	if (ts1_hour24 != ts2_hour24) return ts1_hour24 > ts2_hour24 ? 1 : -1;

	/* compare minutes */
	if (ts1->min != ts2->min) return ts1->min > ts2->min ? 1 : -1;
	else return 0;

	perror("Error comparing times.");
}

word_list * word_list_new(char * word) {
	/* declare struct */
	word_list * list;
	list = (word_list *)malloc(sizeof(word_list));
	if (list == NULL) {
		perror("Error mallocing word_list.");
		return;
	}

	/* zero struct (required for hashing) */
	memset(list, 0, sizeof(word_list));

	/* set struct attributes */
	list->head = NULL;
	list->word = (char *)malloc(sizeof(char) * strlen(word) + 1);
	strcpy(list->word, word);


	return list;
}

message * message_new() {
	/* allocate memory */
	message * m;
	m = (message *)malloc(sizeof(message));
	if (m == NULL)perror("Error mallocing message");

	/* set attributes */
	m->tstamp = (tstamp_t *)malloc(sizeof(tstamp_t));
	m->content = dString_new(10);


	return m;
}

void message_set_tstamp(message * m, int hour, int min, int ampm, int wday, int month, int mday, int year) {
	if (hour < 1 || hour > 12 || min < 1 || min > 60 || ampm < 0 || ampm > 1 || wday < 0 || wday > 6 || mday < 0 || mday > 31 || year < 0) perror("Unable to set timestamp.");
	m->tstamp->hour = hour;
	m->tstamp->min = min;
	m->tstamp->ampm = ampm;
	m->tstamp->wday = wday;
	m->tstamp->month = month;
	m->tstamp->mday = mday;
	m->tstamp->year = year;

	return;
}

void word_list_add_node(word_list * list, message * m) {
	LL_PREPEND(list->head, m);
	return;
}

word_hash * word_hash_init(){

	/* declare the struct */
	word_hash * hash;
	hash = (word_hash *)malloc(sizeof(word_hash));

	/* Set the head to NULL */
	hash->head = NULL;

	return hash;

}

void word_hash_add_list(word_hash * hash, word_list * list) {
	if (list->word == NULL) { perror("Cant add a list with a null word."); return; }
	HASH_ADD_KEYPTR(hh, hash->head, list->word, strlen(list->word), list);
	return;
}

word_list * word_hash_find_list(word_hash * hash, char * word, word_list * search) {
	HASH_FIND_STR(hash->head, word, search);
	return search;
}

void message_delete(message * m) {
	free(m->tstamp);
	free(m->content);
}

void word_list_delete(word_list * list) {
	message *current, *tmp;
	LL_FOREACH_SAFE(list->head, current, tmp) {
		message_delete(current);
		LL_DELETE(list->head, current);
		free(current);
	}
	free(list->word);
	free(list->head);
	return;
}

void word_hash_delete(word_hash * hash) {
	word_list *current, *tmp;
	HASH_ITER(hh, hash->head, current, tmp) {
		word_list_delete(current);
		HASH_DEL(hash->head, current);
		free(current);
	}
	free(tmp);
}