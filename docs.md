# Documentação Didática – Interpretador *mybc*

## Grupo

- Caio Bohlhalter
- Jeferson Patrick
- Lucas Quintas

## Sumário

1. [Analisador Léxico (`lexer.c`)](#analisador-léxico-lexerc)  
   - [Funções Principais](#funções-principais)  
   - [Identificadores](#identificadores)  
   - [Números (DEC, FLT, EE)](#números-dec-flt-ee)  
   - [Operadores e Espaços](#operadores-e-espaços)  
2. [Analisador Sintático e Interpretador (`parser.c`)](#analisador-sintático-e-interpretador-parserc)  
   - [Estrutura Geral](#estrutura-geral)  
   - [Tabela de Símbolos e Memória Virtual](#tabela-de-símbolos-e-memória-virtual)  
   - [Função `E()` – Avaliação de Expressões](#função-e--avaliação-de-expressões)  
   - [Função `cmd()` – Interpretação de Comandos](#função-cmd--interpretação-de-comandos)  
   - [Função `match()` – Verificação de Tokens](#função-match--verificação-de-tokens)  
3. [Fluxo de Execução do Interpretador](#fluxo-de-execução-do-interpretador)  
4. [Tratamento de Erros e Sinais](#tratamento-de-erros-e-sinais)

---

## Analisador Léxico (`lexer.c`)

O analisador léxico lê o fluxo de caracteres da entrada (`stdin` ou arquivo) e identifica **tokens** — as menores unidades significativas da linguagem, como identificadores, números, operadores, ou palavras-chave.

### Funções Principais

| Função | Descrição |
|--------|------------|
| `isID(FILE *tape)` | Reconhece identificadores (`a`, `valor1`, `x123`). |
| `isDEC(FILE *tape)` | Reconhece números decimais inteiros. |
| `isEE(FILE *tape)` | Detecta notação exponencial (`1.2e3`). |
| `isNUM(FILE *tape)` | Reconhece números inteiros, flutuantes e exponenciais. |
| `isASGN(FILE *tape)` | Reconhece o operador de atribuição `:=`. |
| `isOCT(FILE *tape)` | Reconhece números octais (`07`, `0123`). |
| `isHEX(FILE *tape)` | Reconhece números hexadecimais (`0xFF`). |
| `skipspaces(FILE *tape)` | Ignora espaços, tabs e quebras de linha, mantendo `lineno` e `columno`. |
| `gettoken(FILE *source)` | Função principal — chama as anteriores e retorna o próximo token. |

---

### Identificadores

```c
int isID(FILE *tape);
```

- **Padrão:** `[A-Za-z][A-Za-z0-9]*`  
- Se o lexema for `"exit"` ou `"quit"`, é tratado como **palavra reservada** (tokens `EXIT`, `QUIT`).  
- Caso contrário, retorna o token `ID`.

---

### Números (DEC, FLT, EE)

#### `isDEC()` – Decimal
- Reconhece inteiros (`0`, `1234`, `99`).

#### `isNUM()` – Números em geral
- Detecta decimais, flutuantes (`3.14`, `.5`) e notação científica (`2e10`).
- Internamente combina as funções `isDEC()`, `isEE()` e regras adicionais.

---

### Operadores e Espaços

#### `isASGN()`
- Reconhece o operador `:=` (atribuição).  
  Exemplo:
  ```
  x := 5
  ```

#### `skipspaces()` e `skipArrow()`
- Ignoram espaços, tabs e setas de teclado (`ESC + [A/B/C/D`).
- Mantêm contagem de linha (`lineno`) e coluna (`columno`), úteis para mensagens de erro.

---

## Analisador Sintático e Interpretador (`parser.c`)

### Estrutura Geral

O parser implementa uma **gramática LL(1)** simplificada para expressões e comandos.

```
mybc -> cmd { cmdsep cmd } EOF
cmd  -> E | exit | quit | ε
E    -> [ '+' | '-' ] T { ('+' | '-') T }
T    -> F { ('*' | '/') F }
F    -> ID [ ':=' E ] | '(' E ')' | número
```

### Tabela de Símbolos e Memória Virtual

Variáveis são armazenadas em uma **tabela de símbolos** (`symtab`), associadas a um vetor de valores (`vmem`).

```c
char symtab[MAXSTENTRIES][MAXIDLEN+1];
double vmem[MAXSTENTRIES];
```

Funções auxiliares:

| Função | Descrição |
|--------|------------|
| `recall(name)` | Retorna o valor atual de uma variável. Cria a variável se não existir. |
| `store(name)` | Atribui o valor do acumulador (`acc`) à variável. |

---

### Função `E()` – Avaliação de Expressões

A função `E()` é o coração do interpretador:  
ela avalia expressões aritméticas e atribuições, controlando precedência e associatividade.

#### Recursos internos

- **`acc` (acumulador)**: guarda o resultado da subexpressão atual.  
- **`stack[]` e `sp`**: pilha para operações intermediárias.  
- **Flags (`isnegate`, `isotimes`, `isoplus`)**: controlam operadores pendentes.

#### Exemplo de execução

Entrada:
```
a := 3 + 4 * 2
```

Fluxo:
1. `a` é reconhecido como `ID`.
2. `:=` indica atribuição → chama `E()` recursivamente para avaliar `3 + 4 * 2`.
3. `E()` calcula `4 * 2 = 8`, depois `3 + 8 = 11`.
4. Valor `11` é armazenado em `a`.
5. `acc = 11`.

---

### Função `cmd()` – Interpretação de Comandos

Responsável por reconhecer comandos completos:

```c
cmd → E | exit | quit | ε
```

- Se `lookahead` for `EXIT` ou `QUIT`, termina o programa.  
- Caso contrário, chama `E()` e imprime o valor do acumulador.

---

### Função `match()` – Verificação de Tokens

`match(expected)` compara o **token atual (`lookahead`)** com o esperado.

- Se coincidem → avança (`lookahead = gettoken(source)`).
- Se diferem → exibe mensagem de erro amigável, com:
  - Linha (`lineno`)
  - Coluna (`columno`)
  - Token esperado e recebido

Além disso, ela realiza **recuperação de erro**, descartando tokens até um ponto de sincronização (`;`, `\n`, `EOF`).

---

## Fluxo de Execução do Interpretador

1. O programa inicia `lookahead = gettoken(source)`.
2. Entra em `mybc()`, que chama `cmd()`.
3. `cmd()` avalia a expressão ou comando atual.
4. `E()` resolve operações aritméticas e atribuições.
5. Resultado é exibido e o loop recomeça.

Exemplo:
```
> x := 5
5
> y := x * 2
10
> y + 3
13
> quit
```

---

## Tratamento de Erros e Sinais

- **Erros de sintaxe**: são tratados em `match()`, com mensagens detalhadas.  
- **Sinal `SIGINT` (Ctrl+C)**: tratado por `handleSigint()`, apenas imprime nova linha para evitar travamentos.  
- **Recuperação de erro**: permite continuar após um erro, sem encerrar o programa.

---
