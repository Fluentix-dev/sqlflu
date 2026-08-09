#pragma once
#include "statements.hpp"
#include "../lexer/tokens.hpp"

namespace frontend {
    struct ParserResult {
        std::shared_ptr<Statement> node;
        std::string error_type; // leave this blank if there's no error
        std::string error_description;

        ParserResult(const std::shared_ptr<Statement> &node, const std::string error_type = "", const std::string error_description = "");
        bool error();
        void display_error();
    };

    struct Parser {
    public:
        Parser(const std::vector<Token> &tokens);
        ParserResult parse();
    private:
        std::vector<Token> tokens;
        size_t index;
        Token current_token;

        void advance();
        bool overflow();
        ParserResult statement();
        ParserResult expression();
        ParserResult additive();
        ParserResult multiplicative();
        ParserResult unary();
        ParserResult primary();
    };
}