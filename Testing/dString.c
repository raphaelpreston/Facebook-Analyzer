#include <stdio.h>
#include <string.h>
#include "dString.h"

//note: whenever comparing idx and size, make sure to cast size to an int, because it's a size_t.
//note: for some reason, when appending a ton of times, when running in visual studio, the size will double until the size reaches 48 and then stay there... running in cmd works fine.

dString * dString_new(size_t size) {
	//declare struct
	dString * dStr;
	dStr = (dString *)malloc(sizeof(dString));
	if (dStr == NULL) {
		perror("Error mallocing dString.");
		return;
	}
	//declare char * and make faux-empty
	char * buffer;
	buffer = (char *)malloc(sizeof(char) * size);
	if (buffer == NULL) {
		perror("Error mallocing dstring.");
		return;
	}
	buffer[0] = '\0';

	//set struct atributes
	dStr->buffer = buffer;
	dStr->idx = -1;
	dStr->size = size;

	return dStr;
}

void dString_delete(dString * dStr) {
	free(dStr->buffer);
	free(dStr);
	return;
}

void dString_fill(dString * dStr, char * string) {
	//check if theres room or not
	if (dStr->size < strlen(string) + 1) dStr->size = strlen(string) + 1;

	//make new buffer
	char * buffer;
	buffer = (char *)malloc(sizeof(char) * dStr->size);
	if (buffer == NULL) {
		perror("Error mallocing string.");
		return;
	}
	strcpy(buffer, string);

	//set struct atributes
	free(dStr->buffer);
	dStr->buffer = buffer;
	dStr->idx = strlen(string) - 1;

	return;
}

void dString_changeChar(dString * dStr, size_t idx, char c) {
	//error checking
	if (idx > dStr->idx) {
		if(idx == dStr->idx + 1) perror("Error: index refers to null terminator.");
		else perror("Error: can only change previously set characters in the string.");
		return;
	}
	if (idx > (int)(dStr->size)) {
		perror("Error: index out of range.");
		return;
	}

	//change the char
	dStr->buffer[idx] = c;
	return;
}

void dString_append(dString * dStr, char c) {
	//determine if a size increase is necessary
	if (dStr->idx >= (int)(dStr->size - 2)) {	//no space
		//make a new buffer with 2x as much space
		char * buffer;
		buffer = malloc(sizeof(char) * dStr->size * 2);
		if (buffer == NULL) {
			perror("Error mallocing string.");
			return;
		}

		//fill it with the old buffer, the new char, and the null terminator
		memcpy(buffer, dStr->buffer, dStr->idx + 1);
		buffer[dStr->idx + 1] = c;
		buffer[dStr->idx + 2] = '\0';


		//free the old memory and reassign buffer
		free(dStr->buffer);
		dStr->buffer = buffer;

		//update size
		dStr->size *= 2;
	}
	else {	//space available
		//put char in place of null terminator
		dStr->buffer[dStr->idx + 1] = c;
		dStr->buffer[dStr->idx + 2] = '\0';
	}

	//update idx
	dStr->idx += 1;
}

void dString_minimize(dString * dStr) {
	size_t len = strlen(dStr->buffer);
	if (dStr->size == len + 1) return;

	/* make the new buffer */
	char * buffer;
	buffer = (char *)malloc(sizeof(char) * len + 1);
	strcpy(buffer, dStr->buffer);

	/* assign the new buffer and free the old one */
	free(dStr->buffer);
	dStr->buffer = buffer;

	/* change size accordingly */
	dStr->size = len + 1;
	return;
}

void dString_clear(dString * dStr) {
	if (strlen(dStr) == 0)return;
	/* re-allocate memory */
	free(dStr->buffer);
	dStr->buffer = (char *)malloc(sizeof(char) * dStr->size);

	/* make it appear empty */
	dStr->buffer[0] = '\0';

	/* change index accordingly */
	dStr->idx = -1;
	return;
}