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
	int stateLengths[] = { 2, 18, 19, 17, 1 };

	int states[maxStates] = { 0 };	// [0,1,1,1,0] binary map of possible states
	bool scanning = false;	//determine if we are in the process of matching
	char c;
	int idx = 0;
	int skip = 0;
	int matched = 0;
	bool end;
	int o;
	while ((c = fgetc(inputP)) != EOF) {
		if (skip == 0) {
			printf("Scanning '%c'\n", c);
			printf("pre states: ");
			for (int i = 0; i < maxStates; i++) {
				printf("%i", states[i]);
			}
			printf("\n");

			/* update binary map for possible states */

			if (c == '<' || c == '>') {	//start or finish scanning
				if (scanning && c == '>') {	//if already scanning
					memset(states, 0, maxStates * sizeof(int));	//reset map
					scanning = false;	//toggle scanning
					printf("Detected '>' (%c).  Reset map and set scanning to false.\n", c);
				}
				else if (scanning == 0 && c == '<') {	//not scanning
					for (int i = 0; i < maxStates; i++) { states[i] = 1; }	//all states are a possibility
					scanning = true;
					printf("Detected '<' (%c).  Set all map values to 1 and set scanning to true.\n", c);
				}
				else {
					perror("Scanning out of sync.\n");
					return 2;
				}
			}
			else {	//either scanning or not scanning a normal character
				if (scanning) {	//if scanning
					for (int k = 0; k < maxStates; k++) {	//for each possible state, compare the characters at the correct index ex. 'i' ?= "s(p)an class..." -> for each text comparison
						printf("Attempting to compare '%c' with statesText[%i][%i] (%c).\n", c, k, idx, statesText[k][idx]);
						if (states[k] == 1) {	//if text string still matches, check this character
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
							printf("Skipping over states[%i] because it's already 0.\n", k);
						}
					}

					/* check to see if theres only one possibility yet*/
					printf("Checking to see if there's only one possibility yet\n");
					end = false;
					o = 0;
					matched = -1;
					while(!end && o < maxStates) {
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
					if (end) {	//if there are still multiple possibilities, increase the idx and continue to next character
						idx++;
						printf("There are still multiple possibilities.  Increased index to %i. \n", idx);
					}
					else {	//there is only one possibility, set amount of skips to get to content and reset index
						skip = stateLengths[matched] - idx + 1;
						idx = 0;
						printf("There is only one possibility.  Setting skips to stateLengths[%i] (%i) - %i + 1, which is %i, and reseting index to 0.\n", matched, stateLengths[matched], idx, skip);
					}
				}
				else {	//not scanning - match has been made
					printf("recording: '%c'\n", c);
				}
			}
		}
		else {
			skip--;
			printf("Skipping '%c'.  Skips now at %i.\n", c, skip);
		}

		fprintf(outputP, " char: %c, post states: ", c);
		for (int i = 0; i < maxStates; i++) {
			fprintf(outputP, "%i", states[i]);
		}
		fprintf(outputP, "\n\n");
		
	}




	/* close all files */
	fclose(inputP);
	fclose(outputP);
}
