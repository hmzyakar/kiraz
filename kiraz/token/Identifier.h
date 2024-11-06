#ifndef KIRAZ_TOKEN_IDENTIFIER_H
#define KIRAZ_TOKEN_IDENTIFIER_H

#include <kiraz/Token.h>

namespace token {

class Identifier : public Token {
public:
    Identifier(const char *text) : Token(IDENTIFIER), m_value(text) {}
    virtual ~Identifier();

    std::string as_string() const override { return fmt::format("Id({})", m_value); }
    void print() { fmt::print("{}\n", as_string()); }

    static int colno;

    auto get_value() const { return m_value; }

private:
    std::string m_value;
};

}

#endif
