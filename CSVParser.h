#ifndef CSV_PARSER_CSV_PARSER_H
#define CSV_PARSER_CSV_PARSER_H

#include <iostream>
#include <vector>


// Reading characters from input stream and parsing them into CSV format.
// The CSV format is based on RFC 4180(https://tools.ietf.org/html/rfc4180)
// Differences from RFC 4180:
//      - the separator character may be other than comma
//      - quote mark is not necessary double inverse comma
//      - the parser is working with CR, LF, CRLF end of the line characters
//      - the number of field can differ
class CSVParser {
    std::istream *src;
    const char separator;
    const char quote_mark;

    void readField(std::string &output);

    void quoteField(std::string &field);

    bool isEndLine();

public:
    // Reading from the input source and pushing the parsed data into the container
    // Returns false if the following record don't have characters in it.
    // If the fields are wrongly formatted, the function throws back:
    //      std::invalid_argument
    bool readRecord(std::vector<std::string> &container);

    // The class can be initialized with default separator and quote mark
    // but the source has to be created until the call of this constructor.
    // The separator and the quote mark character cannot be change, if the format of csv files
    // differ, need to create different parsers.
    explicit CSVParser(std::istream &is, char separator=',', char quote_mark='"');

    // The setup of the input source is necessary
    CSVParser() = delete;
};


#endif //CSV_PARSER_CSV_PARSER_H
