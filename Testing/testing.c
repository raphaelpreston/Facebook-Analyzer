#include <stdio.h>
#include "XMLTranslation.h"
#include <sys/timeb.h>
#include <stdbool.h>

int main(int argc, char * argv[])
{















	// struct timeb start, end;
	// double diff;
	//for (int i = 0; i < 30; i++) {
	//	ftime(&start);
	//	fileToXML("C:/Users/IAMFRANK/Documents/FB Testing/messages.htm", "C:/Users/IAMFRANK/Documents/FB Testing/output.txt");
	//	ftime(&end);
	//	diff = (double)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
	//	printf("%.0f\n", diff);
	//}
	//fileToXML("C:/Users/IAMFRANK/Documents/FB Testing/Testing Files/656537", "C:/Users/IAMFRANK/Documents/FB Testing/output.txt");

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

