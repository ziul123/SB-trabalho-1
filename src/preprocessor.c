#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int is_section(const char *token, const char *section) {
	char *str = malloc(strlen(token) * sizeof(char));
	strcpy(str, token);
	char *t;
	int result = 0;
	t = strtok_r(str, " ", &str);
	result |= strcmp(t, "SECTION");
	if((t = strtok_r(str, " ", &str)))
		result |= strcmp(t, section);
	return result == 0;
}


char *preprocess_file(char *filename){
	char *ppd_filename = malloc((strlen(filename) + 5) * sizeof(char));
	strcpy(ppd_filename, filename);
	strcat(ppd_filename, ".pre");
	FILE *file, *ppd_file;

	char *orig, *begin, *end, *text, *data;
	long int str_len;
	
	file = fopen(filename, "r");
	if(!file) {
		printf("Arquivo <%s> nao existe\n", filename);
		return NULL;
	}
	fseek(file, 0L, SEEK_END);
	str_len = ftell(file);
	rewind(file);

	orig = calloc(str_len, sizeof(char));
	char *aux = orig;
	char c;
	while((c = fgetc(file)) != EOF) {
		*aux = toupper(c);
		aux++;
	}
	fclose(file);

	begin = calloc(str_len, sizeof(char));
	end = calloc(4, sizeof(char));
	text = calloc(str_len, sizeof(char));
	data = calloc(str_len, sizeof(char));
	char *buffer = begin;
	char *token;
	aux = orig;

	while((token = strtok_r(aux, "\n", &aux))) {
		if(is_section(token, "TEXT")){
			buffer = text;
		} else if(is_section(token, "DATA")) {
			buffer = data;
		} else if(strcmp(token, "END") == 0) {
			buffer = end;
		}

		while(*token!='\0') {
			*buffer = *token;
			buffer++;
			token++;
		}
		*buffer = '\n';
		buffer++;
	}

	ppd_file = fopen(ppd_filename, "w+");
	fputs(begin, ppd_file);
	fputs(text, ppd_file);
	fputs(data, ppd_file);
	fputs(end, ppd_file);
	fclose(ppd_file);
	free(orig);
	free(begin);
	free(text);
	free(data);
	free(end);
	return ppd_filename;
}

