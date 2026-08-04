#include <iostream>
#include <sstream>
#include <fstream>
#include "lexer/lexer.hpp"

int main() {
    std::string fn = "files/main.sqlflu";
    std::ifstream src(fn);
    if (!src) {
        std::cout << "File " << fn << " cannot be found!\n";
        return 0;
    }

    std::stringstream code;
    std::string line;
    while (src.is_open() && std::getline(src, line)) {
        code << line << "\n";
    }

    std::string real = code.str();
    if (real.size() > 0) {
        real.erase(std::prev(real.end())); // Remove the ending newline
    }

    frontend::Lexer lexer = frontend::Lexer(real);
    frontend::LexerResult tokens = lexer.tokenize();
    if (tokens.error()) {
        tokens.display_error();
        return 0;
    }

    for (const frontend::Token &token : tokens.tokens) {
        std::cout << static_cast<int>(token.type) << " " << token.value << "\n";
    }

    return 0;
}