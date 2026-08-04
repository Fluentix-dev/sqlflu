#pragma once
#include "expressions.hpp"
#include <vector>

namespace frontend {
    struct BlockStatement : public Statement {
        std::vector<Statement> body;
        
        BlockStatement(const std::vector<Statement> &body);
    };
}