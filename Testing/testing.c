#include <stdio.h>
#include "XMLTranslation.h"
#include <sys/timeb.h>

int main(int argc, char * argv[])
{
	/*struct timeb start, end;
	double diff;
	for (int i = 0; i < 30; i++) {
		ftime(&start);
		fileToXML("C:/Users/IAMFRANK/Documents/FB Testing/shannon.htm", "C:/Users/IAMFRANK/Documents/FB Testing/output_shannon.txt");
		ftime(&end);
		diff = (double)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
		printf("%.0f\n", diff);
	}*/
	fileToXML("C:/Users/IAMFRANK/Documents/FB Testing/messages.htm", "C:/Users/IAMFRANK/Documents/FB Testing/output.txt");
	return 0;
}

