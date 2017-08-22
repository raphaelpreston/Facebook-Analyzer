#include <stdio.h>
#include "XMLTranslation.h"
#include "hashset.h"
#include <stdbool.h>
#include <string.h>

#define MAXSTATES 5
#define ENUM "C:/Users/IAMFRANK/Documents/FB Testing/ENUMERATE"

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
	int numSpeakers;
	int numWords;

	//check to see if ENUMERATE file is able to be read, if so, get the data from it
	FILE * enumFile;
	enumFile = fopen(ENUM, "r");
	if (enumFile != NULL) {
		numSpeakers = getFromEnum("numSpeakers");
		numWords = getFromEnum("numWords");
		//successfully loaded data from enums file, can now make hashmaps
	}
	fclose(enumFile);
	//reopen for writing data into it


	FILE * input;
	input = fopen(fileName, "r");
	if (input == NULL) {
		perror("Error opening input file!!");
		return 1;
	}
	//hashsets to enumerate unique words and speakers
	hashset_t speakers = hashset_create();
	hashset_t words = hashset_create();
	if (speakers == NULL || words == NULL) {
		printf("Failed to create a hashtable instance.");
		return 3;
	}

	char c;
	while ((c = fgetc(input)) != EOF) {
		//loop until we hit a 

	}
}
#define NUM 5
int getFromEnum(char * keyNoTerm) {
	FILE * enumFile;
	enumFile = fopen(ENUM, "r");
	if (enumFile == NULL) {
		printf("Error opening file.");
		return -1;
	}
	long int loc;
	int f = 6;
	char str[NUM*5];
	char * key;
	key = (char *)malloc(strlen(keyNoTerm) + 3 + 1);
	strcpy(key, keyNoTerm);
	strcat(key, " : ");	//add terminating string to the key

	int rec = false;
	const int size = strlen(key);	
	char * buffer;
	buffer = (char *)malloc(size + 1);	//allocate space for buffer string
	if (buffer == NULL) perror("Error in malloc.");
	int i = 0;	//keep track of position in char array
	//loop through file until match is made
	char c;
	char * test = (char *)malloc(5);
	strcpy(test, "test");
	test = cAppend(test, "f");
	printf("string: %s\n", test);
	//while ((c = fgetc(enumFile)) != EOF) {
	//	printf("Analyzing %c\n", c);
	//	if (i == size) {	//if a match has been found, start reading in the data
	//		loc = ftell(enumFile);
	//		fprintf("Key found at location %ld, reading in data starting with %c\n", loc, c);
	//		// free(buffer);
	//		rec = true;
	//		return -1;
	//	}
	//	else {	//no match made yet
	//		if (c == key[i]) {	//the characters match
	//			buffer = cAppend(buffer, c);
	//			i++;
	//			printf("Characters match, string is now %s\n", buffer);
	//		}
	//		else {	//chars don't match, so restart
	//			//free(buffer);
	//			if (rec) {
	//				buffer = (char *)malloc(size + 1);	//reallocate buffer
	//				if (buffer == NULL) perror("Error in malloc.");
	//				printf("String has been reset.\n");
	//				rec = false;
	//			}
	//			else {
	//				printf("Nothing to reset.\n");
	//			}
	//			i = 0;
	//			
	//		}
	//	}
	//}
	fclose(enumFile);
	return -1;
}


char * cAppend(char * str, char c) {	//takes a string and a char, appends them and returns the new string
	size_t len = strlen(str);

	//allocate space for temp string
	char * tmp;
	tmp = (char *)malloc(len + 2);	//enough room for '\0' and the char
	if (tmp == NULL) perror("Error in malloc.");
	printf("Malloced space for %i chars\n", len + 2);
	//copy str into tmp
	strcpy(tmp, str);
	printf("str is now %s\n", tmp);

	//append the char
	printf("char at tmp[len]: %c\n", tmp[len]);
	tmp[len] = 'f';
	tmp[len + 1] = '\0';
	printf("new tmp: %s\n", tmp);

	//free old str and return new one
	free(str);
	return tmp;
}