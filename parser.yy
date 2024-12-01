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
%token L_BOOLEAN
%token L_FLOAT

%token IDENTIFIER

%token ARGUMENT_LIST

%token REJECTED

%left OP_PLUS OP_MINUS
%left OP_MULT OP_DIVF
%left OP_ASSIGN
%left OP_EQUALS OP_GREATERTHAN OP_LESSTHAN OP_GREATEREQUALS OP_LESSEQUALS
%%


program:
    lines {$$ = Node::add<ast::Module>($1);}
    | %empty
    ;

lines:
    lines line {ast::lines* lines_instance=(ast::lines*)$1.get();$$ = Node::add<ast::lines>(lines_instance->add_line($2));}
    |line {$$ = Node::add<ast::lines>($1); }
    ;

line:
    function OP_SEMICOLON
    |let OP_SEMICOLON
    |stmt OP_SEMICOLON
    |assign OP_SEMICOLON
    |import OP_SEMICOLON
    |return OP_SEMICOLON
    |while OP_SEMICOLON
    |if OP_SEMICOLON
    |class OP_SEMICOLON
;

stmt:
    OP_LPAREN stmt OP_RPAREN {$$ = $2;}
    | addsub 
    | cmp
    | call
    | dot
    ;

import:
    KW_IMPORT identifier {$$ = Node::add<ast::KwImport>($2);}
    ;

let:
    KW_LET identifier OP_ASSIGN stmt  {$$ = Node::add<ast::KwLet>($2, nullptr, $4);}
    |KW_LET identifier OP_COLON identifier  {$$ = Node::add<ast::KwLet>($2, $4, nullptr);}
    |KW_LET identifier OP_COLON identifier OP_ASSIGN stmt  {$$ = Node::add<ast::KwLet>($2, $4, $6);}
    ;

function:
    KW_FUNC identifier OP_LPAREN func_arg_list OP_RPAREN OP_COLON identifier OP_LCURLYBR lines OP_RCURLYBR{$$ = Node::add<ast::KwFunc>($2, $4, $7, $9); }
    |KW_FUNC identifier OP_LPAREN OP_RPAREN OP_COLON identifier OP_LCURLYBR lines OP_RCURLYBR{$$ = Node::add<ast::KwFunc>($2, nullptr, $6, $8); }
    |KW_FUNC identifier OP_LPAREN func_arg_list OP_RPAREN OP_COLON identifier OP_LCURLYBR OP_RCURLYBR{$$ = Node::add<ast::KwFunc>($2, $4, $7, nullptr); }
    |KW_FUNC identifier OP_LPAREN OP_RPAREN OP_COLON identifier OP_LCURLYBR OP_RCURLYBR{$$ = Node::add<ast::KwFunc>($2, nullptr, $6, nullptr); }
    ;
 

func_arg_list:
     identifier OP_COLON identifier { $$ = Node::add<ast::FuncArgList>($1,$3); }
    | func_arg_list OP_COMMA identifier OP_COLON identifier{ ast::FuncArgList* FuncArgListValue=(ast::FuncArgList*)$1.get();
		        $$ = Node::add<ast::FuncArgList>(FuncArgListValue->add_arg($3,$5));}
    ;


return:
    KW_RETURN stmt {$$ = Node::add<ast::KwReturn>($2);}
    ;

while:
    KW_WHILE OP_LPAREN stmt OP_RPAREN OP_LCURLYBR OP_RCURLYBR {$$ = Node::add<ast::KwWhile>($3, nullptr);}
    |KW_WHILE OP_LPAREN stmt OP_RPAREN OP_LCURLYBR lines OP_RCURLYBR {$$ = Node::add<ast::KwWhile>($3, $6);}
    ;

