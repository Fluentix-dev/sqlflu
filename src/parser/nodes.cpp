#include "nodes.hpp"

namespace frontend {
    Statement::Statement(const NodeType type) : type(type) {}
    Expression::Expression(const NodeType type) : Statement(type) {}
}