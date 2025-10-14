#pragma once
#include <string>
#include <vector>

namespace CSV {
	std::string escape(const std::string& s);
	std::vector<std::string> splitLine(const std::string& line);
}
