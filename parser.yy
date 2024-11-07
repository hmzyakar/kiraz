%{
#include "lexer.hpp"

#include <kiraz/ast/Operator.h>
#include <kiraz/ast/Literal.h>
#include <kiraz/ast/Keyword.h>
#include <kiraz/ast/Identifier.h>
#include <kiraz/ast/Module.h>

#include <kiraz/token/Literal.h>
#include <kiraz/token/Identifier.h>
#include <kiraz/token/Keyword.h>
#include <kiraz/token/Operator.h>


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

%left OP_PLUS OP_MINUS
%left OP_MULT OP_DIVF

%%


program:
    lines {$$ = Node::add<ast::Module>($1);}
    | %empty
    ;

lines:
    lines line
    |line;
    ;


stmt:
    OP_LPAREN stmt OP_RPAREN {$$ = $2;}
    | addsub 
    | cmp
    | assign
    ;

import:
    KW_IMPORT identifier {$$ = Node::add<ast::KwImport>($2);}
    ;

let:
    KW_LET identifier OP_ASSIGN stmt  {$$ = Node::add<ast::KwLet>($2, nullptr, $4);}
    | KW_LET identifier OP_COLON typeanot  {$$ = Node::add<ast::KwLet>($2, $4, nullptr);}
    | KW_LET identifier OP_COLON typeanot OP_ASSIGN stmt  {$$ = Node::add<ast::KwLet>($2, $4, $6);}
    ;

return:
    KW_RETURN l_integer {$$ = Node::add<ast::KwReturn>($2);}
    | KW_RETURN l_string {$$ = Node::add<ast::KwReturn>($2);}
    | KW_RETURN cmp {$$ = Node::add<ast::KwReturn>($2);}

line:
    let OP_SEMICOLON
    |stmt 
    |stmt OP_SEMICOLON
    |import OP_SEMICOLON
    |return OP_SEMICOLON
;

addsub:
    muldiv
    |stmt OP_PLUS stmt {$$ = Node::add<ast::OpAdd>($1, $3);}
    |stmt OP_MINUS stmt {$$ = Node::add<ast::OpSub>($1, $3);}
    ;

muldiv:
    posneg
    | stmt OP_MULT stmt { $$ = Node::add<ast::OpMult>($1, $3);}
    | stmt OP_DIVF stmt { $$ = Node::add<ast::OpDivF>($1, $3);}
    ;

posneg:
    l_integer
    | OP_PLUS stmt { $$ = Node::add<ast::SignedNode>(OP_PLUS, $2);}
    | OP_MINUS stmt { $$ = Node::add<ast::SignedNode>(OP_MINUS, $2);}
    | identifier
    ;

cmp:
    identifier OP_EQUALS identifier { $$ = Node::add<ast::OpEq>($1, $3);}
    |identifier OP_GREATEREQUALS identifier { $$ = Node::add<ast::OpGe>($1, $3);}
    |identifier OP_GREATERTHAN identifier { $$ = Node::add<ast::OpGt>($1, $3);}
    |identifier OP_LESSEQUALS identifier { $$ = Node::add<ast::OpLe>($1, $3);}
    |identifier OP_LESSTHAN identifier { $$ = Node::add<ast::OpLt>($1, $3);}
    ;

assign:
    identifier OP_ASSIGN stmt { $$ = Node::add<ast::OpAssign>($1, $3);}
    ;

identifier:
    IDENTIFIER { $$ = Node::add<ast::Identifier>(curtoken);}
    ;

typeanot:
    IDENTIFIER { $$ = Node::add<ast::Identifier>(curtoken);}
    ;

l_integer:
    L_INTEGER { $$ = Node::add<ast::Integer>(curtoken);}
    ;

l_string:
    L_STRING { $$ = Node::add<ast::String>(curtoken);}
    ;
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
