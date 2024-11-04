#ifndef KIRAZ_TOKEN_OPERATOR_H
#define KIRAZ_TOKEN_OPERATOR_H

#include <kiraz/Token.h>

namespace token {
class Operator : public Token {
public:
    Operator(int type) : Token(type) {}
};


class OpLCurlyBr: public Operator{
public:
    OpLCurlyBr() : Operator(OP_LCURLYBR){}
    std::string as_string() const override {return "OP_LCURLYBR";}
};

class OpRCurlyBr: public Operator{
public:
    OpRCurlyBr() : Operator(OP_RCURLYBR){}                                
    std::string as_string() const override {return "OP_RCURLYBR";}
};

class OpLParen: public Operator{
public:
    OpLParen() : Operator(OP_LPAREN){}
    std::string as_string() const override {return "OP_LPAREN";}
};

class OpRParen: public Operator{
public:
    OpRParen() : Operator(OP_RPAREN){}                                
    std::string as_string() const override {return "OP_RPAREN";}
};

class OpPlus : public Operator {
public:
    OpPlus(): Operator(OP_PLUS) {}
    std::string as_string() const override {return "OP_PLUS";}
};

class OpMinus : public Operator{
public:
    OpMinus() : Operator(OP_MINUS){}
    std::string as_string() const override {return "OP_MINUS";}
};

class OpDivF: public Operator{
public:
    OpDivF() : Operator(OP_DIVF){}
    std::string as_string() const override {return "OP_DIVF";}
};

class OpMult: public Operator{
public:
    OpMult() : Operator(OP_MULT){}
    std::string as_string() const override {return "OP_MULT";}
};

class OpLessThan: public Operator{
public:
    OpLessThan() : Operator(OP_LESSTHAN){}
    std::string as_string() const override {return "OP_LESSTHAN";}
};

class OpAssign: public Operator{
public:
    OpAssign() : Operator(OP_ASSIGN){}                                
    std::string as_string() const override {return "OP_ASSIGN";}
};

class OpGreaterThan: public Operator{
public:
    OpGreaterThan() : Operator(OP_GREATERTHAN){}
    std::string as_string() const override {return "OP_GREATERTHAN";}
};

class OpGreaterEquals: public Operator{
public:
    OpGreaterEquals() : Operator(OP_GREATEREQUALS){}                                
    std::string as_string() const override {return "OP_GREATEREQUALS";}
};

class OpLessEquals : public Operator {
public:
    OpLessEquals(): Operator(OP_LESSEQUALS) {}
    std::string as_string() const override {return "OP_LESSEQUALS";}
};

class OpEquals : public Operator{
public:
    OpEquals() : Operator(OP_EQUALS){}
    std::string as_string() const override {return "OP_EQUALS";}
};

class OpNotEquals: public Operator{
public:
    OpNotEquals() : Operator(OP_NOTEQUALS){}
    std::string as_string() const override {return "OP_NOTEQUALS";}
};

class OpColon: public Operator{
public:
    OpColon() : Operator(OP_COLON){}
    std::string as_string() const override {return "OP_COLON";}
};

class OpSemiColon : public Operator{
public:
    OpSemiColon() : Operator(OP_SEMICOLON){}
    std::string as_string() const override {return "OP_SEMICOLON";}
};

class OpComma: public Operator{
public:
    OpComma() : Operator(OP_COMMA){}
    std::string as_string() const override {return "OP_COMMA";}
};

class OpDot: public Operator{
public:
    OpDot() : Operator(OP_DOT){}
    std::string as_string() const override {return "OP_DOT";}
};

}

#endif
