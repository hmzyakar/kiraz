#ifndef KIRAZ_AST_LITERAL_H
#define KIRAZ_AST_LITERAL_H

#include <kiraz/Node.h>

namespace ast {
class Integer : public Node {
public:
    Integer(Token::Ptr);

    std::string as_string() const override { return fmt::format("Integer({})", m_value); }

private:
    int64_t m_value;
};

class SignedNode : public Node {
public:
    SignedNode(int op, Node::Cptr integer) : Node(L_INTEGER), m_operator(op), m_integer(integer) {}

    std::string as_string() const override {
        return fmt::format("Integer({},{})", m_operator, m_integer->as_string());
    }

private:
    int m_operator;
    Node::Cptr m_integer;
};
}

#endif
