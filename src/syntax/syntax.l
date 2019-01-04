%{
	#include <stdio.h>
	#include "utils.h"
	#include "y.tab.h"
%}

%%


"/*"((\*+[^/*])|([^*]))*\**"*/"		{}

"//"[^\n]*"\n"						{}

#pragma[ \t]+MPC[ \t]+				{
										pragmaOn = 1;
										return PRAGMA; 
									}

[0-9]*\.[0-9]*[fd]?([eE][-+]?[0-9]+)? {
										if (pragmaBlocOn)
										{
											yylval.fvalue = atof(yytext);
											return FLOAT;
										}
										else
										{
											write_file(yytext);
										}
									}
[{]									{
										if ((pragmaBlocOn))
										{
											pragmaBlocIndex++;
											return yytext[0];
										}
										else
										{
											write_file(yytext);
										}
									}
[}]									{
										if ((pragmaBlocOn) )
										{
											pragmaBlocIndex--;
											if (pragmaBlocIndex == 0)
											{
												pragmaBlocOn = 0;
											}
											return yytext[0];
										}
										else
										{
											write_file(yytext);
										}
									}
[0-9]+                              { /*[+-]?0|[1-9][0-9]*/
										if ((pragmaBlocOn) || (pragmaOn))
										{
											yylval.value = atoi(yytext);
											return INTEGER;
										}
										else
										{
											write_file(yytext);
										}
									}

if                                  {
                                        if (pragmaBlocOn)
											return IF;
										else
										{
											write_file(yytext);
										}
                                    }

else                                {
										if (pragmaBlocOn)
                                        	return ELSE;
										else
										{
											write_file(yytext);
										}
                                    }

while								{ // PB A REGLER : VARIABLE QUI SAPPELLE WHILE1
										if (pragmaBlocOn)
											return WHILE;
										else
										{
											write_file(yytext);
										}
									}

do                                  {
										if (pragmaBlocOn)
                                        	return DO;
										else
										{
											write_file(yytext);
										}
                                    }

for									{
										if (pragmaBlocOn)
											return FOR;
										else
										{
											write_file(yytext);
										}
									}

[<][=]                              {
										if (pragmaBlocOn)
											return LTE;
										else
										{
											write_file(yytext);
										}
									}

[>][=]                              {
										if (pragmaBlocOn)
											return GTE;
										else
										{
											write_file(yytext);
										}
									}

[=][=]                              {
										if (pragmaBlocOn)
											return EQ;
										else
										{
											write_file(yytext);
										}
									}

[!][=]                              {
										if (pragmaBlocOn)
											return NEQ;
										else
										{
											write_file(yytext);
										}
									}

[&][&]                              {
										if (pragmaBlocOn)
											return AND;
										else
										{
											write_file(yytext);
										}
									}

[|][|]                              {
										if (pragmaBlocOn)
											return OR;
										else
										{
											write_file(yytext);
										}
									}
precision 							{
										if(pragmaOn)
											return PRECISION;
										else
										{
											write_file(yytext);
										}
									}
rounding  							{
										if(pragmaOn)
											return ROUNDING;
										else
										{
											write_file(yytext);
										}
									}

[_a-zA-Z][_a-zA-Z0-9]*				{
										if ((pragmaBlocOn) || (pragmaOn))
										{
											yylval.string = strdup(yytext); 
									  		return SYMBOL;
										}
										else
										{
											write_file(yytext);
										}

									}
[\n]+								{
										if(pragmaOn)
										{
											pragmaOn = 0;
											pragmaBlocOn = 1;
											return BACKSLASH;
										}
										else
										{
											write_file(yytext);
										}
									} // cOUNT ONLY FIRST AFTER PRAGMA
[ \t]+								{
										write_file(yytext);
									} 
.									{ 
										if ((pragmaBlocOn) || (pragmaOn))
											return yytext[0];
										else
										{
											write_file(yytext);
										}
									}/*

EOF									{
										close_file(void);
									}*/

%%

