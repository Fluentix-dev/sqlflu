#pragma once
#include "expressions.hpp"
#include <vector>

namespace frontend {
    struct BlockStatement : public Statement {
        std::vector<std::shared_ptr<Statement>> body;
        
        BlockStatement(const std::vector<std::shared_ptr<Statement>> &body);
    };
}