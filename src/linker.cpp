#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include "files.h"
#include "preprocessor.h"
#include "tables.h"
#include "utils.h"

obj_file_t read_obj(std::string filename) {
	obj_file_t obj_file;
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

			std::vector<int> *uses_vector;
			auto it = general_use_table.find(key);
			if (it == general_use_table.end()) {
				uses_vector = new std::vector<int>();
				general_use_table[key] = *uses_vector;
			} else {
				*uses_vector = general_use_table[key]; 
			}

			for (const &addr : key_value.second)
				uses_vector->push_back(addr + correction);
		}

		for (const &byte : obj_file.code)
			merged_code.push_back(byte);

		for (const &addr : obj_file.relative)
			relative_solver[addr + correction] = correction;

		correction += obj_file.code.size();
	}
	
	for (const auto &uses : general_use_table) {
		int label_addr = general_def_table[uses.first];

		for (const &use : uses.second)
			relative_solver[use] = label_addr;

		delete &uses.second;
	}

	for (const auto &relative : relative_solver)
		merged_code[relative.first] += relative.second;

	return 0;
}
