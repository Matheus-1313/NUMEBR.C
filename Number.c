#include <string.h>
#include <stdlib.h>

typedef struct Number {
    char* magnitude;
    int exp;
} Number;

#define CREATE_NUMBER(nome, valor) do {\
    Number nome;\
    size_t tamanhoValor = strlen(#valor) + 1;\
    nome.magnitude = malloc(tamanhoValor * sizeof(char));\

    char* num = malloc(tamanhoValor * sizeof(char));\
    strcpy(num, #valor);\

    if (num[0] - '0' > 0) {\
        int qAntesVirgula = 0;\
        for (size_t i = 0; i < tamanhoValor; i++) {\
            if (num[i] == '.' || num[i] == ',') {\
                break;\
            }\

            qAntesVirgula++;\
        }\

        nome.exp = qAntesVirgula - 1;\
        nome.magnitude[0] = num[0];\
        nome.magnitude[1] = '.';\

        //CORRIGIR AQUI
        for (size_t i = 2; i < tamanhoValor; i++) {\
            if (num[i] == '.' || num[i] == ',') {\
                continue;\
            }\

            nome.magnitude[i] = num[i];\
        }\
    }\

    else {\
        int qZerosDepoisVirgula = 0;\
        for (size_t i = 2; i < tamanhoValor; i++) {\
            if (num[i] != '0') {\
                break;\
            }\

            qZerosDepoisVirgula++;\
        }\

        nome.exp = -(qZerosDepoisVirgula + 1);\
        nome.magnitude[0] = num[qZerosDepoisVirgula + 3];\
        nome.magnitude[1] = '.';\

        size_t j;\
        for (size_t i = qZerosDepoisVirgula + 4; i < tamanhoValor; i++) {\
            j = i - (qZerosDepoisVirgula + 4);\
            nome.magnitude[j] = num[i];\
        }

        char *tmp = realloc(nome.magnitude, (qZerosDepoisVirgula + j + 2) * sizeof(char));\
        if (tmp == NULL) {\
            //erro
        }\

        nome.magnitude = tmp;\
    }\

    free(num);\

} while(0)
