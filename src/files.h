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
		void write_file(std::string filename) override {
			filename = filename + ".obj";
			FILE *file = fopen(filename.c_str(), "w+");
			fputs("USO\n", file);
			for (auto i: this->use_table) {
				std::string s = i.first + " ";
				fputs(s.c_str(), file);
				for (auto a: i.first) {
					fputs(std::to_string(a).c_str(), file);
					fputc(' ', file);
				}
				fputc('\n', file);
			}
			fputs("RELATIVOS\n", file);
			for (auto a: this->relative) {
				fputs(std::to_string(a).c_str(), file);
				fputc(' ', file);
			}
			fputs("\nCODE\n", file);
			for (auto a: this->code) {
				fputs(std::to_string(a).c_str(), file);
				fputc(' ', file);
			}
			fclose(file);
		} 
};

class exc_file_t: public out_file_t {
	public:
		exc_file_t(): out_file_t(){};
		void write_file(std::string filename) override {
			filename = filename + ".exc";
			FILE *file = fopen(filename.c_str(), "w+");
			for (auto x: this->code) {
				fputs(std::to_string(x).c_str(), file);
				fputc(' ', file);
			}
			fclose(file);
		}
};


#endif
