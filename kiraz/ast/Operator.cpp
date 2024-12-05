#include "Operator.h"
#include "Identifier.h"
#include "kiraz/Compiler.h"

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
    std::cout << "mrb cnm2\n";

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
    set_cur_symtab(st.get_cur_symtab());

    // Compute left type
    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    // Compute right type
    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    // Debugging: Check the types of the left and right operands
    auto left_type = get_left()->get_stmt_type();
    auto right_type = get_right()->get_stmt_type();

    // Set error if not both sides are Integer64 or String
    if (! left_type || ! right_type) {
        auto error_msg = fmt::format("Addition requires valid operands, but got '{}' and '{}'",
                left_type ? left_type->get_symbol(st).name : "null",
                right_type ? right_type->get_symbol(st).name : "null");
        return set_error(error_msg);
    }

    // Get the type names for cleaner code
    const auto &left_name = left_type->get_symbol(st).name;
    const auto &right_name = right_type->get_symbol(st).name;

    // Check if both operands are integers
    bool both_integers = (left_name == "Integer64" && right_name == "Integer64");
    // Check if both operands are strings
    bool both_strings = (left_name == "String" && right_name == "String");

    if (! both_integers && ! both_strings) {
        auto error_msg = fmt::format(
                "Addition requires either two integers or two strings, but got '{}' and '{}'",
                left_name, right_name);
        return set_error(error_msg);
    }

    // Set result type as integer
    set_stmt_type(left_type);
    return nullptr;
}

Node::Ptr OpSub::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    if (get_left()->get_stmt_type()->get_symbol(st).name != "Integer64"
            || get_right()->get_stmt_type()->get_symbol(st).name != "Integer64") {
        set_error(fmt::format("Subtraction requires integer operands, but got '{}' and '{}'",
                get_left()->get_stmt_type()->get_symbol(st).name,
                get_right()->get_stmt_type()->get_symbol(st).name));
        return shared_from_this();
    }

    set_stmt_type(get_left()->get_stmt_type());
    return nullptr;
}

Node::Ptr OpMult::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    if (get_left()->get_stmt_type()->get_symbol(st).name != "Integer64"
            || get_right()->get_stmt_type()->get_symbol(st).name != "Integer64") {
        set_error(fmt::format("Multiplication requires integer operands, but got '{}' and '{}'",
                get_left()->get_stmt_type()->get_symbol(st).name,
                get_right()->get_stmt_type()->get_symbol(st).name));
        return shared_from_this();
    }

    set_stmt_type(get_left()->get_stmt_type());
    return nullptr;
}

Node::Ptr OpDivF::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    if (get_left()->get_stmt_type()->get_symbol(st).name != "Integer64"
            || get_right()->get_stmt_type()->get_symbol(st).name != "Integer64") {
        set_error(fmt::format("Division requires integer operands, but got '{}' and '{}'",
                get_left()->get_stmt_type()->get_symbol(st).name,
                get_right()->get_stmt_type()->get_symbol(st).name));
        return shared_from_this();
    }

    set_stmt_type(get_left()->get_stmt_type());
    return nullptr;
}

// S. A. for COMPARISONS (LOGIC)

Node::Ptr OpEq::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    // Set error if not both have the same type
    if (get_left()->get_stmt_type()->get_symbol(st).name
            != get_right()->get_stmt_type()->get_symbol(st).name) {
        set_error(fmt::format(
                "Equality comparison requires operands of the same type, but got '{}' and '{}'",
                get_left()->get_stmt_type()->get_symbol(st).name,
                get_right()->get_stmt_type()->get_symbol(st).name));
        return shared_from_this();
    }

    // Set result type as boolean
    set_stmt_type(Node::add<ast::Identifier>("Boolean"));
    return nullptr;
}

Node::Ptr OpGt::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    // Set error if not boths sides are Integer64
    if (get_left()->get_stmt_type()->get_symbol(st).name != "Integer64"
            || get_right()->get_stmt_type()->get_symbol(st).name != "Integer64") {
        set_error(fmt::format(
                "Greater-than comparison requires integer operands, but got '{}' and '{}'",
                get_left()->get_stmt_type()->get_symbol(st).name,
                get_right()->get_stmt_type()->get_symbol(st).name));
        return shared_from_this();
    }

    // Set result type as boolean
    set_stmt_type(Node::add<ast::Identifier>("Boolean"));
    return nullptr;
}

Node::Ptr OpGe::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }
    auto right_as_str = get_left()->get_stmt_type()->get_symbol(st).name;
    return set_error(fmt::format(" right: {}", right_as_str));

    if (get_left()->get_stmt_type()->get_symbol(st).name != "Integer64"
            || get_right()->get_stmt_type()->get_symbol(st).name != "Integer64") {
        set_error(fmt::format(
                "Greater-equals comparison requires integer operands, but got '{}' and '{}'",
                get_left()->get_stmt_type()->get_symbol(st).name,
                get_right()->get_stmt_type()->get_symbol(st).name));
        return shared_from_this();
    }

    set_stmt_type(Node::add<ast::Identifier>("Boolean"));
    return nullptr;
}

Node::Ptr OpLe::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    if (get_left()->get_stmt_type()->get_symbol(st).name != "Integer64"
            || get_right()->get_stmt_type()->get_symbol(st).name != "Integer64") {
        set_error(fmt::format(
                "Less-equals comparison requires integer operands, but got '{}' and '{}'",
                get_left()->get_stmt_type()->get_symbol(st).name,
                get_right()->get_stmt_type()->get_symbol(st).name));
        return shared_from_this();
    }

    set_stmt_type(Node::add<ast::Identifier>("Boolean"));
    return nullptr;
}

Node::Ptr OpLt::compute_stmt_type(SymbolTable &st) {
    set_cur_symtab(st.get_cur_symtab());

    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    if (get_left()->get_stmt_type()->get_symbol(st).name != "Integer64"
            || get_right()->get_stmt_type()->get_symbol(st).name != "Integer64") {
        set_error(
                fmt::format("Less-than comparison requires integer operands, but got '{}' and '{}'",
                        get_left()->get_stmt_type()->get_symbol(st).name,
                        get_right()->get_stmt_type()->get_symbol(st).name));
        return shared_from_this();
    }

    set_stmt_type(Node::add<ast::Identifier>("Boolean"));
    return nullptr;
}

}
