#include "csv_parser.h"
#include <sstream>
#include <algorithm>

CSVParser::CSVParser(char delimiter, bool hasHeader, bool skipEmptyLines) 
    : delimiter_(delimiter), hasHeader_(hasHeader), skipEmptyLines_(skipEmptyLines) {}

std::vector<std::vector<std::string>> CSVParser::parse(const std::string& csvData) {
    std::vector<std::vector<std::string>> result;
    std::istringstream stream(csvData);
    std::string line;
    bool firstLine = true;
    
    while (std::getline(stream, line)) {
        // Skip empty lines if requested
        if (skipEmptyLines_ && line.empty()) {
            continue;
        }
        
        // Trim whitespace from line
        line = trim(line);
        
        std::vector<std::string> row = parseLine(line);
        
        // Store header if this is the first line and hasHeader is true
        if (firstLine && hasHeader_) {
            headers_ = row;
            firstLine = false;
            continue;
        }
        
        result.push_back(row);
        firstLine = false;
    }
    
    return result;
}

std::vector<std::string> CSVParser::parseLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;
    bool escapeNext = false;
    
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        
        if (escapeNext) {
            field += c;
            escapeNext = false;
            continue;
        }
        
        if (c == '\\') {
            escapeNext = true;
            continue;
        }
        
        if (c == '"') {
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                // Handle escaped quotes within quoted field
                field += '"';
                ++i; // Skip the next quote
            } else {
                inQuotes = !inQuotes;
            }
            continue;
        }
        
        if (c == delimiter_ && !inQuotes) {
            // End of field
            fields.push_back(trim(field));
            field.clear();
            continue;
        }
        
        field += c;
    }
    
    // Add the last field
    fields.push_back(trim(field));
    
    return fields;
}

std::string CSVParser::trim(const std::string& str) {
    const std::string whitespace = " \t\r\n";
    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

std::vector<std::map<std::string, std::string>> CSVParser::parseToObjects(const std::string& csvData) {
    std::vector<std::map<std::string, std::string>> result;
    
    if (!hasHeader_) {
        return result; // Cannot create objects without headers
    }
    
    auto rows = parse(csvData);
    
    for (const auto& row : rows) {
        std::map<std::string, std::string> obj;
        
        for (size_t i = 0; i < std::min(headers_.size(), row.size()); ++i) {
            obj[headers_[i]] = row[i];
        }
        
        result.push_back(obj);
    }
    
    return result;
}

const std::vector<std::string>& CSVParser::getHeaders() const {
    return headers_;
}