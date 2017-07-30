#include <stdio.h>
#include "XMLTranslation.h"
#include <sys/timeb.h>
#include <stdbool.h>

int main(int argc, char * argv[])
{
	//struct timeb start, end;
	//double diff;
	//for (int i = 0; i < 30; i++) {
	//	ftime(&start);
	//	fileToXML("C:/Users/IAMFRANK/Documents/FB Testing/messages.htm", "C:/Users/IAMFRANK/Documents/FB Testing/output.txt");
	//	ftime(&end);
	//	diff = (double)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
	//	printf("%.0f\n", diff);
	//}
	//// fileToXML("C:/Users/IAMFRANK/Documents/FB Testing/messages.htm", "C:/Users/IAMFRANK/Documents/FB Testing/output.txt");

	FILE * messagesP;
	FILE * outputP;
	char c;
	bool end = false;
	int charsMax;
	int charsAnalyzed;

	for (charsMax = 1; charsMax <= 83380072; charsMax+=656536) {	//executes 127 times
		char fullPath[61 + 1];
		snprintf(fullPath, sizeof(fullPath), "C:/Users/IAMFRANK/Documents/FB Testing/Testing Files/%i", charsMax);
		messagesP = fopen("C:/Users/IAMFRANK/Documents/FB Testing/messages.htm", "r");
		outputP = fopen(fullPath, "w");
		charsAnalyzed = 0;
		while ((c = fgetc(messagesP)) != EOF && charsAnalyzed < charsMax) {
			fprintf(outputP, "%c", c);
			charsAnalyzed++;
		}
		fclose(outputP);
		fclose(messagesP);
	}




	return 0;
}

