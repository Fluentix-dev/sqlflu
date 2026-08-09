#include "statements.hpp"

namespace frontend {
    BlockStatement::BlockStatement(const std::vector<std::shared_ptr<Statement>> &body) : Statement(NodeType::BlockStmt), body(body) {}
}