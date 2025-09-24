%{
#include "tab.h"
#include <string.h>
%}

%%

"true"      { return TRUE; }
"false"     { return FALSE; }

"and"       { return AND; }
"or"        { return OR; }
"not"       { return NOT; }

"if"        { return IF; }
"then"      { return THEN; }

"=="        { return EQ; }
"="         { return ASSIGN; }
"<"         { return LT; }
">"         { return GT; }

[0-9]+      { yylval.str = strdup(yytext); return NUM; }
[A-Za-z_][A-Za-z0-9_]*   { yylval.str = strdup(yytext); return ID; }

";"         { return ';'; }
"("         { return '('; }
")"         { return ')'; }

[ \t\n]     ;   /* пропуск пробелов */

.           { return yytext[0]; }

%%

int yywrap(void) { return 1; }
