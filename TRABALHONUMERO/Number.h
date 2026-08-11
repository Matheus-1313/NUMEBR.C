#ifndef NUMBER_H
#define NUMBER_H

typedef struct Number {
    char* magnitude;
    size_t tamanhoMagnitude;
    int sinal;
    size_t exp;
} Number;

Number* novoNumero(char* valor);
int isNumeroNulo(Number *numero);



// TEM QUE IMPLEMENTAR ESSAS FUNÇÕES
Number somarPotenciaDeDez(Number *numero);
void fecharNumero(Number *numero);
void fecharTodosOsNumeros();
int isMaiorQue(Number *numero1, Number *numero2);

/*
FUNÇÕES QUE O USUÁRIO NÃO TEM ACESSO
void normalizarNumero(Number *numero);
void definirSinalNumero(Number *numero);
size_t contarDigitosAntesVirgula(Number *numero);
size_t contarZerosSeguidosDepoisVirgula(Number *numero);
void ajustarTamanhoNumero(Number *numero, size_t novoTamanho);
int isNumeroNormalizado(Number *numero);
int isValorValido(char *valor);
void registrarNumero(Number *numero);
*/



#endif