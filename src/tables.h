#ifndef TABLES_H
#define TABLES_H
#include <vector>
#include <map>
#include <string>

typedef struct symbol_cell {
	int address;
	bool defined;
	bool external;
	std::vector<int> dependencies;
} symbol_cell_t;


typedef std::map<std::string, symbol_cell_t> symbol_table_t;

typedef std::map<std::string, int> def_table_t;

typedef std::map<std::string, std::vector<int>> use_table_t;

symbol_cell_t create_s_cell(int addr, bool def, bool external);

#endif
