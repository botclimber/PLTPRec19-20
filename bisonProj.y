%{
	#include "func.c"
	int yylex();
	int yyerror(char* message);
%}


%union {
	char* str;
	//LIST
}

%token<str> VAR
%token LOAD SAVE SELECT PRINT AS FROM  

%type<str> listvar
%%

sql : program { printf("first function (startTable)"); };

program : comand
	| comand program { printf("command program %s", $1); }
	;

comand : SELECT listvar FROM VAR AS VAR ';' 	{ printf("SELECT  %s | %s ", $4, $6); }
	| PRINT listvar FROM VAR ';' 		{ printf("[PRINT] listvar = %s | VAR = %s \n ", $2, $4); }
	| LOAD '"'VAR'"' AS VAR ';'		{ /*$$ = LoadTable(NULL, $3, $6 );*/ $$ = 'teste'; }
	;

listvar : VAR 					{ printf("%s \n", $1); }
	| listvar ',' VAR 			{ printf("listvar = %s | VAR = %s \n", $1, $3); }
	;

%%

int yyerror(char* message) {
	fprintf(stderr, "ERRO: %s \n", message);
	return 0;
}

