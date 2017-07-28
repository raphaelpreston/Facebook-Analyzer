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
	const char spanClassUser[20] = "span class=\"user\"";	//state 2
	const char spanClassMeta[18] = "span class=\"meta\"";	//state 3
	const char p[2] = "p";		//state 4
	
	char * statesText[] = {h1, divClassThread, spanClassUser, spanClassMeta, p };
	int stateLengths[] = { 3, 19, 20, 18, 2 };

	int states[maxStates] = { 0 };	// [0,1,1,1,0] binary map of possible states
	bool scanning = false;	//determine if we are in the process of matching
	char c;
	int idx = 0;
	int matched = 0;
	bool end;
	int o;
	int l;
	bool found;
	int yes;
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
				end = false;
				o = 0;
				matched = -1;
				while (!end && o < maxStates) {
					printf("Checking states[%i]\n", o);
					if (states[o] == 1) {
						if (matched == -1) {	//if a 1 hasn't been found yet
							matched = o;
							printf("The first 1 was found.  Set matched to %i.\n", matched);
						}
						else {
							end = true;	//termination because found a second 1
							printf("A second 1 was found.  Set premature termination.\n");
						}
					}
					o++;
				}
				if (end) {	//premature termination because a second 1 was found
					perror("Error.  Found a second 1 in binary map.\n");
					return 2;
				}
				if (matched == -1) {	//no 1s - tag isn't a match
					printf("No matches found.  Skip is still active.  Scanning is true (but is overridden by skip).\n");
				}
				else {	//theres only a single match
					scanning = false;	//toggle scanning - time to record
					skip = false;
					printf("There is only one match!  Set scanning to false (it's time to record), and set skip to false\n");
				}
				memset(states, 0, maxStates * sizeof(int));	//reset map
				idx = 0;	//set index to zero
				printf("Reset map, set index to 0, set skip to false, and set scanning to false.\n", c);
			}
			else if (c == '<') {	//hit beginning of tag
				for (int i = 0; i < maxStates; i++) { states[i] = 1; }	//all states are a possibility
				scanning = true;
				skip = false;
				printf("Detected '<' (%c).  Set all map values to 1, skip to false, and set scanning to true.\n", c);
			}
		}
		else {	//either scanning or not scanning a normal character
			if (skip == true) {
				printf("Skipping analyzing '%c'\n", c);
			}
			else {
				if (scanning) {	//if scanning
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
									printf("Characters were equal.  Keeping state[%i] at 1.\n", k);
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
					printf("Checking to see if there are any 1s in binary map.\n");
					l = 0;
					yes = -1;
					while (yes == -1 && l < maxStates) {
						printf("Checking states[%i]\n", l);
						if (states[l] == 1) {
							yes = l;
							printf("A 1 was found.  Set yes to %i.\n", yes);
						}
						l++;
					}
					if (yes == -1) {	//all zeroes
						skip = true;
						printf("All zeroes.  Set skip to true.\n");
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
