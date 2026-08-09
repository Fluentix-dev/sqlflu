#pragma once
#include "nodes.hpp"
#include <memory>
#include <string>

namespace frontend {
    struct BinaryExpression : public Expression {
        std::shared_ptr<Expression> lhs;
        std::string op;
        std::shared_ptr<Expression> rhs;

        BinaryExpression(const std::shared_ptr<Expression> &lhs, const std::string op, const std::shared_ptr<Expression> &rhs);
    };

    struct UnaryExpression : public Expression {
        std::string op;
        std::shared_ptr<Expression> value;

        UnaryExpression(const std::string op, const std::shared_ptr<Expression> &value);
    };

    struct IntExpression : public Expression {
        std::string value;

        IntExpression(const std::string &value);
    };

    struct FloatExpression : public Expression {
        std::string value;

        FloatExpression(const std::string &value);
    };

    struct IdentifierExpression : public Expression {
        std::string symbol;

        IdentifierExpression(const std::string &value);
    };
}