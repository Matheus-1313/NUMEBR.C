#ifndef NUMBER_H
#define NUMBER_H

typedef struct Number Number;

Number* novo_number(char* valor);
int isNulo_number(Number *numero);
void fechar_number(Number *numero);
void fecharTodos_number();
int isMaior_number(Number *antecedente, Number *consequente);
int isIgual_number(Number *numero1, Number *numero2);
int isMenor_number(Number *antecedente, Number *consequente);
Number* soma_number(Number *numero1, Number *numero2);
Number* subtracao_number(Number *minuendo, Number *subtraendo);


/*
FUNÇÕES QUE O USUÁRIO NÃO TEM ACESSO
void normalizar_number(Number *numero);
void definirSinal_number(Number *numero);
size_t contarDigitosAntesVirgula_number(Number *numero);
size_t contarZerosSeguidosDepoisVirgula_number(Number *numero);
void ajustarTamanho_number(Number *numero, size_t novoTamanho);
int isNormalizado_number(Number *numero);
int isValorValido_number(char *valor);
void registrar_number(Number *numero);
size_t encontrarEmTodos_number(Number *numero);
void removerEmTodos_number(Number *numero);
void igualarCasasDecimais_number(Number *numero1, Number *numero2);
void anexarDigitoAEsquerda_number(Number *numero, char digito);
Number* somarPotenciaDeDez_number(Number *numero, ssize_t exp);
void somarPotenciaDeDezESalvar_number(Number *numero, ssize_t exp);
static Number* duplicar_number(Number *numero);
*/
#endif