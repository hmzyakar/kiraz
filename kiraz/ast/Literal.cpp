
#include "Literal.h"
#include "kiraz/Compiler.h"
#include <cassert>
#include <iostream>
#include <kiraz/token/Literal.h>

namespace ast {
Integer::Integer(Token::Ptr t) : Node(L_INTEGER) {
    assert(t->get_id() == L_INTEGER);
    auto token_int = std::static_pointer_cast<const token::Integer>(t);
    auto base = token_int->get_base();
    try {
        m_value = std::stoll(token_int->get_value(), nullptr, base);
    }
    catch (std::out_of_range &e) {
    }
}

Node::Ptr Integer::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());

    auto integer64 = st.get_symbol("Integer64").stmt;
    if (! integer64) {
        return set_error("Integer64 type not found");
    }
    set_stmt_type(integer64);

    return nullptr;
}
Node::SymTabEntry Integer::get_symbol(const SymbolTable &st) const {
    auto integer64 = st.get_symbol("Integer64");
    if (! integer64) {
        return {};
    }

    return integer64;
}

String::String(Token::Ptr t) : Node(L_STRING) {
    assert(t->get_id() == L_STRING);
    auto token_str = std::static_pointer_cast<const token::String>(t);
    try {
        m_value = token_str->get_value();
    }
    catch (std::out_of_range &e) {
    }
}

Node::Ptr String::compute_stmt_type(SymbolTable &st) {
    set_stmt_type(st.get_symbol("String").stmt);
    return nullptr;
}

Node::SymTabEntry String::get_symbol(const SymbolTable &st) const {
    auto string = st.get_symbol("String");
    if (! string) {
        return {};
    }
    return string;
}
}
