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
	while ((c = fgetc(inputP)) != EOF) {
		printf("Scanning '%c'\n", c);
		printf("pre states: ");
		for (int i = 0; i < maxStates; i++) {
			printf("%i", states[i]);
		}
		printf("\n");

		/* update binary map for possible states */

		if (c == '<' || c == '>') {	//start or finish scanning
			if (c == '>') {	//hit end of tag
				/* check to see if theres only one possibility yet*/
				printf("Checking to see if there is a match.\n");
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
						printf("There is only one match, and it's the correct length: '%i'!  Set scanning to false (it's time to record), and set skip to false\n", matched);
					}
				}
				printf("Set skip to false, and set scanning to false.\n", c);
			}
			else if (c == '<') {	//hit beginning of tag
				for (int i = 0; i < maxStates; i++) { states[i] = 1; }	//all states are a possibility
				scanning = true;
				idx = 0;
				skip = false;
				matched = -1;
				printf("Detected '<' (%c).  Set all map values to 1, index to zero, matched to -1, skip to false, and set scanning to true.\n", c);
			}
		}
		else {	//not a < or >
			if (skip == true) {
				printf("Skipping analyzing '%c'\n", c);
			}
			else {
				if (scanning) {	//if scanning
					foundOne = false;	//use to determine if there is at least one 1 in the binary tree, so we know to continue scanning or just start skipping
					if (c == '/') {
						skip = true;
						printf("Ending tag detected, setting skip to true.\n");
					}
					else {
						for (int k = 0; k < maxStates; k++) {	//for each possible state, compare the characters at the correct index ex. 'i' ?= "s(p)an class..." -> for each text comparison
							printf("Attempting to compare '%c' with statesText[%i][%i] (%c).\n", c, k, idx, statesText[k][idx]);
							if (states[k] == 1) {	//if text string still matches and it's not overflowing, check this character
								if (idx < stateLengths[k] - 1) {
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
										}
									}
								}
								else {
									printf("Skipping over states[%i] because it's too long to be a match.  idx (%i) was not less than stateLengths[%i] (%i) - 1.\n", k, idx, k, stateLengths[k]);
								}
							}
							else {
								printf("Skipping over states[%i] because it's already 0.\n", k);
							}
						}

						/* determine if there are any 1s in binary map */
						if (!foundOne) {	//all zeroes
							skip = true;
							printf("All zeroes.  Set skip to true.\n");
						}
					}
					idx++;
					printf("Increased index to %i. \n", idx);
				}
				else {	//not scanning - match has been made
					printf("recording: '%c'\n", c);
				}
			}
		}
		fprintf(outputP, " char: %c, post states: ", c);
		for (int i = 0; i < maxStates; i++) {
			fprintf(outputP, "%i", states[i]);
		}
		fprintf(outputP, "\n\n");
		
		printf("post states: ");
		for (int i = 0; i < maxStates; i++) {
			printf("%i", states[i]);
		}
		printf("\n\n");
	}




	/* close all files */
	fclose(inputP);
	fclose(outputP);
}
