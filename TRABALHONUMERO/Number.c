#include <string.h>
#include <stdlib.h>
#include "Number.h"

Number novoNumero(char *valor) {
    Number numero;
    size_t tamanhoValor = strlen(valor) + 1;

    if (valor[0] == '-') {
        numero.sinal = 1;
        for (size_t i = 1; i < tamanhoValor; i++) {
            valor[i - 1] = valor[i];
        }
        tamanhoValor--;
    }

    numero.magnitude = malloc(tamanhoValor * sizeof(char));
    char* num = malloc(tamanhoValor * sizeof(char));
    strcpy(num, valor);

    if (num[0] != '0') {
        int qAntesVirgula = 0;
        for (size_t i = 0; i < tamanhoValor; i++) {
            if (num[i] == '.' || num[i] == ',') {
                break;
            }

            qAntesVirgula++;
        }

        numero.exp = qAntesVirgula - 1;
        numero.magnitude[0] = num[0];
        numero.magnitude[1] = '.';

        size_t j = 2;
        for (size_t i = 1; i < tamanhoValor; i++, j++) {
            if (num[i] == '.' || num[i] == ',') {
                j--;
                continue;
            }

            numero.magnitude[j] = num[i];
        }
    }

    else {
        int qZerosDepoisVirgula = 0;
        for (size_t i = 2; i < tamanhoValor; i++) {
            if (num[i] != '0') {
                break;
            }

            qZerosDepoisVirgula++;
        }

        if (tamanhoValor - qZerosDepoisVirgula == 3) {
            numero.exp = 0;
            strcpy(numero.magnitude, "0.0");
        }

        numero.exp = -(qZerosDepoisVirgula + 1);
        numero.magnitude[0] = num[qZerosDepoisVirgula + 2];
        numero.magnitude[1] = '.';

        size_t j = 2;
        for (size_t i = qZerosDepoisVirgula + 3; i < tamanhoValor; i++, j++) {
            numero.magnitude[j] = num[i];
            if (num[i] == '\0') {
                break;
            }
        }

        char *tmp = realloc(numero.magnitude, (j + 1) * sizeof(char));
        if (tmp == NULL) {
            free(numero.magnitude);
            free(num);
            free(valor);
            exit(EXIT_FAILURE);
        }

        numero.magnitude = tmp;
    }

    return numero;

    free(num);
    free(valor);
}