%{
	#include <stdio.h>
	#include "utils.h"
	#include "y.tab.h"
%}

%option nounput
%option noinput

%%


"/*"((\*+[^/*])|([^*]))*\**"*/"		{
										if (!(pragmaBlocOn || pragmaOn))
											write_file(yytext); 
									}

"//"[^\n]*"\n"						{ 

										if (!(pragmaBlocOn || pragmaOn))
											write_file(yytext);									}

#pragma[ \t]+MPC[ \t]+				{
										if (pragmaMet == 0)
										{
											pragmaOn = 1;
											write_file("\n");
											return PRAGMA;
										}
										else 
										{
											//printf("yytext pragma : %s\n", yytext);
											write_file(yytext);
										}
									}

[0-9]*\.[0-9]*[fd]?([eE][-+]?[0-9]+)? {
										if (pragmaBlocOn)
										{
											yylval.fvalue = atof(yytext);
											return FLOAT;
										}
										else
										{
											//printf("yytext float : %s\n", yytext);
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
											//printf("yytext {: %s\n", yytext);
											write_file(yytext);
										}
									}
[}]									{
										if ((pragmaBlocOn) )
										{
											pragmaBlocIndex--;
											if (pragmaBlocIndex == 0)
											{
												pragmaMet = 1;
												pragmaBlocOn = 0;
											}
											return yytext[0];
										}
										else
										{

											//printf("yytext } : %s\n", yytext);
											write_file(yytext);
										}
									}
[0-9]+                              {
										if ((pragmaBlocOn) || (pragmaOn))
										{
											yylval.value = atoi(yytext);
											return INTEGER;
										}
										else
										{
											//printf("yytext integer : %s\n", yytext);
											write_file(yytext);
										}
									}

if                                  {
                                        if (pragmaBlocOn)
											return IF;
										else
										{
											//printf("yytext if : %s\n", yytext);
											write_file(yytext);
										}
                                    }

else                                {
										if (pragmaBlocOn)
                                        	return ELSE;
										else
										{
											//printf("yytext else : %s\n", yytext);
											write_file(yytext);
										}
                                    }

while								{ // PB A REGLER : VARIABLE QUI SAPPELLE WHILE1
										if (pragmaBlocOn)
											return WHILE;
										else
										{
											//printf("yytext while : %s\n", yytext);
											write_file(yytext);
										}
									}

do                                  {
										if (pragmaBlocOn)
                                        	return DO;
										else
										{
											//printf("yytext do : %s\n", yytext);
											write_file(yytext);
										}
                                    }

for									{
										if (pragmaBlocOn)
											return FOR;
										else
										{
											//printf("yytext for : %s\n", yytext);
											write_file(yytext);
										}
									}

[<][=]                              {
										if (pragmaBlocOn)
											return LTE;
										else
										{
											//printf("yytext <=: %s\n", yytext);
											write_file(yytext);
										}
									}

[>][=]                              {
										if (pragmaBlocOn)
											return GTE;
										else
										{
											//printf("yytext >=: %s\n", yytext);
											write_file(yytext);
										}
									}

[=][=]                              {
										if (pragmaBlocOn)
											return EQ;
										else
										{
											//printf("yytext == : %s\n", yytext);
											write_file(yytext);
										}
									}

[!][=]                              {
										if (pragmaBlocOn)
											return NEQ;
										else
										{
											//printf("yytext !=: %s\n", yytext);
											write_file(yytext);
										}
									}

[&][&]                              {
										if (pragmaBlocOn)
											return AND;
										else
										{
											//printf("yytext &&: %s\n", yytext);
											write_file(yytext);
										}
									}

[|][|]                              {
										if (pragmaBlocOn)
											return OR;
										else
										{
											//printf("yytext || : %s\n", yytext);
											write_file(yytext);
										}
									}
precision 							{
										if(pragmaOn)
											return PRECISION;
										else
										{
											//printf("yytext precision: %s\n", yytext);
											write_file(yytext);
										}
									}
rounding  							{
										if(pragmaOn)
											return ROUNDING;
										else
										{
											//printf("yytext rounding: %s\n", yytext);
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
											//printf("yytext symbol: %s\n", yytext);
											write_file(yytext);
										}

									}
[\n]+								{
										if ((pragmaOn))
										{
											pragmaOn = 0;
											pragmaBlocOn = 1;
											return BACKSLASH;
										}
										else if (!(pragmaBlocOn))
										{
											//printf("yytext saut: %s\n", yytext);
											write_file(yytext);
										}
									} // cOUNT ONLY FIRST AFTER PRAGMA
[ \t]+								{
										if (!(pragmaBlocOn || pragmaOn))
											write_file(yytext);
									} 
.									{ 
										if ((pragmaBlocOn) || (pragmaOn))
											return yytext[0];
										else
										{
											//printf("yytext carac spec : %s\n", yytext);
											write_file(yytext);
										}
									}


%%

