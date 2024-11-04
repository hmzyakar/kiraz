%{
#include "lexer.hpp"

#include <kiraz/ast/Operator.h>
#include <kiraz/ast/Literal.h>

#include <kiraz/token/Literal.h>

int yyerror(const char *msg);
extern std::shared_ptr<Token> curtoken;
extern int yylineno;
%}

%token KW_RETURN
%token KW_CLASS
%token KW_ELSE
%token KW_FUNC
%token KW_IF
%token KW_IMPORT
%token KW_LET
%token KW_WHILE

%token  OP_ASSIGN
%token  OP_COLON
%token  OP_COMMA
%token  OP_DIVF
%token  OP_DOT
%token  OP_EQUALS
%token  OP_GREATEREQUALS
%token  OP_GREATERTHAN
%token  OP_LCURLYBR
%token  OP_LESSEQUALS
%token  OP_LESSTHAN
%token  OP_LPAREN
%token  OP_MINUS
%token  OP_MULT
%token  OP_NOTEQUALS
%token  OP_PLUS
%token  OP_RCURLYBR
%token  OP_RPAREN
%token  OP_SEMICOLON

%token L_INTEGER
%token L_STRING

%token IDENTIFIER

%token  REJECTED

%%

/* TODO */

stmt: %empty

%%

int yyerror(const char *s) {
    if (curtoken) {
        fmt::print("** Parser Error at {}:{} at token: {}\n",
            yylineno, Token::colno, curtoken->as_string());
    }
    else {
        fmt::print("** Parser Error at {}:{}, null token\n",
            yylineno, Token::colno);
    }
    Token::colno = 0;
    Node::reset_root();
    return 1;
}
