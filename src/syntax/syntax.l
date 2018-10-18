%{
	#include <stdio.h>
	#include <string.h>
	#include "y.tab.h"
%}


%%



[\ \t]+ { yylval = ' ';
	  return SPACES;}
[A-Z a-z 0-9]  { yylval = yytext[0];
	  return LETTERORNUMBER;}
[0-9]+  { yylval = atoi(yytext);
	  return ENTIER;}
[#()+*\\$]  { return yytext[0];}

[\n]    { return yytext[0];}
.	{ printf("car inconnu : %s\n",yytext); }

%%

