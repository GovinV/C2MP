%{
	#include <stdio.h>
	#include <string.h>
	#include "y.tab.h"
%}


%%


[a\ ]	{ return yytext[0];}
[\n]	{ yylval = yytext[0]; return NEWLINE;}
.	{ printf("car inconnu : %s\n",yytext); }


%%

