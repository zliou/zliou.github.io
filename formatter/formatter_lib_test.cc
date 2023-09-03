#include "formatter_lib.cc"

#include <iostream>
#include <string>


template<class T>
void Test(std::string description, T actual, T expected) {
    bool match = expected == actual;
    std::cout << "[";
    if (match) {
        std::cout << " pass ";
    } else {
        std::cout << "FAILED";
    }
    std::cout << "] " << description << std::endl;
}

void RunTests() {
    // TranslateQuoteFromOptions
    Test("DoubleQuoteTranslationSuccess",
         Quote::DOUBLE, TranslateQuoteFromOptions(/*add_double_quotes=*/true,
                                                  /*add_single_quotes=*/false));
    Test("SingleQuoteTranslationSuccess",
         Quote::SINGLE, TranslateQuoteFromOptions(/*add_double_quotes=*/false,
                                                  /*add_single_quotes=*/true));
    Test("NoQuotesTranslationSuccess",
         Quote::NONE, TranslateQuoteFromOptions(/*add_double_quotes=*/false,
                                                  /*add_single_quotes=*/false));
    Test("UseDoubleQuoteWhenAmbiguous",
         Quote::DOUBLE, TranslateQuoteFromOptions(/*add_double_quotes=*/true,
                                                  /*add_single_quotes=*/true));

    // Join
    Test("JoinBasicSuccess", Join({"a","b","c"}, ","), std::string("a,b,c"));
    Test("JoinTrailingDelimiterSuccess",
         Join({"a","b","c"}, ",", /*use_trailing_delimiter=*/true),
         std::string("a,b,c,"));
    Test("JoinLongDelimiterSuccess",
         Join({"a","b","c"}, "longlong"), std::string("alonglongblonglongc"));
    Test("JoinEmptyDelimiterSuccess",
         Join({"a","b","c"}, ""), std::string("abc"));

    // TranslateDelimiterFromOptions
    Test("CommaDelimiterFromOptions",
         TranslateDelimiterFromOptions(/*add_commas=*/true,
                                       /*add_space=*/false,
                                       /*add_newline=*/false),
         std::string(","));
    Test("SpaceDelimiterFromOptions",
         TranslateDelimiterFromOptions(/*add_commas=*/false,
                                       /*add_space=*/true,
                                       /*add_newline=*/false),
         std::string(" "));
    Test("NewlineDelimiterFromOptions",
         TranslateDelimiterFromOptions(/*add_commas=*/false,
                                       /*add_space=*/false,
                                       /*add_newline=*/true),
         std::string("\n"));
    Test("MultipleDelimitersFromOptions",
         TranslateDelimiterFromOptions(/*add_commas=*/true,
                                       /*add_space=*/true,
                                       /*add_newline=*/true),
         std::string(", \n"));

    // RemoveQuotes
    std::string test;
    test = "\"test\"";
    RemoveQuotes(test);
    Test("RemoveDoubleQuotesSuccess", test, std::string("test"));

    test = "'test'";
    RemoveQuotes(test);
    Test("RemoveSingleQuotesSuccess", test, std::string("test"));

    test = "\"test";
    RemoveQuotes(test);
    Test("RemoveLeftUnmatchedQuoteSuccess", test, std::string("test"));

    test = "test\"";
    RemoveQuotes(test);
    Test("RemoveRightUnmatchedQuoteSuccess", test, std::string("test"));

    test = "test";
    RemoveQuotes(test);
    Test("RemoveNoQuotesSuccess", test, std::string("test"));

    // AddQuotes
    test = "test";
    AddQuotes(test, Quote::DOUBLE);
    Test("AddDoubleQuotesSuccess", test, std::string("\"test\""));

    test = "test";
    AddQuotes(test, Quote::SINGLE);
    Test("AddSingleQuotesSuccess", test, std::string("'test'"));

    test = "test";
    AddQuotes(test, Quote::NONE);
    Test("NoQuotesSuccess", test, std::string("test"));

    // Split
    Test("SplitBasicSuccess", Split("a,b,c", ","), {"a","b","c"});
    Test("SplitMultiCharDelimiterSuccess",
         Split("a, b, c", ", "), {"a","b","c"});
    Test("SplitLongDelimiterSuccess",
         Split("alonglongblonglongc", "longlong"), {"a","b","c"});
    Test("SplitEmptyDelimiterSuccess", Split("abc", ""), {"a","b","c"});
    Test("SplitTrailingDelimiterSuccess",
         Split("a,b,c,", ",", /*has_trailing_delimiter=*/true), {"a","b","c"});

    // Returning a list with single empty string matches py3 behavior.
    Test("SplitEmptyListSuccess", Split("", ","), {""});
    Test("SplitEmptyListEmptyDelimiterSuccess", Split("", ""), {});
}

int main() {
    RunTests();
    return 0;
}
