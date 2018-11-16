%{
	#include <stdio.h>
	#include <string.h>
	#include "y.tab.h"
%}

%%

"/*"((\*+[^/*])|([^*]))*\**"*/"	{ yylval = 'm'; return COMMENT; } // extremely unefficient way to catch a multiple line C comment
"//"[^\n]*"\n"			{ yylval = yytext[2]; return COMMENT; } // single line C comment

[a\n]				{ return yytext[0]; }
.				{ printf("car inconnu : %s\n",yytext); }


%%

