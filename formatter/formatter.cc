#include "formatter_lib.cc"

#include <algorithm>
#include <emscripten/bind.h>
#include <iostream>
#include <string>
#include <vector>


// Given an input and input/output Options, output the reformatted string.
std::string Reformat(std::string input,
                     Options input_options, Options output_options) {
    std::vector<std::string> tokens = Split(
            input, input_options.delimiter, input_options.trailing_delimiter);
    if (output_options.sorted) {
        std::sort(tokens.begin(), tokens.end());
    }

    // If the input and output quotes are mismatched, we'll have to remove
    // and add quotes. We start here by removing the quotes.
    if (input_options.quote != output_options.quote) {
        for (std::string& s : tokens) {
            RemoveQuotes(s);
        }
        // Add the specified quotes.
        if (output_options.quote != Quote::NONE) {
            for (std::string& s : tokens) {
                AddQuotes(s, output_options.quote);
            }
        }
    }

    return Join(tokens, output_options.delimiter,
                output_options.trailing_delimiter);
}

// Converts form-provided values to Option structs and calls Reformat.
std::string MakeOptionsAndReformat(
        std::string input,
        // Input options
        bool input_has_double_quotes,
        bool input_has_single_quotes,
        bool input_has_single_spaces,
        bool input_has_commas,
        bool input_has_newlines,
        bool input_has_trailing_delimiter,
        // Output options
        bool add_double_quotes_to_output,
        bool add_single_quotes_to_output,
        bool add_commas_to_output,
        bool add_spaces_to_output,
        bool add_newlines_to_output,
        bool add_trailing_delimiter_to_output,
        bool sort_output) {
    Options input_options(
            TranslateDelimiterFromOptions(input_has_commas,
                                          input_has_single_spaces,
                                          input_has_newlines),
            TranslateQuoteFromOptions(input_has_double_quotes,
                                      input_has_single_quotes),
            /*sorted=*/false,
            /*trailing_delimiter=*/input_has_trailing_delimiter);
    Options output_options(
            TranslateDelimiterFromOptions(add_commas_to_output,
                                          add_spaces_to_output,
                                          add_newlines_to_output),
            TranslateQuoteFromOptions(add_double_quotes_to_output,
                                      add_single_quotes_to_output),
            /*sorted=*/sort_output,
            /*trailing_delimiter=*/add_trailing_delimiter_to_output);
    return Reformat(input, input_options, output_options);
}


EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("Reformat", &Reformat);
    emscripten::function("MakeOptionsAndReformat", &MakeOptionsAndReformat);
}

