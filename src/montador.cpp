#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <regex>
#include "files.h"
#include "preprocessor.h"
#include "tables.h"
#include "utils.h"

#define TOKEN_SEP " \t"

#define LABEL_MESMA_LINHA(TOKEN, LINE)\
	if (is_label(TOKEN))\
		printf("Erro na linha<%d>: duas labels na mesma linha\n", LINE)

#define LABEL_VALIDA(LABEL, LINE)\
	if (!valid_label(LABEL))\
		printf("Erro na linha <%d>: label inválida\n", LINE)

#define NEXT_TOKEN\
	token_aux = strtok_r(line, TOKEN_SEP, &line);\
	if (token_aux)\
		token.assign(token_aux);\
	else\
		token = ""

#define UNARY_INST(INST)\
	code.push_back(INST);\
	address++;\
	NEXT_TOKEN;\
	LABEL_MESMA_LINHA(token, line_count);\
	LABEL_VALIDA(token, line_count);\
	add_section(sections, token, INST);\
	if (linhas.find(token) == linhas.end())\
		linhas[token] = line_count;\
	int addr = search_symbol_table(symbol_table, token, address);\
	if (addr == -1 && link) {\
		add_use(use_table, token, address);\
		addr = 0;\
	}\
	NEXT_TOKEN;\
	LABEL_MESMA_LINHA(token, line_count);\
	if (token == "+") {\
		NEXT_TOKEN;\
		LABEL_MESMA_LINHA(token, line_count);\
		addr += std::stoi(token, nullptr, 0);\
	}\
	relative.push_back(address++);\
	code.push_back(addr)

#define BINARY_INST(INST)\
	UNARY_INST(INST);\
	if (!(token == ","))\
		NEXT_TOKEN;\
	LABEL_MESMA_LINHA(token, line_count);\
	NEXT_TOKEN;\
	LABEL_MESMA_LINHA(token, line_count);\
	LABEL_VALIDA(token, line_count);\
	add_section(sections, token, INST);\
	if (linhas.find(token) == linhas.end())\
		linhas[token] = line_count;\
	addr = search_symbol_table(symbol_table, token, address);\
	if (addr == -1 && link){\
		add_use(use_table, token, address);\
		addr = 0;\
	}\
	NEXT_TOKEN;\
	LABEL_MESMA_LINHA(token, line_count);\
	if (token == "+") {\
		NEXT_TOKEN;\
		LABEL_MESMA_LINHA(token, line_count);\
		addr += std::stoi(token, nullptr, 0);\
	}\
	relative.push_back(address++);\
	code.push_back(addr)


bool is_linked(const char *str, char *modulo) {
	char *line = (char*) malloc((strlen(str) + 1) * sizeof(char));
	char *aux = line;
	sscanf(str, "%[^n]s", line);
	strcpy(modulo, strtok_r(aux, TOKEN_SEP, &aux));
	char *token = strtok_r(aux, " \t\n", &aux);
	bool result = strcmp(token, "BEGIN") == 0;
	free(line);
	return result;
}

bool is_label(const std::string token) {
	if (token == "")
		return false;
	return token[token.length()-1] == ':';
}

bool valid_label(const std::string label) {
	std::regex label_exp("[a-zA-Z_][a-zA-Z0-9_]*");
	return regex_match(label, label_exp);
}

void add_label(symbol_table_t &st, std::string label, int addr, bool defined, bool external) {
	symbol_cell_t sc = create_s_cell(addr, defined, external);
	st[label] = sc;
	return;
}

void solve_dependencies(symbol_table_t &st, std::string label, int addr, std::vector<int> &code) {
	st[label].address = addr;
	st[label].defined = true;
	for (auto &address: st[label].dependencies) {
		code[address] += addr;
	}
	st[label].dependencies.clear();
	return;
}

int search_symbol_table(symbol_table_t &st, const std::string label, int addr) {
	if (st.find(label) == st.end())
		add_label(st, label, -1, false, false);
	if (st[label].defined) {
		if(st[label].external)
			return -1;
		return st[label].address;
	} else {
		st[label].dependencies.push_back(addr);
		return 0;
	}
}

void add_use(use_table_t &ut, std::string label, int addr) {
	if (ut.find(label) == ut.end()) {
		std::vector<int> uses;
		uses.push_back(addr);
		ut[label] = uses;
	} else {
		ut[label].push_back(addr);
	}
}

void add_section(std::map<std::string, std::string> &sections, std::string label, int inst) {
	if (sections.find(label) == sections.end()) {
		switch (inst) {
			case 5:
			case 6:
			case 7:
			case 8:
				sections[label] = "TEXT";
				break;
			default:
				sections[label] = "DATA";
		}
	}
}

