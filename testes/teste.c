#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "bisonProj.tab.h"

//struct
typedef struct cell{
	float val;
	struct cell *next;
}cells;

typedef struct line{
	cells *cell;
	struct line *next;
}lines;

typedef struct title{
	char *title;
	struct title *next;
}titles;


typedef struct table{
	char *nome;
	titles *titulos;
	lines *linhas;
	struct table *next;
}tabels;

struct table *t = NULL;

//prototipos de funções ->LOAD "ficheiro.tsv" AS notas;
void LoadTable(char *nome_fich, char *nome_tab);
//<faz parte do load>
titles *InsertTitles(FILE *f, int tam, int now);
lines *InsertLines(FILE *f, int tam, int now);
cells *InsertCells(FILE *f, int tam, int now);
int charcount( FILE *const fin );
//</faz parte do load>
//imprimir 2 colunas ->PRINT nr, nota1 FROM notas;
void Print2Columns(char *columns, char *name_tab);
//PRINT nr, nota1 FROM notas WHERE nota3 < 10;
void Print2ColumnsVAL(char *columns, char *name_tab, char *column3, char *operador, float val);
//PRINT nr, nota1 FROM notas WHERE nota3 < nota1;
void Print2ColumnsOPR(char *columns, char *name_tab, char *column3, char *operador, char *column4);
//SELECT nr, nota1, nota3 FROM notas AS pauta;
void SelectSimple(char *columns, char *name_tab, char *new_tab);
titles *SelectTitles(char *column1, char *column2, char *column3, int count);
lines *SelectLines(lines *linha, int col1, int col2, int col3);
cells *SelectCells(cells *celula, int col1, int col2, int col3, int count);
int num_col(titles *titulos, char *name);
//SELECT nr, nota1, nota3 FROM notas AS pauta WHERE nota2 < nota3;
void SelectComplex(char *columns, char *name_tab, char *new_tab, char *column4, char *operador, char *column5);
lines *SelectLinesOPR(lines *linha, int col1, int col2, int col3, int col4, int col5, char *operador);
cells *PrintCells(cells *point, int col1, int col2, int col3, int count);
//PRINT * FROM notas WHERE nota3 < 10;
void PrintAll(char *name_tab, char *column1, char *operador, float grade);
void PrintAllSimple(lines *linha, int col1, char *operador, float grade);
//SELECT * FROM notas AS pauta WHERE nota2 < nota3;
void SelectAll(char *name_tab, char *new_tab, char *column1, char *operador, char *column2);
titles *SelectAllTitles(titles *titulo);
lines *SelectAllLines(lines *linha, int col1, int col2, char *operador);
cells *SelectALLCells(cells *celula);
//SAVE notas AS "ficheiro2.tsv";
void SaveTable(char *nameTab, char *nameFich);
char *dataStr(char *param1, char *param2); // CONCATENA

//Funções
int main(){

	//LOAD "ficheiro.tsv" AS notas;
	LoadTable("teste2.txt", "notas");
	//PrintAll("notas","nota1","<",20);
	SelectComplex("nr,nota1,nota2", "notas", "xyz", "nota1", "<", "nota2");
	PrintAll("xyz","nota1","<",20);
	SelectComplex("nr,nota1,nota2", "notas", "xyz", "nota1", "<", "nota2");
	PrintAll("xyz","nota1","<",20);

	//SelectSimple("nota1,nota2,nota3", "notas", "xyz");	
	//PrintAll("xyz","nota1","<",20);

	/*printf("Com sucesso1!\n\n");
        LoadTable("teste2.txt", "hoje2");
	printf("Com sucesso2!\n\n");
        LoadTable("teste3.txt", "hoje3");
	printf("Com sucesso3!\n\n");
        //PRINT nr, nota1 FROM notas;
        Print2Columns("nota3,nota1", "hoje3");
        printf("\n\n");
        //PRINT nr, nota1 FROM notas WHERE nota3 < 10;
        Print2ColumnsVAL("nota1,nota2", "hoje2", "notaF", ">=", 10);
        puts("\n\n");
        //PRINT nr, nota1 FROM notas WHERE nota3 < nota1;
        Print2ColumnsOPR("nota1,nota2", "hoje1", "nota3", "<", "nota1");
        puts("\n\n");
        //SELECT nr, nota1, nota3 FROM notas AS pauta;
        SelectSimple("nr,nota2,nota1", "hoje3", "hoje4");
        Print2Columns("nr,nota1", "hoje4");
        puts("\n\n");
        //SELECT nr, nota1, nota3 FROM notas AS pauta WHERE nota2 < nota3;
        SelectComplex("nr,nota1,nota3", "hoje3", "hoje5", "nota1", "<", "nota3");
        Print2Columns("nr,nota3", "hoje5");
        puts("\n\n");
        //PRINT * FROM notas WHERE nota3 < 10;
        PrintAll("hoje3", "nota1", "<", 20);
        //puts("\n\n");
        //SELECT * FROM notas AS pauta WHERE nota2 < nota3;
        //SelectAll("hoje3", "daniel", "nr", "<", "nota2");
        //PrintAll("daniel", "nota1", "<", 20);
        //puts("\n\n");
        //SAVE notas AS "ficheiro2.tsv";
        //SaveTable("hoje4", "danielPanilas.txt");
        //system("pause");
	/*yyparse();
	return 0;*/

	//yyparse();
	return 0;
}

char *dataStr(char *param1, char *param2){
	
	strcat(param1, ",");
	return strcat(param1 ,param2);
}

//cria nova tabela com o nome dado pelo user

