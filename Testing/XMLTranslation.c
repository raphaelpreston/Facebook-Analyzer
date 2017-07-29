#include <stdio.h>
#include "XMLTranslation.h"
#include <stdbool.h>

#define maxStates 5

int fileToXML(char * input, char * output)
{
	/* open necessary files */
	FILE * inputP;
	FILE * outputP;
	inputP = fopen(input, "r");
	outputP = fopen(output, "w");

	/* error checking */
	if (inputP == NULL) {
		perror("Error opening input file");
		return 1;
	}
	if (outputP == NULL) {
		perror("Error opening output file");
		return 1;
	}
	
	/* start translation from stupid facebook HTML to XML*/

	const char h1[3] = "h1";	//state 0
	const char divClassThread[19] = "div class=\"thread\"";	//state 1
	const char spanClassUser[18] = "span class=\"user\"";	//state 2
	const char spanClassMeta[18] = "span class=\"meta\"";	//state 3
	const char p[2] = "p";		//state 4
	
	char * statesText[] = {h1, divClassThread, spanClassUser, spanClassMeta, p };
	int stateLengths[] = { 3, 19, 18, 18, 2 };

	int states[maxStates] = { 0 };	// [0,1,1,1,0] binary map of possible states
	bool scanning = false;	//determine if we are in the process of matching or recording content
	char c;
	int idx = 0;
	int matched = -1;
	bool end;
	bool foundTwo;
	int o;
	int l;
	bool foundOne;
	bool skip = false;
	bool goodSkip = false;
	while ((c = fgetc(inputP)) != EOF) {
		/* update binary map for possible states */

		if (c == '<' || c == '>') {	//start or finish scanning
			if (c == '>') {	//hit end of tag
				/* check to see if theres only one possibility yet*/
				printf("Found '>'. Checking to see if there is a match.\n");
				if (skip || matched == -1) {	//no match because skip is active or there is no match
					printf("Skip is active or matched is -1 which means that no match exists.  Scanning is true (but is overridden by skip).\n");
				}
				else {
					if (stateLengths[matched] - 1 != idx) {	//lengths aren't correct
						printf("The length of the tag analyzed - 1(%i) is not equal to the length of the supposed match (%i).  Skip is still active, scanning is true (but is overriden by skip).\n", idx, stateLengths[matched] - 1);
					}
					else {	//theres only a single match
						scanning = false;	//toggle scanning - time to record
						skip = false;
						goodSkip = false;
						foundTwo = false;
						printf("There is only one match, and it's the correct length: '%i'!  Set scanning to false (it's time to record), and set skip and goodSkip to false\n", matched);
					}
				}
			}
			else if (c == '<') {	//hit beginning of tag
				for (int i = 0; i < maxStates; i++) { states[i] = 1; }	//all states are a possibility
				scanning = true;
				foundTwo = false;
				idx = 0;
				skip = false;
				goodSkip = false;
				matched = -1;
				printf("Found '<'.  Set all map values to 1, index to zero, matched to -1, and set scanning to true.\n", c);
			}
		}
		else {	//not a < or >
			if (skip) {
				printf("Skipping analyzing '%c'\n", c);
			}
			else if (goodSkip) {
				printf("Skipping analyzing '%c'\n", c);
				idx++;
				printf("Increased index to %i. \n", idx);
			}
			else {
				if (scanning) {	//if scanning
					printf("Scanning '%c'\n", c);
					printf("pre states: ");
					for (int i = 0; i < maxStates; i++) {
						printf("%i", states[i]);
					}
					printf("\n");
					foundOne = false;	//use to determine if there is at least one 1 in the binary tree, so we know to continue scanning or just start skipping
					if (c == '/') {
						skip = true;
						printf("Ending tag detected, setting skip to true.\n");
					}
					else {
						if (foundTwo == true) {	//either state 2 or state 3
							for (int k = 2; k < 4; k++) {
								printf("Attempting to compare '%c' with statesText[%i][%i] (%c).\n", c, k, idx, statesText[k][idx]);
								if (statesText[k][idx] != c) {	//if the character being examined is the same as the character at index "idx" in the text pattern to match it to, set that state to one to continue searching, or 0 to disable searching for that specific text
									states[k] = 0;
									printf("Characters were not equal.  Set states[%i] to 0.\n", k);
								}
								else {
									if (!foundOne) {
										matched = k;
										foundOne = true;
										printf("Found first instance of equal characters.  Keeping state[%i] at 1, set 'foundOne' to true, and matched to %i.\n", k, matched);
									}
									else {
										printf("Found second instance of equal characters. \n");
										//foundTwo is already true
									}
								}
							}
						}
						else if (matched != -1) {	//only compare the already matched value
							printf("Attempting to compare '%c' with statesText[%i][%i] (%c).\n", c, matched, idx, statesText[matched][idx]);
							if (statesText[matched][idx] != c) {	//if the character being examined is the same as the character at index "idx" in the text pattern to match it to, set that state to one to continue searching, or 0 to disable searching for that specific text
								states[matched] = 0;
								printf("Characters were not equal.  Set states[%i] to 0.\n", matched);
							}
							else {
								//matched is already set
								foundOne = true;
								printf("Found first instance of equal characters.  Keeping state[%i] at 1, set 'foundOne' to true.\n", matched);
							}
						}
						else {
							for (int k = 0; k < maxStates; k++) {	//for each possible state, compare the characters at the correct index ex. 'i' ?= "s(p)an class..." -> for each text comparison
								printf("Attempting to compare '%c' with statesText[%i][%i] (%c).\n", c, k, idx, statesText[k][idx]);
								if (states[k] == 1) {	//if text string still matches and it's not overflowing, check this character
									printf("Checking states[%i] because it's still 1.\n", k);
									if (statesText[k][idx] != c) {	//if the character being examined is the same as the character at index "idx" in the text pattern to match it to, set that state to one to continue searching, or 0 to disable searching for that specific text
										states[k] = 0;
										printf("Characters were not equal.  Set states[%i] to 0.\n", k);
									}
									else {
										if (!foundOne) {
											matched = k;
											foundOne = true;
											printf("Found first instance of equal characters.  Keeping state[%i] at 1, set 'foundOne' to true, and matched to %i.\n", k, matched);
										}
										else {
											printf("Found second instance of equal characters. \n");
											foundTwo = true;
										}
									}
								}
								else {
									printf("Skipping over states[%i] because it's already 0.\n", k);
								}
							}
						}

						/* determine if there are any 1s in binary map */
						if (!foundOne) {	//all zeroes
							skip = true;
							printf("All zeroes.  Set skip to true.\n");
						}

						/* determine if there is only one actual possibility left (no point in checking state 0 or 4)*/
						if (matched == 1 && c == 't' && idx == 11) {	//<div class="t
							goodSkip = true;
							printf("positive that match is state 1.  Set goodSkip to true.\n");
						}
						else if (matched == 2 && c == 'u' && idx == 12) {	//<span class="u
							goodSkip = true;
							printf("positive that match is state 2.  Set goodSkip to true.\n");
						}
						else if (matched == 3 && c == 'm' && idx == 12) {	//<span class="m
							goodSkip = true;
							printf("positive that match is state 3.  Set goodSkip to true.\n");
						}
					}
					idx++;
					printf("Increased index to %i. \n", idx);
					printf("post states: ");
					for (int i = 0; i < maxStates; i++) {
						printf("%i", states[i]);
					}
					printf("\n");
				}
				else {	//not scanning - match has been made
					printf("recording: '%c'\n", c);
				}
			}
		}
		printf("\n");
	}

	/* close all files */
	fclose(inputP);
	fclose(outputP);
}
