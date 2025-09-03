# Makefile on 21/08/2025

HDIR = .
CFLAGS = -g -I$(HDIR)

mybc: lexer.o main.o parser.o
	$(CC) $^ -o $@

clean:
	$(RM) *.o

mostlyclean: clean
	$(RM) *~

targz: # Para entregar pro prof
	tar zcvf mybc.tar.gz Makefile *.[ch]

# targzx: # Para extrair
# 	tar zxvf mybc.tar.gz Makefile *.[ch]