void LoadTable(char *nome_fich, char *nome_tab) {
    tabels *aux_t, *new;
    FILE *f;
    int i = 0;


    aux_t = t;

    f = fopen(nome_fich, "rt");
    if (f == NULL) {
        printf("Erro ao abrir o ficheiro %s!\n", nome_fich);

    }

    i = charcount(f);
    fseek(f, 0, SEEK_SET);

    if (t == NULL) {
        t = (tabels *) malloc(sizeof (tabels));
        t->next = NULL;
        t->nome = nome_tab;
        t->titulos = InsertTitles(f, i, 0);
        t->linhas = InsertLines(f, i, 0);
    
    } else {
        while (aux_t->next != NULL)
            if ((strcmp(aux_t->nome, nome_tab)) == 0) {
                printf("A tabela com o nome %s já existe!\n", nome_tab);
      
            } else
                aux_t = aux_t->next;
        if ((strcmp(aux_t->nome, nome_tab)) == 0) {
            printf("A tabela com o nome %s já existe!\n", nome_tab);
   
        } else {
            new = (tabels *) malloc(sizeof (tabels));
            new->nome = nome_tab;
            new->titulos = InsertTitles(f, i, 0);
            new->linhas = InsertLines(f, i, 0);
            new->next = NULL;
            aux_t->next=new;
        }
    }


    fclose(f);

}

//tamanho da 1 linha
int charcount( FILE *fin ){
    int count;
    char c;
    count = 0;
    c=fgetc(fin);
    for( ;; )
    {
        c = fgetc( fin );
        if( c == '\n' ){
            ++count;
            return count;
        }
        if (c=='\t')
            ++count;
    }
}

//Acrescenta novos titulos
titles *InsertTitles(FILE *f, int tam, int now){
    titles *aux;

    aux=(titles *)malloc(sizeof(titles));
    aux->title = strdup("a");
    //now+=(fscanf(f, "%s", aux->title));
    now= now + fscanf(f, "%s", aux->title);
    if (now<tam){
    	aux->next=InsertTitles(f, tam, now);
    }else{
	aux->next=NULL;
    }

    return aux;
}

//inserir valores nas linhas e celulas
lines *InsertLines(FILE *f, int tam, int now){
    lines *aux_line;
    if(feof(f)){
        return NULL;        
    }else{
        aux_line=(lines *)malloc(sizeof(lines));
        aux_line->cell=InsertCells(f, tam, 0);
        now++;
        aux_line->next=InsertLines(f, tam, now);     
        }
    
    return aux_line;
}

cells *InsertCells(FILE *f, int tam, int now){
        float i;
        cells *new;
	new=(cells *)malloc(sizeof(cells));
        new->val=0;
        //now= now + fscanf(f, "%f", new->val);
        now+=(fscanf(f, "%f", &i));
        new->val=i;
	if (now < tam)
		new->next=InsertCells(f, tam, now);
	else{
		new->next=NULL;
	}
	return new;
}

void Print2Columns(char *columns, char *name_tab) {
    tabels *aux_t = t;
    titles *aux_tit;
    lines *aux_l;
    cells *aux_c;
    int col1 = 0, col2 = 0;
	
	/*------------- SPLIT MADE BY PROFESSIONAL DEVELOPER -----------------*/
	char *param = columns;
	int tam, i = 0;	
	char col[500];
	char *token;	
	
	for (tam = 0; param[tam]; param[tam] == ',' ? tam++ : *param++);	
	char cols[tam+1][150];

	strcpy(col, columns);
	token = strtok(col, ",");
	
	while(token != NULL){
		strcpy(cols[i], token); i++;
		token = strtok(NULL, ",");
	}
	/*-------------------------------------------------------------------*/	


    while ((strcmp(aux_t->nome, name_tab)) != 0)
        aux_t = aux_t->next;

    aux_l = aux_t->linhas;
    aux_tit = aux_t->titulos;



    while (aux_tit != NULL && (strcmp(aux_tit->title, cols[0])) != 0) {
        aux_tit = aux_tit->next;
        col1++;
    }
    if (aux_tit == NULL) {
        printf("Nao existe a coluna %s\n", cols[0]);
        return;
    }

    aux_tit = aux_t->titulos;

    while (aux_tit != NULL && (strcmp(aux_tit->title, cols[1])) != 0) {
        aux_tit = aux_tit->next;
        col2++;
    }
    if (aux_tit == NULL) {
        printf("Nao existe a coluna %s\n", cols[1]);
        return;
    }

    aux_tit = aux_t->titulos;
  
    if (col1 < col2) {
        for (int i = 0; i < col1; i++)
            aux_tit = aux_tit->next;

        printf("%s\t", aux_tit->title);

        for (int i = col1; i < col2; i++)
            aux_tit = aux_tit->next;

        printf("%s\n", aux_tit->title);

        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            for (int i = 0; i < col1; i++)
                aux_c = aux_c->next;
            printf("%.1f\t", aux_c->val);
            for (int i = col1; i < col2; i++)
                aux_c = aux_c->next;
            printf("%.1f\n", aux_c->val);
            aux_l = aux_l->next;
        }
    } else {
        for (int i = 0; i < col2; i++)
            aux_tit = aux_tit->next;

        printf("%s\t", aux_tit->title);

        for (int i = col2; i < col1; i++)
            aux_tit = aux_tit->next;

        printf("%s\n", aux_tit->title);

        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            for (int i = 0; i < col2; i++)
                aux_c = aux_c->next;
            printf("%.1f\t", aux_c->val);
            for (int i = col2; i < col1; i++)
                aux_c = aux_c->next;
            printf("%.1f\n", aux_c->val);
            aux_l = aux_l->next;
        }
    }
}

