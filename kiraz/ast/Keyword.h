#ifndef KIRAZ_AST_KEYWORD_H
#define KIRAZ_AST_KEYWORD_H

#include <cassert>
#include <kiraz/Node.h>
#include <iostream>

namespace ast {

class KwFunc : public Node{
    public:
        KwFunc(const Node::Ptr func_name, const Node::Ptr func_arg_list,const Node::Ptr type_annot,const Node::Ptr func_scope)
        : m_func_name(func_name), m_func_arg_list(func_arg_list), m_type_annot(type_annot), m_func_scope(func_scope)  { 
        }
        std::string as_string() const override{
            
            std::string func_name_as_string=m_func_name->as_string();
            std::string func_arg_list_as_string;
            std::string type_annot_as_string=m_type_annot->as_string();
            std::string m_func_scope_as_string;
            
            if (m_func_arg_list == nullptr) {
                func_arg_list_as_string = "[]";
            }
            else {
                func_arg_list_as_string = m_func_arg_list->as_string();
            }

            if (m_func_scope == nullptr) {
                m_func_scope_as_string= "[]";
            }
            else {
                m_func_scope_as_string += m_func_scope->as_string();
            }
            return fmt::format("Func(n={}, a={}, r={}, s={})",func_name_as_string, func_arg_list_as_string, type_annot_as_string, m_func_scope_as_string);
        }
    private:
        Node::Ptr m_func_name;
        Node::Ptr m_func_arg_list;
        Node::Ptr m_type_annot;
        Node::Ptr m_func_scope;
};

class FuncArgList: public Node {
    private:
        std::vector<Node::Ptr> m_identifiers;
        std::vector<Node::Ptr> m_types_list;

    public:
        FuncArgList(Node::Ptr identifier, Node::Ptr type) :Node(){
            m_identifiers.push_back(identifier);
            m_types_list.push_back(type);
        }
        FuncArgList(FuncArgList *other):Node(){ *this = *other; }
        FuncArgList *add_arg(Node::Ptr iden, Node::Ptr type) {
            m_identifiers.push_back(iden);
            m_types_list.push_back(type);
            return this;
        }

        std::string as_string() const override { 
            std::string func_arg_string = "FuncArgs([";
            for (size_t i = 0; i < m_identifiers.size(); i++) {
                func_arg_string += "FArg(n=" + m_identifiers[i]->as_string() + ", t=" + m_types_list[i]->as_string() + ")";
                if (i != m_identifiers.size() - 1) {
                    func_arg_string += ", ";
                }
            }
        func_arg_string = func_arg_string+"])";
        return func_arg_string;
        }   
};


class lines : public Node {
    private:
    std::vector<Node::Ptr> scope_lines;
    public:
        lines(Node::Ptr line) :Node(){ scope_lines.push_back(line); }
        lines(lines *other) :Node() { *this = *other; }
        lines *add_line(Node::Ptr line) {
            scope_lines.push_back(line);
            return this;
        }
    std::string as_string() const override {
        std::string node_list_string = "[";
        for (int i = 0; i < scope_lines.size(); i++) {
            node_list_string += scope_lines[i]->as_string();
            if (i != scope_lines.size() - 1) {
                node_list_string += ", ";
            }
        }
        node_list_string =node_list_string+ "]";
        return node_list_string;
    }

};


class KwLet : public Node {

public:
    KwLet(Node::Ptr variable_node, Node::Ptr type_node, Node::Ptr init_value_node)
            : m_variable_node(variable_node)
            , m_type_node(type_node)
            , m_init_value_node(init_value_node) {}

