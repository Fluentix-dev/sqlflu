#pragma once
#include "expressions.hpp"
#include <vector>

namespace frontend {
    enum struct Component {
        Database,
        Table
    };
    
    struct BlockStatement : public Statement {
        std::vector<std::shared_ptr<Statement>> body;
        
        BlockStatement(const std::vector<std::shared_ptr<Statement>> &body);
    };

    struct CreateStatement : public Statement {
        Component type;
        std::string name;

        CreateStatement(const Component type, const std::string &name);
    };

    struct DropStatement : public Statement {
        Component type;
        std::string name;

        DropStatement(const Component type, const std::string &name);
    };

    struct UseStatement : public Statement {
        std::string database_name;

        UseStatement(const std::string &database_name);
    };
}