void Print2ColumnsVAL(char *columns, char *name_tab, char *column3, char *operador, float val){

    tabels *aux_t = t;
    titles *aux_tit;
    lines *aux_l;
    cells *aux_c;
    int col1 = 0, col2 = 0, col3 = 0, aux = 0;
	
	/*------------- SPLIT MADE BY PROFESSIONAL DEVELOPER -----------------*/
	char *param = columns;
	int tam, i = 0;	
	char col[500];
	char *token;	
	
	for (tam = 0; param[tam]; param[tam] == ',' ? tam++ : *param++);	
	char cols[tam+1][150];

	strcpy(col, columns);
	token = strtok(col, ",");
	
	while(token != NULL){
		strcpy(cols[i], token); i++;
		token = strtok(NULL, ",");
	}
	/*-------------------------------------------------------------------*/	

    while ((strcmp(aux_t->nome, name_tab)) != 0)
        aux_t = aux_t->next;

    aux_l = aux_t->linhas;
    aux_tit = aux_t->titulos;

    while (aux_tit != NULL && (strcmp(aux_tit->title, cols[0])) != 0) {
        aux_tit = aux_tit->next;
        col1++;
    }
    if (aux_tit == NULL) {
        printf("Nao existe a coluna %s\n", cols[0]);
        return;
    }

    aux_tit = aux_t->titulos;

    while (aux_tit != NULL && (strcmp(aux_tit->title, cols[1])) != 0) {
        aux_tit = aux_tit->next;
        col2++;
    }
    if (aux_tit == NULL) {
        printf("Nao existe a coluna %s\n", cols[1]);
        return;
    }

    aux_tit = aux_t->titulos;

    while (aux_tit != NULL && (strcmp(aux_tit->title, column3)) != 0) {
        aux_tit = aux_tit->next;
        col3++;
    }
    if (aux_tit == NULL) {
        printf("Nao existe a coluna %s\n", column3);
        return;
    }


    aux_tit = aux_t->titulos;

    if (col1 > col2) {
        aux = col1;
        col1 = col2;
        col2 = aux;
    }

    for (int i = 0; i < col1; i++)
        aux_tit = aux_tit->next;

    printf("%s\t", aux_tit->title);

    for (int i = col1; i < col2; i++)
        aux_tit = aux_tit->next;

    printf("%s\n", aux_tit->title);



    if ((strcmp(operador, "<")) == 0) {
        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            for (int i = 0; i < col3; i++)
                aux_c = aux_c->next;
            if (aux_c->val < val) {
                aux_c = aux_l->cell;
                for (int i = 0; i < col1; i++)
                    aux_c = aux_c->next;
                printf("%.1f\t", aux_c->val);
                for (int i = col1; i < col2; i++)
                    aux_c = aux_c->next;
                printf("%.1f\n", aux_c->val);

            }
            aux_l = aux_l->next;

        }

    } else if ((strcmp(operador, "<=")) == 0) {
        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            for (int i = 0; i < col3; i++)
                aux_c = aux_c->next;
            if (aux_c->val <= val) {
                aux_c = aux_l->cell;
                for (int i = 0; i < col1; i++)
                    aux_c = aux_c->next;
                printf("%.1f\t", aux_c->val);
                for (int i = col1; i < col2; i++)
                    aux_c = aux_c->next;
                printf("%.1f\n", aux_c->val);

            }
            aux_l = aux_l->next;

        }
    } else if ((strcmp(operador, ">")) == 0) {
        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            for (int i = 0; i < col3; i++)
                aux_c = aux_c->next;
            if (aux_c->val > val) { 
                aux_c = aux_l->cell;
                for (int i = 0; i < col1; i++)
                    aux_c = aux_c->next;
                printf("%.1f\t", aux_c->val);
                for (int i = col1; i < col2; i++)
                    aux_c = aux_c->next;
                printf("%.1f\n", aux_c->val);

            }
            aux_l = aux_l->next;


        }
    } else if ((strcmp(operador, ">=")) == 0) {
        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            for (int i = 0; i < col3; i++)
                aux_c = aux_c->next;
            if (aux_c->val >= val) { 
                aux_c = aux_l->cell;
                for (int i = 0; i < col1; i++)
                    aux_c = aux_c->next;
                printf("%.1f\t", aux_c->val);
                for (int i = col1; i < col2; i++)
                    aux_c = aux_c->next;
                printf("%.1f\n", aux_c->val);

            }
            aux_l = aux_l->next;


        }
    } else if ((strcmp(operador, "=")) == 0 || (strcmp(operador, "==")) == 0) {
        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            for (int i = 0; i < col3; i++)
                aux_c = aux_c->next;
            if (aux_c->val == val) { 
                aux_c = aux_l->cell;
                for (int i = 0; i < col1; i++)
                    aux_c = aux_c->next;
                printf("%.1f\t", aux_c->val);
                for (int i = col1; i < col2; i++)
                    aux_c = aux_c->next;
                printf("%.1f\n", aux_c->val);

            }
            aux_l = aux_l->next;


        }
    } else if ((strcmp(operador, "!=")) == 0) {
        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            for (int i = 0; i < col3; i++)
                aux_c = aux_c->next;
            if (aux_c->val != val) { /*AQUI*/
                aux_c = aux_l->cell;
                for (int i = 0; i < col1; i++)
                    aux_c = aux_c->next;
                printf("%.1f\t", aux_c->val);
                for (int i = col1; i < col2; i++)
                    aux_c = aux_c->next;
                printf("%.1f\n", aux_c->val);

            }
            aux_l = aux_l->next;


        }
    }else{
        printf("\nOperador nao reconhecido!\n");
        return;
    }
}

