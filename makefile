
mainProj: func.o bisonProj.tab.o lex.yy.o
	gcc -o mainProj bisonProj.tab.o lex.yy.o

bisonProj.tab.h bisonProj.tab.c: bisonProj.y
	bison -d bisonProj.y

lex.yy.c: lexProj.l
	flex lexProj.l

func.o: func.c bisonProj.tab.h
