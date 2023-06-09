#ifndef FILES_H
#define FILES_H

#include <cstdio>
#include "tables.h"

class out_file_t {
	public:
		std::vector<int> code;
		out_file_t() {}
		virtual void write_file(std::string filename) = 0;
};

class obj_file_t: public out_file_t {
	public:
		use_table_t use_table;
		def_table_t def_table;
		std::vector<int> relative;
		obj_file_t(): out_file_t() {}
		void write_file(std::string filename) override; 
};

class exc_file_t: public out_file_t {
	public:
		exc_file_t(): out_file_t(){};
		void write_file(std::string filename) override; 
};


#endif
