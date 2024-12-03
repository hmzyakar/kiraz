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
            StrOP = "Assign";
            break;
        case OP_EQUALS:
            StrOP = "OpEq";
            break;
        case OP_GREATERTHAN:
            StrOP = "OpGt";
            break;
        case OP_GREATEREQUALS:
            StrOP = "OpGe";
            break;
        case OP_LESSTHAN:
            StrOP = "OpLt";
            break;
        case OP_LESSEQUALS:
            StrOP = "OpLe";
            break;
        case OP_DOT:
            StrOP = "Dot";
            break;       

        default:
            break;
        }
        return fmt::format(
                "{}(l={}, r={})", StrOP, get_left()->as_string(), get_right()->as_string());
    }

private:
    Node::Ptr m_left, m_right;
};

class OpAdd : public Operator {
public:
    OpAdd(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_PLUS, left, right) {}
    Node::Ptr compute_stmt_type(SymbolTable& st) override; 
};

class OpSub : public Operator {
public:
    OpSub(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_MINUS, left, right) {}
    Node::Ptr compute_stmt_type(SymbolTable& st) override; 
};

class OpMult : public Operator {
public:
    OpMult(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_MULT, left, right) {}
    Node::Ptr compute_stmt_type(SymbolTable& st) override; 
};

class OpDivF : public Operator {
public:
    OpDivF(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_DIVF, left, right) {}
    Node::Ptr compute_stmt_type(SymbolTable& st) override; 
};

class OpAssign : public Operator {
public:
    OpAssign(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_ASSIGN, left, right) {}
    Node::Ptr compute_stmt_type(SymbolTable& st) override; 
};

class OpEq : public Operator {
public:
    OpEq(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_EQUALS, left, right) {}
    Node::Ptr compute_stmt_type(SymbolTable& st) override; 
};

class OpGt : public Operator {
public:
    OpGt(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_GREATERTHAN, left, right) {}
    Node::Ptr compute_stmt_type(SymbolTable& st) override; 
};

class OpGe : public Operator {
public:
    OpGe(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_GREATEREQUALS, left, right) {}
    Node::Ptr compute_stmt_type(SymbolTable& st) override; 
};

class OpLt : public Operator {
public:
    OpLt(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_LESSTHAN, left, right) {}
    Node::Ptr compute_stmt_type(SymbolTable& st) override; 
};

class OpLe : public Operator {
public:
    OpLe(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_LESSEQUALS, left, right) {}
    Node::Ptr compute_stmt_type(SymbolTable& st) override; 
};

class OpDot : public Operator{
public:
    OpDot(const Node::Ptr &left, const Node::Ptr &right) : Operator(OP_DOT, left, right){}
};

}

#endif // KIRAZ_AST_OPERATOR_H
