#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char password[5];
	char comment[3];
	int result;

	printf("Enter the password:");
	scanf("%4[^\n]", password);
	
	result = strcmp("stars", password);
	printf("Enter the comment:");
	getchar();
	
	scanf("%[^\n]", comment);
	
	if (result)
		printf("Failed! The comment is discarded :(");
	else
		printf("Success: The comment is \"%s\"", comment);
	return 0;
}