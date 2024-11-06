#ifndef KIRAZ_AST_OPERATOR_H
#define KIRAZ_AST_OPERATOR_H

#include <cassert>
#include <kiraz/Node.h>

namespace ast {
class Operator : public Node {
protected:
    explicit Operator(int m_op, const Node::Ptr &left, const Node::Ptr &right)
            : Node(m_op), m_left(left), m_right(right) {
        assert(left);
        assert(right);
    }

public:
    auto get_left() const { return m_left; }
    auto get_right() const { return m_right; }

    std::string as_string() const override {
        assert(get_left());
        assert(get_right());

        std::string StrOP;
        switch (get_id()) {
        case OP_PLUS:
            StrOP = "Add";
            break;
        case OP_MINUS:
            StrOP = "Sub";
            break;
        case OP_MULT:
            StrOP = "Mult";
            break;
        case OP_DIVF:
            StrOP = "DivF";
            break;
        case OP_ASSIGN:
            StrOP = "OP_ASSIGN";
            break;
        default:
            break;
        }
        return fmt::format("{}(l={}, r={})", StrOP, get_left()->as_string(), get_right()->as_string());
    }

private:
    Node::Ptr m_left, m_right;
};

class OpAdd : public Operator {
public:
    OpAdd(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_PLUS, left, right) {}
};

class OpSub : public Operator {
public:
    OpSub(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_MINUS, left, right) {}
};

class OpMult : public Operator {
public:
    OpMult(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_MULT, left, right) {}
};

class OpDivF : public Operator {
public:
    OpDivF(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_DIVF, left, right) {}
};

class OpAssign : public Operator {
public:
    OpAssign(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_ASSIGN, left, right) {}
};

}

#endif // KIRAZ_AST_OPERATOR_H
