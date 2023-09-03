//#include "formatter_lib.h"

#include <string>
#include <vector>


namespace {

enum Quote { NONE, SINGLE, DOUBLE };

struct Options {
    Options(std::string delimiter, Quote quote, bool sorted, bool trailing_delimiter)
            : delimiter(delimiter), quote(quote), sorted(sorted), 
              trailing_delimiter(trailing_delimiter) {}

    std::string delimiter = ",";
    Quote quote = NONE;
    bool sorted = false;
    bool trailing_delimiter = false;
};

// Given a string s, return a vector<string> of s split by the provided
// delimiter.
std::vector<std::string> Split(
        const std::string& s, const std::string& delimiter,
        bool has_trailing_delimiter = false) {
    std::vector<std::string> result;
    if (delimiter.empty()) {
        for (const char& c : s) {
            result.push_back(std::string(/*size=*/1, c));
        }
        return result;
    }

    size_t left = 0;
    size_t right = s.find(delimiter, left);
    while (right != std::string::npos) {
        result.push_back(s.substr(left, right - left));
        left = right + delimiter.size();
        right = s.find(delimiter, left);
    }
    // If there is a trailing delimiter, ignore the content after the last delimiter.
    if (!has_trailing_delimiter) {
        result.push_back(s.substr(left));
    }
    return result;
}

// Given a vector of strings, combine all strings into a single string,
// separated by the provided delimiter.
std::string Join(const std::vector<std::string>& strings,
                 const std::string& delimiter,
                 bool use_trailing_delimiter = false) {
    if (strings.empty()) {
        return "";
    }
    std::string result = strings[0];
    for (size_t i = 1; i < strings.size(); ++i) {
        result += delimiter;
        result += strings[i];
    }
    if (use_trailing_delimiter) {
        result += delimiter;
    }
    return result;
}

// Remove quotes from a given string.
void RemoveQuotes(std::string& s) {
    if (s[0] == '"' || s[0] == '\'') {
        s.erase(s.begin());
    }
    if (s.back() == '"' || s.back() == '\'') {
        s.erase(s.end() - 1);
    }
}

// Add quotes to a given string, in-place. Double quotes by default.
void AddQuotes(std::string& s, const Quote& quote_type) {
    if (quote_type == Quote::NONE) {
        return;
    }
    char quote = '"';
    if (quote_type == Quote::SINGLE) {
        quote = '\'';
    }
    s.insert(s.begin(), quote);
    s.push_back(quote);
}

Quote TranslateQuoteFromOptions(
        bool add_double_quotes, bool add_single_quotes) {
    if (add_double_quotes) {
        return Quote::DOUBLE;
    } else if (add_single_quotes) {
        return Quote::SINGLE;
    } else {
        return Quote::NONE;
    }
}

std::string TranslateDelimiterFromOptions(
        bool add_commas, bool add_space, bool add_newline) {
    std::string delimiter = "";
    if (add_commas) {
        delimiter += ",";
    }
    if (add_space) {
        delimiter += " ";
    }
    if (add_newline) {
        delimiter += "\n";
    }
    return delimiter;
}

}  // namespace