void Print2ColumnsOPR(char *columns, char *name_tab, char *column3, char *operador, char *column4){
    tabels *aux_t = t;
    titles *aux_tit;
    lines *aux_l;
    cells *aux_c, *aux_c2;
    int col1 = 0, col2 = 0, col3 = 0, col4=0, aux = 0;
    	
	/*------------- SPLIT MADE BY PROFESSIONAL DEVELOPER -----------------*/
	char *param = columns;
	int tam, i = 0;	
	char col[500];
	char *token;	
	
	for (tam = 0; param[tam]; param[tam] == ',' ? tam++ : *param++);	
	char cols[tam+1][150];

	strcpy(col, columns);
	token = strtok(col, ",");
	
	while(token != NULL){
		strcpy(cols[i], token); i++;
		token = strtok(NULL, ",");
	}
	/*-------------------------------------------------------------------*/	

    while ((strcmp(aux_t->nome, name_tab)) != 0)
        aux_t = aux_t->next;

    aux_l = aux_t->linhas;
    aux_tit = aux_t->titulos;
    
    while (aux_tit != NULL && (strcmp(aux_tit->title, cols[0])) != 0) {
        aux_tit = aux_tit->next;
        col1++;
    }
    if (aux_tit == NULL) {
        printf("Nao existe a coluna %s\n", cols[0]);
        return;
    }

    aux_tit = aux_t->titulos;

    while (aux_tit != NULL && (strcmp(aux_tit->title, cols[1])) != 0) {
        aux_tit = aux_tit->next;
        col2++;
    }
    if (aux_tit == NULL) {
        printf("Nao existe a coluna %s\n", cols[1]);
        return;
    }

    aux_tit = aux_t->titulos;

    while (aux_tit != NULL && (strcmp(aux_tit->title, column3)) != 0) {
        aux_tit = aux_tit->next;
        col3++;
    }
    if (aux_tit == NULL) {
        printf("Nao existe a coluna %s\n", column3);
        return;
    }
    
    aux_tit = aux_t->titulos;

    while (aux_tit != NULL && (strcmp(aux_tit->title, column4)) != 0) {
        aux_tit = aux_tit->next;
        col4++;
    }
    if (aux_tit == NULL) {
        printf("Nao existe a coluna %s\n", column4);
        return;
    }
    
    aux_tit = aux_t->titulos;

    if (col1 > col2) {
        aux = col1;
        col1 = col2;
        col2 = aux;
    }
    
    for (int i = 0; i < col1; i++)
        aux_tit = aux_tit->next;

    printf("%s\t", aux_tit->title);

    for (int i = col1; i < col2; i++)
        aux_tit = aux_tit->next;

    printf("%s\n", aux_tit->title);
    
    if ((strcmp(operador, "<")) == 0) {
        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            aux_c2=aux_l->cell;
            for (int i = 0; i < col3; i++)
                aux_c = aux_c->next;
            for(int i=0; i < col4; i++)
                aux_c2=aux_c2->next;
            if (aux_c->val < aux_c2->val) { 
                aux_c = aux_l->cell;
                for (int i = 0; i < col1; i++)
                    aux_c = aux_c->next;
                printf("%.1f\t", aux_c->val);
                for (int i = col1; i < col2; i++)
                    aux_c = aux_c->next;
                printf("%.1f\n", aux_c->val);

            }
            aux_l = aux_l->next;

        }

    }else if((strcmp(operador, "<=")) == 0){
        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            aux_c2=aux_l->cell;
            for (int i = 0; i < col3; i++)
                aux_c = aux_c->next;
            for(int i=0; i < col4; i++)
                aux_c2=aux_c2->next;
            if (aux_c->val <= aux_c2->val) { 
                aux_c = aux_l->cell;
                for (int i = 0; i < col1; i++)
                    aux_c = aux_c->next;
                printf("%.1f\t", aux_c->val);
                for (int i = col1; i < col2; i++)
                    aux_c = aux_c->next;
                printf("%.1f\n", aux_c->val);

            }
            aux_l = aux_l->next;

        }
        
    }else if((strcmp(operador, ">")) == 0){
        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            aux_c2=aux_l->cell;
            for (int i = 0; i < col3; i++)
                aux_c = aux_c->next;
            for(int i=0; i < col4; i++)
                aux_c2=aux_c2->next;
            if (aux_c->val > aux_c2->val) { 
                aux_c = aux_l->cell;
                for (int i = 0; i < col1; i++)
                    aux_c = aux_c->next;
                printf("%.1f\t", aux_c->val);
                for (int i = col1; i < col2; i++)
                    aux_c = aux_c->next;
                printf("%.1f\n", aux_c->val);

            }
            aux_l = aux_l->next;

        }
        
    }else if((strcmp(operador, ">=")) == 0){
        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            aux_c2=aux_l->cell;
            for (int i = 0; i < col3; i++)
                aux_c = aux_c->next;
            for(int i=0; i < col4; i++)
                aux_c2=aux_c2->next;
            if (aux_c->val >= aux_c2->val) { 
                aux_c = aux_l->cell;
                for (int i = 0; i < col1; i++)
                    aux_c = aux_c->next;
                printf("%.1f\t", aux_c->val);
                for (int i = col1; i < col2; i++)
                    aux_c = aux_c->next;
                printf("%.1f\n", aux_c->val);

            }
            aux_l = aux_l->next;

        }
        
    }else if((strcmp(operador, "=")) == 0 || (strcmp(operador, "==")) == 0){
        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            aux_c2=aux_l->cell;
            for (int i = 0; i < col3; i++)
                aux_c = aux_c->next;
            for(int i=0; i < col4; i++)
                aux_c2=aux_c2->next;
            if (aux_c->val == aux_c2->val) {
                aux_c = aux_l->cell;
                for (int i = 0; i < col1; i++)
                    aux_c = aux_c->next;
                printf("%.1f\t", aux_c->val);
                for (int i = col1; i < col2; i++)
                    aux_c = aux_c->next;
                printf("%.1f\n", aux_c->val);

            }
            aux_l = aux_l->next;

        }
        
    }else if((strcmp(operador, "!=")) == 0){
        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            aux_c2=aux_l->cell;
            for (int i = 0; i < col3; i++)
                aux_c = aux_c->next;
            for(int i=0; i < col4; i++)
                aux_c2=aux_c2->next;
            if (aux_c->val != aux_c2->val) {
                aux_c = aux_l->cell;
                for (int i = 0; i < col1; i++)
                    aux_c = aux_c->next;
                printf("%.1f\t", aux_c->val);
                for (int i = col1; i < col2; i++)
                    aux_c = aux_c->next;
                printf("%.1f\n", aux_c->val);

            }
            aux_l = aux_l->next;

        }
        
    }else{
        printf("\nOperador nao reconhecido!\n");
        return;
    }
}

