#ifndef KIRAZ_AST_IDENTIFIER_H
#define KIRAZ_AST_IDENTIFIER_H

#include <kiraz/Node.h>

namespace ast {

class Identifier : public Node {
public:
    Identifier(Token::Ptr);
    std::string as_string() const override { return fmt::format("Id({})", m_value); }

private:
    std::string m_value;
};
}

#endif