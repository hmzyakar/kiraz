#ifndef KIRAZ_TOKEN_LITERAL_H
#define KIRAZ_TOKEN_LITERAL_H

#include <kiraz/Token.h>

namespace token {

class Integer : public Token {
public:
    Integer(int64_t base, std::string_view value)
            : Token(L_INTEGER), m_base(base), m_value(value) {}
    virtual ~Integer();

    std::string as_string() const override { return fmt::format("Int({})", m_value); }
    void print() { fmt::print("{}\n", as_string()); }

    static int colno;

    auto get_base() const { return m_base; }
    auto get_value() const { return m_value; }

private:
    int64_t m_base;
    std::string m_value;
};

class String : public Token {
public:
    String(std::string_view value) : Token(L_STRING), m_value(value) {}
    virtual ~String();

    std::string as_string() const override { return fmt::format("Str({})", m_value); }
    void print() { fmt::print("{}\n", as_string()); }

    static int colno;

    auto get_value() const { return m_value; }

private:
    std::string m_value;
};

class Boolean : public Token {
public:
    Boolean(std::string_view boolean_text) : Token(L_BOOLEAN), m_value(boolean_text) {}
    virtual ~Boolean();

    std::string as_string() const override { return fmt::format("Bool({})", m_value); }
    void print() { fmt::print("{}\n", as_string()); }

    static int colno;

    auto get_value() const { return m_value; }

private:
    std::string m_value;
};

}

#endif // KIRAZ_TOKEN_LITERAL_H
