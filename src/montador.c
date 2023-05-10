#include <string.h>
#include "preprocessor.h"

int main(int argc, char **argv) {
	char *filename = malloc((strlen(argv[1]) + 5) * sizeof(char));
	strcpy(filename, argv[1]);
	strcat(filename, ".exc");

	return 0;
}
