# Questionário

### Web page by Gemini
https://gemini.google.com/share/fe94945071ae

### 1. Que é o processo de varredura e qual sua importância na análise léxica?
O processo de varredura consiste em um fluxo contínuo de leitura sequencial de caracteres de um arquivo de texto, produzindo tokens quando estados finais são atingidos. Sua importância consiste em simular o comportamento de um autômato finito, consumindo o caractere e avaliando se pertence ou não à linguagem.

### 2. Quais exigências ou restrições são feitas quanto à maneira de implementar AFNs para funcionarem como analisadores léxicos?
São feitas três restrições:

1. Eliminar transições múltiplas, as tornando em funções parciais;
2. Simular transições épsilon pela sequência getc(source) e ungetc(source);
3. Trocar estados de rejeição ou de transição vazia por estado de aceitação confirmativa ou aceitação de palavra vazia;

### 3.  Como se simula a transição épsilon na codificação C?
Se simula pelo uso sequencial das funções getc(source) e ungetc(source);

### 4. Modifique o comando skipspaces para que este contabilize o número de linhas avistados. Defina uma variável global int linenum = 1;
```
int linenum = 1;

void skipspaces(FILE *tape) {
    int ch;
    while ( isspace(ch = getc(tape)) ) {
        if (ch == '\n') {
            linenum++;
        }
    }
    ungetc(ch, tape);
}
```

### 5. Qual a necessidade da linha 7 no template da função gettoken?
A linha 7 representa o caso padrão para tokens de caractere único, pois já passaram pelas validações anteriores que reconhecem um token multicaractere. Assim, retorna o caractere bruto, nem necessidade de uma função predicado especial