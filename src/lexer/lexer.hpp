#pragma once
#include <vector>
#include "tokens.hpp"

namespace frontend {
    struct LexerResult {
        std::vector<Token> tokens;
        std::string error_type; // leave this blank if there's no error
        std::string error_description;

        LexerResult(const std::vector<Token> &tokens, const std::string error_type = "", const std::string error_description = "");
        bool error();
        void display_error();
    };

    struct Lexer {
    public:
        Lexer(const std::string &src);
        LexerResult tokenize();
    private:
        std::string src;
        size_t index;
        char current_char;

        void advance();
    };
}