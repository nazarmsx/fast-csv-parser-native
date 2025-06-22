#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <string>
#include <vector>
#include <map>

class CSVParser {
public:
    CSVParser(char delimiter = ',', bool hasHeader = true, bool skipEmptyLines = true);
    
    // Parse CSV data and return as 2D vector
    std::vector<std::vector<std::string>> parse(const std::string& csvData);
    
    // Parse CSV data and return as array of objects (requires headers)
    std::vector<std::map<std::string, std::string>> parseToObjects(const std::string& csvData);
    
    // Get headers (if hasHeader was true)
    const std::vector<std::string>& getHeaders() const;

private:
    char delimiter_;
    bool hasHeader_;
    bool skipEmptyLines_;
    std::vector<std::string> headers_;
    
    std::vector<std::string> parseLine(const std::string& line);
    std::string trim(const std::string& str);
};

#endif // CSV_PARSER_H