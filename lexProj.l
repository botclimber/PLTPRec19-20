
%{
	#include "bisonProj.tab.h"
%}

file_name	([a-zA-Z0-9]+)\.[A-Za-z]+
var		[A-Za-z0-9]+
car		(>|<|=|!)+
_flt		([0-9]+)[.0-9]*

%%

LOAD|load	{ return LOAD; }

AS|as		{ return AS; }

SAVE|save 	{ return SAVE; }

PRINT|print	{ return PRINT; }

SELECT|select	{ return SELECT;}

FROM|from	{ return FROM; }

WHERE|where	{ return WHERE;}

DELETE|delete	{ return DELETE; }

{file_name}	{  yylval.str = strdup(yytext); return FILE_NAME; }
{_flt}		{  yylval.flt = atof(yytext); return _FLT; }
{var}		{  yylval.str = strdup(yytext); return VAR; }
{car}		{  yylval.str = strdup(yytext); return CAR; }

[ \n\t]+	{}

.		{ return yytext[0]; }

%%

int yywrap() { return 1; }
