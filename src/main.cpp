#include <iostream>
#include <sstream>
#include <fstream>
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"

void debug_expr(const std::shared_ptr<frontend::Expression> &expr) {
    if (expr->type == frontend::NodeType::BinaryExpr) {
        std::shared_ptr<frontend::BinaryExpression> binary_expr = std::static_pointer_cast<frontend::BinaryExpression>(expr);
        std::cout << "(";
        debug_expr(binary_expr->lhs);
        std::cout << binary_expr->op;
        debug_expr(binary_expr->rhs);
        std::cout << ")";
        return;
    }

    if (expr->type == frontend::NodeType::UnaryExpr) {
        std::shared_ptr<frontend::UnaryExpression> unary_expr = std::static_pointer_cast<frontend::UnaryExpression>(expr);
        std::cout << "(" << unary_expr->op;
        debug_expr(unary_expr->value);
        std::cout << ")";
        return;
    }

    if (expr->type == frontend::NodeType::IntExpr) {
        std::shared_ptr<frontend::IntExpression> int_expr = std::static_pointer_cast<frontend::IntExpression>(expr);
        std::cout << "(" << int_expr->value << ")";
        return;
    }

    if (expr->type == frontend::NodeType::FloatExpr) {
        std::shared_ptr<frontend::FloatExpression> float_expr = std::static_pointer_cast<frontend::FloatExpression>(expr);
        std::cout << "(" << float_expr->value << ")";
        return;
    }

    if (expr->type == frontend::NodeType::IdentifierExpr) {
        std::shared_ptr<frontend::IdentifierExpression> identifier_expr = std::static_pointer_cast<frontend::IdentifierExpression>(expr);
        std::cout << "(" << identifier_expr->symbol << ")";
        return;
    }

    std::cout << "node type undefined: " << static_cast<int>(expr->type) << "\n";
}

std::string component_type_to_string(frontend::Component type) {
    if (type == frontend::Component::Database) {
        return "DATABASE";
    }

    if (type == frontend::Component::Table) {
        return "TABLE";
    }

    return "undefined";
}

void debug_stmt(const std::shared_ptr<frontend::Statement> &stmt, const size_t indentation = 0) {
    for (size_t i = 0; i < indentation; i++) {
        std::cout << "    ";
    }

    if (stmt->type == frontend::NodeType::BlockStmt) {
        std::shared_ptr<frontend::BlockStatement> body = std::static_pointer_cast<frontend::BlockStatement>(stmt);
        std::cout << "(BLOCK, len=" << body->body.size() << ")\n";
        for (std::shared_ptr<frontend::Statement> &s : body->body) {
            debug_stmt(s, indentation+1);
            std::cout << "\n";
        }

        return;
    }

    if (stmt->type == frontend::NodeType::CreateStmt) {
        std::shared_ptr<frontend::CreateStatement> create = std::static_pointer_cast<frontend::CreateStatement>(stmt);
        std::cout << "CREATE " << component_type_to_string(create->type) << " " << create->name;
        return;
    }

    if (stmt->type == frontend::NodeType::DropStmt) {
        std::shared_ptr<frontend::DropStatement> create = std::static_pointer_cast<frontend::DropStatement>(stmt);
        std::cout << "DROP " << component_type_to_string(create->type) << " " << create->name;
        return;
    }
    
    debug_expr(std::static_pointer_cast<frontend::Expression>(stmt));
}

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

    frontend::Parser parser = frontend::Parser(tokens.tokens);
    frontend::ParserResult ast = parser.parse();
    if (ast.error()) {
        ast.display_error();
        return 0;
    }

    debug_stmt(ast.node);
    return 0;
}