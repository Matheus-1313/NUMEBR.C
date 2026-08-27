#include "Number.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Number{
    char* magnitude;
    size_t tamanhoMagnitude;
    int sinal;
    ssize_t exp;
} Number;

static Number **todosOsNumeros = NULL;
static size_t quantidadeNumeros = 0;

static void definirSinal_number(Number *numero) {
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

static size_t contarDigitosAntesVirgula_number(Number *numero) {
    size_t qAntesVirgula = 0;
    for (size_t i = 0; i < numero->tamanhoMagnitude; i++) {
        if (numero->magnitude[i] == '.' || numero->magnitude[i] == ',') {
            break;
        }

        qAntesVirgula++;
    }

    return qAntesVirgula;
}

static size_t contarZerosSeguidosDepoisVirgula_number(Number *numero) {
    size_t posicao = contarDigitosAntesVirgula_number(numero);

    for (size_t i = posicao + 1; i < numero->tamanhoMagnitude; i++) {
        if (numero->magnitude[i] != '0') {
            break;
        }

        posicao++;
    }
}

static void ajustarTamanho_number(Number *numero, size_t novoTamanho) {
    char *tmp = realloc(numero->magnitude, (novoTamanho) * sizeof(char));
        if (tmp == NULL) {
            free(numero->magnitude);
            free(numero);
            exit(EXIT_FAILURE);
        } //ERRO

        numero->magnitude = tmp;
        numero->tamanhoMagnitude = novoTamanho;
}

static int isNormalizado_number(Number *numero) {
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

static void registrar_number(Number *numero) {
    Number **tmp = realloc(todosOsNumeros, (quantidadeNumeros + 1) * sizeof(Number*));
    if (tmp == NULL) {
        free(tmp);
        free(numero);
        fecharTodos_number();
        exit(EXIT_FAILURE);
    }

    todosOsNumeros = tmp;
    todosOsNumeros[quantidadeNumeros] = numero;
    quantidadeNumeros++;
}

static int isValorValido_number(char *valor) {
    size_t tamanhoValor = strlen(valor) + 1;

    if (tamanhoValor < 4) {
        return 0;
    }

    if (valor[0] == '0' && (valor[1] != ',' || valor[1] != '.')) {
        return 0;
    }

    int quantidadeVirgulas;
    for (size_t i = 0; i < tamanhoValor - 1; i++) {
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

static size_t encontrarEmTodos_number(Number *numero) {//FUNÇÃO DA ERRO
    if (todosOsNumeros == NULL) {
        return 0;
    }

    size_t indiceNumero = 0;
    for (size_t i = 0; i < quantidadeNumeros; i++) {
        if (todosOsNumeros[i] == numero) {
            indiceNumero = i;
            break;
        }
    }

    return indiceNumero;
}

static void removerEmTodos_number(Number *numero) {
    size_t indiceNumero = encontrarEmTodos_number(numero);

    if (todosOsNumeros[indiceNumero] != numero) {
        return;//ERRO?
    }

    for (size_t i = indiceNumero; i < quantidadeNumeros - 1; i++) {
        todosOsNumeros[i] = todosOsNumeros[i + 1];
    }

    Number **tmp = realloc(todosOsNumeros, (quantidadeNumeros - 1) * sizeof(Number));
    if (tmp == NULL) {
        free(tmp);
        fecharTodos_number();
        exit(EXIT_FAILURE);
    }

    todosOsNumeros = tmp;
}

static void normalizar_number(Number *numero) {
    if (isNormalizado_number(numero)) {
        return;
    }

    ssize_t expoenteAnterior = numero->exp;
    char* magnitudeCopia;
    strcpy(magnitudeCopia, numero->magnitude);

    if (numero->magnitude[0] != '0') {
        size_t qAntesVirgula = contarDigitosAntesVirgula_number(numero);
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
        if (isNulo_number(numero)) {
            numero->exp = 0;
            strcpy(numero->magnitude, "0.0");
            return;
        }

        size_t qZerosDepoisVirgula = contarZerosSeguidosDepoisVirgula_number(numero);

        numero->exp = -(qZerosDepoisVirgula + 1);
        numero->magnitude[0] = magnitudeCopia[qZerosDepoisVirgula + 2];

        size_t j = 2;
        for (size_t i = qZerosDepoisVirgula + 3; i < numero->tamanhoMagnitude; i++, j++) {
            numero->magnitude[j] = magnitudeCopia[i];
            if (magnitudeCopia[i] == '\0') {
                break;
            }
        }

        ajustarTamanho_number(numero, j + 1);
    }

    numero->exp += expoenteAnterior;
    free(magnitudeCopia);
}

static void igualarCasasDecimais_number(Number *numero1, Number *numero2) {
    if (numero1->tamanhoMagnitude == numero2->tamanhoMagnitude) {
        return;
    }

    normalizar_number(numero1);
    normalizar_number(numero2);

    Number *numeroMaiorTamanho;
    Number *numeroMenorTamanho;

    if (numero1->tamanhoMagnitude > numero2->tamanhoMagnitude) {
        numeroMaiorTamanho = numero1;
        numeroMenorTamanho = numero2;
    }

    else {
        numeroMaiorTamanho = numero2;
        numeroMenorTamanho = numero1;
    }

    size_t maiorTamanho = numeroMaiorTamanho->tamanhoMagnitude;
    size_t menorTamanho = numeroMenorTamanho->tamanhoMagnitude;

    ajustarTamanho_number(numeroMenorTamanho, maiorTamanho);
    for (size_t i = menorTamanho - 1; i < maiorTamanho - 1; i++) {
        numeroMenorTamanho->magnitude[i] = '0';
    }

    numeroMenorTamanho->magnitude[maiorTamanho - 1] = '\0';
}

static void anexarDigitoAEsquerda_number(Number *numero, char digito) {
    ajustarTamanho_number(numero, numero->tamanhoMagnitude + 1);
    for (size_t i = numero->tamanhoMagnitude - 1; i > 0; i--) {
        numero->magnitude[i] = numero->magnitude[i - 1];
    }

    numero->magnitude[0] = digito;
}

static Number* somarPotenciaDeDez_number(Number *numero, ssize_t exp) {
    normalizar_number(numero);
    size_t tamanhoNumero = numero->tamanhoMagnitude;
    char *valorResultado = malloc(tamanhoNumero * sizeof(char)); //TRATAR EM CASO DE ERRO
    Number *resultado;

    strcpy(valorResultado, numero->magnitude);
    ssize_t indiceRelativoNoVetor = numero->exp - exp; // NOME VARIAVEL RUIM

    if (indiceRelativoNoVetor > 0) {
        indiceRelativoNoVetor++;
        if (valorResultado[indiceRelativoNoVetor] != '9') {
            valorResultado[indiceRelativoNoVetor]++;
            resultado = novo_number(valorResultado);

            return resultado;
        }

        size_t j;
        for (size_t i = indiceRelativoNoVetor; i >= 0; i--) {
            if (valorResultado[i] == '.' || valorResultado[i] == ',') {
                continue;
            }

            if (valorResultado[i] == '9') {
                valorResultado[i] = '0';
                j = i;
            }

            else {
                break;
            }
        }

        resultado = novo_number(valorResultado);

        if (j == 0) {
            anexarDigitoAEsquerda_number(resultado, '1');
            normalizar_number(resultado);
            return resultado;
        }

        resultado->magnitude[j - 1]++;
        return resultado;
    }

    if (indiceRelativoNoVetor == 0) {
        if (valorResultado[indiceRelativoNoVetor] != '9') {
            valorResultado[indiceRelativoNoVetor]++;
            resultado = novo_number(valorResultado);

            return resultado;
        }
    }

    if (indiceRelativoNoVetor < 0) {
        size_t quantidadeCasasPraEsquerda = -indiceRelativoNoVetor;
        resultado = novo_number(valorResultado);
        
        for (size_t i = 0; i < quantidadeCasasPraEsquerda - 1; i++) {
            anexarDigitoAEsquerda_number(resultado, '0');
        }
        anexarDigitoAEsquerda_number(resultado, '1');
        
        normalizar_number(resultado);
        return resultado;
    }

    free(valorResultado);
}

static void somarPotenciaDeDezESalvar_number(Number *numero, ssize_t exp) {
    Number *tmp = somarPotenciaDeDez_number(numero, exp);
    fechar_number(numero);
    numero = tmp;
}

static Number* duplicar_number(Number *numero) {
    Number *copiaNumero = novo_number(numero->magnitude);
    copiaNumero->exp = numero->exp;
    copiaNumero->sinal = numero->sinal;

    return copiaNumero;
}

Number* novo_number(char *valor) {
    if (!isValorValido_number(valor)) {
        return NULL; //ERRO
    }
    size_t tamanhoValor = strlen(valor) + 1;
    
    Number *ptrNumero = malloc(1 * sizeof(Number));
    char* copiaValor = malloc(tamanhoValor * sizeof(char));
    //TRATAR CASO DÊ ERRO
    strcpy(copiaValor, valor);


    ptrNumero->magnitude = copiaValor;
    ptrNumero->tamanhoMagnitude = tamanhoValor;
    ptrNumero->exp = 0;
    definirSinal_number(ptrNumero);

    normalizar_number(ptrNumero);
    registrar_number(ptrNumero);

    return ptrNumero;
}

int isNulo_number(Number *numero) {
    size_t qZerosDepoisVirgula = contarZerosSeguidosDepoisVirgula_number(numero);
    if (numero->magnitude[0] == '0' && numero->tamanhoMagnitude - qZerosDepoisVirgula == 3) {
        return 1;
    }

    return 0;
}

void fecharTodos_number() {
    if (todosOsNumeros == NULL) {
        return;
    }

    for (int i = 0; i < quantidadeNumeros; i++) {
        free(todosOsNumeros[i]);
    }

    free(todosOsNumeros);
}

void fechar_number(Number *numero) {
    removerEmTodos_number(numero);
    free(numero);
}

int isMaior_number(Number *antecedente, Number *consequente) {
    normalizar_number(antecedente);
    normalizar_number(consequente);

    if (antecedente->exp > consequente->exp) {
        if (antecedente->sinal == 0) {
            return 1;
        }

        if (antecedente->sinal == 1) {
            return 0;
        }
    }

    if (antecedente->exp < consequente->exp) {
        if (antecedente->sinal == 0) {
            return 0;
        }

        if (antecedente->sinal == 1) {
            return 1;
        }
    }

    igualarCasasDecimais_number(antecedente, consequente);
    size_t tamanhoNumeros = antecedente->tamanhoMagnitude;

    for (size_t i = 0; i < tamanhoNumeros; i++) {
        if (antecedente->magnitude[i] == '.' || antecedente->magnitude[i] == ',' ) {
            continue;
        }

        if (antecedente->magnitude[i] - '0' > consequente->magnitude[i] - '0') {
            return 1;
        }

        if (antecedente->magnitude[i] - '0' < consequente->magnitude[i] - '0') {
            return 0;
        }
    }

    return 0;
}

int isIgual_number(Number *numero1, Number *numero2) {
    igualarCasasDecimais_number(numero1, numero2);
    if (numero1->exp != numero2->exp) {
        return 0;
    }

    size_t tamanhoNumeros = numero1->tamanhoMagnitude;
    for (int i = 0; i < tamanhoNumeros; i++) {
        if (i == 1) {
            continue;
        }

        if (numero1->magnitude[i] != numero2->magnitude[i]) {
            return 0;
        }
    }

    return 1;
}

int isMenor_number(Number *antecedente, Number *consequente) {
    if (!(isMaior_number(antecedente, consequente) || isIgual_number(antecedente, consequente))) {
        return 1;
    }

    return 0;
}

Number* soma_number(Number *numero1, Number *numero2) {
    if (numero1->sinal == numero2->sinal) {
        Number *numeroMaiorTamanho;
        Number *numeroMenorTamanho;

        if (numero1->tamanhoMagnitude > numero2->tamanhoMagnitude) {
            numeroMaiorTamanho = numero1;
            numeroMenorTamanho = numero2;
        }

        else {
            numeroMaiorTamanho = numero2;
            numeroMenorTamanho = numero1;
        }

        Number *resultado = duplicar_number(numeroMaiorTamanho);

        ssize_t exp = numeroMenorTamanho->exp;
        for (size_t i = 0; i < numeroMenorTamanho->tamanhoMagnitude; i++) {
            if (numeroMenorTamanho->magnitude[i] == '.' || numeroMenorTamanho->magnitude[i] == ',') {
                continue;
            }

            for (int j = 0; j < numeroMenorTamanho->magnitude[i] - '0'; j++) {
                somarPotenciaDeDezESalvar_number(resultado, exp);
            }
            exp--;
        }

        return resultado;
    }

    Number *numeroPositivo;
    Number *numeroNegativo;

    if (numero1->sinal == 0) {
        numeroPositivo = numero1;
        numeroNegativo = duplicar_number(numero2);
    }

    else {
        numeroPositivo = numero2;
        numeroNegativo = duplicar_number(numero1);
    }

    numeroNegativo->sinal = 0;
    Number *resultado = subtracao_number(numeroPositivo, numeroNegativo);
    fechar_number(numeroNegativo);

    return resultado;
}

Number* subtracao_number(Number *minuendo, Number *subtraendo) {
    if (minuendo->sinal == subtraendo->sinal) {
        int sinal = minuendo->sinal;
        if (sinal == 1) {
            Number *tmp = subtraendo;
            subtraendo = minuendo;
            minuendo = tmp;
        }

        //FALTA CONTINUAR A LÓGICA DA SUBTRAÇÃO (SOMA N FUCNIONA AINDA)
    }
}