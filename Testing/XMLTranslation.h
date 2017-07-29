#pragma once

/* Function to translate the facebook HTML file into a pretty XML file.  Assumes there are no < or > characters in file, which is fine because Facebook converts them to &lt and &gt.

Return Codes:
0: Success
1: Failure with opening files
2: Binary map error
*/
int fileToXML(char * input, char * output);