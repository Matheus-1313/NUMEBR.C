#include <stdio.h>
#include "Number.h"

int main() {
    Number *numero = novoNumero("67214.47218");
    printf("%s %d", numero->magnitude, numero->exp);

    return 0;
}
