#include <stdio.h>
#include "XMLTranslation.h"
#include <stdbool.h>

#define MAXSTATES 5

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
	/* double charNum = 0;
	// double totalChars = 83327132;
	*/

	while ((c = fgetc(inputP)) != EOF) {
		// charNum++;
		// printf("\rProgress: %%%.4f", (charNum/totalChars)*100);
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
				if (!scanning) {	//supposed to be recording, means its a meaningful tag with empty content
					printf("empty tag detected.\n");	////this isn't working, WORKING HERE 
				}
				for (int i = 0; i < MAXSTATES; i++) { states[i] = 1; }	//all states are a possibility
				scanning = true;
				foundTwo = false;
				idx = 0;
				skip = false;
				goodSkip = false;
				if (printedTag) {
					fprintf(outputP, matched == 0 ? "</user>\n" : matched == 1 ? "</thread>\n" : matched == 2 ? "</speaker>\n" : matched == 3 ? "</meta>\n" : matched == 4 ? "</content>\n" : "</ERROR>\n");
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
						fprintf(outputP, matched == 0 ? "<user>" : matched == 1 ? "<thread>" : matched == 2 ? "<speaker>" : matched == 3 ? "<meta>" : matched == 4 ? "<content>" : "<ERROR>");
						printedTag = true;
					}
					fprintf(outputP, "%c", c);
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