out_file_t* single_pass(std::string filename) {
	//tabelas necessarias
	symbol_table_t symbol_table;
	def_table_t def_table;
	use_table_t use_table;
	std::vector<int> relative;
	std::vector<int> code;
	//tabelas para erros
	std::map<std::string, int> linhas;	//label->1a linha que foi usada
	std::map<std::string, std::string> sections;	//label->section
	long int str_len;
	char *orig;
	FILE *file = fopen(filename.c_str(), "r");

	fseek(file, 0L, SEEK_END);
	str_len = ftell(file);
	rewind(file);

	orig = (char *) malloc(str_len * sizeof(char));
	char *orig_aux = orig;
	char c;
	while ((c = fgetc(file)) != EOF) {
		*orig_aux = c;
		orig_aux++;
	}
	fclose(file);

	char *aux = orig;
	char *modulo = (char *) malloc(str_len * sizeof(char));

	bool link = is_linked(orig, modulo);
	int line_count = 1;
	if(link) {
		modulo[strlen(modulo)-1] = '\0';
		std::string s(modulo);
		add_label(symbol_table, s, 0, true, false);
		strtok_r(aux, "\n", &aux);
		line_count++;
	}
	free(modulo);

	char *line;
	int address = 0;

	while((line = strtok_r(aux, "\n", &aux))) {
		char *token_aux;
		std::string token(strtok_r(line, TOKEN_SEP, &line));

		if(token.substr(0, token.length()-1) == "EXTERN") {
			if(link) {
				NEXT_TOKEN;
				add_label(symbol_table, token, 0, true, true);
			} else {
				printf("Error at line <%d>: EXTERN in file without BEGIN and END\n", line_count);
			}
			line_count++;
			continue;
		}

		if(is_label(token)) {
			std::string label = token.substr(0, token.length()-1);
			if(valid_label(label)) {
				if(symbol_table.find(label) == symbol_table.end()) {
					add_label(symbol_table, label, address, true, false);
				} else {
					solve_dependencies(symbol_table, label, address, code);
				}
			} else {
				printf("Erro na linha <%d>: label inválida\n", line_count);
			}
			NEXT_TOKEN;
		}
		LABEL_MESMA_LINHA(token, line_count);

		//diretivas
		if (token == "PUBLIC") {
			if (!link) {
				printf("Erro na linha <%d>: PUBLIC em arquivo sem BEGIN e END\n", line_count);
				continue;
			}
			NEXT_TOKEN;
			LABEL_MESMA_LINHA(token, line_count);
			LABEL_VALIDA(token, line_count);
			def_table[token] = -1;
		} else if (token == "SPACE") {
			NEXT_TOKEN;
			LABEL_MESMA_LINHA(token, line_count);
			if (token != "") {
				int space = std::stoi(token);
				for (int i=0;i<space;i++) {
					code.push_back(0);
					address++;
				}
			} else {
				code.push_back(0);
				address++;
			}
		} else if (token == "CONST") {
			NEXT_TOKEN;
			LABEL_MESMA_LINHA(token, line_count);
			int constant = std::stoi(token, nullptr, 0);
			code.push_back(constant);
			address++;
		//instruções
		} else if (token == "ADD") {
			UNARY_INST(1);
		} else if (token == "SUB") {
			UNARY_INST(2);
		} else if (token == "MUL") {
			UNARY_INST(3);
		} else if (token == "DIV") {
			UNARY_INST(4);
		} else if (token == "JMP") {
			UNARY_INST(5);
		} else if (token == "JMPN") {
			UNARY_INST(6);
		} else if (token == "JMPP") {
			UNARY_INST(7);
		} else if (token == "JMPZ") {
			UNARY_INST(8);
		} else if (token == "COPY") {
			BINARY_INST(9);
		} else if (token == "LOAD") {
			UNARY_INST(10);
		} else if (token == "STORE") {
			UNARY_INST(11);
		} else if (token == "INPUT") {
			UNARY_INST(12);
		} else if (token == "OUTPUT") {
			UNARY_INST(13);
		} else if (token == "STOP") {
			code.push_back(14);
			address++;
		}
		line_count++;
	}
	free(orig);
	
	for (auto i: symbol_table) {
		if (!(i.second).dependencies.empty())
			printf("Erro na linha <%d>: label '%s' não definida na seção %s\n", linhas[i.first], i.first.c_str(), sections[i.first].c_str());
	}

	if (link) {
		for (auto i: def_table) {
			def_table[i.first] = symbol_table[i.first].address;
		}
		obj_file_t* obj_file = new obj_file_t();
		obj_file->type = OBJ;
		obj_file->use_table = use_table;
		obj_file->def_table = def_table;
		obj_file->relative = relative;
		obj_file->code = code;
		return obj_file;
	} else {
		exc_file_t* exc_file = new exc_file_t();
		exc_file->type = EXC;
		exc_file->code = code;
		return exc_file;
	}
}

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Please, input the files you want to assemble when calling the program.");
		return 0;
	}

	std::vector<std::string> filenames(argc-1);
	for (int i = 1; i < argc; i++) {
		std::string filename(argv[i]);
		std::string asm_filename = filename + ".asm";
		std::string ppd_filename = preprocess_file(asm_filename);
		out_file_t* out_file = single_pass(ppd_filename);
		if (out_file->type == EXC)
			filename = filename + ".exc";
		else if (out_file->type == OBJ)
			filename = filename + ".obj";
		out_file->write_file(filename);
		free(out_file);
		filenames[i-1] = filename;
	}
	printf("Assembled given files into %s\n", strVectorJoin(filenames, ", ").c_str());
	
	return 0;
}
