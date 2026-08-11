#pragma once
#include <string>

namespace frontend {
    enum struct TokenType {
        EndOfFile,
        Plus,
        Minus,
        Multiply,
        Divide,
        LeftParen,
        RightParen,
        Semicolon,
        Int,
        Float,
        Identifier,
        Comma,
        Create,
        Database,
        Drop
    };

    struct Token {
        TokenType type;
        std::string value;

        Token(const TokenType type, const std::string value);
    };
}