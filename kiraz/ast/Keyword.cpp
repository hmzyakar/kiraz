#include "Keyword.h"
#include "Identifier.h"
#include "kiraz/Compiler.h"
#include <cassert>
#include <kiraz/token/Keyword.h>

namespace ast {
Node::Ptr KwFunc::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());
    if (! m_func_name) {
        return set_error("Function name is missing.");
    }

    if (m_func_arg_list && ! m_func_arg_list->is_funcarg_list()) {
        return set_error("Invalid function argument list.");
    }

    // Check if the return type exists in the symbol table
    if (m_type_annot) {
        auto return_type_name = IDENTIFIER_STATIC_CAST(m_type_annot)->m_value;
        auto func_name = IDENTIFIER_STATIC_CAST(m_func_name)->m_value;

        if (! st.get_symbol(return_type_name)) {
            return set_error(fmt::format(
                    "Return type '{}' of function '{}' is not found", return_type_name, func_name));
        }
    }

    // Enter a new scope for the function body
    auto func_scope_ref = st.enter_scope(ScopeType::Func, shared_from_this());

    // Add function arguments to the symbol table
    if (m_func_arg_list) {
        auto func_arg_list = std::static_pointer_cast<FuncArgList>(m_func_arg_list);
        for (size_t i = 0; i < func_arg_list->m_identifiers.size(); ++i) {
            auto arg = func_arg_list->m_identifiers[i];
            auto arg_type = func_arg_list->m_types_list[i];

            auto arg_name = IDENTIFIER_STATIC_CAST(arg)->m_value;
            auto func_name = IDENTIFIER_STATIC_CAST(m_func_name)->m_value;
            auto arg_type_as_str = IDENTIFIER_STATIC_CAST(arg_type)->m_value;

            // Check if the argument type exists in the symbol table
            if (! st.get_symbol(arg_type_as_str)) {
                return set_error(fmt::format(
                        "Identifier '{}' in type of argument '{}' in function '{}' is not found",
                        arg_type_as_str, arg_name, func_name));
            }

            if (st.get_symbol(arg_name)) {
                return set_error(fmt::format(
                        "Identifier '{}' in argument list of function '{}' is already in symtab",
                        arg_name, func_name));
            }
            st.add_symbol(arg_name, arg_type);
        }
    }

    // Check the function body for semantic errors
    if (m_func_scope) {
        auto scope_error = m_func_scope->compute_stmt_type(st);
        if (scope_error) {
            return scope_error;
        }
    }

    return nullptr;
}

Node::Ptr KwFunc::add_to_symtab_forward(SymbolTable &st) {
    // Add the function name to the symbol table
    if (! m_func_name) {
        return set_error("Function name is missing.");
    }

    auto func_name_str = IDENTIFIER_STATIC_CAST(m_func_name)->m_value;
    if (st.get_symbol(func_name_str)) {
        return set_error("Function name '" + func_name_str + "' is already defined.");
    }

    // Add the function to the symbol table with its type
    st.add_symbol(func_name_str, shared_from_this());
    return nullptr; // No errors
}

Node::Ptr FuncArgList::compute_stmt_type(SymbolTable &st) {
    // Validate each argument's type
    for (size_t i = 0; i < m_identifiers.size(); ++i) {
        if (! m_types_list[i]) {
            auto arg = m_identifiers[i];
            auto arg_name = IDENTIFIER_STATIC_CAST(arg)->m_value;
            return set_error("Type annotation missing for argument: " + arg_name);
        }
    }

    return nullptr; // No errors
}
}
