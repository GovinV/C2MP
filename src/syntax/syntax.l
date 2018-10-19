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
<<EOF>> { static int once = 0; return once++ ? 0 : '\n'; }
.	{ printf("car inconnu : %s\n",yytext); }


%%

