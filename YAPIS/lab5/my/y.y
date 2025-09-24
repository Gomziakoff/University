%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char *s);

extern char *yytext;
%}

/* Определяем тип значений */
%union {
    char* str;
}

/* Токены с полезной нагрузкой */
%token <str> ID
%token <str> NUM

/* Токены без данных */
%token TRUE FALSE
%token AND OR NOT
%token IF THEN
%token EQ GT LT
%token ASSIGN

%%

program:
      /* пусто */
    | program stmt ';'   { printf("Expression evaluated\n\n"); }
;

stmt:
      logic_expr
    | if_stmt
    | assignment
    | proc_call
;

if_stmt:
      IF logic_expr THEN op {
          printf("Keyword: if\n");
          printf("Keyword: then\n");
      }
;

op:
      assignment
    | proc_call
;

assignment:
      ID ASSIGN ID {
          printf("Identifier: %s\n", $1);
          printf("Operator: =\n");
          printf("Identifier: %s\n", $3);
          free($1); free($3);
      }
;

proc_call:
      ID '(' ')' {
          printf("Procedure call: %s()\n", $1);
          free($1);
      }
;

logic_expr:
      factor
    | logic_expr AND logic_expr { printf("Operator: and\n"); }
    | logic_expr OR  logic_expr { printf("Operator: or\n"); }
    | NOT logic_expr            { printf("Operator: not\n"); }
    | logic_expr EQ logic_expr  { printf("Operator: ==\n"); }
    | logic_expr LT logic_expr  { printf("Operator: <\n"); }
    | logic_expr GT logic_expr  { printf("Operator: >\n"); }
;

factor:
      ID      { printf("Identifier: %s\n", $1); free($1); }
    | NUM     { printf("Number constant: %s\n", $1); free($1); }
    | TRUE    { printf("Boolean constant: true\n"); }
    | FALSE   { printf("Boolean constant: false\n"); }
    | '(' logic_expr ')'
;

%%

int main(void) {
    return yyparse();
}

void yyerror(const char *s) {
    fprintf(stderr, "Ошибка: %s\n", s);
}
