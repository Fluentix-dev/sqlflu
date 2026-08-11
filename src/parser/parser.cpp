#include "parser.hpp"
#include <iostream>

namespace frontend {
    ParserResult::ParserResult(const std::shared_ptr<Statement> &node, const std::string error_type, const std::string error_description) : node(node), error_type(error_type), error_description(error_description) {}
    bool ParserResult::error() {
        return this->error_type != "";
    }

    void ParserResult::display_error() {
        std::cout << this->error_type << ": " << this->error_description << "\n";
    }

    Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens), index(-1), current_token(tokens[0]) {
        this->advance();
    }

    void Parser::advance() {
        this->current_token = (++this->index < this->tokens.size() ? this->tokens[this->index] : this->tokens[this->tokens.size()-1]);
    }

    bool Parser::overflow() {
        return this->index >= this->tokens.size();
    }

    ParserResult Parser::parse() {
        std::vector<std::shared_ptr<Statement>> body = {};
        while (!this->overflow() && this->current_token.type != TokenType::EndOfFile) {
            ParserResult stmt = this->statement();
            if (stmt.error()) {
                return stmt;
            }

            body.push_back(stmt.node);
        }

        return ParserResult(std::make_shared<BlockStatement>(body));
    }

    ParserResult Parser::statement() {
        ParserResult stmt = ParserResult(nullptr);
        if (this->current_token.type == TokenType::Create) {
            stmt = this->create_statement();
        } else if (this->current_token.type == TokenType::Drop) {
            stmt = this->drop_statement();
        } else if (this->current_token.type == TokenType::Use) {
            stmt = this->use_statement();
        } else {
            stmt = this->expression();
        }

        if (stmt.error()) {
            return stmt;
        }

        if (this->current_token.type != TokenType::Semicolon) {
            return ParserResult(nullptr, "Syntax Error", "expected ';' at end of statement");
        }

        this->advance();
        return stmt;
    }

    ParserResult Parser::create_statement() {
        this->advance();
        Component type = Component::Database;
        if (this->current_token.type == TokenType::Database) {
            // database was set default, there's no need to change the type
        } else {
            return ParserResult(nullptr, "Syntax Error", "expected 'database', got '" + this->current_token.value + "'");
        }

        this->advance();
        if (this->current_token.type != TokenType::Identifier) {
            return ParserResult(nullptr, "Syntax Error", "expected a name, not a keyword, got '" + this->current_token.value + "'");
        }

        std::string name = this->current_token.value;
        this->advance();

        return ParserResult(std::make_shared<CreateStatement>(type, name));
    }

    ParserResult Parser::drop_statement() {
        this->advance();
        Component type = Component::Database;
        if (this->current_token.type == TokenType::Database) {
            // database was set default, there's no need to change the type
        } else if (this->current_token.type == TokenType::Table) {
            type = Component::Table;
        } else {
            return ParserResult(nullptr, "Syntax Error", "expected 'database' or 'table', got '" + this->current_token.value + "'");
        }

        this->advance();
        if (this->current_token.type != TokenType::Identifier) {
            return ParserResult(nullptr, "Syntax Error", "expected a name, not a keyword, got '" + this->current_token.value + "'");
        }

        std::string name = this->current_token.value;
        this->advance();

        return ParserResult(std::make_shared<DropStatement>(type, name));
    }

    ParserResult Parser::use_statement() {
        this->advance();
        if (this->current_token.type != TokenType::Identifier) {
            return ParserResult(nullptr, "Syntax Error", "expected a name, not a keyword, got '" + this->current_token.value + "'");
        }

        std::string database_name = this->current_token.value;
        this->advance();

        return ParserResult(std::make_shared<UseStatement>(database_name));
    }

    ParserResult Parser::expression() {
        return this->additive();
    }

    ParserResult Parser::additive() {
        ParserResult lhs = this->multiplicative();
        if (lhs.error()) {
            return lhs;
        }

        while (this->current_token.type == TokenType::Plus || this->current_token.type == TokenType::Minus) {
            std::string op = this->current_token.value;
            this->advance();
            ParserResult rhs = this->multiplicative();
            if (rhs.error()) {
                return rhs;
            }

            lhs.node = std::make_shared<BinaryExpression>(std::static_pointer_cast<Expression>(lhs.node), op, std::static_pointer_cast<Expression>(rhs.node));
        }

        return lhs;
    }

    ParserResult Parser::multiplicative() {
        ParserResult lhs = this->unary();
        if (lhs.error()) {
            return lhs;
        }

        while (this->current_token.type == TokenType::Multiply || this->current_token.type == TokenType::Divide) {
            std::string op = this->current_token.value;
            this->advance();
            ParserResult rhs = this->unary();
            if (rhs.error()) {
                return rhs;
            }

            lhs.node = std::make_shared<BinaryExpression>(std::static_pointer_cast<Expression>(lhs.node), op, std::static_pointer_cast<Expression>(rhs.node));
        }

        return lhs;
    }

    ParserResult Parser::unary() {
        if (this->current_token.type != TokenType::Plus && this->current_token.type != TokenType::Minus) {
            return this->primary();
        }

        std::string op = this->current_token.value;
        this->advance();

        ParserResult value = this->unary();
        if (value.error()) {
            return value;
        }

        return ParserResult(std::make_shared<UnaryExpression>(op, std::static_pointer_cast<Expression>(value.node)));
    }

    ParserResult Parser::primary() {
        if (this->current_token.type == TokenType::Int) {
            std::string value = this->current_token.value;
            this->advance();
            return ParserResult(std::make_shared<IntExpression>(value));
        }

        if (this->current_token.type == TokenType::Float) {
            std::string value = this->current_token.value;
            this->advance();
            return ParserResult(std::make_shared<FloatExpression>(value));
        }

        if (this->current_token.type == TokenType::Identifier) {
            std::string value = this->current_token.value;
            this->advance();
            return ParserResult(std::make_shared<IdentifierExpression>(value));
        }

        if (this->current_token.type == TokenType::LeftParen) {
            this->advance();
            ParserResult value = this->expression();
            if (value.error()) {
                return value;
            }

            if (this->current_token.type != TokenType::RightParen) {
                return ParserResult(nullptr, "Syntax Error", "expected ')', got '" + this->current_token.value + "'");
            }

            this->advance();
            return value;
        }

        return ParserResult(nullptr, "Syntax Error", "unexpected token '" + this->current_token.value + "'");
    }
}