#include "Identifier.h"
#include "kiraz/Compiler.h"
#include <cassert>
#include <fmt/format.h>
#include <kiraz/token/Identifier.h>

namespace ast {

Identifier::Identifier(Token::Ptr t) : Node(IDENTIFIER) {
    assert(t->get_id() == IDENTIFIER);
    auto token_id = std::static_pointer_cast<const token::Identifier>(t);
    try {
        m_value = token_id->get_value();
    }
    catch (std::out_of_range &e) {
        set_error("Invalid token: out of range error");
    }
}

Identifier::Identifier(const std::string &name) : Node(IDENTIFIER), m_value(name) {
    // This constructor is used by FUNCTION1 and FUNCTION2 macros
}

Node::Ptr Identifier::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());

    auto symbol = st.get_symbol(m_value);
    if (m_value == "true" || m_value == "false") {
        return nullptr;
    }
    if (symbol.stmt == nullptr) {
        return set_error(fmt::format("Identifier '{}' is not found", m_value));
    }

    // Set the statement type
    set_stmt_type(symbol.stmt);
    return nullptr;
}

Node::SymTabEntry Identifier::get_symbol(const SymbolTable &st) const {
    return st.get_symbol(m_value);
}

Node::SymTabEntry Identifier::get_symbol() const {
    return get_cur_symtab()->get_symbol(m_value);
}
}
