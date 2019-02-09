#include "CSVParser.h"
#include <fstream>
#include <iterator>

bool CSVParser::readRecord(std::vector<std::string> &container) {
    std::string field;

    if (isEndLine()) {
        return false;
    }

    while (true) {
        readField(field);
        container.push_back(field);

        if (isEndLine()) {
            int c = src->get();
            if (c == '\r') {
                c = src->peek();
                if (c == '\n')
                    src->get();
            }
            return true;
        }
    }
}

CSVParser::CSVParser(std::istream &is, char separator, char quote_mark) : src(&is), separator(separator),
                                                                          quote_mark(quote_mark) {}

void CSVParser::readField(std::string &output) {
    output.clear();

    int c = src->peek();

    if (c == quote_mark) {
        output += (char) src->get();
        quoteField(output);
        return;
    }

    while (true) {
        c = src->get();

        if (c == separator) {
            if (isEndLine())
                throw std::invalid_argument("EndLine after separator.");
            return;
        } else if (isEndLine()) {
            output += (char) c;
            return;
        } else if (c == quote_mark) {
            if (src->peek() != quote_mark)
                throw std::invalid_argument("Single quote mark in the field.");
            output += (char) c;
        } else {
            output += (char) c;
        }
    }
}

void CSVParser::quoteField(std::string &field) {
    while (true) {
        int c = src->get();

        if (c == quote_mark) {
            c = src->peek();
            if (c == separator || isEndLine()) {
                field += quote_mark;
                src->get();
                return;
            } else if (c != quote_mark)
                throw std::invalid_argument("Single quote mark int the field");
            field += quote_mark;
            c = src->get();
        }

        field += (char) c;
    }
}

bool CSVParser::isEndLine() {
    int c = src->peek();
    return c == '\n' || c == '\r' || c == std::streambuf::traits_type::eof();
}
