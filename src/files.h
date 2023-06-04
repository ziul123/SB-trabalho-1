#ifndef FILES_H
#define FILES_H
#include "tables.h"

typedef enum filetype {OBJ, EXC} filetype;

typedef struct obj_file_t {
	use_table_t use_table;
	def_table_t def_table;
	std::vector<int> relative;
	std::vector<int> code;
} obj_file_t;

typedef std::vector<int> exc_file_t;

typedef struct out_file_t {
	filetype type;
	union {
		obj_file_t obj_file;
		exc_file_t exc_file;
	};
} out_file_t;

#endif
