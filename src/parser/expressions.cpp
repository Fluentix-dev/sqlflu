#include "expressions.hpp"

namespace frontend {
    BinaryExpression::BinaryExpression(const std::shared_ptr<Expression> &lhs, const std::string op, const std::shared_ptr<Expression> &rhs) : Expression(NodeType::BinaryExpr), lhs(lhs), op(op), rhs(rhs) {}
    UnaryExpression::UnaryExpression(const std::string op, const std::shared_ptr<Expression> &value) : Expression(NodeType::UnaryExpr), op(op), value(value) {}
    IntExpression::IntExpression(const std::string &value) : Expression(NodeType::IntExpr), value(value) {}
    FloatExpression::FloatExpression(const std::string &value) : Expression(NodeType::FloatExpr), value(value) {}
    IdentifierExpression::IdentifierExpression(const std::string &symbol) : Expression(NodeType::IdentifierExpr), symbol(symbol) {}
}