    std::string as_string() const override {
        std::string m_variable_node_string = m_variable_node->as_string();
        std::string m_type_node_string;
        std::string m_init_value_node_string;
        if (m_type_node == nullptr) {
            m_init_value_node_string = m_init_value_node->as_string();
            return fmt::format("Let(n={}, i={})", m_variable_node_string, m_init_value_node_string);
        }
        else if (m_init_value_node == nullptr) {
            m_type_node_string = m_type_node->as_string();
            return fmt::format("Let(n={}, t={})", m_variable_node_string, m_type_node_string);
        }
        else {
            m_type_node_string = m_type_node->as_string();
            m_init_value_node_string = m_init_value_node->as_string();
            return fmt::format("Let(n={}, t={}, i={})", m_variable_node_string, m_type_node_string,
                    m_init_value_node_string);
        }
    }

private:
    Node::Ptr m_variable_node, m_type_node, m_init_value_node;
};

class KwImport : public Node {

public:
    KwImport(Node::Ptr lib_node) : m_lib_node(lib_node) {}
    std::string as_string() const override {
        return fmt::format("Import({})", m_lib_node->as_string());
    }

private:
    Node::Ptr m_lib_node;
};

class KwReturn : public Node {

public:
    KwReturn(Node::Ptr ret_node) : m_ret_node(ret_node) {}
    std::string as_string() const override {
        return fmt::format("Return({})", m_ret_node->as_string());
    }

private:
    Node::Ptr m_ret_node;
};

class KwWhile : public Node{
    public:
        KwWhile(Node::Ptr check, Node::Ptr repeat) : m_check(check), m_repeat(repeat){ 
        }
        std::string as_string() const override{
            std::string check_as_string=m_check->as_string();
            std::string repeat_as_string;            
            if (m_repeat == nullptr) {repeat_as_string = "[]";}
            else {repeat_as_string = m_repeat->as_string();}
            return fmt::format("While(?={}, repeat={})",check_as_string, repeat_as_string);
        }
    private:
        Node::Ptr m_check;
        Node::Ptr m_repeat;
};



class KwIf : public Node{
    public:
        KwIf(Node::Ptr if_check,Node::Ptr else_if_check,Node::Ptr if_scope,Node::Ptr else_if_scope,Node::Ptr else_scope)
        : m_if_check(if_check),m_else_if_check(else_if_check),m_if_scope(if_scope),m_else_if_scope(else_if_scope),m_else_scope(else_scope){}
        std::string as_string() const override{
            std::string if_check_as_string=m_if_check->as_string();
            std::string if_scope_as_string;
            std::string else_scope_as_string;
            if (m_if_scope == nullptr) {if_scope_as_string = "[]";}
            else {if_scope_as_string = m_if_scope->as_string();}
            if (m_else_scope == nullptr) {else_scope_as_string= "[]";}
            else {else_scope_as_string += m_else_scope->as_string();}
            return fmt::format("If(?={}, then={}, else={})",if_check_as_string,if_scope_as_string,else_scope_as_string);
}
    private:
        Node::Ptr m_if_check;
        Node::Ptr m_else_if_check;
        Node::Ptr m_if_scope;
        Node::Ptr m_else_if_scope;
        Node::Ptr m_else_scope;
};

class KwClass : public Node {
private:
    Node::Ptr m_class_name; 
    Node::Ptr m_class_scope; 

public:
    KwClass(const Node::Ptr class_name, const Node::Ptr class_scope = nullptr)
        : m_class_name(class_name), m_class_scope(class_scope){
        assert(class_name); 
    }
    std::string as_string() const override{
        std::string class_name_as_string=m_class_name->as_string();
        std::string m_class_scope_as_string;
    
        if (m_class_scope == nullptr){
            m_class_scope_as_string = "[]";                
        }
        else {
            m_class_scope_as_string = m_class_scope->as_string();
        }
        return fmt::format("Class(n={}, s={})",class_name_as_string, m_class_scope_as_string );
    }
};


class KwCall : public Node{
    public:
        KwCall(const Node::Ptr func_name, const Node::Ptr arg_list) : m_func_name(func_name), m_arg_list(arg_list) {}
        std::string as_string() const override{
            std::string func_name_as_string=m_func_name->as_string();
            std::string arg_list_as_string;
            
            if (m_arg_list == nullptr) {
                arg_list_as_string = "[]";
            }
            else {
                arg_list_as_string = m_arg_list->as_string();
            } 
            return fmt::format("Call(n={}, a={})",func_name_as_string, arg_list_as_string);
        }
    private:
        Node::Ptr m_func_name;
        Node::Ptr m_arg_list;
};


class CallArgList: public Node {
    private:
        std::vector<Node::Ptr> m_identifiers;

    public:
        CallArgList(Node::Ptr identifier) :Node(){
            m_identifiers.push_back(identifier);
        }

        CallArgList(CallArgList *other):Node(){ *this = *other; }
        CallArgList *add_arg(Node::Ptr iden) {
            m_identifiers.push_back(iden);
            return this;
        }
        
        std::string as_string() const override { 
            /*if (m_identifiers.size() == 1){
                return m_identifiers[0]->as_string();
            }*/
            std::string func_arg_string = "FuncArgs([";
            for (size_t i = 0; i < m_identifiers.size(); i++) {
                func_arg_string +=m_identifiers[i]->as_string();
                if (i != m_identifiers.size() - 1) {
                    func_arg_string += ", ";
                }
            }

        func_arg_string = func_arg_string+"])";
        return func_arg_string;
        }   
};
};

#endif // KIRAZ_AST_OPERATOR_H
