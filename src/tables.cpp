#include <vector>
#include "tables.h"

symbol_cell_t create_s_cell(int addr, bool def, bool external) {
	symbol_cell_t s_cell;
	std::vector<int> dependencies;
	s_cell.address = addr;
	s_cell.defined = def;
	s_cell.external = external;
	s_cell.dependencies = dependencies;
	return s_cell;
}