void SelectSimple(char *columns, char *name_tab, char *new_tab){
    tabels *aux_t=t, *new;
    int col1, col2, col3;
    
	
	/*------------- SPLIT MADE BY PROFESSIONAL DEVELOPER -----------------*/
	char *param = columns;
	int tam, i = 0;	
	char col[500];
	char *token;	
	
	for (tam = 0; param[tam]; param[tam] == ',' ? tam++ : *param++);	
	char cols[tam+1][150];

	strcpy(col, columns);
	token = strtok(col, ",");
	
	while(token != NULL){
		strcpy(cols[i], token); i++;
		token = strtok(NULL, ",");
	}
	/*-------------------------------------------------------------------*/	
	

    //para criar nova tabela
    while((strcmp(aux_t->nome, name_tab)) != 0 && aux_t!=NULL)
        aux_t=aux_t->next;
    
    if(aux_t!=NULL){
        new=(tabels *) malloc(sizeof (tabels));
        new->nome=new_tab;
        new->titulos=SelectTitles(cols[0], cols[1], cols[2], 1);
        col1=num_col(aux_t->titulos, cols[0]);
        col2=num_col(aux_t->titulos, cols[1]);
        col3=num_col(aux_t->titulos, cols[2]);
        new->linhas=SelectLines(aux_t->linhas, col1, col2, col3);
        new->next=NULL;
    }else{
        printf("Nao existe a tabela %s!", name_tab);
        return;
    }
   
    aux_t=t;
    
    if((strcmp(aux_t->nome, new_tab))==0){
        new->next=aux_t->next;
        t=new;
        return;
    }else{
     while(aux_t->next!=NULL){
         if((strcmp(aux_t->next->nome, new_tab))==0){
             new->next=aux_t->next->next;
             aux_t->next=new;
             return;
         }
         aux_t=aux_t->next;
     }   
    }
    if((strcmp(aux_t->nome, new_tab))==0){
        new->next=aux_t->next->next;
             aux_t->next=new;
             return;
    }else{
        aux_t->next=new;
    }
}

titles *SelectTitles(char *column1, char *column2, char *column3, int count){
    titles *aux;

    aux=(titles *)malloc(sizeof(titles));
    
    switch(count)
    {
        case 1:
            aux->title=column1;
            aux->next=SelectTitles(column1, column2, column3, 2);
            break;
        case 2:
            aux->title=column2;
            aux->next=SelectTitles(column1, column2, column3, 3);
            break;
        case 3:
            aux->title=column3;
            aux->next=NULL;
            break;
    }
    return aux;
}

int num_col(titles *titulos, char *name){
    titles *aux=titulos;
    int count=1;
    while((strcmp(aux->title, name)) != 0){
        count++;
        aux=aux->next;
    }
    return count;
}

lines *SelectLines(lines *linha, int col1, int col2, int col3) {
    lines *aux = linha, *new;
    if (aux != NULL) {
        new = (lines *) malloc(sizeof (lines));
        new->cell = SelectCells(aux->cell, col1, col2, col3, 1);
	if(aux->next != NULL) 
	        new->next = SelectLines(aux->next, col1, col2, col3);
    	else
		new->next = NULL;
	
    } else {
        return NULL;
    }
    return new;
}

cells *SelectCells(cells *celula, int col1, int col2, int col3, int count) {
    cells *aux_c = celula, *new;

    new = (cells *) malloc(sizeof (cells));

    switch (count) {
        case 1:
            for (int i = 1; i < col1; i++)
                aux_c = aux_c->next;
            new->val = aux_c->val;
            new->next = SelectCells(celula, col1, col2, col3, 2);
            break;
        case 2:
            for (int i = 1; i < col2; i++)
                aux_c = aux_c->next;
            new->val = aux_c->val;
            new->next = SelectCells(celula, col1, col2, col3, 3);
            break;
        case 3:
            for (int i = 1; i < col3; i++)
                aux_c = aux_c->next;
            new->val = aux_c->val;
            new->next = NULL;
            break;
    }
    return new;
}

void SelectComplex(char *columns, char *name_tab, char *new_tab, char *column4, char *operador, char *column5) {
    tabels *aux_t = t, *new, *aux_tt;
    int col1, col2, col3, col4, col5;
	
	/*------------- SPLIT MADE BY PROFESSIONAL DEVELOPER -----------------*/
	char *param = columns;
	int tam, i = 0;	
	char col[500];
	char *token;	
	
	for (tam = 0; param[tam]; param[tam] == ',' ? tam++ : *param++);	
	char cols[tam+1][150];

	strcpy(col, columns);
	token = strtok(col, ",");
	
	while(token != NULL){
		strcpy(cols[i], token); i++;
		token = strtok(NULL, ",");
	}
	/*-------------------------------------------------------------------*/		

    if ((strcmp(operador, "<")) != 0 && (strcmp(operador, "<=")) != 0 && (strcmp(operador, ">")) != 0 && (strcmp(operador, ">=")) != 0 && (strcmp(operador, "=")) != 0 && (strcmp(operador, "==")) != 0 && (strcmp(operador, "!=")) != 0) {
        printf("Operador nao reconhecido!\n");
        return;
    }

    //para criar nova tabela
    while ((strcmp(aux_t->nome, name_tab)) != 0 && aux_t != NULL)
        aux_t = aux_t->next;

    if (aux_t != NULL) {
        new = (tabels *) malloc(sizeof (tabels));
        new->nome = new_tab;
        new->titulos = SelectTitles(cols[0], cols[1], cols[2], 1);
        col1 = num_col(aux_t->titulos, cols[0]);
        col2 = num_col(aux_t->titulos, cols[1]);
        col3 = num_col(aux_t->titulos, cols[2]);
        col4 = num_col(aux_t->titulos, column4);
        col5 = num_col(aux_t->titulos, column5);

        new->linhas = SelectLinesOPR(aux_t->linhas, col1, col2, col3, col4, col5, operador);
        new->next = NULL;
    } else {
        printf("Nao existe a tabela %s!", name_tab);
        return;
    }

    aux_t = t;
    aux_tt=t->next;
    if ((strcmp(aux_t->nome, new_tab)) == 0) {
        new->next = aux_t->next;
        t = new;
        return;
    } else if(aux_tt ==NULL){
        aux_t->next=new;
        return;
    }else{
        while (aux_t->next != NULL) {
            if ((strcmp(aux_t->next->nome, new_tab)) == 0) {
                new->next = aux_t->next->next;
                aux_t->next = new;
                return;
            }
            aux_t = aux_t->next;
        }
    }
    if ((strcmp(aux_t->nome, new_tab)) == 0) {
        new->next = aux_t->next->next;
        aux_t->next = new;
        return;
    } else {
        aux_t->next->next = new;
    }
}

