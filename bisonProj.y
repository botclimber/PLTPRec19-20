%{
	#include "func.c"
	int yylex();
	int yyerror(char* message);
%}


%union {
	char* str;
	float flt;
}

%token<flt> _FLT
%token<str> VAR FILE_NAME CAR
%token LOAD SAVE SELECT PRINT AS FROM WHERE  

%type<str> listvar
%%

sql 	: program
	;

program : command
	| command program
	;

command : SELECT listvar FROM VAR AS VAR ';' 			{ SelectSimple($2, $4, $6); }
	| SELECT '*' FROM VAR AS VAR WHERE VAR CAR VAR ';'	{ SelectAll($4,$6,$8, $9, $10); }
	| SELECT listvar FROM VAR AS VAR WHERE VAR CAR VAR ';'  { SelectComplex($2, $4, $6, $8, $9, $10); }
	| PRINT listvar FROM VAR WHERE VAR CAR _FLT ';'		{ Print2ColumnsVAL($2, $4, $6, $7, $8); }
	| PRINT listvar FROM VAR WHERE VAR CAR VAR ';'		{ Print2ColumnsOPR($2, $4, $6, $7, $8); }
	| PRINT '*' FROM VAR WHERE VAR CAR _FLT ';'		{ PrintAll($4,$6,$7,$8); }
	| PRINT listvar FROM VAR ';' 				{ Print2Columns($2, $4); }
	| LOAD '"'FILE_NAME'"' AS VAR ';'			{ LoadTable($3, $6); }
	| SAVE VAR AS '"'FILE_NAME'"' ';'			{ SaveTable($2, $5); }
	;

listvar : VAR 					{ $$ = $1; }
	| listvar','VAR 			{ $$ = dataStr($1,$3); }
	;

%%

int yyerror(char* message) {
	fprintf(stderr, "ERRO: %s \n", message);
	return 0;
}

