#include <stdio.h>
#include <stdlib.h>
#include "XMLTranslation.h"
#include "hashset.h"
#include <stdbool.h>
#include <string.h>
#include "utlist.h"
#include <ctype.h>

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
				if (!(skip || matched == -1)) {	//theres a match
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
			if (!skip && !goodSkip) {	//not skipping
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
					/* print the character to the file unless it's one that needs to be changed */
					if (c == '[') fprintf(outputP, "%s", "&#91;");
					else if (c == ']') fprintf(outputP, "%s", "&#93;");
					else if (c == '{') fprintf(outputP, "%s", "&#123;");
					else if (c == '}') fprintf(outputP, "%s", "&#125;");
					else if (c == '<') fprintf(outputP, "%s", "&lt;");
					else if (c == '>') fprintf(outputP, "%s", "&gt;");

					else fprintf(outputP, "%c", c);
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

int loadXML(char * fileName, word_hash * w_hash) {
	//error checking for input file
	FILE * input;
	input = fopen(fileName, "r");
	if (input == NULL) {
		perror("Error opening input file!!");
		return 1;
	}

	/* create b+ tree for timestamps */


	/* variables for keeping track of data */
	message * message = message_new();

	dString * name = dString_new(DSTRING_LENGTH);
	dString * thread = dString_new(DSTRING_LENGTH);
	dString * speaker = dString_new(DSTRING_LENGTH);
	dString * tstamp = dString_new(DSTRING_LENGTH);
	dString * content = dString_new(DSTRING_LENGTH);

	dString * wday = dString_new(10);	//0
	dString * month = dString_new(10);//1
	dString * mday = dString_new(3); //2
	dString * year = dString_new(5); //3
	dString * hour = dString_new(3); //4
	dString * min = dString_new(3);	//5
	dString * ampm = dString_new(3); //6

	int month_s_count = 0;	//count the spaces to know when to advance when reading month
	int mday_s_count = 0;	//same but for the mday
	int hour_s_count = 0;
	int min_c_count = 0;

	int t_reading = 0;		//keep track of what time object is being read in


	dString * word = dString_new(DSTRING_LENGTH);

	char reading = 'x';	//x for nothing, [ for name, ] for thread data, { for speaker, } for time stamp, < for content

	/* load in data */
	char c;
	while ((c = fgetc(input)) != EOF) {
		/* if reading */
		if (reading == '[' || reading == ']' || reading == '{' || reading == '}' || reading == '<') {
			/* stop reading if it's any of these and save to memory what it read */
			if (c == '[') {
				dString_clear(name);
				reading = 'x';
			}
			else if (c == ']') {
				//don't clear thread here
				reading = 'x';
			}
			else if (c == '{') {
				/* assign speaker to the message then clear it */
				dString * tmp = dString_new(strlen(speaker->buffer) + 1);
				dString_fill(tmp, speaker->buffer);
				message->speaker = tmp;

				dString_clear(speaker);
				reading = 'x';
			}
			else if (c == '}') {
				// printf("Attempting to set: \nHour: %s\nMin: %s\nAmpm: %s\nWday: %s\nMonth: %s\nMday: %s\nYear: %s\n\n", hour->buffer, min->buffer, ampm->buffer, wday->buffer, month->buffer, mday->buffer, year->buffer);
				message_set_tstamp(message, atoi(hour->buffer), atoi(min->buffer), atoi(ampm->buffer), strcmp(wday->buffer, "Sunday") == 0 ? 0 : strcmp(wday->buffer, "Monday") == 0 ? 1 : strcmp(wday->buffer, "Tuesday") == 0 ? 2 : strcmp(wday->buffer, "Wednesday") == 0 ? 3 : strcmp(wday->buffer, "Thursday") == 0 ? 4 : strcmp(wday->buffer, "Friday") == 0 ? 5 : strcmp(wday->buffer, "Saturday") == 0 ? 6 : -1, strcmp(month->buffer, "January") == 0 ? 0 : strcmp(month->buffer, "February") == 0 ? 1 : strcmp(month->buffer, "March") == 0 ? 2 : strcmp(month->buffer, "April") == 0 ? 3 : strcmp(month->buffer, "May") == 0 ? 4 : strcmp(month->buffer, "June") == 0 ? 5 : strcmp(month->buffer, "July") == 0 ? 6 : strcmp(month->buffer, "August") == 0 ? 7 : strcmp(month->buffer, "September") == 0 ? 8 : strcmp(month->buffer, "October") == 0 ? 9 : strcmp(month->buffer, "November") == 0 ? 10 : strcmp(month->buffer, "December") == 0 ? 11 : -1, atoi(mday->buffer), atoi(year->buffer));
				dString_clear(wday);
				dString_clear(month);
				dString_clear(mday);
				dString_clear(year);
				dString_clear(hour);
				dString_clear(min);
				dString_clear(ampm);
				t_reading = 0;	//reset timestamp scanner and other counters
				month_s_count = 0;
				mday_s_count = 0;
				hour_s_count = 0;
				min_c_count = 0;

				reading = 'x';
			}
			else if (c == '<') {
				/* add the last word to the hash */
				word_hash_add_word(w_hash, word->buffer, message);
				dString_clear(word);

				/* read in the content */
				dString_fill(message->content, content->buffer);
				dString_clear(content);

				/* assign the thread to the message */
				dString * tmp = dString_new(strlen(thread->buffer) + 1);
				dString_fill(tmp, thread->buffer);
				message->thread = tmp;

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
					/* determine what part of the timestamp we're reading */
					// caution: "If it's stupid and works, it's not stupid."
					if (t_reading == 0) {
						if (c == ',') t_reading++;
						else dString_append(wday, c);
					}
					else if (t_reading == 1) {
						if (c == ' ') {
							if (month_s_count == 1) {	//advance to next element
								t_reading++;
								month_s_count = 0;	//reset the count
							}
							else month_s_count++;
						}
						else dString_append(month, c);
					}
					else if (t_reading == 2) {
						if (c == ',') t_reading++;
						else dString_append(mday, c);
					}
					else if (t_reading == 3) {
						if (c == ' ') {
							if (month_s_count == 1) {	//advance to next element
								t_reading++;
							}
							else month_s_count++;
						}
						else dString_append(year, c);
					}
					else if (t_reading == 4) {
						if (hour_s_count != 1) {	//passing through " at "
							if (c == ' ') {
								hour_s_count++;
							}
						}
						else {
							if (c == ':') t_reading++;
							else dString_append(hour, c);
						}
					}
					else if (t_reading == 5) {
						if (min_c_count <= 1) {	//only count two characters
							dString_append(min, c);
							if (min_c_count == 1) t_reading++;
							min_c_count++;
						}
					}
					else if (t_reading == 6) {
						if (c == ' ') t_reading++;
						else dString_append(ampm, c);
						//else scanning through timezone, which isn't scanned in
					}
					// else we are scanning through timezone
				}
				else if (reading == '<') {	//content
					if (c == ' ') {	//end of a word
						word_hash_add_word(w_hash, word->buffer, message);
						dString_clear(word);
					}
					else {	//reading in a word
						dString_append(word, c);
					}
					dString_append(content, c);
				}
			}

		}

		/* not reading, but we hit an indicator */
		else if (c == '[' || c == ']' || c == '{' || c == '}' || c == '<') {
			if (c == '{') {	//indicator of new message
				message = message_new();
			}
			else if (c == ']') {
				dString_clear(thread);
			}

			/* begin reading */
			reading = c;

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
	if (list->word == NULL) perror("Unable to malloc for creation of word list -> word");
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
	if (m->tstamp == NULL) perror("Unable to malloc for creation of message -> tstamp");
	m->content = dString_new(10);


	return m;
}

void message_set_tstamp(message * m, int hour, int min, int ampm, int wday, int month, int mday, int year) {
	if (hour < 0 || hour > 12 || min < 0 || min > 60 || ampm < 0 || ampm > 1 || wday < 0 || wday > 6 || mday < 0 || mday > 31 || year < 0) {
		perror("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n!!!!!!!!Unable to set timestamp: \n");
		printf(":::\nHour: %i\nMin: %i\nAmpm: %i\nWday: %i\nMonth: %i\nMday: %i\nYear: %i\n\n", hour, min, ampm, wday, month, mday, year);
	}
	m->tstamp->hour = hour;
	m->tstamp->min = min;
	m->tstamp->ampm = ampm;
	m->tstamp->wday = wday;
	m->tstamp->month = month;
	m->tstamp->mday = mday;
	m->tstamp->year = year;

	return;
}

m_node * m_node_new(message * m) {
	if (m == NULL)perror("The message was null.");

	/* declare the struct */
	m_node * mn;
	mn = (m_node *)malloc(sizeof(m_node));
	if (mn == NULL) perror("Failed to allocate for message node.");

	/* set attributes */
	mn->message = m;

	return;
}

void word_list_add_node(word_list * list, message * m) {
	m_node * mn = m_node_new(m);
	LL_PREPEND(list->head, mn);
	return;
}

word_hash * word_hash_init() {

	/* declare the struct */
	word_hash * hash;
	hash = (word_hash *)malloc(sizeof(word_hash));
	if (hash == NULL) perror("Unable to malloc for creation of word hash");

	/* Set the head to NULL */
	hash->head = NULL;

	/* create the pointer hash to keep track of deletions */
	ptr_hash * phash;
	phash = ptr_hash_new();
	hash->ptrhash = phash;

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

bool message_mark_deletion(word_hash * hash, message * m) {
	if (!ptr_hash_exists_ptr(hash->ptrhash, m)) {	//not been added to deletion array yet
		//* free all the memory */
		dString_delete(m->content);
		dString_delete(m->speaker);
		dString_delete(m->thread);
		free(m->tstamp);


		/* add the pointer to the list of pointers to be deleted */
		ptr_hash_add_ptr(hash->ptrhash, m);

		return false;
	}
	return true;	//a true return signals that it had already been deleted
}

void word_list_delete(word_hash * hash, word_list * list) {
	bool b;
	m_node *current, *tmp;

	LL_FOREACH_SAFE(list->head, current, tmp) {
		message_mark_deletion(hash, current->message);	//only adds to deletion array
		LL_DELETE(list->head, current);
		free(current);	//free the node, i think...
	}
	free(list->word);
	free(list->head);
	return;
}

void word_hash_delete(word_hash * hash) {
	word_list *current, *tmp;
	HASH_ITER(hh, hash->head, current, tmp) {
		// printf("Attempting to delete word_list with word \"%s\"\n", current->word);
		word_list_delete(hash, current);
		HASH_DEL(hash->head, current);
		free(current);
	}
	free(tmp);
	ptr_hash_delete(hash->ptrhash);		//actually delete all the messages
}

bool ispunc(char c) {
	return (c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' || c == '*' || c == '(' || c == ')' || c == '_' || c == '-' || c == '=' || c == '+' || c == '[' || c == '{' || c == ']' || c == '}' || c == '\\' || c == '|' || c == ';' || c == ':' || c == '\'' || c == '\"' || c == ',' || c == '<' || c == '.' || c == '>' || c == '/' || c == '?' || c == '`' || c == '~');
}

int word_hash_add_word(word_hash * hash, char * word, message * message) {
	/* convert the string to lowercase and remove all punctuation */
	dString * lower = dString_new(strlen(word) + 1);
	for (int i = 0; i < strlen(word); i++) {
		if (!ispunc(word[i])) {	//valid character
			dString_append(lower, tolower(word[i]));
		}
	}
	//losing memory by not freeing word?
	word = lower->buffer;

	/* check if the word is already in the hash */
	word_list * search;
	search = NULL;
	word_list * found = word_hash_find_list(hash, word, search);
	free(search);
	if (found) {
		/* add the message to the word_list */
		word_list_add_node(found, message);
		// printf("\"%s\" was already in the hash. Added the message to the list.\n", word);
		return 1;
	}

	else {
		/* make a new wordlist */
		word_list * new_list = word_list_new(word);

		/* add the message to it */
		word_list_add_node(new_list, message);

		/* add the list to the hash */
		word_hash_add_list(hash, new_list);
		// printf("Had to make a new wordlist for \"%s\".\n", word);

		return 0;
	}
	return -1;
}

ptr_hash * ptr_hash_new() {
	/* declare the struct */
	ptr_hash * hash;
	hash = (ptr_hash *)malloc(sizeof(ptr_hash));
	if (hash == NULL) perror("Unable to malloc for creation of ptr hash");

	/* Set the head to NULL */
	hash->head = NULL;

	return hash;
}

void ptr_hash_add_ptr(ptr_hash * ptrhash, void * ptr) {
	/* add a node to hold the pointer */
	phash_t * e = (phash_t *)malloc(sizeof(phash_t));
	if (!e) perror("Couldn't malloc for pointer.");
	e->key = ptr;

	/* add the node to the hash */
	HASH_ADD_PTR(ptrhash->head, key, e);
}

bool ptr_hash_exists_ptr(ptr_hash * ptrhash, void * ptr) {
	phash_t * d;
	HASH_FIND_PTR(ptrhash->head, &ptr, d);
	if (d) {
		return true;
		//can't free d here for some reason 
	}
	else {
		//can't free d here for some reason 
		return false;
	}
}

void ptr_hash_delete(ptr_hash * ptrhash) {
	phash_t *current, *tmp;
	HASH_ITER(hh, ptrhash->head, current, tmp) {
		HASH_DEL(ptrhash->head, current);
		free(current);
	}
	free(tmp);
	return;
}


//printing functions

void word_hash_print(word_hash * hash) {
	word_list * list;
	word_list * tmp;

	printf("Hash:\n");
	HASH_ITER(hh, hash->head, list, tmp) {
		printf(" \"%s\" ->\n", list->word);
		m_node * mn;
		LL_FOREACH(list->head, mn) {
			printf("   (\n    \"%s\"\n    \"%s\"\n    \"%s\"\n    ", mn->message->thread->buffer, mn->message->content->buffer, mn->message->speaker->buffer);
			print_time(mn->message->tstamp);
			printf("\n    )%s", mn->next != NULL ? ",\n" : "");
		}
		printf("\n");
	}

}
