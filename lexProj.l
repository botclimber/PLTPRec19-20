
%{
	#include "bisonProj.tab.h"
%}



%%

LOAD|load	{  return LOAD; }

SAVE|save 	{  return SAVE; }

([A-Za-z0-9]+)\.[a-z]+	{  yylval.str = strdup(yytext); return VAR; }

.		{ return yytext[0]; }

%%

int yywrap() { return 1; }
