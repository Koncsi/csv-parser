#include "CSVParser.h"
#include <gtest/gtest.h>

class BasicTest : public ::testing::Test {
protected:

    void SetUp() override {
        istringstream1 = new std::istringstream(input1);
        istringstream2 = new std::istringstream(input2);
        istringstream3 = new std::istringstream(input3);
        istringstream4 = new std::istringstream(input4);

        parser1 = new CSVParser(*istringstream1);
        parser2 = new CSVParser(*istringstream2);
        parser3 = new CSVParser(*istringstream3);
        parser4 = new CSVParser(*istringstream4);
    }

    void TearDown() override {
        delete parser1;
        delete parser2;
        delete parser3;
        delete parser4;

        delete istringstream1;
        delete istringstream2;
        delete istringstream3;
        delete istringstream4;
    }

    std::string input1 = "aaa,bbb,ccc\n";
    std::string input2 = "\"aaa\",\"bbb\",\"ccc\"\n";
    std::string input3 = "\"aaa\",bbb,\"ccc\"\n";
    std::string input4 = "aaa,bbb,ccc\nddd,eee,fff\n";

    std::istringstream *istringstream1;
    std::istringstream *istringstream2;
    std::istringstream *istringstream3;
    std::istringstream *istringstream4;

    CSVParser *parser1;
    CSVParser *parser2;
    CSVParser *parser3;
    CSVParser *parser4;
};

TEST_F(BasicTest, WithoutQuote) {
    std::vector<std::string> result;
    parser1->readRecord(result);

    ASSERT_STREQ("aaa", result[0].c_str());
    ASSERT_STREQ("bbb", result[1].c_str());
    ASSERT_STREQ("ccc", result[2].c_str());
}

TEST_F(BasicTest, WithQuote) {
    std::vector<std::string> result;
    parser2->readRecord(result);

    ASSERT_STREQ("\"aaa\"", result[0].c_str());
    ASSERT_STREQ("\"bbb\"", result[1].c_str());
    ASSERT_STREQ("\"ccc\"", result[2].c_str());
}

TEST_F(BasicTest, WithAndWithoutQuote) {
    std::vector<std::string> result;
    parser3->readRecord(result);

    ASSERT_STREQ("\"aaa\"", result[0].c_str());
    ASSERT_STREQ("bbb", result[1].c_str());
    ASSERT_STREQ("\"ccc\"", result[2].c_str());
}

TEST_F(BasicTest, MultipleRows) {
    std::vector<std::string> result1;
    std::vector<std::string> result2;
    parser4->readRecord(result1);
    parser4->readRecord(result2);

    ASSERT_STREQ("aaa", result1[0].c_str());
    ASSERT_STREQ("bbb", result1[1].c_str());
    ASSERT_STREQ("ccc", result1[2].c_str());

    ASSERT_STREQ("ddd", result2[0].c_str());
    ASSERT_STREQ("eee", result2[1].c_str());
    ASSERT_STREQ("fff", result2[2].c_str());
}

TEST(EndlineInField, ClosedInQuotes) {
    std::string input = "aaa,\"bb\nbb\",ccc\n";
    std::istringstream istringstream(input);
    CSVParser parser = CSVParser(istringstream);
    std::vector<std::string> result;
    parser.readRecord(result);

    ASSERT_STREQ("aaa", result[0].c_str());
    ASSERT_STREQ("\"bb\nbb\"", result[1].c_str());
    ASSERT_STREQ("ccc", result[2].c_str());
}

TEST(EndlineInField, NotClosedInQuotes) {
    std::string input = "aaa,bb\nbb,ccc\n";
    std::istringstream istringstream(input);
    CSVParser parser = CSVParser(istringstream);
    std::vector<std::string> result;
    parser.readRecord(result);

    ASSERT_STREQ("aaa", result[0].c_str());
    ASSERT_STREQ("bb", result[1].c_str());
}

TEST(QuoteInField, NotClosedInQuotes) {
    std::string input = "aaa,b\"bb,ccc\n";
    std::istringstream istringstream(input);
    CSVParser parser = CSVParser(istringstream);
    std::vector<std::string> result;

    ASSERT_THROW(parser.readRecord(result), std::invalid_argument);
}

TEST(QuoteInField, SingleClosedInQuotes) {
    std::string input = "aaa,\"bb\"b\",ccc\n";
    std::istringstream istringstream(input);
    CSVParser parser = CSVParser(istringstream);
    std::vector<std::string> result;

    ASSERT_THROW(parser.readRecord(result), std::invalid_argument);
}

