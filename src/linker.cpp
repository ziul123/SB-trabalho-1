#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include "files.h"
#include "tables.h"

obj_file_t read_obj(std::string str_filename) {
	obj_file_t obj_file;

	const char *filename = str_filename.c_str();
	FILE *file = fopen(filename, "r");
	if (!file) {
		printf("File <%s> doesn't exist.\n", filename);
		throw;
	}
	
	fseek(file, 0L, SEEK_END);
	size_t str_len = ftell(file);
	rewind(file);

	char orig[str_len + 1], *aux, *line, *token;

	size_t bytes_read = fread(orig, sizeof(char), str_len, file);
	if (bytes_read != str_len) {
		printf("Error reading the file: <%s>.\n", filename);
		fclose(file);
		throw;
	}

	fclose(file);
	orig[str_len] = '\0';

	aux = orig;
	int mode = -1;
	while ((line = strtok_r(aux, "\n", &aux))) {
		std::string line_str(line); 
		if(line_str == "USO") {
			mode = 0;
		} else if(line_str == "DEF") {
			mode = 1;
		} else if(line_str == "RELATIVOS") {
			mode = 2;
		} else if(line_str == "CODE") {
			mode = 3;
		} else if (mode == 0) {
			use_table_t *uses = &obj_file.use_table;
			char *key = strtok_r(line, " ", &line);
			auto it = uses->find(key);
			if (it == uses->end()) {
				(*uses)[key] = std::vector<int>();
			}
			while ((token = strtok_r(line, " ", &line)))
				(*uses)[key].push_back(atoi(token));
		} else if (mode == 1) {
			char *key = strtok_r(line, " ", &line);
			char *value = strtok_r(line, " ", &line);
			obj_file.def_table[key] = atoi(value);
		} else if (mode == 2) {
			while ((token = strtok_r(line, " ", &line)))
				obj_file.relative.push_back(atoi(token));
		} else if (mode == 3) {
			while ((token = strtok_r(line, " ", &line)))
				obj_file.code.push_back(atoi(token));
		}
	}
	
	return obj_file;
}

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Please, input the files you want to link when calling the program.");
		return 0;
	}

	def_table_t general_def_table;
	use_table_t general_use_table;
	std::map<int, int> relative_solver;
	exc_file_t merged_code;

	int correction = 0;
	for (int i = 1; i < argc; i++) {
		std::string filename(argv[i]);
		std::string obj_filename = filename + ".obj";
		obj_file_t obj_file = read_obj(obj_filename);

		for (const auto &key_value : obj_file.def_table)
			general_def_table[key_value.first] = key_value.second + correction;

		for (const auto &key_value : obj_file.use_table) {
			std::string key = key_value.first;

			auto it = general_use_table.find(key);
			if (it == general_use_table.end()) {
				general_use_table[key] = std::vector<int>();
			}

			for (const int &addr : key_value.second)
				general_use_table[key].push_back(addr + correction);
		}

		for (const int &byte : obj_file.code)
			merged_code.code.push_back(byte);

		for (const int &addr : obj_file.relative)
			relative_solver[addr + correction] = correction;

		correction += obj_file.code.size();
	}
	
	for (const auto &uses : general_use_table) {
		int label_addr = general_def_table[uses.first];

		for (const int &use : uses.second)
			relative_solver[use] = label_addr;
	}

	for (const auto &relative : relative_solver)
		merged_code.code[relative.first] += relative.second;

	std::string filename(argv[1]);
	std::string exc_filename = filename + ".exc";
	merged_code.write_file(exc_filename);

	printf("Linked given files into %s\n", exc_filename.c_str());

	return 0;
}
