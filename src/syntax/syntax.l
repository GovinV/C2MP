%{
	#include <stdio.h>
	#include "utils.h"
	#include "y.tab.h"

	int pragmaOn = 0;
%}

%%


"/*"((\*+[^/*])|([^*]))*\**"*/"		{}

"//"[^\n]*"\n"						{}

#pragma[ \t]+MPC[ \t]+				{
										pragmaOn = 1;
										return PRAGMA; 
									}

[0-9]*\.[0-9]*[fd]?([eE][-+]?[0-9]+)? {
										yylval.fvalue = atof(yytext);
										return FLOAT;
									}

[0-9]+                              { /*[+-]?0|[1-9][0-9]*/
										yylval.value = atoi(yytext);
										return INTEGER;
									}

if                                  {
                                        return IF;
                                    }

else                                {
                                        return ELSE;
                                    }

while								{ // PB A REGLER : VARIABLE QUI SAPPELLE WHILE1
										return WHILE;
									}

do                                  {
                                        return DO;
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

[&][&]                              {
										return AND;
									}

[|][|]                              {
										return OR;
									}
precision 							{
										if(pragmaOn)
											return PRECISION;
									}
rounding  							{
										if(pragmaOn)
											return ROUNDING;
									}

[_a-zA-Z][_a-zA-Z0-9]*	{ /*  */
										yylval.string = strdup(yytext); 
									  	return SYMBOL; 
									}
[\n]+								{
										if(pragmaOn)
										{
											pragmaOn = 0;
											return BACKSLASH;
										}
									} // cOUNT ONLY FIRST AFTER PRAGMA
[ \t]+								{} 
.									{ 
										return yytext[0]; 
									}

%%

