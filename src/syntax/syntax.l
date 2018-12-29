%{
	#include <stdio.h>
	#include <string.h>
	#include "y.tab.h"
	#include "utils.h"
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


[_a-zA-Z][_a-zA-Z0-9]*	{ /*  */
										yylval.string = strdup(yytext);
										if(pragmaOn)
										{
											int type = checkExtension(yylval.string);
											if(type == 1)
												return PRECISION;
											if(type == 2)
												return ROUNDING;
										}
										 
									  	return SYMBOL; 
									}
[\n]+								{
										if(pragmaOn)
										{
											pragmaOn = 0;
											return yytext[0];
										}
									} // cOUNT ONLY FIRST AFTER PRAGMA
[ \t\n]+							{} 
.									{ 
										return yytext[0]; 
									}

%%

