#include "statements.hpp"

namespace frontend {
    BlockStatement::BlockStatement(const std::vector<std::shared_ptr<Statement>> &body) : Statement(NodeType::BlockStmt), body(body) {}
    CreateStatement::CreateStatement(const Component type, const std::string &name) : Statement(NodeType::CreateStmt), type(type), name(name) {}
    DropStatement::DropStatement(const Component type, const std::string &name) : Statement(NodeType::DropStmt), type(type), name(name) {}
    UseStatement::UseStatement(const std::string &database_name) : Statement(NodeType::UseStmt), database_name(database_name) {}
}