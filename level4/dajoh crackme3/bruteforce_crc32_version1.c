#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIALISATION_SIZE 0x12800
#define LINE_LEN 255

#define CHECKSUM_TO_FIND 0x331252C3

unsigned int compute_hash(char *str, unsigned char *initialisation, unsigned int lookuptable[256]) {
	int len = strlen(str);

	unsigned int checksum = 0;	
	int i = 0;
	do
	{
		unsigned char index = initialisation[i] ^ str[i % len];
		checksum = lookuptable[(unsigned char)checksum ^ index] ^ (checksum >> 8);
		++i;
	}
	while ( i < INITIALISATION_SIZE );
	
	return checksum;
}

void find_collision_from_random(char *str, unsigned char *resource, unsigned int lookuptable[256]) {
	unsigned int checksum = 0;	
	unsigned int j = 0;
	while (j < 0xFFFFFFFF) {
		sprintf(str, "%i", j);
		
		checksum = compute_hash(str, resource, lookuptable);
		
		if (checksum == CHECKSUM_TO_FIND) {
			return true;
		}
		
		j++;
	}
	return false;
}


int main(int argc, char *argv[]) {
	unsigned char *resource = malloc(INITIALISATION_SIZE);
	unsigned int *lookuptable = malloc(256 * sizeof(unsigned int));
	
	FILE *resourceFile = fopen("resource.bin", "rb");
	fread(resource, sizeof(char), INITIALISATION_SIZE, resourceFile);
	fclose(resourceFile);

	FILE *lookuptableFile = fopen("lookuptable.bin", "rb");	
	fread(lookuptable, sizeof(unsigned int), 256, resourceFile);
	fclose(lookuptableFile);
	
    char str[LINE_LEN];
	int found = find_collision_from_random(str, resource, lookuptable);
		
	if (found)
		printf("%s %X\n", str, CHECKSUM_TO_FIND);
	
	free(resource);
	free(lookuptable);
	
	return 0;
}