TEST(QuoteInField, DoubleClosedInQuotes) {
    std::string input = "aaa,\"bb\"\"bb\",ccc\n";
    std::istringstream istringstream(input);
    CSVParser parser = CSVParser(istringstream);
    std::vector<std::string> result;
    parser.readRecord(result);

    ASSERT_STREQ("aaa", result[0].c_str());
    ASSERT_STREQ("\"bb\"\"bb\"", result[1].c_str());
    ASSERT_STREQ("ccc", result[2].c_str());
}

TEST(EndlineTests, NoEndlineChar) {
    std::string input = "aaa,bbb,ccc";
    std::istringstream istringstream(input);
    CSVParser parser = CSVParser(istringstream);
    std::vector<std::string> result;
    parser.readRecord(result);

    ASSERT_STREQ("aaa", result[0].c_str());
    ASSERT_STREQ("bbb", result[1].c_str());
    ASSERT_STREQ("ccc", result[2].c_str());
}

TEST(EndlineTests, EndLineCR) {
    std::string input = "aaa,bbb,ccc\rddd,eee,fff\r";
    std::istringstream istringstream(input);
    CSVParser parser = CSVParser(istringstream);
    std::vector<std::string> result;
    parser.readRecord(result);
    result.clear();
    parser.readRecord(result);

    ASSERT_STREQ("ddd", result[0].c_str());
    ASSERT_STREQ("eee", result[1].c_str());
    ASSERT_STREQ("fff", result[2].c_str());
}

TEST(EndlineTests, EndLineLF) {
    std::string input = "aaa,bbb,ccc\nddd,eee,fff\n";
    std::istringstream istringstream(input);
    CSVParser parser = CSVParser(istringstream);
    std::vector<std::string> result;
    parser.readRecord(result);
    result.clear();
    parser.readRecord(result);

    ASSERT_STREQ("ddd", result[0].c_str());
    ASSERT_STREQ("eee", result[1].c_str());
    ASSERT_STREQ("fff", result[2].c_str());
}

TEST(EndlineTests, EndLineCRLF) {
    std::string input = "aaa,bbb,ccc\r\nddd,eee,fff\r\n";
    std::istringstream istringstream(input);
    CSVParser parser = CSVParser(istringstream);
    std::vector<std::string> result;
    parser.readRecord(result);
    result.clear();
    parser.readRecord(result);

    ASSERT_STREQ("ddd", result[0].c_str());
    ASSERT_STREQ("eee", result[1].c_str());
    ASSERT_STREQ("fff", result[2].c_str());
}

TEST(SeparatorTests, SepNotComma) {
    std::string input = "aaa-b,bb-ccc\n";
    std::istringstream istringstream(input);
    CSVParser parser = CSVParser(istringstream, '-', '"');
    std::vector<std::string> result;
    parser.readRecord(result);

    ASSERT_STREQ("aaa", result[0].c_str());
    ASSERT_STREQ("b,bb", result[1].c_str());
    ASSERT_STREQ("ccc", result[2].c_str());
}

TEST(SeparatorTests, SimpleQuote) {
    std::string input = "'aaa'-'b\nbb'-'ccc'\n";
    std::istringstream istringstream(input);
    CSVParser parser = CSVParser(istringstream, '-', '\'');
    std::vector<std::string> result;
    parser.readRecord(result);

    ASSERT_STREQ("'aaa'", result[0].c_str());
    ASSERT_STREQ("'b\nbb'", result[1].c_str());
    ASSERT_STREQ("'ccc'", result[2].c_str());
}

TEST(EmptyInput, EmptyRecord) {
    std::string input = "";
    std::istringstream istringstream(input);
    CSVParser parser = CSVParser(istringstream);
    std::vector<std::string> result;
    bool gotRecord = parser.readRecord(result);

    ASSERT_EQ(false, gotRecord);
}

TEST(EmptyInput, EmptyField) {
    std::string input = "aaa,,ccc\n";
    std::istringstream istringstream(input);
    CSVParser parser = CSVParser(istringstream);
    std::vector<std::string> result;
    parser.readRecord(result);

    ASSERT_STREQ("aaa", result[0].c_str());
    ASSERT_STREQ("", result[1].c_str());
    ASSERT_STREQ("ccc", result[2].c_str());
}

TEST(WrongFormat, EndlineAfterSep) {
    std::string input = "aaa,bbb,\n";
    std::istringstream istringstream(input);
    CSVParser parser = CSVParser(istringstream);
    std::vector<std::string> result;

    ASSERT_THROW(parser.readRecord(result), std::invalid_argument);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}