cells *PrintCells(cells *point, int col1, int col2, int col3, int count){

    cells *aux=point, *new;
    switch (count){

        case 1:
            new = (cells *) malloc(sizeof (cells));
            for(int i=1; i<col1; i++)
                aux=aux->next;
            new->val=aux->val;
            new->next=PrintCells(point, col1, col2, col3, 2);
            break;

        case 2:
            new = (cells *) malloc(sizeof (cells));
            for(int i=1; i<col2; i++)
                aux=aux->next;
            new->val=aux->val;
            new->next=PrintCells(point, col1, col2, col3, 3);
            break;

        case 3:
            new = (cells *) malloc(sizeof (cells));
            for(int i=1; i<col3; i++)
                aux=aux->next;
            new->val=aux->val;
            new->next=NULL;
            break;
    }
    return new;

}

lines *SelectLinesOPR(lines *linha, int col1, int col2, int col3, int col4, int col5, char *operador) {
    float val1, val2;
    cells *aux_c=linha->cell;
    lines *new= (lines *)malloc(sizeof(lines));

    if (linha !=NULL) {
        aux_c = linha->cell;
        if ((strcmp(operador, "<")) == 0) {
            for (int i = 1; i < col4; i++)
                aux_c = aux_c->next;
            val1 = aux_c->val;
            aux_c = linha->cell;
            for (int i = 1; i < col5; i++)
                aux_c = aux_c->next;
            val2 = aux_c->val;
            if (val1 < val2) {
                new->cell = PrintCells(linha->cell, col1, col2, col3, 1);
                if (linha->next != NULL)
                    new->next = SelectLinesOPR(linha->next, col1, col2, col3, col4, col5, operador);
                else{
                    new->next=NULL;
                    return new;
                }
            } else {
                if (linha->next != NULL) {
                    free(new);
                    return SelectLinesOPR(linha->next, col1, col2, col3, col4, col5, operador);
                } else
                    return NULL;
            }
                
        } else if ((strcmp(operador, "<=")) == 0) {
            for (int i = 1; i < col4; i++)
                aux_c = aux_c->next;
            val1 = aux_c->val;
            aux_c = linha->cell;
            for (int i = 1; i < col5; i++)
                aux_c = aux_c->next;
            val2 = aux_c->val;
            if (val1 <= val2) {
                new->cell = PrintCells(linha->cell, col1, col2, col3, 1);
                if (linha->next != NULL)
                    new->next = SelectLinesOPR(linha->next, col1, col2, col3, col4, col5, operador);
                else{
                    new->next=NULL;
                    return new;
                }
            } else {
                if (linha->next != NULL) {
                    free(new);
                    return SelectLinesOPR(linha->next, col1, col2, col3, col4, col5, operador);
                } else
                    return NULL;
            }

        } else if ((strcmp(operador, ">")) == 0) {
            for (int i = 1; i < col4; i++)
                aux_c = aux_c->next;
            val1 = aux_c->val;
            aux_c = linha->cell;
            for (int i = 1; i < col5; i++)
                aux_c = aux_c->next;
            val2 = aux_c->val;
            if (val1 > val2) {
                new->cell = PrintCells(linha->cell, col1, col2, col3, 1);
                if (linha->next != NULL)
                    new->next = SelectLinesOPR(linha->next, col1, col2, col3, col4, col5, operador);
                else{
                    new->next=NULL;
                    return new;
                }
            } else {
                if (linha->next != NULL) {
                    free(new);
                    return SelectLinesOPR(linha->next, col1, col2, col3, col4, col5, operador);
                } else
                    return NULL;
            }

        } else if ((strcmp(operador, ">=")) == 0) {
            for (int i = 1; i < col4; i++)
                aux_c = aux_c->next;
            val1 = aux_c->val;
            aux_c = linha->cell;
            for (int i = 1; i < col5; i++)
                aux_c = aux_c->next;
            val2 = aux_c->val;
            if (val1 >= val2) {
                new->cell = PrintCells(linha->cell, col1, col2, col3, 1);
                if (linha->next != NULL)
                    new->next = SelectLinesOPR(linha->next, col1, col2, col3, col4, col5, operador);
                else{
                    new->next=NULL;
                    return new;
                }
            } else {
                if (linha->next != NULL) {
                    free(new);
                    return SelectLinesOPR(linha->next, col1, col2, col3, col4, col5, operador);
                } else
                    return NULL;
            }

        } else if ((strcmp(operador, "=")) == 0 || (strcmp(operador, "==")) == 0) {
            for (int i = 1; i < col4; i++)
                aux_c = aux_c->next;
            val1 = aux_c->val;
            aux_c = linha->cell;
            for (int i = 1; i < col5; i++)
                aux_c = aux_c->next;
            val2 = aux_c->val;
            if (val1 == val2) {
                new->cell = PrintCells(linha->cell, col1, col2, col3, 1);
                if (linha->next != NULL)
                    new->next = SelectLinesOPR(linha->next, col1, col2, col3, col4, col5, operador);
                else{
                    new->next=NULL;
                    return new;
                }
            } else {
                if (linha->next != NULL) {
                    free(new);
                    return SelectLinesOPR(linha->next, col1, col2, col3, col4, col5, operador);
                } else
                    return NULL;
            }

        } else if ((strcmp(operador, "!=")) == 0) {
            for (int i = 1; i < col4; i++)
                aux_c = aux_c->next;
            val1 = aux_c->val;
            aux_c = linha->cell;
            for (int i = 1; i < col5; i++)
                aux_c = aux_c->next;
            val2 = aux_c->val;
            if (val1 != val2) {/*Aqui*/
                new->cell = PrintCells(linha->cell, col1, col2, col3, 1);
                if (linha->next != NULL)
                    new->next = SelectLinesOPR(linha->next, col1, col2, col3, col4, col5, operador);
                else{
                    new->next=NULL;
                    return new;
                }
            } else {
                if (linha->next != NULL) {
                    free(new);
                    return SelectLinesOPR(linha->next, col1, col2, col3, col4, col5, operador);
                } else
                    return NULL;
            }

        }
        return new;
    } else
        return NULL;

}

