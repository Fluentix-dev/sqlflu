#include "lexer.hpp"
#include <iostream>

namespace frontend {
    LexerResult::LexerResult(const std::vector<Token> &tokens, const std::string error_type, const std::string error_description) : tokens(tokens), error_type(error_type), error_description(error_description) {}

    bool LexerResult::error() {
        return this->error_type != "";
    }

    void LexerResult::display_error() {
        std::cout << this->error_type << ": " << this->error_description << "\n";
    }

    Lexer::Lexer(const std::string &src) : src(src), index(-1), current_char('\0') {
        this->advance();
    }

    void Lexer::advance() {
        this->current_char = (++this->index < this->src.size() ? this->src[this->index] : '\0');
    }

    LexerResult Lexer::tokenize() {
        std::vector<Token> tokens = {};
        while (this->current_char != '\0') {
            if (this->current_char == ' ' || this->current_char == '\n' || this->current_char == '\r' || this->current_char == '\t') {
                this->advance();
                continue;
            }

            if (this->current_char == '+') {
                tokens.push_back(Token(TokenType::Plus, "+"));
                this->advance();
                continue;
            }

            if (this->current_char == '-') {
                tokens.push_back(Token(TokenType::Minus, "-"));
                this->advance();
                continue;
            }

            if (this->current_char == '*') {
                tokens.push_back(Token(TokenType::Multiply, "*"));
                this->advance();
                continue;
            }

            if (this->current_char == '/') {
                tokens.push_back(Token(TokenType::Divide, "/"));
                this->advance();
                continue;
            }

            if (this->current_char == '(') {
                tokens.push_back(Token(TokenType::LeftParen, "("));
                this->advance();
                continue;
            }

            if (this->current_char == ')') {
                tokens.push_back(Token(TokenType::RightParen, ")"));
                this->advance();
                continue;
            }

            if (this->current_char == ';') {
                tokens.push_back(Token(TokenType::Semicolon, ";"));
                this->advance();
                continue;
            }

            if (this->current_char == ',') {
                tokens.push_back(Token(TokenType::Comma, ","));
                this->advance();
                continue;
            }

            if (this->current_char == '.' || ('0' <= this->current_char && this->current_char <= '9')) {
                bool dot = this->current_char == '.';
                std::string literal = "";
                while (this->current_char != '\0' && (this->current_char == '.' || ('0' <= this->current_char && this->current_char <= '9'))) {
                    if (this->current_char == '.') {
                        if (dot) {
                            return LexerResult(tokens, "Syntax Error", "extra '.' in numeric literal");
                        }

                        dot = true;
                    }

                    literal += this->current_char;
                    this->advance();
                }

                tokens.push_back(Token((dot ? TokenType::Float : TokenType::Int), literal));
                continue;
            }

            if (this->current_char == '_' || ('a' <= this->current_char && this->current_char <= 'z') || ('A' <= this->current_char && this->current_char <= 'Z')) {
                std::string identifier = "";
                while (this->current_char != '\0' && (this->current_char == '_' || ('a' <= this->current_char && this->current_char <= 'z') || ('A' <= this->current_char && this->current_char <= 'Z'))) {
                    identifier += this->current_char;
                    this->advance();
                }

                tokens.push_back(Token(TokenType::Identifier, identifier));
                continue;
            }

            return LexerResult(tokens, "Syntax Error", std::string("invalid character '") + this->current_char + "'");
        }

        tokens.push_back(Token(TokenType::EndOfFile, ""));
        return LexerResult(tokens);
    }
}