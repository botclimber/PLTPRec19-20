%{
	#include "func.c"
	int yylex();
	int yyerror(char* message);
%}


%union {
	char* str;
}

%token<str> VAR
%token LOAD SAVE SELECT PRINT AS FROM  

%%

sql : program { printf("first function (startTable)"); };

program : comand
	| comand program
	;

comand : SELECT listvar FROM VAR AS VAR ';' 	{ printf("SELECT  %s | %s ", $4, $6); }
	| PRINT listvar FROM VAR ';' 		{ printf("PRINT %s ", $4); }
	| LOAD '"'VAR'"' AS VAR ';'		{ printf("works!"); }
	;

listvar : VAR 					{ printf("VAR = %s", $1); }
	| listvar ',' VAR 			{ printf("listar = %s | VAR = ", $3); }
	;

%%

int yyerror(char* message) {
	fprintf(stderr, "ERRO: %s \n", message);
	return 0;
}

