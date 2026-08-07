#include <string.h>
#include <stdlib.h>

#ifndef NUMBER_H
#define NUMBER_H

typedef struct Number {
    char* magnitude;
    size_t tamanhoMagnitude;
    int sinal;
    size_t exp;
} Number;

Number novoNumero(char* valor);
void normalizarNumero(Number *numero);
void definirSinalNumero(Number *numero);
size_t contarDigitosAntesVirgula(Number *numero);
size_t contarZerosSeguidosDepoisVirgula(Number *numero);
int isNumeroNulo(Number *numero);

#endif