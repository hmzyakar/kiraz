// ast/Identifier.cpp
#include "Identifier.h"
#include "kiraz/Compiler.h"
#include <cassert>
#include <kiraz/token/Identifier.h>

namespace ast {

Identifier::Identifier(Token::Ptr t) : Node(IDENTIFIER) {
    assert(t->get_id() == IDENTIFIER);
    auto token_id = std::static_pointer_cast<const token::Identifier>(t);
    try {
        m_value = token_id->get_value();
    }
    catch (std::out_of_range &e) {
        // TO DO mark this node as invalid
    }
}

Identifier::Identifier(const std::string &name) : Node(IDENTIFIER), m_value(name) {
    // This constructor is used by FUNCTION1 and FUNCTION2 macros
}

Node::SymTabEntry Identifier::get_symbol(const SymbolTable &st) const {
    return st.get_symbol(m_value);
}

Node::SymTabEntry Identifier::get_symbol() const {
    return get_cur_symtab()->get_symbol(m_value);
}

}
