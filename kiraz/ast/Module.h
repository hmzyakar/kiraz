#ifndef KIRAZ_AST_MODULE_H
#define KIRAZ_AST_MODULE_H

#include <kiraz/Node.h>

namespace ast {

class Module : public Node {
public:
    Module(Node::Cptr node) : m_node(node) {};

    std::string as_string() const override {

        return fmt::format("Module({})",m_node->as_string());
    }

private:
    Node::Cptr m_node;
};


class lines : public Node {
    private:
    std::vector<Node::Ptr> scope_lines;
    public:
        lines(Node::Ptr line) :Node(){ scope_lines.push_back(line); }
        lines(lines *other) :Node() { *this = *other; }
        lines *add_line(Node::Ptr line) {
            scope_lines.push_back(line);
            return this;
        }

    Node::Ptr compute_stmt_type(SymbolTable& st) override;

    std::string as_string() const override {
        std::string node_list_string = "[";
        for (int i = 0; i < scope_lines.size(); i++) {
            node_list_string += scope_lines[i]->as_string();
            if (i != scope_lines.size() - 1) {
                node_list_string += ", ";
            }
        }
        node_list_string =node_list_string+ "]";
        return node_list_string;
    }

};

}

#endif
