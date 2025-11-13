#!/bin/bash

# Script para testar o mybc com casos de rastreamento de linhas e colunas

MYBC="./mybc"

echo "=========================================="
echo "TESTES MYBC - Rastreamento de Erros"
echo "Validação de lineno e columno"
echo "=========================================="
echo ""

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Função para executar teste
run_test() {
    local desc="$1"
    local input="$2"
    local expected_line="$3"
    local expected_col="$4"
    
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo "Teste: $desc"
    echo "Entrada: $(echo -n "$input" | od -c | head -1)"
    echo "Esperado: line $expected_line, column $expected_col"
    
    # Executar e capturar erro
    output=$(echo -e "$input" | $MYBC 2>&1)
    error_line=$(echo "$output" | grep "ERROR at line" | head -1)
    
    if [ -z "$error_line" ]; then
        echo -e "${RED}✗ FAIL${NC}: Nenhum erro foi gerado"
        return 1
    fi
    
    # Extrair lineno e columno
    actual_line=$(echo "$error_line" | grep -oP 'line \K[0-9]+')
    actual_col=$(echo "$error_line" | grep -oP 'column \K[0-9]+')
    
    echo "Recebido: line $actual_line, column $actual_col"
    
    # Validar
    if [ "$actual_line" = "$expected_line" ] && [ "$actual_col" = "$expected_col" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        return 0
    else
        echo -e "${RED}✗ FAIL${NC}"
        if [ "$actual_line" != "$expected_line" ]; then
            echo -e "  ${RED}Line mismatch: got $actual_line, expected $expected_line${NC}"
        fi
        if [ "$actual_col" != "$expected_col" ]; then
            echo -e "  ${RED}Column mismatch: got $actual_col, expected $expected_col${NC}"
        fi
        return 1
    fi
    echo ""
}

# Rastreamento de resultados
PASSED=0
FAILED=0

echo ""
echo "=== TESTE 1: RASTREAMENTO DE COLUNA (LINHA ÚNICA) ==="
echo ""

# Caso 1.1
if run_test "Erro na coluna 1" "^" "1" "1"; then
    ((PASSED++))
else
    ((FAILED++))
fi
echo ""

# Caso 1.2
if run_test "Erro na coluna 3" "1 ^" "1" "3"; then
    ((PASSED++))
else
    ((FAILED++))
fi
echo ""

# Caso 1.3
if run_test "Erro na coluna 7" "1 + 2 ^" "1" "7"; then
    ((PASSED++))
else
    ((FAILED++))
fi
echo ""

# Caso 1.4
if run_test "Erro segundo ID na coluna 3" "a b" "1" "3"; then
    ((PASSED++))
else
    ((FAILED++))
fi
echo ""

# Caso 1.5
if run_test "Múltiplos espaços, erro coluna 9" "1       ^" "1" "9"; then
    ((PASSED++))
else
    ((FAILED++))
fi
echo ""

echo ""
echo "=== TESTE 2: RASTREAMENTO DE LINHA (MÚLTIPLAS LINHAS) ==="
echo ""

# Caso 2.1
if run_test "Erro linha 2, coluna 1" "1\n^" "2" "1"; then
    ((PASSED++))
else
    ((FAILED++))
fi
echo ""

# Caso 2.2
if run_test "Erro linha 2, coluna 5" "1 + 2\nabc ^" "2" "5"; then
    ((PASSED++))
else
    ((FAILED++))
fi
echo ""

# Caso 2.3
if run_test "Erro linha 3, coluna 3" "1\n2\n  ^" "3" "3"; then
    ((PASSED++))
else
    ((FAILED++))
fi
echo ""

# Caso 2.4
if run_test "Múltiplas linhas, erro linha 3 col 7" "1 + 2\n3 * 4\n  5 - ^" "3" "7"; then
    ((PASSED++))
else
    ((FAILED++))
fi
echo ""

echo ""
echo "=== TESTE 3: ESPAÇAMENTO E COLUNA ==="
echo ""

# Caso 3.1
if run_test "ID seguido de erro" "abc^" "1" "4"; then
    ((PASSED++))
else
    ((FAILED++))
fi
echo ""

# Caso 3.2
if run_test "Número seguido de ID (erro)" "123abc" "1" "4"; then
    ((PASSED++))
else
    ((FAILED++))
fi
echo ""

echo ""
echo "=== TESTE 4: CASOS COMPLEXOS ==="
echo ""

# Caso 4.1
if run_test "Atribuição com erro" "x := 1 2" "1" "8"; then
    ((PASSED++))
else
    ((FAILED++))
fi
echo ""

# Caso 4.2
if run_test "Atribuição multilinha com erro" "x := 1\ny := 2 ^" "2" "8"; then
    ((PASSED++))
else
    ((FAILED++))
fi
echo ""

echo ""
echo "=========================================="
echo "RESUMO DOS RESULTADOS"
echo "=========================================="
echo -e "${GREEN}PASSOU: $PASSED${NC}"
echo -e "${RED}FALHOU: $FAILED${NC}"
TOTAL=$((PASSED + FAILED))
echo "TOTAL: $TOTAL"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}✓ TODOS OS TESTES PASSARAM!${NC}"
    exit 0
else
    echo -e "\n${RED}✗ ALGUNS TESTES FALHARAM${NC}"
    exit 1
fi

