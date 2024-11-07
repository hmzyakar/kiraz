#ifndef KIRAZ_AST_KEYWORD_H
#define KIRAZ_AST_KEYWORD_H

#include <cassert>
#include <kiraz/Node.h>

namespace ast {
class KwLet : public Node {

public:
    KwLet(Node::Ptr variable_node, Node::Ptr type_node, Node::Ptr init_value_node)
            : m_variable_node(variable_node)
            , m_type_node(type_node)
            , m_init_value_node(init_value_node) {}

    std::string as_string() const override {
        std::string m_variable_node_string = m_variable_node->as_string();
        std::string m_type_node_string;
        std::string m_init_value_node_string;
        if (m_type_node == nullptr) {
            m_init_value_node_string = m_init_value_node->as_string();
            return fmt::format("Let(n={}, i={})", m_variable_node_string, m_init_value_node_string);
        }
        else if (m_init_value_node == nullptr) {
            m_type_node_string = m_type_node->as_string();
            return fmt::format("Let(n={}, t={})", m_variable_node_string, m_type_node_string);
        }
        else {
            m_type_node_string = m_type_node->as_string();
            m_init_value_node_string = m_init_value_node->as_string();
            return fmt::format("Let(n={}, t={}, i={})", m_variable_node_string, m_type_node_string,
                    m_init_value_node_string);
        }
    }

private:
    Node::Ptr m_variable_node, m_type_node, m_init_value_node;
};

class KwImport : public Node {

public:
    KwImport(Node::Ptr lib_node) : m_lib_node(lib_node) {}
    std::string as_string() const override {
        return fmt::format("Import({})", m_lib_node->as_string());
    }

private:
    Node::Ptr m_lib_node;
};


class KwReturn : public Node {

public:
    KwReturn(Node::Ptr ret_node) : m_ret_node(ret_node) {}
    std::string as_string() const override {
        return fmt::format("Return({})", m_ret_node->as_string());
    }

private:
    Node::Ptr m_ret_node;
};

}

#endif // KIRAZ_AST_OPERATOR_H
