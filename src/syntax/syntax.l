%{
	#include <stdio.h>
	#include <string.h>
	#include "y.tab.h"
%}

%%


"/*"((\*+[^/*])|([^*]))*\**"*/"		{}

"//"[^\n]*"\n"						{}

#pragma[ \t]+MPC[ \t]+				{
										return PRAGMA; 
									}

[+-]?0|[1-9][0-9]*					{
										yylval.value = atoi(yytext);
										return INTEGER;
									}

[+-]?(0|[1-9][0-9]*[.])?[0-9]+		{
										yylval.fvalue = atof(yytext);
										return FLOAT;
									}


([_]|[a-z]|[A-Z])([_]|[a-z]|[A-Z]|[0-9])*	{ 
										yylval.string = strdup(yytext); 
									  	return SYMBOL; 
									}

[ \t\n]+							{}
.									{ 
										return yytext[0]; 
									}

%%

