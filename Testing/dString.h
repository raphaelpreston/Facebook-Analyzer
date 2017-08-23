#pragma once

//struct

typedef struct dString {
	char * buffer;
	size_t size;	//max size of string
	size_t idx;		//last filled index
} dString;

//initialize the dynamic string at the given size
dString * dString_new(size_t);

//free everything
void dString_delete(dString *);

//fill the dString with a string
void dString_fill(dString *, char *);

//change an existing char at a given index in the string
void dString_changeChar(dString *, size_t, char);

//append a char to the end of a string
void dString_append(dString *, char);