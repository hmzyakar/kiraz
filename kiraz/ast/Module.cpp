#include "Module.h"
#include "kiraz/Compiler.h"
#include <cassert>

namespace ast {
Node::Ptr lines::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());
    for (auto line : scope_lines) {
        if (auto ret = line->add_to_symtab_forward(st)) {
            return ret;
        }
    }
    for (auto line : scope_lines) {
        if (auto ret = line->compute_stmt_type(st)) {
            return ret;
        }
    }
    return nullptr;
}

Node::Ptr Module::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());
    auto ret = m_scope->compute_stmt_type(st);
    return ret;
}

}
