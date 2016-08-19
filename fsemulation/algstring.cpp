#include "algstring.h"

#include <sstream>
#include <cctype>

bool iequals(const std::string& a, const std::string& b)
{
	unsigned int sz = a.size();
	if (b.size() != sz) {
		return false;
	}
	for (unsigned int i = 0; i < sz; ++i) {
		if (tolower(a[i]) != tolower(b[i])) {
			return false;
		}
	}
	return true;
}

void split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}
