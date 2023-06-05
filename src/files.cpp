#include <sstream>

#include "files.h"
#include "utils.h"

void obj_file_t::write_file(std::string filename) {
  std::stringstream result;
  
  result << "USO\n";
  for (const auto &use : use_table)
      result << use.first << " " << intVectorJoin(use.second, " ") << '\n';
  result << "DEF\n";
  for (const auto &def : def_table)
      result << def.first << " " << def.second << '\n';
  result << "RELATIVOS\n" << intVectorJoin(relative, " ") << '\n';
  result << "CODE\n" << intVectorJoin(code, " ");
  
  FILE *file;
  file = fopen(filename.c_str(), "w+");
  fputs(result.str().c_str(), file);
  fclose(file);
} 

void exc_file_t::write_file(std::string filename) {
  FILE *file;
  file = fopen(filename.c_str(), "w+");
  for (const int &byte : code)
    fprintf(file, "%d ", byte);
  fclose(file);
}
