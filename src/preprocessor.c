#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"

#define TOKEN_SEP " \t"

#define PREP_STRTOK(ORIG, STR, AUX, TOK)\
	char *(STR) = malloc((strlen(line) + 1) * sizeof(char));\
	char *(AUX) = (STR);\
	strcpy((STR), (ORIG));\
	char *(TOK)

int is_section(const char *line, const char *section) {
	PREP_STRTOK(line, str, aux, token);
	int result = 0;
	token = strtok_r(aux, TOKEN_SEP, &aux);
	result |= strcmp(token, "SECTION");
	if((token = strtok_r(aux, TOKEN_SEP, &aux)))
		result |= strcmp(token, section);
	free(str);
	return result == 0;
}


int is_empty(const char *line) {
	PREP_STRTOK(line, str, aux, token);
	token = strtok_r(aux, TOKEN_SEP, &aux);
	int result = token == NULL;
	free(str);
	return result;
}

int is_end(const char *line) {
	PREP_STRTOK(line, str, aux, token);
	token = strtok_r(aux, TOKEN_SEP, &aux);
	int result = strcmp(token, "END");
	free(str);
	return result == 0;
}


int is_label(const char *line) {
	PREP_STRTOK(line, str, aux, token);
	token = strtok_r(aux, TOKEN_SEP, &aux);
	int len = strlen(token);
	int result = token[len-1] == ':';
	token = strtok_r(aux, TOKEN_SEP, &aux);
	result &= token == NULL;
	free(str);
	return result;
}


void strip_comment(char *line) {
	int len = strlen(line);
	for(int i = 0; i < len; i++) {
		if(line[i]==';') {
			line[i] = '\0';
			break;
		}
	}
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
	//pegar tamanho do arquivo
	fseek(file, 0L, SEEK_END);
	str_len = ftell(file);
	rewind(file);

	//ler o arquivo e tranformar para maiuscula
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
	char *line;
	aux = orig;

	while((line = strtok_r(aux, "\n", &aux))) {
		strip_comment(line);
		if(is_empty(line))
			continue;	//ignorar linhas vazias

		if(is_section(line, "TEXT")){
			buffer = text;
		} else if(is_section(line, "DATA")) {
			buffer = data;
		} else if(is_end(line)) {
			buffer = end;
		}

		char sep = '\n';
		if(is_label(line))
			sep = ' ';

		while(*line!='\0') {
			*buffer = *line;
			buffer++;
			line++;
		}
		*buffer = sep;
		buffer++;
	}

	if(strlen(text) == 0) {
		printf("Erro! Faltando seção TEXT");
		exit(1);
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


int main(int argc, char **argv) {
	char * ppd_filename = preprocess_file(argv[1]);
	printf("preprocessed file written to %s\n", ppd_filename);
	return 0;
}
