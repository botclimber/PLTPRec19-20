
%{
	#include "bisonProj.tab.h"
%}



%%

LOAD|load	{  return LOAD; }
AS|as		{ return AS; }

SAVE|save 	{  return SAVE; }

PRINT|print	{ return PRINT; }
FROM|from	{ return FROM; }

[a-z][.a-zA-Z0-9]*	{  yylval.str = strdup(yytext); return VAR; }

[ \n\t]+	{}

.		{ return yytext[0]; }

%%

int yywrap() { return 1; }
