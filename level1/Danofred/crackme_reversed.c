#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int offset_scanf_input;
	int offset_key_int = 19082004;
	
	printf("\n\n********************************************\n");
	printf("*            Crackme by @Danofred0         *\n");
	printf("********************************************\n");
	
	printf("\nVeuillez entrer le mot de passe pour acceder au contenu:   ");
	scanf("%ld", &offset_scanf_input);
	if (offset_scanf_input == offset_key_int)
		printf("\nTrue !!\n");
	else
		printf("\nFail !!\n\n");
	system("pause");
	
	return 0;
}