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

}

#endif
