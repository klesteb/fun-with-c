
#include <stdio.h>
#include <string.h>

extern char *ltrim(char *);

int main(void) {

	char string[32];

	strcpy(string, "	 This is a really long string");

	printf("string = %s\n", string);

	ltrim(string);

	printf("string = %s\n", string);

}

