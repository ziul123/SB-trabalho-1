#ifndef FILES_H
#define FILES_H

#include "tables.h"

class out_file_t {
	public:
		std::vector<int> code;
		out_file_t() { /*this->code = std::vector<int>;*/ }
		virtual void write_file(std::string filename) = 0;
};

class obj_file_t: public out_file_t {
	public:
		use_table_t use_table;
		def_table_t def_table;
		std::vector<int> relative;
		obj_file_t(): out_file_t() {
			/*
			this->use_table = std::map<std::string, std::vector<int>>;
			this->def_table = std::map<std::string, int>;
			this->relative = std::vector<int>;
			*/
		}
		void write_file(std::string filename) override;
};

class exc_file_t: public out_file_t {
	public:
		exc_file_t(): out_file_t() { };
		void write_file(std::string filename) override;
};


#endif
