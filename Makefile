parser: syntax.y test.l main.c node.h semantic.h type.h symbol_table.h intercode.c 
	bison -d syntax.y
	flex test.l
	gcc -g main.c syntax.tab.c intercode.c -lfl -ly -o parser
clean:
	rm lex.yy.c syntax.tab.* parser
