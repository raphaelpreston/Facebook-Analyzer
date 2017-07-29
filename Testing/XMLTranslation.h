#pragma once

/* Function to translate the facebook HTML file into a pretty XML file 

Return Codes:
0: Success
1: Failure with opening files
2: Binary map error
*/
int fileToXML(char * input, char * output);