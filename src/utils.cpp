#include <string>
#include <vector>
#include <sstream>

#include "utils.h"

std::string intVectorJoin(const std::vector<int> &elements, const std::string &separator) {
	std::stringstream result;
	for (size_t i = 0; i < elements.size(); ++i) {
		if (i != 0)
			result << separator;
		result << elements[i];
	}
	return result.str();
}

std::string strVectorJoin(const std::vector<std::string> &elements, const std::string &separator) {
	std::string result;
	for (size_t i = 0; i < elements.size(); ++i) {
		if (i != 0)
			result += separator;
		result += elements[i];
	}
	return result;
}