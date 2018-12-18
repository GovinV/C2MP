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

[-+]?[0-9]*\.[0-9]*[fd]?([eE][-+]?[0-9]+)? {
										yylval.fvalue = atof(yytext);
										return FLOAT;
									}

[+-]?[0-9]+					{ /*[+-]?0|[1-9][0-9]*/
										yylval.value = atoi(yytext);
										return INTEGER;
									}

if									{
										return IF;
									}

while								{ // PB A REGLER : VARIABLE QUI SAPPELLE WHILE1
										return WHILE;
									}

for									{
										return FOR;
									}

[<][=]                              {
										return LTE;
									}

[>][=]                              {
										return GTE;
									}

[=][=]                              {
										return EQ;
									}

[!][=]                              {
										return NEQ;
									}


a	{ /* ([_]|[a-z]|[A-Z])([_]|[a-z]|[A-Z]|[0-9])* */
										yylval.string = strdup(yytext); 
									  	return SYMBOL; 
									}

[ \t\n]+							{} // cOUNT ONLY FIRST AFTER PRAGMA
.									{ 
										return yytext[0]; 
									}

%%

