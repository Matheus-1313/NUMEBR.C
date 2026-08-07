#include <string.h>
#include <stdlib.h>
#include "Number.h"

Number* novoNumero(char *valor) {
    Number *ptrNumero = malloc(1 * sizeof(Number));

    ptrNumero->magnitude = valor;
    ptrNumero->tamanhoMagnitude = strlen(valor) + 1;

    definirSinalNumero(ptrNumero);
    normalizarNumero(ptrNumero);

    return ptrNumero;
}

void normalizarNumero(Number *numero) {
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

        char *tmp = realloc(numero->magnitude, (j + 1) * sizeof(char));
        if (tmp == NULL) {
            free(numero->magnitude);
            free(magnitudeCopia);
            free(numero);
            exit(EXIT_FAILURE);
        }

        numero->magnitude = tmp;
    }

    free(magnitudeCopia);
}

void definirSinalNumero(Number *numero) {
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

size_t contarDigitosAntesVirgula(Number *numero) {
    size_t qAntesVirgula = 0;
    for (size_t i = 0; i < numero->tamanhoMagnitude; i++) {
        if (numero->magnitude[i] == '.' || numero->magnitude[i] == ',') {
            break;
        }

        qAntesVirgula++;
    }

    return qAntesVirgula;
}

size_t contarZerosSeguidosDepoisVirgula(Number *numero) {
    size_t posicaoVirgula = contarDigitosAntesVirgula(numero);

    for (size_t i = posicaoVirgula + 1; i < numero->tamanhoMagnitude; i++) {
        if (numero->magnitude[i] != '0') {
            break;
        }

        qZerosDepoisVirgula++;
    }
}

int isNumeroNulo(Number *numero) {
    size_t qZerosDepoisVirgula = contarZerosSeguidosDepoisVirgula(numero);
    if (numero->magnitude[0] == '0' && numero->tamanhoMagnitude - qZerosDepoisVirgula == 3) {
        return 1;
    }

    return 0;
}

