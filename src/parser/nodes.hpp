#pragma once

namespace frontend {
    enum struct NodeType {
        BlockStmt,
        BinaryExpr,
        UnaryExpr,
        IntExpr,
        FloatExpr
    };

    struct Statement {
        NodeType type;
        Statement(const NodeType type);
    };

    struct Expression : public Statement {
        Expression(const NodeType type);
    };
}