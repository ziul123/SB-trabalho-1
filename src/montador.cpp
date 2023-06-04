#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include "files.h"
#include "preprocessor.h"
#include "tables.h"

#define TOKEN_SEP " \t"

bool is_linked(const char *str, char *modulo) {
	char *line = malloc((strlen(str) + 1) * sizeof(char));
	char *aux = line;
	sscanf(str, "%[^n]s", line);
	modulo = strtok_r(aux, " ", &aux);
	char *token = strtok_r(aux, " ", &aux);
	bool result = strcmp(token, "BEGIN") == 0;
	free(line);
	return result;
}

bool is_extern(std::string token) {
	return true;
}

bool is_label(std::string token) {
	return true;
}

out_file_t single_pass(std::string filename) {
	out_file_t out_file;
	symbol_table_t symbol_table;
	def_table_t def_table;
	use_table_t use_table;
	std::vector<int> relative;
	std::vector<int> code;
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
	char *modulo = malloc(str_len * sizeof(char))

	bool link = is_linked(orig, modulo);
	if(link) {
		out_file.type = OBJ;
		modulo[strlen(modulo)-1] = '\0';
		std::string s(modulo);
		symbol_cell_t sc = create_s_cell(0, true, false);
		symbol_table[s] = sc;
		strtok_r(aux, "\n", &aux);

	} else {
		out_file.type = EXC;
	}
	free(modulo);

	char *line;
	int line_count = 1;
	int address = 0;

	while((line = strtok_r(aux, "\n", &aux))) {
		std::string token(strtok_r(line, TOKEN_SEP, &line));

		if(is_extern(token)) {
			if(link) {
				token.assign(strtok_r(line, TOKEN_SEP, &line));
				def_table[token] = -1;
			} else {
				printf("Erro na linha <%s>: EXTERN em arquivo sem BEGIN e END\n", line_count);
			}
		}

		if(is_label(token)) {
			symbol_table
		}
		//diretivas
	}
	free(orig);
	return out_file;
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
