
%option noyywrap

%{
// https://stackoverflow.com/questions/9611682/flexlexer-support-for-unicode/9617585#9617585
#include "parser.hpp"
#include "main.h"
#include <kiraz/token/Literal.h>
#include <kiraz/token/Operator.h>
#include <kiraz/token/Identifier.h>
#include <kiraz/token/Keyword.h>
#include <kiraz/token/Rejected.h>

static auto &colno = Token::colno;
using namespace token;
extern std::shared_ptr<Token> curtoken;
%}

%option yylineno

%%


/*Keywords*/
"return" {colno +=yyleng; curtoken = Token::New<KwReturn>(); return KW_RETURN;}
"let" {colno +=yyleng; curtoken = Token::New<KwLet>(); return KW_LET;}
"import" {colno +=yyleng; curtoken = Token::New<KwImport>(); return KW_IMPORT;}
"func" {colno +=yyleng; curtoken = Token::New<KwFunc>(); return KW_FUNC;}
"if" {colno +=yyleng; curtoken = Token::New<KwIf>(); return KW_IF;}
"else" {colno +=yyleng; curtoken = Token::New<KwElse>(); return KW_ELSE;}
"while" {colno +=yyleng; curtoken = Token::New<KwWhile>(); return KW_WHILE;}
"class" {colno +=yyleng; curtoken = Token::New<KwClass>(); return KW_CLASS;}


/*Operators*/
"{" {colno +=yyleng; curtoken = Token::New<OpLCurlyBr>(); return OP_LCURLYBR;}
"}" {colno +=yyleng; curtoken = Token::New<OpRCurlyBr>(); return OP_RCURLYBR;}
"(" {colno +=yyleng; curtoken = Token::New<OpLParen>(); return OP_LPAREN;}
")" {colno +=yyleng; curtoken = Token::New<OpRParen>(); return OP_RPAREN;}
"+" {colno +=yyleng; curtoken = Token::New<OpPlus>(); return OP_PLUS;}
"-" {colno +=yyleng; curtoken = Token::New<OpMinus>(); return OP_MINUS;}
"/" {colno +=yyleng; curtoken = Token::New<OpDivF>(); return OP_DIVF;}
"*" {colno +=yyleng; curtoken = Token::New<OpMult>(); return OP_MULT;}
"<" {colno +=yyleng; curtoken = Token::New<OpLessThan>(); return OP_LESSTHAN;}
"=" {colno +=yyleng; curtoken = Token::New<OpAssign>(); return OP_ASSIGN;}
">" {colno +=yyleng; curtoken = Token::New<OpGreaterThan>(); return OP_GREATERTHAN;}
":" {colno +=yyleng; curtoken = Token::New<OpColon>(); return OP_COLON;}
";" {colno +=yyleng; curtoken = Token::New<OpSemiColon>(); return OP_SEMICOLON;}
"," {colno +=yyleng; curtoken = Token::New<OpComma>(); return OP_COMMA;}


/*Literals*/
[0-9]+ {colno +=yyleng; curtoken = Token::New<Integer>(10,yytext); return L_INTEGER;}
\"(\\.|[^"\\])*\" {colno +=yyleng; curtoken = Token::New<String>(yytext); return L_STRING;}

/*Spacing*/
[ \n\t]+ {colno +=yyleng;}


/*Rejected*/
. {colno+=yyleng; curtoken=Token::New<Rejected>("tmp"); return YYUNDEF;}


