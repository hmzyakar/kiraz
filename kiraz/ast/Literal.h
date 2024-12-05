#ifndef KIRAZ_AST_LITERAL_H
#define KIRAZ_AST_LITERAL_H

#include <kiraz/Node.h>

namespace ast {

class Integer : public Node {
public:
    Integer(Token::Ptr);

    std::string as_string() const override { return fmt::format("Int({})", m_value); }

    Node::Ptr compute_stmt_type(SymbolTable &st) override;
    Node::SymTabEntry get_symbol(const SymbolTable &st) const override;

private:
    int64_t m_value;
};

class Boolean : public Node {
public:
    Boolean(Token::Ptr);

    std::string as_string() const override { return fmt::format("Boolean({})", m_value); }

    // Node::Ptr compute_stmt_type(SymbolTable &st) override;

private:
    std::string m_value;
};

class String : public Node {
public:
    String(Token::Ptr);

    std::string as_string() const override { return fmt::format("Str({})", m_value); }

    Node::Ptr compute_stmt_type(SymbolTable &st) override;
    Node::SymTabEntry get_symbol(const SymbolTable &st) const override;

private:
    std::string m_value;
};

class SignedNode : public Node {
public:
    SignedNode(int sign, Node::Cptr node) : m_sign(sign), m_node(node) {};

    std::string as_string() const override {
        std::string StrOP;
        switch (m_sign) {
        case OP_PLUS:
            StrOP = "OP_PLUS";
            break;
        case OP_MINUS:
            StrOP = "OP_MINUS";
            break;
        default:
            break;
        }
        return fmt::format("Signed({}, {})", StrOP, m_node->as_string());
    }

private:
    int m_sign;
    Node::Cptr m_node;
};

}

#endif
