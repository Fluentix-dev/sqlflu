#include "statements.hpp"

namespace frontend {
    BlockStatement::BlockStatement(const std::vector<Statement> &body) : Statement(NodeType::BlockStmt), body(body) {}
}