HDIR = .
CFLAGS = -g -I$(HDIR)

# Executar mybc
mybc: lexer.o main.o parser.o
	$(CC) $^ -o $@

# Apagar arquivos objeto
clean:
	$(RM) *.o

# Apagar arquivos objeto e arquivos de backup
mostlyclean: clean
	$(RM) *~

# Cria um arquivo tar
targz:
	tar zcvf mybc_grp02.tar.gz Makefile *.[ch] *.md

# pra extrair:
# 	tar zxvf mybc.tar.gz
