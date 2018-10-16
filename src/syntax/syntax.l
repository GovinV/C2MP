%{
	#include <stdio.h>
	#include <string.h>
	#include "y.tab.h"
%}


%%



[0-9]+  { yylval = atoi(yytext);
	  return ENTIER;}
[()+*]  { return yytext[0];}

[\n]    { return yytext[0];}
.	{ printf("car inconnu : %s\n",yytext); }

%%

