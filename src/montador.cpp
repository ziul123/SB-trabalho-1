#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include "files.h"
#include "preprocessor.h"
#include "tables.h"
#include "utils.h"

#define TOKEN_SEP " \t"

bool is_linked(const char *str, char *modulo) {
	char *line = (char*) malloc((strlen(str) + 1) * sizeof(char));
	char *aux = line;
	sscanf(str, "%[^n]s", line);
	strcpy(modulo, strtok_r(aux, " ", &aux));
	char *token = strtok_r(aux, " ", &aux);
	bool result = strcmp(token, "BEGIN") == 0;
	free(line);
	return result;
}

bool is_extern(const std::string token) {
	if (token.substr(0, token.length()-1) == "EXTERN") 
		return true;
	else
		return false;
}

bool is_label(std::string token) {
	return true;
}

out_file_t* single_pass(std::string filename) {
	out_file_t* outfile;
	symbol_table_t symbol_table;
	def_table_t def_table;
	use_table_t use_table;
	std::vector<int> relative;
	std::vector<int> code;
	long int str_len;
	char *orig;
	FILE *file = fopen(filename.c_str(), "r");

	fseek(file, 0L, SEEK_END);
	str_len = ftell(file);
	rewind(file);

	orig = (char *) calloc(str_len, sizeof(char));
	fgets(orig, str_len, file);
	fclose(file);

	char *aux = orig;
	char *modulo = (char *) malloc(str_len * sizeof(char));

	bool link = is_linked(orig, modulo);
	if(link) {
		modulo[strlen(modulo)-1] = '\0';
		std::string s(modulo);
		symbol_cell_t sc = create_s_cell(0, true, false);
		symbol_table[s] = sc;
		strtok_r(aux, "\n", &aux);
		*outfile = obj_file_t();
	} else {
		*outfile = exc_file_t();
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
				printf("Erro na linha <%d>: EXTERN em arquivo sem BEGIN e END\n", line_count);
			}
		}

		if(is_label(token)) {
			//symbol_table
		}
		//diretivas
	}
	free(orig);
	return outfile;
}

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Please, input the files you want to pre-process when calling the program.");
		return 0;
	}

	std::vector<std::string> filenames(argc-1);
	for (int i = 1; i < argc; i++) {
		std::string filename(argv[i]);
		std::string asm_filename = filename + ".asm";
		std::string ppd_filename = preprocess_file(asm_filename);
		out_file_t* out_file = single_pass(ppd_filename);
		out_file->write_file(filename);
		filenames[i-1] = filename;
	}
	printf("Assembled given files into %s\n", strVectorJoin(filenames, ", ").c_str());
	
	return 0;
}
