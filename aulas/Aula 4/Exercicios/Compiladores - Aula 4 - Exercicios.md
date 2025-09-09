# Questionário

### Web page by Gemini
https://gemini.google.com/share/fe94945071ae

### 1. Que vem a ser um analisador sintático LL(1)?
É um analisador que emula uma gramática LL(1) por meio de um autômato de pilha, consistindo em:
- Buffer de entrada
- Pilha
- Tabela gramatical
- Saída

---
### 2. Qual a finalidade da tabela gramatical?
Sua finalidade é servir de dicionário para o autômato, eliminando o não-determinismo e guiando o autômato de pilha em cada passo.

---
### 3. Para que transições o autômato LL(1) recorre à tabela?
O autômato LL(1) sempre irá recorrer à tabela para transições cujo o topo da pilha é um não-terminal

---
### 4. Sob que circunstâncias a pilha do autômato LL(1) sofre efetivamente uma operação pop
A operação pop acontece somente quando o lookahead bate com o topo da pilha.
Caso seja um terminal, é feito apenas o pop, mas se for um não terminal, acontece o pop e push
- Pilha: [... $ n]
- Entrada: [n + n $]

Match! Ação de pop

- Pilha: [... $ E]
- Entrada: [n + n $]
- Tabela[E, n] diz para usar E -> TR

É feito o pop de E e push de TR

- Nova pilha: [... $ R T]
- Entrada: [n + n $]
