#include <stdio.h>
#include "XMLTranslation.h"
#include "utlist.h"
#include <sys/timeb.h>
#include <stdbool.h>
#include "dString.h"
#include <time.h>


int main(int argc, char * argv[])
{
	//hash testing

	word_list * list = word_list_new("word");

	message * m1 = message_new();
	message_set_tstamp(m1, 6, 45, 0, 3, 6, 21, 1999);
	message * m2 = message_new();
	message_set_tstamp(m2, 1, 29, 1, 6, 10, 21, 2000);
	message * m3 = message_new();
	message_set_tstamp(m3, 6, 45, 0, 3, 6, 22, 1999);
	message * m4 = message_new();
	message_set_tstamp(m4, 7, 45, 0, 3, 6, 24, 1999);
	message * m5 = message_new();
	message_set_tstamp(m5, 12, 45, 0, 3, 6, 21, 1999);

	word_list_add_node(list, m1);
	word_list_add_node(list, m2);
	word_list_add_node(list, m3);
	word_list_add_node(list, m4);
	word_list_add_node(list, m5);

	message * search;
	LL_FOREACH(list->head, search) {
		printf("%s was said at ", list->word);
		print_time(search->tstamp);
		printf("\n");
	}
	





	/*word_list l;
	word_list * p;
	word_list * r;
	word_list * tmp;
	word_list * lists = NULL;

	r = (word_list*)malloc(sizeof(word_list));
	memset(r, 0, sizeof(word_list));
	r->head = (message *)malloc(sizeof(message));
	if (r->head == NULL) return -1;
	r->head->a = 'a';
	r->head->b = 1;

	HASH_ADD(hh, lists, head, sizeof(message), r);
	
	memset(&l, 0, sizeof(word_list));
	l.head = (message *)malloc(sizeof(message));
	l.head->a = 'a';
	l.head->b = 1;

	HASH_FIND(hh, lists, &l.head, sizeof(message), p);

	if (p) printf("found %c %d\n", p->head->a, p->head->b);
	else
		printf("Not found.\n");

	HASH_ITER(hh, lists, p, tmp) {
		HASH_DEL(lists, p);
		free(p);
	}
	return 0;*/


	///////////////////////
	 
	 
	//typedef struct {
	//	char a;
	//	int b;
	//} record_key_t;

	//typedef struct {
	//	record_key_t key;
	//	/* ... other data ... */
	//	UT_hash_handle hh;
	//} record_t;


	//record_t l, *p, *r, *tmp, *records = NULL;

	//r = (record_t*)malloc(sizeof(record_t));
	//memset(r, 0, sizeof(record_t));
	//r->key.a = 'a';
	//r->key.b = 1;
	//HASH_ADD(hh, records, key, sizeof(record_key_t), r);

	//memset(&l, 0, sizeof(record_t));
	//l.key.a = 'a';
	//l.key.b = 1;
	//HASH_FIND(hh, records, &l.key, sizeof(record_key_t), p);

	//if (p) printf("found %c %d %c\n", p->key.a, p->key.b);
	//else printf("Not found.\n");
	//HASH_ITER(hh, records, p, tmp) {
	//	HASH_DEL(records, p);
	//	free(p);
	//}
	//return 0;











	// linked list testing

	/*word_node * head = NULL;

	word_node * n1 = (word_node *)malloc(sizeof(word_node));
	word_node * n2 = (word_node *)malloc(sizeof(word_node));
	word_node * n3 = (word_node *)malloc(sizeof(word_node));

	n1->word = (char *)malloc(sizeof(char) * 6);
	strcpy(n1->word, "node1");
	n1->tstamp.min = 12;
	n1->tstamp.hour = 4;
	n1->tstamp.mday = 14;
	n1->tstamp.month = 4;
	n1->tstamp.year = 1925;
	n1->tstamp.wday = 6;
	n1->tstamp.ampm = 0;

	n2->word = (char *)malloc(sizeof(char) * 6);
	strcpy(n2->word, "node2");
	n2->tstamp.min = 11;
	n2->tstamp.hour = 4;
	n2->tstamp.mday = 14;
	n2->tstamp.month = 4;
	n2->tstamp.year = 1925;
	n2->tstamp.wday = 6;
	n2->tstamp.ampm = 0;

	n3->word = (char *)malloc(sizeof(char) * 6);
	strcpy(n3->word, "node3");
	n3->tstamp.min = 54;
	n3->tstamp.hour = 1;
	n3->tstamp.mday = 7;
	n3->tstamp.month = 3;
	n3->tstamp.year = 1926;
	n3->tstamp.wday = 0;
	n3->tstamp.ampm = 0;

	printf("Prepending:\n");
	printf("%s: ", n1->word);
	print_time(&(n1->tstamp));
	printf("\n%s: ", n2->word);
	print_time(&(n2->tstamp));
	printf("\n%s: ", n3->word);
	print_time(&(n3->tstamp));
	printf("\n");

	LL_PREPEND(head, n1);
	LL_PREPEND(head, n2);
	LL_PREPEND(head, n3);

	word_node * elt;

	printf("\nBefore sort:\n");
	DL_FOREACH(head, elt) {
		printf("%s: ", elt->word);
		print_time(&(elt->tstamp));
		printf("\n");
	}
	printf("\nAfter sort:\n");
	LL_SORT(head, tstamp_comp);
	DL_FOREACH(head, elt) {
		printf("%s: ", elt->word);
		print_time(&(elt->tstamp));
		printf("\n");
	}*/
	

































	//typedef struct word_node {
	//	char * word;
	//	struct tm tstamp;
	//	struct element * next;
	//}word_node;

	//word_node * head = NULL;

	//word_node * n1 = (word_node *)malloc(sizeof(word_node));
	//word_node * n2 = (word_node *)malloc(sizeof(word_node));
	//word_node * n3 = (word_node *)malloc(sizeof(word_node));


	//n1->word = (char *)malloc(sizeof(char) * 6);
	//strcpy(n1->word, "node1");
	//n1->tstamp.tm_sec = 0;		/* seconds, range 0 to 59        */
	//n1->tstamp.tm_min = 12;		/* minutes, range 0 to 59        */
	//n1->tstamp.tm_hour = 6;		/* hours,   range 0 to 23        */
	//n1->tstamp.tm_mday = 15;	/* day of  month, range 1 to 31  */
	//n1->tstamp.tm_mon = 4;		/* month, range 0 to 11          */
	//n1->tstamp.tm_year = 25;	/* number of years since 1900    */
	//n1->tstamp.tm_wday = 5;		/* day of the week, range 0 to 6 */

	//char * time;
	//time = (char *)malloc(100);
	//strftime(time, 99, "%A, %B %#d, %Y at %#I:%#M%p", &(n1->tstamp));
	//printf("%s\n", time);

	////LL_PREPEND(head, n1);
	////LL_PREPEND(head, n2);
	////LL_PREPEND(head, n3);

	////word_node search;

















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

