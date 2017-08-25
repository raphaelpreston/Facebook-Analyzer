#include <stdio.h>
#include "XMLTranslation.h"
#include "utlist.h"
#include <sys/timeb.h>
#include <stdbool.h>
#include "dString.h"
#include <time.h>

int main(int argc, char * argv[])
{
	// linked list testing

	typedef struct word_node {
		char * word;
		struct tm tstamp;
		struct element * next;
	}word_node;

	word_node * head = NULL;

	word_node * n1 = (word_node *)malloc(sizeof(word_node));
	word_node * n2 = (word_node *)malloc(sizeof(word_node));
	word_node * n3 = (word_node *)malloc(sizeof(word_node));


	n1->word = (char *)malloc(sizeof(char) * 6);
	strcpy(n1->word, "node1");
	n1->tstamp.tm_sec = 0;	/* seconds,  range 0 to 59          */
	n1->tstamp.tm_min = 12;	/* minutes, range 0 to 59           */
	n1->tstamp.tm_hour = 6;	/* hours, range 0 to 23             */
	n1->tstamp.tm_mday = 
	n1->tstamp.tm_mon;
	n1->tstamp.tm_year;
	n1->tstamp.tm_wday;



	LL_PREPEND(head, n1);
	LL_PREPEND(head, n2);
	LL_PREPEND(head, n3);

	word_node search;

















	//getFromEnum("enumtest");
	//d string testing:
	/*printf("Initializing dString...\n\n");
	dString * dStr;
	dStr = dString_new(5);
	dString * dStrSmall;
	dStrSmall = dString_new(4);
	dString * dStrBig;
	dStrBig = dString_new(6);
	

	printf("Filling dString with \"test\"...\n");
	printf("Before: \"%s\" | Size: %i | Idx: %i\n", dStr->buffer, dStr->size, dStr->idx);
	dString_fill(dStr, "test");
	printf("After: \"%s\" | Size: %i | Idx: %i\n\n", dStr->buffer, dStr->size, dStr->idx);

	printf("Filling dStringSmall with \"test\"...\n");
	printf("Before: \"%s\" | Size: %i | Idx: %i\n", dStrSmall->buffer, dStrSmall->size, dStrSmall->idx);
	dString_fill(dStrSmall, "test");
	printf("After: \"%s\" | Size: %i | Idx: %i\n\n", dStrSmall->buffer, dStrSmall->size, dStrSmall->idx);

	printf("Filling dStringBig with \"test\"...\n");
	printf("Before: \"%s\" | Size: %i | Idx: %i\n", dStrBig->buffer, dStrBig->size, dStrBig->idx);
	dString_fill(dStrBig, "test");
	printf("After: \"%s\" | Size: %i | Idx: %i\n\n", dStrBig->buffer, dStrBig->size, dStrBig->idx);

	printf("Changing dString[2] to '*'...\n");
	printf("Before: \"%s\" | Size: %i | Idx: %i\n", dStr->buffer, dStr->size, dStr->idx);
	dString_changeChar(dStr, 2, '*');
	printf("After: \"%s\" | Size: %i | Idx: %i\n\n", dStr->buffer, dStr->size, dStr->idx);

	printf("Changing dString[-1] to '*'...\n");
	printf("Before: \"%s\" | Size: %i | Idx: %i\n", dStr->buffer, dStr->size, dStr->idx);
	dString_changeChar(dStr, -1, '*');
	printf("After: \"%s\" | Size: %i | Idx: %i\n\n", dStr->buffer, dStr->size, dStr->idx);

	printf("Changing dString[4] to '*'...\n");
	printf("Before: \"%s\" | Size: %i | Idx: %i\n", dStr->buffer, dStr->size, dStr->idx);
	dString_changeChar(dStr, 4, '*');
	printf("After: \"%s\" | Size: %i | Idx: %i\n\n", dStr->buffer, dStr->size, dStr->idx);

	printf("Changing dStringBig[4] to '*'...\n");
	printf("Before: \"%s\" | Size: %i | Idx: %i\n", dStrBig->buffer, dStrBig->size, dStrBig->idx);
	dString_changeChar(dStrBig, 4, '*');
	printf("After: \"%s\" | Size: %i | Idx: %i\n\n", dStrBig->buffer, dStrBig->size, dStrBig->idx);

	printf("Changing dStringBig[5] to '*'...\n");
	printf("Before: \"%s\" | Size: %i | Idx: %i\n", dStrBig->buffer, dStrBig->size, dStrBig->idx);
	dString_changeChar(dStrBig, 5, '*');
	printf("After: \"%s\" | Size: %i | Idx: %i\n\n", dStrBig->buffer, dStrBig->size, dStrBig->idx);

	for (int i = 0; i < 10000; i++) {
		printf("%i: Appending \"&\" to dStringBig\n", i);
		dString_append(dStrBig, '&');
		printf("After: Size: %i | Idx: %i\n\n", dStrBig->size, dStrBig->idx);
	}

	printf("Deleting dString...\n\n");
	dString_delete(dStr);

	printf("Everything completed.\n");*/

















	// struct timeb start, end;
	// double diff;
	//for (int i = 0; i < 30; i++) {
	//	ftime(&start);
	//	fileToXML("C:/Users/IAMFRANK/Documents/FB Testing/messages.htm", "C:/Users/IAMFRANK/Documents/FB Testing/output.txt");
	//	ftime(&end);
	//	diff = (double)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
	//	printf("%.0f\n", diff);
	//}
	// fileToXML("C:/Users/IAMFRANK/Documents/FB Testing/Testing Files/656537", "C:/Users/IAMFRANK/Documents/FB Testing/output.txt");

	////////////////// runs through all the characters and does nothing to them

	/*FILE * messagesP;
	
	FILE * outputP;
	FILE * testing;
	testing = fopen("C:/Users/IAMFRANK/Documents/FB Testing/testing.txt", "w");
	
	struct timeb start, end;
	double diff;
	for (int i = 0; i < 5; i++) {
		printf("%i\t", i);
		fprintf(testing, "%i\t", i);
		outputP = fopen("C:/Users/IAMFRANK/Documents/FB Testing/output.txt", "w");
		messagesP = fopen("C:/Users/IAMFRANK/Documents/FB Testing/messages.htm", "r");
		ftime(&start);
		char c;
		while ((c = fgetc(messagesP)) != EOF) {
		}
		ftime(&end);
		diff = (double)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
		printf("%.0f\n", diff);
		fprintf(testing, "%.0f\n", diff);
		fclose(messagesP);
		fclose(outputP);
	}

	fclose(testing);*/
	//////////////^^^^^^////////////


	//FILE * testing;
	//double diff;
	//testing = fopen("C:/Users/IAMFRANK/Documents/FB Testing/bigtesting.txt", "w");
	//struct timeb start, end;
	//int fileNum;
	//for (fileNum = 1; fileNum <= 83380072; fileNum += 656536) {	//executes for every file
	//	char fileName[100];
	//	snprintf(fileName, sizeof(fileName), "C:/Users/IAMFRANK/Documents/FB Testing/Testing Files/%i", fileNum);
	//	//test thirty times to get average
	//	printf("%i\t", fileNum);
	//	fprintf(testing, "%i\t", fileNum);
	//	for (int i = 0; i < 30; i++) {
	//		ftime(&start);
	//		fileToXML(fileName, "C:/Users/IAMFRANK/Documents/FB Testing/output.txt");
	//		ftime(&end);
	//		diff = (double)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
	//		fprintf(testing, "%.3f\t", diff);
	//		printf("%.3f\t", diff);
	//	}
	//	fprintf(testing, "\n");
	//	printf("\n");
	//}
	//fclose(testing);
	/////////////////////////////////


	//int charsMax;
	//int charsAnalyzed;
	//for (charsMax = 1; charsMax <= 83380072; charsMax+=656536) {	//executes 127 times
	//	char fullPath[61 + 1];
	//	snprintf(fullPath, sizeof(fullPath), "C:/Users/IAMFRANK/Documents/FB Testing/Testing Files/%i", charsMax);
	//	messagesP = fopen("C:/Users/IAMFRANK/Documents/FB Testing/messages.htm", "r");
	//	outputP = fopen(fullPath, "w");
	//	charsAnalyzed = 0;
	//	while ((c = fgetc(messagesP)) != EOF && charsAnalyzed < charsMax) {
	//		fprintf(outputP, "%c", c);
	//		charsAnalyzed++;
	//	}
	//	fclose(outputP);
	//	fclose(messagesP);
	//}




	return 0;
}

