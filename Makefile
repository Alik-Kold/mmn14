CC = gcc
CREATE_BIN = $(CC) -g -ansi -pedantic -Wall
CREATE_O = $(CC) -c

assembler: main.o artifacts.o code.o common.o compilation.o precompilation.o test_funcs.o

	$(CREATE_BIN) main.o artifacts.o code.o common.o compilation.o precompilation.o test_funcs.o -o assembler

main.o: artifacts.h code.h main.c common.h compilation.h precompilation.h structs.h test_funcs.h

	$(CREATE_O) main.c -o main.o

test_funcs.o: artifacts.h common.h structs.h test_funcs.c test_funcs.h

	$(CREATE_O) test_funcs.c -o test_funcs.o

precompilation.o: artifacts.h common.h precompilation.c precompilation.h structs.h

	$(CREATE_O) precompilation.c -o precompilation.o

compilation.o: artifacts.h code.h common.h compilation.c compilation.h structs.h test_funcs.h

	$(CREATE_O) compilation.c -o compilation.o

common.o: artifacts.h common.c common.h

	$(CREATE_O) common.c -o common.o

code.o: artifacts.h code.h code.c common.h compilation.h structs.h test_funcs.h

	$(CREATE_O) code.c -o code.o

artifacts.o: artifacts.c

	$(CREATE_O) artifacts.c -o artifacts.o

clean:

	$(RM) assembler *.o *~%