void PrintAll(char *name_tab, char *column1, char *operador, float grade){
    tabels *aux=t;
    titles *aux_tit;
    int col1;
    if(aux==NULL){
        printf("Nao existem tabelas na memoria.\n");
        return;
    }
    
    while(aux!=NULL && (strcmp(aux->nome, name_tab)) != 0)
        aux=aux->next;
    aux_tit=aux->titulos;
    if(aux!=NULL){
        while(aux_tit!=NULL){
            printf("%s\t", aux_tit->title);
            aux_tit=aux_tit->next;
        }
        printf("\n");
        col1=num_col(aux->titulos, column1);
        PrintAllSimple(aux->linhas, col1, operador, grade);        
    }else{
        printf("Nao existe a tabela com o nome %s!\n", name_tab);
        return;
    }
    
    
}

void PrintAllSimple(lines *linha, int col1, char *operador, float grade) {
    cells *aux = linha->cell;

    if (aux != NULL) {
        if ((strcmp(operador, "<")) == 0) {
            for (int i = 1; i < col1; i++)
                aux = aux->next;
            if (aux->val < grade) {
                aux = linha->cell;
                while (aux != NULL) {
                    printf("%.1f\t", aux->val);
                    aux = aux->next;
                }
                puts("\n");
            }

        } else if ((strcmp(operador, "<=")) == 0) {
            for (int i = 1; i < col1; i++)
                aux = aux->next;
            if (aux->val <= grade) {
                aux = linha->cell;
                while (aux != NULL) {
                    printf("%.1f\t", aux->val);
                    aux = aux->next;
                }
                puts("\n");
            }

        } else if ((strcmp(operador, ">")) == 0) {
            for (int i = 1; i < col1; i++)
                aux = aux->next;
            if (aux->val > grade) {
                aux = linha->cell;
                while (aux != NULL) {
                    printf("%.1f\t", aux->val);
                    aux = aux->next;
                }
                puts("\n");
            }

        } else if ((strcmp(operador, ">=")) == 0) {
            for (int i = 1; i < col1; i++)
                aux = aux->next;
            if (aux->val >= grade) {
                aux = linha->cell;
                while (aux != NULL) {
                    printf("%.1f\t", aux->val);
                    aux = aux->next;
                }
                puts("\n");
            }

        } else if ((strcmp(operador, "=")) == 0 || (strcmp(operador, "==")) == 0) {
            for (int i = 1; i < col1; i++)
                aux = aux->next;
            if (aux->val == grade) {
                aux = linha->cell;
                while (aux != NULL) {
                    printf("%.1f\t", aux->val);
                    aux = aux->next;
                }
                puts("\n");
            }

        } else if ((strcmp(operador, "!=")) == 0) {
            for (int i = 1; i < col1; i++)
                aux = aux->next;
            if (aux->val != grade) {
                aux = linha->cell;
                while (aux != NULL) {
                    printf("%.1f\t", aux->val);
                    aux = aux->next;
                }
                puts("\n");
            }

        }
    }
    if (linha->next != NULL)
        PrintAllSimple(linha->next, col1, operador, grade);
}

void SelectAll(char *name_tab, char *new_tab, char *column1, char *operador, char *column2){
    tabels *aux_t = t, *aux_tt, *new;
    int col1, col2;

    if ((strcmp(operador, "<")) != 0 && (strcmp(operador, "<=")) != 0 && (strcmp(operador, ">")) != 0 && (strcmp(operador, ">=")) != 0 && (strcmp(operador, "=")) != 0 && (strcmp(operador, "==")) != 0 && (strcmp(operador, "!=")) != 0) {
        printf("Operador nao reconhecido!\n");
        return;
    }

    while ((strcmp(aux_t->nome, name_tab)) != 0 && aux_t != NULL)
        aux_t = aux_t->next;

    if (aux_t != NULL) {
        new = (tabels *) malloc(sizeof (tabels));
        new->nome = new_tab;
        new->titulos = SelectAllTitles(aux_t->titulos);
        col1 = num_col(aux_t->titulos, column1);
        col2 = num_col(aux_t->titulos, column2);
        new->linhas = SelectAllLines(aux_t->linhas, col1, col2, operador);
        new->next = NULL;
    } else {
        printf("Nao existe a tabela %s!", name_tab);
        return;
    }

    aux_t = t;
    aux_tt=t->next;
    if ((strcmp(aux_t->nome, new_tab)) == 0) {
        new->next = aux_t->next;
        t = new;
        return;
    } else if(aux_tt ==NULL){
        aux_t->next=new;
        return;
    }else{
        while (aux_t->next != NULL) {
            if ((strcmp(aux_t->next->nome, new_tab)) == 0) {
                new->next = aux_t->next->next;
                aux_t->next = new;
                return;
            }
            aux_t = aux_t->next;
        }
    }
    if ((strcmp(aux_t->nome, new_tab)) == 0) {
        new->next = aux_t->next->next;
        aux_t->next = new;
        return;
    } else {
        aux_t->next->next = new;
    }
}

titles *SelectAllTitles(titles *titulo){
    titles *aux=titulo, *new;
    
    if(aux!=NULL){
        new=(titles *) malloc(sizeof (titles));
        new->title=aux->title;
        new->next=SelectAllTitles(aux->next);
    }else
        return NULL;
    return new;
    
}

