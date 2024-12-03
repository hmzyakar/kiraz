#ifndef KIRAZ_AST_IDENTIFIER_H
#define KIRAZ_AST_IDENTIFIER_H

#include <kiraz/Node.h>
#include <kiraz/Token.h>
#include <string>

namespace ast {

class Identifier : public Node {
public:
    explicit Identifier(Token::Ptr t);
    explicit Identifier(const std::string &name); // Only declare it here, don't define

    std::string as_string() const override { return fmt::format("Id({})", m_value); }

private:
    Node::SymTabEntry get_symbol(const SymbolTable &st) const override;
    Node::SymTabEntry get_symbol() const override;

    const std::string &get_value() const { return m_value; }

private:
    std::string m_value;
};

} // namespace ast
#endif
