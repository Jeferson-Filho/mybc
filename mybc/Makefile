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
	tar zcvf mybc_grupo2.tar.gz Makefile *.[ch]

# pra extrair:
# 	tar zxvf mybc.tar.gz
