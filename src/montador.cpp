#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include "files.h"
#include "preprocessor.h"
#include "tables.h"

#define TOKEN_SEP " \t"

bool is_linked(const char *str) {
	char *line = malloc((strlen(str) + 1) * sizeof(char));
	char *aux = line;
	sscanf(str, "%[^n]s", line);
	char *token = strtok_r(aux, " ", &aux);
	bool result = strcmp(token, "BEGIN") == 0;
	free(line);
	return result;
}

out_file_t single_pass(std::string filename) {
	out_file_t out_file;
	symbol_table_t symbol_table;
	def_table_t def_table;
	use_table_t use_table;
	long int str_len;
	char *orig;
	FILE *file = fopen(filename, "r");

	fseek(file, 0L, SEEK_END);
	str_len = ftell(file);
	rewind(file);

	orig = (char *) calloc(str_len, sizeof(char));
	fgets(orig, str_len, file);
	fclose(file);

	char *aux = orig;

	bool link = is_linked(orig);
	if(link) {
		out_file.type = OBJ;
		strtok_r(aux, "\n", &aux);
	} else {
		out_file.type = EXC;
	}

	char *line;
	int line_count = 1;

	while((line = strtok_r(aux, "\n", &aux))) {
		std::string token(strtok_r(line, TOKEN_SEP, &line));
		//diretivas
		if(token == "EXTERN") {
			if(link) {
				//strtok_r(line, TOKEN_SEP, &line);
			} else {
				printf("Erro na linha <%s>: EXTERN em arquivo sem BEGIN e END\n", line_count);
			}
	}
	free(orig);
}

void write_file(std::string filename, out_file_t out_file) {

}

int main(int argc, char **argv) {
	std::string filename(argv[1]);
	std::string asm_filename = filename + ".asm";
	std::string ppd_filename = preprocess_file(asm_filename);
	out_file_t out_file = single_pass(ppd_filename);
	write_file(filename, out_file);

	return 0;
}