lines *SelectAllLines(lines *linha, int col1, int col2, char *operador) {
    float val1, val2;
    cells *aux_c = linha->cell;
    lines *new=(lines *) malloc(sizeof (lines));

    if (linha != NULL) {
        if ((strcmp(operador, "<")) == 0) {
            for (int i = 1; i < col1; i++)
                aux_c = aux_c->next;
            val1 = aux_c->val;
            aux_c = linha->cell;
            for (int i = 1; i < col2; i++)
                aux_c = aux_c->next;
            val2 = aux_c->val;
            if (val1 < val2) {
                new->cell = SelectALLCells(linha->cell);
                if (linha->next != NULL)
                    new->next = SelectAllLines(linha->next, col1, col2, operador);
                else{
                    new->next=NULL;
                    return new;
                }
            } else {
                if (linha->next != NULL) {
                    free(new);
                    return SelectAllLines(linha->next, col1, col2, operador);
                } else
                    return NULL;
            }

        } else if ((strcmp(operador, "<=")) == 0) {
            for (int i = 1; i < col1; i++)
                aux_c = aux_c->next;
            val1 = aux_c->val;
            aux_c = linha->cell;
            for (int i = 1; i < col2; i++)
                aux_c = aux_c->next;
            val2 = aux_c->val;
            if (val1 <= val2) {
                new->cell = SelectALLCells(linha->cell);
                if (linha->next != NULL)
                    new->next = SelectAllLines(linha->next, col1, col2, operador);
                else{
                    new->next=NULL;
                    return new;
                }
            } else {
                if (linha->next != NULL) {
                    free(new);
                    return SelectAllLines(linha->next, col1, col2, operador);
                } else
                    return NULL;
            }

        } else if ((strcmp(operador, ">")) == 0) {
            for (int i = 1; i < col1; i++)
                aux_c = aux_c->next;
            val1 = aux_c->val;
            aux_c = linha->cell;
            for (int i = 1; i < col2; i++)
                aux_c = aux_c->next;
            val2 = aux_c->val;
            if (val1 > val2) {
               new->cell = SelectALLCells(linha->cell);
                if (linha->next != NULL)
                    new->next = SelectAllLines(linha->next, col1, col2, operador);
                else{
                    new->next=NULL;
                    return new;
                }
            } else {
                if (linha->next != NULL) {
                    free(new);
                    return SelectAllLines(linha->next, col1, col2, operador);
                } else
                    return NULL;
            }

        } else if ((strcmp(operador, ">=")) == 0) {
            for (int i = 1; i < col1; i++)
                aux_c = aux_c->next;
            val1 = aux_c->val;
            aux_c = linha->cell;
            for (int i = 1; i < col2; i++)
                aux_c = aux_c->next;
            val2 = aux_c->val;
            if (val1 >= val2) {
                new->cell = SelectALLCells(linha->cell);
                if (linha->next != NULL)
                    new->next = SelectAllLines(linha->next, col1, col2, operador);
                else{
                    new->next=NULL;
                    return new;
                }
            } else {
                if (linha->next != NULL) {
                    free(new);
                    return SelectAllLines(linha->next, col1, col2, operador);
                } else
                    return NULL;
            }
        } else if ((strcmp(operador, "=")) == 0 || (strcmp(operador, "==")) == 0) {
            for (int i = 1; i < col1; i++)
                aux_c = aux_c->next;
            val1 = aux_c->val;
            aux_c = linha->cell;
            for (int i = 1; i < col2; i++)
                aux_c = aux_c->next;
            val2 = aux_c->val;
            if (val1 == val2) {
                new->cell = SelectALLCells(linha->cell);
                if (linha->next != NULL)
                    new->next = SelectAllLines(linha->next, col1, col2, operador);
                else{
                    new->next=NULL;
                    return new;
                }
            } else {
                if (linha->next != NULL) {
                    free(new);
                    return SelectAllLines(linha->next, col1, col2, operador);
                } else
                    return NULL;
            }

        } else if ((strcmp(operador, "!=")) == 0) {
            for (int i = 1; i < col1; i++)
                aux_c = aux_c->next;
            val1 = aux_c->val;
            aux_c = linha->cell;
            for (int i = 1; i < col2; i++)
                aux_c = aux_c->next;
            val2 = aux_c->val;
            if (val1 != val2) {
                new->cell = SelectALLCells(linha->cell);
                if (linha->next != NULL)
                    new->next = SelectAllLines(linha->next, col1, col2, operador);
                else{
                    new->next=NULL;
                    return new;
                }
            } else {
                if (linha->next != NULL) {
                    free(new);
                    return SelectAllLines(linha->next, col1, col2, operador);
                } else
                    return NULL;
            }

        }
        return new;
    } else
        return NULL;
}

cells *SelectALLCells(cells *celula){
    cells *new;
    
    if (celula != NULL) {
        new = (cells *) malloc(sizeof (cells));
        new->val=celula->val;
        new->next=SelectALLCells(celula->next);
    }else
        return NULL;
    return new;
}



//gravar tabela
void SaveTable(char *nameTab, char *nameFich){
    titles *aux_tit;
    tabels *aux_t=t;
    lines *aux_l;
    cells *aux_c;
    FILE *f;
    
    while ((strcmp(aux_t->nome, nameTab)) != 0){
        aux_t=aux_t->next;
    }
    
    if(aux_t == NULL){
        printf("Tabela nao encontrada.\n");
        return;
    }
    //criar novo ficheiro e abrir
    f = fopen(nameFich, "wt");
    
    if (f==NULL){
        printf("Ficheiro criado/aberto sem sucesso!\n");
        return;
    }
    
    //imprimir titulos
    aux_tit=aux_t->titulos;
    while (aux_tit->next != NULL){
        fprintf(f, "%s\t", aux_tit->title);
        aux_tit=aux_tit->next;
    }
    fprintf(f, "%s\n", aux_tit->title);
    
    //imprimir restantes dados no ficheiro
    aux_l=aux_t->linhas;
    aux_c=aux_l->cell;
    
    while(aux_l != NULL){
        if (aux_c->next != NULL){
            fprintf(f, "%.1f\t", aux_c->val);
            aux_c=aux_c->next;
        }else if(aux_c->next==NULL){
            fprintf(f, "%.1f\n", aux_c->val);
            aux_c=aux_c->next;
            aux_l=aux_l->next;
            if(aux_l == NULL)
                break;
            else
                aux_c=aux_l->cell;
        }    
    }
    
    fclose(f);
    
    printf("\n\tGravado com sucesso!");   
}
