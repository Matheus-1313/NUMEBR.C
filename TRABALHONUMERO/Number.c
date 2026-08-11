#include "Number.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static Number **todosOsNumeros = NULL;
static size_t quantidadeNumeros = 0;

Number* novoNumero(char *valor) {
    if (!isValorValido(valor)) {
        return NULL; //ERRO
    }
    
    Number *ptrNumero = malloc(1 * sizeof(Number));
    //TRATAR CASO DÊ ERRO

    ptrNumero->magnitude = valor;
    ptrNumero->tamanhoMagnitude = strlen(valor) + 1;

    definirSinalNumero(ptrNumero);
    normalizarNumero(ptrNumero);
    registrarNumero(ptrNumero);

    return ptrNumero;
}

static void normalizarNumero(Number *numero) {
    if (isNumeroNormalizado(numero)) {
        return;
    }

    char* magnitudeCopia;
    strcpy(magnitudeCopia, numero->magnitude);

    if (numero->magnitude[0] != '0') {
        size_t qAntesVirgula = contarDigitosAntesVirgula(numero);
        numero->exp = qAntesVirgula - 1;
        numero->magnitude[1] = '.';

        for (size_t i = 1, j = 2; i < numero->tamanhoMagnitude; i++, j++) {
            if (magnitudeCopia[i] == '.' || magnitudeCopia[i] == ',') {
                j--;
                continue;
            }

            numero->magnitude[j] = magnitudeCopia[i];
        }
    }

    else {
        size_t qZerosDepoisVirgula = contarZerosSeguidosDepoisVirgula(numero);

        if (isNumeroNulo(numero)) {
            numero->exp = 0;
            strcpy(numero->magnitude, "0.0");
        }

        numero->exp = -(qZerosDepoisVirgula + 1);
        numero->magnitude[0] = magnitudeCopia[qZerosDepoisVirgula + 2];

        size_t j = 2;
        for (size_t i = qZerosDepoisVirgula + 3; i < numero->tamanhoMagnitude; i++, j++) {
            numero->magnitude[j] = magnitudeCopia[i];
            if (magnitudeCopia[i] == '\0') {
                break;
            }
        }

        ajustarTamanhoNumero(numero, j + 1);
    }

    free(magnitudeCopia);
}

static void definirSinalNumero(Number *numero) {
    if (numero->magnitude[0] == '-') {
        numero->sinal = 1;
        for (size_t i = 1; i < numero->tamanhoMagnitude; i++) {
            numero->magnitude[i - 1] = numero->magnitude[i];
        }
        numero->tamanhoMagnitude--;
    }

    else {
        numero->sinal = 0;
    }
}

static size_t contarDigitosAntesVirgula(Number *numero) {
    size_t qAntesVirgula = 0;
    for (size_t i = 0; i < numero->tamanhoMagnitude; i++) {
        if (numero->magnitude[i] == '.' || numero->magnitude[i] == ',') {
            break;
        }

        qAntesVirgula++;
    }

    return qAntesVirgula;
}

static size_t contarZerosSeguidosDepoisVirgula(Number *numero) {
    size_t posicao = contarDigitosAntesVirgula(numero);

    for (size_t i = posicao + 1; i < numero->tamanhoMagnitude; i++) {
        if (numero->magnitude[i] != '0') {
            break;
        }

        posicao++;
    }
}

int isNumeroNulo(Number *numero) {
    size_t qZerosDepoisVirgula = contarZerosSeguidosDepoisVirgula(numero);
    if (numero->magnitude[0] == '0' && numero->tamanhoMagnitude - qZerosDepoisVirgula == 3) {
        return 1;
    }

    return 0;
}

static void ajustarTamanhoNumero(Number *numero, size_t novoTamanho) {
    char *tmp = realloc(numero->magnitude, (novoTamanho) * sizeof(char));
        if (tmp == NULL) {
            free(numero->magnitude);
            free(numero);
            exit(EXIT_FAILURE);
        } //ERRO

        numero->magnitude = tmp;
        numero->tamanhoMagnitude = novoTamanho;
}

static int isNumeroNormalizado(Number *numero) {
    if (numero->magnitude[1] != '.' || numero->magnitude[1] != ',') {
        return 0;
    }

    for (size_t i = 0; i < numero->tamanhoMagnitude - 1; i++) {
        if (numero->magnitude[i] == '.' || numero->magnitude[i] == ',') {
            continue;
        }

        if (!isdigit(numero->magnitude[i])) {
            return 0;
        }
    }

    return 1;
}

static int isValorValido(char *valor) {
    size_t tamanhoValor = strlen(valor) + 1;

    if (valor[0] == '0' && (valor[1] != ',' || valor[1] != '.')) {
        return 0;
    }

    int quantidadeVirgulas;
    for (size_t i = 0; i < tamanhoValor; i++) {
        if (valor[i] == ',' || valor[i] =='.') {
            quantidadeVirgulas++;
            if (quantidadeVirgulas > 1) {
                return 0;
            }
            continue;
        }

        if (!isdigit(valor[i])) {
            return 0;
        }
    }

    return 1;
}

static void registrarNumero(Number *numero) {
    Number **tmp = realloc(todosOsNumeros, (quantidadeNumeros + 1) * sizeof(Number*));
    if (tmp == NULL) {
        free(tmp);
        free(numero);
        fecharTodosOsNumeros();
        free(todosOsNumeros);

        exit(EXIT_FAILURE);
    }

    todosOsNumeros = tmp;
    todosOsNumeros[quantidadeNumeros] = numero;
    quantidadeNumeros++;
}

void fecharTodosOsNumeros() {
    //FALTA IMPLEMENTAR AQUI
}

void fecharNumero(Number *numero) {
    //FALTA IMPLEMENTAR AQUI
}
