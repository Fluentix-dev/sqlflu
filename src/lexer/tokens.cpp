#include "tokens.hpp"

namespace frontend {
    Token::Token(const TokenType type, const std::string value) : type(type), value(value) {}
}