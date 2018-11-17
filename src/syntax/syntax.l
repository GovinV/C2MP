%{
	#include <stdio.h>
	#include <string.h>
	#include "y.tab.h"
%}

%%

"/*"((\*+[^/*])|([^*]))*\**"*/"	{ yylval = 'm'; return COMMENT; } // extremely unefficient way to catch a multiple line C comment
"//"[^\n]*"\n"			{ yylval = yytext[0]; return COMMENT; } // single line C comment
([a-z]|[A-Z])([a-z]|[A-Z]|[0-9])*	{ yylval.string = strdup(yytext); return SYMBOL; };

[a\n]				{ return yytext[0]; }
.				{ printf("car inconnu : %s\n",yytext); }


%%

