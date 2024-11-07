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

class KwWhile : public Node {

public:
    KwWhile(Node::Ptr check, Node::Ptr repeat) : m_check(check), m_repeat(repeat) {}

    std::string as_string() const override {
        if (m_repeat == nullptr) {
            return fmt::format("While(?={}, repeat=[])", m_check->as_string());
        }
        else {
            return fmt::format(
                    "While(?={}, repeat=[{}])", m_check->as_string(), m_repeat->as_string());
        }
    }

private:
    Node::Ptr m_check, m_repeat;
};

class lines : public Node {
private:
    std::vector<Node::Ptr> line_as_vec;

public:
    lines(Node::Ptr line) { line_as_vec.push_back(line); }
    lines(lines *val) { *this = *val; }
    lines *push_line(Node::Ptr line) {
        line_as_vec.push_back(line);
        return this;
    }

    std::string as_string() const override {
        std::string lines_str;
        for (int i = 0; i < line_as_vec.size(); i++) {
            lines_str += line_as_vec[i]->as_string();
            if (i != line_as_vec.size() - 1) {
                lines_str += ", ";
            }
        }
        return lines_str;
    }
};
class KwIf : public Node {

public:
    KwIf(Node::Ptr check, Node::Ptr then, Node::Ptr else_case)
            : m_check(check), m_then(then), m_else(else_case) {}

    std::string as_string() const override {
        if (m_then == nullptr && m_else == nullptr) {
            return fmt::format("If(?={}, then=[], else=[])", m_check->as_string());
        }
        else if (m_else == nullptr) {
            return fmt::format(
                    "If(?={}, then=[{}], else=[])", m_check->as_string(), m_then->as_string());
        }
        else if (m_then == nullptr) {
            std::string else_case_as_str = m_else->as_string();
            if (else_case_as_str.rfind("If", 0) == 0) {
                return fmt::format(
                        "If(?={}, then=[], else={})", m_check->as_string(), else_case_as_str);
            }
            else {
                return fmt::format(
                        "If(?={}, then=[], else=[{}])", m_check->as_string(), else_case_as_str);
            }
        }
        else {
            std::string else_case_as_str = m_else->as_string();
            if (else_case_as_str.rfind("If", 0) == 0) {
                return fmt::format("If(?={}, then=[{}], else={})", m_check->as_string(),
                    m_then->as_string(), m_else->as_string());
            }
            else {
               return fmt::format("If(?={}, then=[{}], else=[{}])", m_check->as_string(),
                    m_then->as_string(), m_else->as_string());
            }
        }
    }

private:
    Node::Ptr m_check, m_then, m_else;
};

class KwClass : public Node {

public:
    KwClass(Node::Ptr id_node, Node::Ptr scope_node)
            : m_id_node(id_node), m_scope_node(scope_node) {}
    std::string as_string() const override {
        if (m_scope_node == nullptr) {
            return fmt::format("Class(n={}, s=[])", m_id_node->as_string());
        }

        else {
            return fmt::format(
                    "Class(n={}, s=[{}])", m_id_node->as_string(), m_scope_node->as_string());
        }
    }

private:
    Node::Ptr m_scope_node, m_id_node;
};
};

#endif // KIRAZ_AST_OPERATOR_H
