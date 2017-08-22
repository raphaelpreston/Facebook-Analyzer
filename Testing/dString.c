#include <stdio.h>
#include <string.h>
#include "dString.h"

dString * dString_new(size_t size) {
	//declare struct
	dString * dStr;
	dStr = (dString *)malloc(sizeof(dString));

	//declare char * and make faux-empty
	char * buffer;
	buffer = (char *)malloc(sizeof(char) * size);
	buffer[0] = '\0';

	//set struct atributes
	dStr->buffer = buffer;
	dStr->idx = 0;
	dStr->size = size;

	return dStr;
}

void dString_delete(dString * dStr) {
	free(dStr->buffer);
	free(dStr);
	return;
}

void dString_fill(dString * dStr, char * string) {
	//error checking
	if (dStr->size < strlen(string) + 1) {
		perror("Error: size doesn't accomodate string.");
		return;
	}

	//make new buffer
	char * buffer;
	buffer = (char *)malloc(sizeof(char) * dStr->size);
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
	if (idx > dStr->size) {
		perror("Error: index out of range.");
		return;
	}

	//change the char
	dStr->buffer[idx] = c;
	return;
}