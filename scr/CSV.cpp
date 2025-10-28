#include "CSV.hpp"
#include <string>
#include <vector>


namespace CSV {
    std::string escape(const std::string& s) {
        bool needQuotes = false;
        std::string out;
        out.reserve(s.size() + 4);
        for (char c : s) {
            if (c == '"' || c == ',' || c == '\n' || c == '\r') needQuotes = true;
            if (c == '"') out += "\"\"";
            else out += c;
        }
        if (needQuotes) return "\"" + out + "\"";
        return out;
    }

    std::vector<std::string> splitLine(const std::string& line) {
        std::vector<std::string> fields;
        std::string cur;
        bool inQuotes = false;
        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (inQuotes) {
                if (c == '"') {
                    if (i + 1 < line.size() && line[i + 1] == '"') {
                        cur.push_back('"');
                        ++i;
                    } else {
                        inQuotes = false;
                    }
                } else {
                    cur.push_back(c);
                }
            } else {
                if (c == '"') {
                    inQuotes = true;
                } else if (c == ',') {
                    fields.push_back(cur);
                    cur.clear();
                } else {
                    cur.push_back(c);
                }
            }
        }
        fields.push_back(cur);
        return fields;
    }
}
