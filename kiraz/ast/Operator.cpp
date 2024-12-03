#include "Operator.h"
#include "Identifier.h"
#include "kiraz/Compiler.h"

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

    // Check if left is builtin
    if (get_left()->get_symbol(st).name == "void" || get_left()->get_symbol(st).name == "and"
            || get_left()->get_symbol(st).name == "or"
            || get_left()->get_symbol(st).name == "not") {
        set_error(fmt::format(
                "Overriding builtin '{}' is not allowed", get_left()->get_symbol(st).name));
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
    set_cur_symtab(st.get_cur_symtab());

    // Compute left type
    if (auto ret = get_left()->compute_stmt_type(st)) {
        return ret;
    }

    // Compute right type
    if (auto ret = get_right()->compute_stmt_type(st)) {
        return ret;
    }

    // Set error if not boths sides are Integer64
    if (get_left()->get_stmt_type()->get_symbol(st).name != "Integer64"
            || get_right()->get_stmt_type()->get_symbol(st).name != "Integer64") {
        set_error(fmt::format("Addition requires integer operands, but got '{}' and '{}'",
                get_left()->get_stmt_type()->get_symbol(st).name,
                get_right()->get_stmt_type()->get_symbol(st).name));
        return shared_from_this();
    }

    // Set result type as integer
    set_stmt_type(get_left()->get_stmt_type());
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

    if (get_left()->get_stmt_type()->get_symbol(st).name != "Integer64"
            || get_right()->get_stmt_type()->get_symbol(st).name != "Integer64") {
        set_error(fmt::format(
                "Greater-than comparison requires integer operands, but got '{}' and '{}'",
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
                "Greater-than comparison requires integer operands, but got '{}' and '{}'",
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
        set_error(fmt::format(
                "Greater-than comparison requires integer operands, but got '{}' and '{}'",
                get_left()->get_stmt_type()->get_symbol(st).name,
                get_right()->get_stmt_type()->get_symbol(st).name));
        return shared_from_this();
    }

    set_stmt_type(Node::add<ast::Identifier>("Boolean"));
    return nullptr;
}

}
