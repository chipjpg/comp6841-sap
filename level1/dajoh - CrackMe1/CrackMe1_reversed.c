#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define FOREGROUND_BRIGHT_WHITE		FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
#define BUFF_LEN 256

void print(char *str, unsigned short wAttributes) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    /* Save current attributes */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, wAttributes);
    printf("%s", str);

    /* Restore original attributes */
    SetConsoleTextAttribute(hConsole, saved_attributes);
}

void getline(char *out, int getlen) {
	fgets(out, getlen, stdin);
	int len = strlen(out);
	if (len > 0 && out[len-1] == '\n') {
		out[len-1] = '\0';
	}
}

int main(int argc, char *argv[]) {
	char input[BUFF_LEN];

	printf("-----------------------------------\n");
	printf("--           CrackMe 1           --\n");
	printf("--      by github.com/dajoh      --\n");
	printf("-----------------------------------\n\n");
	
	print("Enter password: ", FOREGROUND_BRIGHT_WHITE);
	getline(input, BUFF_LEN);
	
	while (strcmp(input, "easypassword")) {
		print("Fail! You entered the wrong password.\n\n", FOREGROUND_INTENSITY | FOREGROUND_RED);
		print("Enter password: ", FOREGROUND_BRIGHT_WHITE);	
		getline(input, BUFF_LEN);
	}

	
	print("Congratulations! You entered the correct password.\n\n", FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	print("Press enter to exit...", FOREGROUND_INTENSITY);

	while (1) {
		char prompt;
		prompt = getchar();
		if (prompt == 0x0A) {
			break;
		}
	}
	
	return 0;
}