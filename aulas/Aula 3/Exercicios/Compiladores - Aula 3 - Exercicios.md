# Questionário

### Web page by Gemini
https://gemini.google.com/share/fe94945071ae

### 1. Explique com poucas palavras, e com suas palavras, o que vem a ser a forma normal de Greibach
A forma normal de Greibach é um conjunto de regras aplicas à uma gramática livre do contexto, onde todas as produções dessa gramáticas necessariamente tem o lado direito iniciado por terminais

---
### 2. Por que o algoritmo utilizado para concluir a demonstração do Teorema de Greibach nem sempre funciona com recursão esquerda?
Isso acontece por que o algoritmo se baseia no conceito de substituição.
Tomemos o exemplo a expressão E -> E + T. Se realizarmos a substituição de E em E + T, teríamos:
- E -> E + T
- E -> (E + T) + T
- E -> ((E + T) + T) + T
- E assim por diante...

Note que isso irá gerar um loop infinito, impossibilitando da regra chegar em um terminal.

---
### 3. Qual a vantagem de se ter a gramática de expressões na forma LL(1) do ponto de vista de implementação simplificada de analisadores sintáticos?
A principal vantagem de uma gramática na forma LL(1) é o seu determinismo, pois isso permite que o analisador sintático tome decisões corretas e definitivas apenas olhando para o próximo token de entrada, sem a necessidade de backtracking.
Do ponto de vista da implementação do parser, isso o torna mais simples, eficiente e fácil de depurar.

---
### 4. Porque S -> aS | aSbS | ε não é LL(1)?
Critérios para ser LL(1):
```
1. 𝐴 ⇏∗ 𝐴𝛼
2. 𝐴 → 𝛼|𝛽 implica ter FIRST(𝛼) ∩ FIRST(𝛽) = ∅
3. Se 𝐴 → 𝛼|𝜀, com 𝛼 ≠ 𝜀, então a escolha 𝐴 → 𝜀 só possível quando 𝑙𝑜𝑜𝑘𝑎ℎ𝑒𝑎𝑑 ∈ FOLLOW(𝐴), com FIRST(𝐴) ∩ FOLLOW(𝐴) = ∅.
```

Note que a gramática citada viola o segundo critério.

Seja S -> aS | aSbS | ε equivalente a S -> 𝛼 | 𝛽 | γ, onde:
- 𝛼 = aS
- 𝛽 = aSbS
- γ = ε

Calculando o `FIRST()` para as expressões 𝛼 e 𝛽, temos:
```
- FIRST(𝛼) = FIRST(aS) = a
- FIRST(𝛽) = FIRST(aSbS) = a
```

Portanto, como `FIRST(𝛼) ∩ FIRST(𝛽) ≠ ∅`, a gramática S -> aS | aSbS | ε não é LL(1)

---
### 5. A gramática S -> aSR, R -> bS | ε, com S sendo o símbolo inicial, é LL(1)? Justifique
Critérios para ser LL(1):
```
1. 𝐴 ⇏∗ 𝐴𝛼
2. 𝐴 → 𝛼|𝛽 implica ter FIRST(𝛼) ∩ FIRST(𝛽) = ∅
3. Se 𝐴 → 𝛼|𝜀, com 𝛼 ≠ 𝜀, então a escolha 𝐴 → 𝜀 só possível quando 𝑙𝑜𝑜𝑘𝑎ℎ𝑒𝑎𝑑 ∈ FOLLOW(𝐴), com FIRST(𝐴) ∩ FOLLOW(𝐴) = ∅.
```

1. O lado esquerdo de cada produção inicia com terminais ou palavra vazia
2. Validando para a produção R -> bS | ε
R -> bS | ε equivale a R -> α | β, onde
- α = bS
- β = ε

Calculando o `FIRST()` para as expressões 𝛼 e 𝛽, temos:
```
- FIRST(𝛼) = FIRST(bS) = b
- FIRST(𝛽) = FIRST(ε) = ε
```
Assim, como `FIRST(𝛼) ∩ FIRST(𝛽) = ∅`, o segundo critério é atendido

3. Ainda na produção R -> bS | ε, R -> ε se e somente se 𝑙𝑜𝑜𝑘𝑎ℎ𝑒𝑎𝑑 ∈ FOLLOW(R), com FIRST R ∩ FOLLOW R = ∅
Calculando o `FIRST(R)` para as expressões 𝛼 e 𝛽, temos:
```
- FIRST(R) = FIRST(bS) ∪ FIRST(ε) = {b} ∪ {ε} = {b, ε}
- Como S -> aSR, com o R no final, FOLLOW(S) = $ está contido em FOLLOW(B). De maneira análoga, R -> bS, então FOLLOW(R) está contido em FOLLOW(S).

```

Calculando o `FOLLOW(R)` para as expressões 𝛼 e 𝛽, temos:
```
- Como S -> aSR, com o R no final, FOLLOW(S) = $ está contido em FOLLOW(B). De maneira análoga, R -> bS, então FOLLOW(R) está contido em FOLLOW(S).
- Dessa forma, temos que FOLLOW(R) = FOLLOW(S) = $
- Então temos FIRST(R) ∩ FOLLOW(R) = {b} ∩ {$} = ∅
```

Portanto, como a gramática `S -> aSR, R -> bS | ε` atende aos três critérios, podemos a considerar LL(1)