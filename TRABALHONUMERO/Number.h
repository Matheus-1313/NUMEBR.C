#include <string.h>
#include <stdlib.h>

#ifndef NUMBER_H
#define NUMBER_H

typedef struct Number {
    char* magnitude;
    int sinal;
    int exp;
} Number;

Number novoNumero(char* valor);

#endif