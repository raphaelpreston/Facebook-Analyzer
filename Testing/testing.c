#include <stdio.h>
#include "XMLTranslation.h"
#include <sys/timeb.h>

int main(int argc, char * argv[])
{
	
	struct timeb start, end;
	double diff;
	ftime(&start);
	fileToXML("C:/Users/IAMFRANK/Documents/FB Testing/messages.htm", "C:/Users/IAMFRANK/Documents/FB Testing/output.txt");
	ftime(&end);
	diff = (double)(1000.0 * (end.time - start.time)
		+ (end.millitm - start.millitm));

	printf("\nOperation took %.0f milliseconds\n", diff);
	return 0;
}

