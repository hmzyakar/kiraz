#ifndef KIRAZ_AST_MODULE_H
#define KIRAZ_AST_MODULE_H

#include <kiraz/Node.h>

// S.A. stands for Semantic Analyze

namespace ast {

class Module : public Node {
public:
    Module(Node::Ptr scope) : m_scope(scope) {};

    // as_string() method override
    std::string as_string() const override {
        return fmt::format("Module({})", m_scope->as_string());
    }

    // S.A. compute_stmt_type() method override declaration
    Node::Ptr compute_stmt_type(SymbolTable &st) override;

private:
    Node::Ptr m_scope;
};

class lines : public Node {

public:
    lines(Node::Ptr line) : Node() { scope_lines.push_back(line); }
    lines(lines *other) : Node() { *this = *other; }
    lines *add_line(Node::Ptr line) {
        scope_lines.push_back(line);
        return this;
    }

    // as_string() method override
    std::string as_string() const override {
        std::string node_list_string = "[";
        for (int i = 0; i < scope_lines.size(); i++) {
            node_list_string += scope_lines[i]->as_string();
            if (i != scope_lines.size() - 1) {
                node_list_string += ", ";
            }
        }
        node_list_string = node_list_string + "]";
        return node_list_string;
    }
    Node::Ptr compute_stmt_type(SymbolTable &st) override;

private:
    std::vector<Node::Ptr> scope_lines;
};

}

#endif