if:
     KW_IF OP_LPAREN identifier OP_RPAREN OP_LCURLYBR OP_RCURLYBR {$$ = Node::add<ast::KwIf>($3, nullptr, nullptr, nullptr, nullptr); }
    |KW_IF OP_LPAREN identifier OP_RPAREN OP_LCURLYBR lines OP_RCURLYBR {$$ = Node::add<ast::KwIf>($3, nullptr, $6, nullptr, nullptr); }
    |KW_IF OP_LPAREN identifier OP_RPAREN OP_LCURLYBR OP_RCURLYBR KW_ELSE OP_LCURLYBR OP_RCURLYBR{$$ = Node::add<ast::KwIf>($3, nullptr, nullptr, nullptr, nullptr); }
    |KW_IF OP_LPAREN identifier OP_RPAREN OP_LCURLYBR lines OP_RCURLYBR KW_ELSE OP_LCURLYBR OP_RCURLYBR {$$ = Node::add<ast::KwIf>($3, nullptr, $6, nullptr, nullptr); }
    |KW_IF OP_LPAREN identifier OP_RPAREN OP_LCURLYBR OP_RCURLYBR KW_ELSE OP_LCURLYBR lines OP_RCURLYBR{$$ = Node::add<ast::KwIf>($3, nullptr, nullptr, nullptr, $9); }
    |KW_IF OP_LPAREN identifier OP_RPAREN OP_LCURLYBR lines OP_RCURLYBR KW_ELSE OP_LCURLYBR lines OP_RCURLYBR {$$ = Node::add<ast::KwIf>($3, nullptr, $6, nullptr, $10); }
    |KW_IF OP_LPAREN identifier OP_RPAREN OP_LCURLYBR lines OP_RCURLYBR KW_ELSE if {$$ = Node::add<ast::KwIf>($3, nullptr, $6, nullptr, $9); }
    |KW_IF OP_LPAREN identifier OP_RPAREN OP_LCURLYBR OP_RCURLYBR KW_ELSE if {$$ = Node::add<ast::KwIf>($3, nullptr, nullptr, nullptr, $8); }
    ;

dot:  
    identifier OP_DOT identifier {  $$ = Node::add<ast::OpDot>($1, $3);} 
    | dot OP_DOT identifier {  $$ = Node::add<ast::OpDot>($1, $3);} 
    ;

call:
    identifier OP_LPAREN call_arg_list OP_RPAREN { $$ = Node::add<ast::KwCall>($1, $3); }
    | identifier OP_LPAREN OP_RPAREN { $$ = Node::add<ast::KwCall>($1, nullptr); }
    | dot OP_LPAREN OP_RPAREN { $$ = Node::add<ast::KwCall>($1, nullptr); }
    | dot OP_LPAREN call_arg_list OP_RPAREN { $$ = Node::add<ast::KwCall>($1, $3); }
    ;

call_arg_list:
    identifier { $$ = Node::add<ast::CallArgList>($1); }
    | call_arg_list OP_COMMA identifier{ ast::CallArgList* CallArgListValue=(ast::CallArgList*)$1.get();
		        $$ = Node::add<ast::CallArgList>(CallArgListValue->add_arg($3));}
    ;

class:
    KW_CLASS identifier OP_LCURLYBR OP_RCURLYBR {$$ = Node::add<ast::KwClass>($2, nullptr);}
    |KW_CLASS identifier OP_LCURLYBR lines OP_RCURLYBR {$$ = Node::add<ast::KwClass>($2, $4);}

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
    l_string
    | l_integer
    | OP_PLUS stmt { $$ = Node::add<ast::SignedNode>(OP_PLUS, $2);}
    | OP_MINUS stmt { $$ = Node::add<ast::SignedNode>(OP_MINUS, $2);}
    | identifier
    ;

cmp:
    stmt OP_EQUALS stmt { $$ = Node::add<ast::OpEq>($1, $3);}
    |stmt OP_GREATEREQUALS stmt { $$ = Node::add<ast::OpGe>($1, $3);}
    |stmt OP_GREATERTHAN stmt { $$ = Node::add<ast::OpGt>($1, $3);}
    |stmt OP_LESSEQUALS stmt { $$ = Node::add<ast::OpLe>($1, $3);}
    |stmt OP_LESSTHAN stmt { $$ = Node::add<ast::OpLt>($1, $3);}
    ;

assign:
    identifier OP_ASSIGN stmt { $$ = Node::add<ast::OpAssign>($1, $3);}
    ;

identifier:
    IDENTIFIER { $$ = Node::add<ast::Identifier>(curtoken);}
    ;


l_string:
    L_STRING  { $$ = Node::add<ast::String>(curtoken);}
    ;
 
l_integer: 
    L_INTEGER { $$ = Node::add<ast::Integer>(curtoken);}
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
