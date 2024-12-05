#include "Operator.h"
#include "Identifier.h"
#include "kiraz/Compiler.h"
#include "kiraz/token/Identifier.h"
#include <iostream>

// S. A. stands for Semantic Analyze

namespace ast {

// S. A. for ASSIGNMENT OPERATION

Node::Ptr OpAssign::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());
    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }
    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }
    auto left_node_name = IDENTIFIER_STATIC_CAST(get_left())->m_value;
    auto right_node_name = IDENTIFIER_STATIC_CAST(get_right())->m_value;

    // Check if left is builtin
    if (left_node_name == "void" || left_node_name == "and" || left_node_name == "or"
            || left_node_name == "not") {
        set_error(fmt::format("Overriding builtin '{}' is not allowed", left_node_name));
        return shared_from_this();
    }

    // Check if left and right types are compatible
    if (get_left()->get_stmt_type()->get_symbol(st).name
            != get_right()->get_stmt_type()->get_symbol(st).name) {
        set_error(fmt::format("Left type '{}' of assignment does not match the right type '{}'",
                get_left()->get_stmt_type()->get_symbol(st).name,
                get_right()->get_stmt_type()->get_symbol(st).name));
        return shared_from_this();
    }

    return nullptr;
}

// S. A. for ARITHMETIC OPERATIONS

Node::Ptr OpAdd::compute_stmt_type(SymbolTable &st) {
    if (auto ret = Node::compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    auto left_type = get_left()->get_stmt_type();
    auto right_type = get_right()->get_stmt_type();

    if (left_type && right_type) {
        auto left_st_entry = left_type->get_symbol(st);
        auto right_st_entry = right_type->get_symbol(st);

        if (left_st_entry.stmt && right_st_entry.stmt) {
            if (left_st_entry.stmt->as_string() != right_st_entry.stmt->as_string()) {
                return set_error(FF("Operator '+' not defined for types 'Integer64' and 'String'"));
            }
        }
    }
    set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));
    return nullptr;
}

Node::Ptr OpSub::compute_stmt_type(SymbolTable &st) {
    if (auto ret = Node::compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    auto left_type = get_left()->get_stmt_type();
    auto right_type = get_right()->get_stmt_type();

    if (left_type && right_type) {
        auto left_st_entry = left_type->get_symbol(st);
        auto right_st_entry = right_type->get_symbol(st);

        if (left_st_entry.stmt && right_st_entry.stmt) {
            if (left_st_entry.stmt->as_string() != right_st_entry.stmt->as_string()) {
                return set_error(FF("Operator '-' not defined for types 'Integer64' and 'String'"));
            }
        }
    }
    set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));
    return nullptr;
}

Node::Ptr OpMult::compute_stmt_type(SymbolTable &st) {
    if (auto ret = Node::compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    auto left_type = get_left()->get_stmt_type();
    auto right_type = get_right()->get_stmt_type();

    if (left_type && right_type) {
        auto left_st_entry = left_type->get_symbol(st);
        auto right_st_entry = right_type->get_symbol(st);

        if (left_st_entry.stmt && right_st_entry.stmt) {
            if (left_st_entry.stmt->as_string() != right_st_entry.stmt->as_string()) {
                return set_error(FF("Operator '*' not defined for types 'Integer64' and 'String'"));
            }
        }
    }
    set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));
    return nullptr;
}

Node::Ptr OpDivF::compute_stmt_type(SymbolTable &st) {
    if (auto ret = Node::compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    auto left_type = get_left()->get_stmt_type();
    auto right_type = get_right()->get_stmt_type();

    if (left_type && right_type) {
        auto left_st_entry = left_type->get_symbol(st);
        auto right_st_entry = right_type->get_symbol(st);

        if (left_st_entry.stmt && right_st_entry.stmt) {
            if (left_st_entry.stmt->as_string() != right_st_entry.stmt->as_string()) {
                return set_error(FF("Operator '/' not defined for types 'Integer64' and 'String'"));
            }
        }
    }
    set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));
    return nullptr;
}
// S. A. for COMPARISONS (LOGIC)

Node::Ptr OpEq::compute_stmt_type(SymbolTable &st) {
    if (auto ret = Node::compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    auto left_type = get_left()->get_stmt_type();
    auto right_type = get_right()->get_stmt_type();

    if (left_type && right_type) {
        auto left_st_entry = left_type->get_symbol(st);
        auto right_st_entry = right_type->get_symbol(st);

        if (left_st_entry.stmt && right_st_entry.stmt) {
            if (left_st_entry.stmt->as_string() != right_st_entry.stmt->as_string()) {
                return set_error(
                        FF("Operator '==' not defined for types 'Integer64' and 'String'"));
            }
        }
    }
    set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));
    return nullptr;
}

Node::Ptr OpGt::compute_stmt_type(SymbolTable &st) {
    if (auto ret = Node::compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    auto left_type = get_left()->get_stmt_type();
    auto right_type = get_right()->get_stmt_type();

    if (left_type && right_type) {
        auto left_st_entry = left_type->get_symbol(st);
        auto right_st_entry = right_type->get_symbol(st);

        if (left_st_entry.stmt && right_st_entry.stmt) {
            if (left_st_entry.stmt->as_string() != right_st_entry.stmt->as_string()) {
                return set_error(FF("Operator '>' not defined for types 'Integer64' and 'String'"));
            }
        }
    }
    set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));
    return nullptr;
}

Node::Ptr OpGe::compute_stmt_type(SymbolTable &st) {
    if (auto ret = Node::compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    auto left_type = get_left()->get_stmt_type();
    auto right_type = get_right()->get_stmt_type();

    if (left_type && right_type) {
        auto left_st_entry = left_type->get_symbol(st);
        auto right_st_entry = right_type->get_symbol(st);

        if (left_st_entry.stmt && right_st_entry.stmt) {
            if (left_st_entry.stmt->as_string() != right_st_entry.stmt->as_string()) {
                return set_error(
                        FF("Operator '>=' not defined for types 'Integer64' and 'String'"));
            }
        }
    }
    set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));
    return nullptr;
}

Node::Ptr OpLe::compute_stmt_type(SymbolTable &st) {
    if (auto ret = Node::compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    auto left_type = get_left()->get_stmt_type();
    auto right_type = get_right()->get_stmt_type();

    if (left_type && right_type) {
        auto left_st_entry = left_type->get_symbol(st);
        auto right_st_entry = right_type->get_symbol(st);

        if (left_st_entry.stmt && right_st_entry.stmt) {
            if (left_st_entry.stmt->as_string() != right_st_entry.stmt->as_string()) {
                return set_error(
                        FF("Operator '<=' not defined for types 'Integer64' and 'String'"));
            }
        }
    }
    set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));
    return nullptr;
}

Node::Ptr OpLt::compute_stmt_type(SymbolTable &st) {
    if (auto ret = Node::compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    auto left_type = get_left()->get_stmt_type();
    auto right_type = get_right()->get_stmt_type();

    if (left_type && right_type) {
        auto left_st_entry = left_type->get_symbol(st);
        auto right_st_entry = right_type->get_symbol(st);

        if (left_st_entry.stmt && right_st_entry.stmt) {
            if (left_st_entry.stmt->as_string() != right_st_entry.stmt->as_string()) {
                return set_error(
                        FF("Operator '>=' not defined for types 'Integer64' and 'String'"));
            }
        }
    }
    set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));
    return nullptr;
}

}
