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

    // Check if the return type exists in the symtab
    if (m_type_annot) {
        auto return_type_name = IDENTIFIER_STATIC_CAST(m_type_annot)->m_value;
        auto func_name = IDENTIFIER_STATIC_CAST(m_func_name)->m_value;

        if (! st.get_symbol(return_type_name)) {
            return set_error(fmt::format(
                    "Return type '{}' of function '{}' is not found", return_type_name, func_name));
        }
    }

    // Enter a new scope for the func body
    auto func_scope_ref = st.enter_scope(ScopeType::Func, shared_from_this());

    // Add func args to the symtab
    if (m_func_arg_list) {
        auto func_arg_list = std::static_pointer_cast<FuncArgList>(m_func_arg_list);
        for (size_t i = 0; i < func_arg_list->m_identifiers.size(); ++i) {
            auto arg = func_arg_list->m_identifiers[i];
            auto arg_type = func_arg_list->m_types_list[i];

            auto arg_name = IDENTIFIER_STATIC_CAST(arg)->m_value;
            auto func_name = IDENTIFIER_STATIC_CAST(m_func_name)->m_value;
            auto arg_type_as_str = IDENTIFIER_STATIC_CAST(arg_type)->m_value;

            // Check if the arg type exists in the symtab
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

    // Check the function body for errors
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
        return set_error(fmt::format("Identifier '{}' is already in symtab", func_name_str));
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

Node::Ptr KwLet::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());

    if (! m_variable_node) {
        return set_error("Variable name is missing.");
    }

    auto var_name = IDENTIFIER_STATIC_CAST(m_variable_node)->m_value;
    // Check if name starts with uppercase

    if (st.get_symbol(var_name).first_letter_uppercase()) {
        return set_error(
                fmt::format("Variable name '{}' can not start with an uppercase letter", var_name));
    }
    // Check if variable already exists in current scope
    if (st.get_symbol(var_name)) {
        return set_error(fmt::format("Identifier '{}' is already in symtab", var_name));
    }

    // If init_value value is provided, check its type
    if (m_init_value_node) {
        if (auto ret = m_init_value_node->compute_stmt_type(st)) {
            return ret;
        }

        // Get the type of the init value
        auto init_type = m_init_value_node->get_stmt_type();
        if (! init_type) {
            return set_error("Cannot determine type of initialization expression");
        }

        // If type annotation was provided, verify it matches
        if (m_type_node) {
            auto type_name = IDENTIFIER_STATIC_CAST(m_type_node)->m_value;
            auto declared_type = st.get_symbol(type_name);
            if (! declared_type) {
                return set_error(fmt::format("Type '{}' is not found", type_name));
            }
            if (declared_type.stmt != init_type) {
                return set_error(fmt::format("Type mismatch: variable '{}' declared with type '{}' "
                                             "but initialized with type '{}'",
                        var_name, type_name, init_type->as_string()));
            }
            set_stmt_type(declared_type);
        }
        else {
            // set type from init value
            set_stmt_type(init_type);
        }
    }
    else if (m_type_node) {
        // If only type anot is provided
        auto type_name = IDENTIFIER_STATIC_CAST(m_type_node)->m_value;
        auto type_symbol = st.get_symbol(type_name);
        if (! type_symbol) {
            return set_error(fmt::format("Type '{}' is not found", type_name));
        }
        set_stmt_type(type_symbol);
    }
    else {
        return set_error("Variable declaration must have either type annotation or initialization");
    }

    // Add variable to symbol table
    st.add_symbol(var_name, shared_from_this());

    return nullptr;
}

Node::Ptr KwWhile::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());
    // Check if in a func scope

    if (! st.get_cur_symtab() || st.get_cur_symtab()->scope_type != ScopeType::Func) {
        return set_error("Misplaced while statement");
    }

    if (! m_check) {
        return set_error("While test expression is missing");
    }

    auto test_error = m_check->compute_stmt_type(st);
    if (test_error) {
        return test_error;
    }

    auto boolean_type = st.get_symbol("Boolean");

    if (m_check->get_stmt_type() != boolean_type.stmt) {
        return set_error("While only accepts tests of type 'Boolean'");
    }

    if (m_repeat) {
        auto body_error = m_repeat->compute_stmt_type(st);
        if (body_error) {
            return body_error;
        }
    }

    // While statement itself has type Void
    auto void_type = st.get_symbol("Void");
    if (void_type) {
        set_stmt_type(void_type.stmt);
    }

    return nullptr;
}

Node::Ptr KwIf::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());

    if (! st.get_cur_symtab() || st.get_cur_symtab()->scope_type != ScopeType::Func) {
        return set_error("Misplaced if statement");
    }

    if (! m_if_check) {
        return set_error("If test expression is missing");
    }

    auto test_error = m_if_check->compute_stmt_type(st);
    if (test_error) {
        return test_error;
    }

    auto boolean_type = st.get_symbol("Boolean");

    if (m_if_check->get_stmt_type() != boolean_type.stmt) {
        return set_error("If only accepts tests of type 'Boolean'");
    }

    // Check then body
    if (m_if_scope) {
        auto then_error = m_if_scope->compute_stmt_type(st);
        if (then_error) {
            return then_error;
        }
    }

    // Check else body if it exists
    if (m_else_scope) {
        auto else_error = m_else_scope->compute_stmt_type(st);
        if (else_error) {
            return else_error;
        }
    }

    auto void_type = st.get_symbol("Void");
    if (void_type) {
        set_stmt_type(void_type.stmt);
    }

    return nullptr;
}
/*
Node::Ptr KwReturn::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());

    // Check if in a func scope
    if (st.get_cur_symtab()->scope_type != ScopeType::Func) {
        return set_error("Misplaced return statement");
    }

    // Handle return expression if present
    if (m_ret_node) {
        if (auto ret = m_ret_node->compute_stmt_type(st)) {
            return ret;
        }
        auto ret_type = m_ret_node->get_stmt_type();
        if (ret_type != func_return_type) {
            auto func_type_name = func_return_type->as_string();
            auto expr_type_name = ret_type->as_string();
            return set_error(fmt::format(
                    "Return statement type '{}' does not match function return type '{}'", ret_type,
                    func_type_name));
        }
    }

    // Set the return statement's type to match the function's return type
    set_stmt_type(func_return_type);

    return nullptr;
}
*/

Node::Ptr KwClass::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());

    auto class_name = IDENTIFIER_STATIC_CAST(m_class_name)->m_value;

    // Class name must start with uppercase
    if (st.get_symbol(class_name).first_letter_lowercase()) {
        return set_error(
                fmt::format("Class name '{}' can not start with an lowercase letter", class_name));
    }

    auto class_scope = st.enter_scope(ScopeType::Class, shared_from_this());

    st.add_symbol(class_name, shared_from_this());

    if (m_class_scope) {
        auto scope_error = m_class_scope->compute_stmt_type(st);
        if (scope_error) {
            return scope_error;
        }
    }

    return nullptr;
}

Node::Ptr KwClass::add_to_symtab_forward(SymbolTable &st) {
    auto class_name = IDENTIFIER_STATIC_CAST(m_class_name)->m_value;

    if (st.get_symbol(class_name)) {
        return set_error("Class name '" + class_name + "' is already defined.");
    }

    st.add_symbol(class_name, shared_from_this());
    return nullptr;
}

}
