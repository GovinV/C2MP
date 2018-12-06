%{
	#include <stdio.h>
	#include <string.h>
	#include "y.tab.h"
%}

%%


"/*"((\*+[^/*])|([^*]))*\**"*/"		// extremely unefficient way to catch a multiple line C comment
									{	 
										yylval = 'm'; 
									  	return COMMENT; 
									}
"//"[^\n]*"\n"						// single line C comment
									{ 
										yylval = yytext[0]; 
									  	return COMMENT; 
									}
^#pragma[ \t]+MPC[ \t]+				// pragma
									{
										return PRAGMA; 
									}
[+-]?0|[1-9][0-9]*						// integer
									{
										yylval = atoi(yytext);
										return INTEGER;
									}
[+-]?(0|[1-9][0-9]*[.])?[0-9]+		// float
									{
										yylval = atof(yytext);
										return FLOAT;
									}
([_]|[a-z]|[A-Z])([_]|[a-z]|[A-Z]|[0-9])+	// text
									{ 
										yylval = strdup(yytext); 
									  	return SYMBOL; 
									}

									/********* STRUCTURES DE CONTROLE *********/
while								{
										return WHILE;
									}
if									{
										return IF;
									}
else								{
										return ELSE;
									}
for									{
										return FOR;
									}
									/********* STRUCTURES DE CONTROLE *********/
									/********* FONCTIONS MATHEMATIQUES *********/
pow									{
										return POW;
									}
sqrt								{
										return SQRT;
									}
sin									{
										return SIN;
									}
log									{
										return LOG;
									}
exp									{
										return EXP;
									}
cos									{
										return COS;
									}
									/********* FONCTIONS MATHEMATIQUES *********/
.									// other character
									{ 
										return yytext[0]; 
									}

%%

