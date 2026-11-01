#include "bigint.h"

#include <iostream>
#include <string>

namespace {

int casos = 0;
int fallos = 0;

// Multiplica a * b con los dos algoritmos y exige que ambos den el valor esperado.
// Compara vectores y no textos porque aString normaliza y esconderia un resultado
// con ceros de sobra.
void comprobar(const std::string& a, const std::string& b, const std::string& esperado) {
    const Num x = desdeString(a);
    const Num y = desdeString(b);
    const Num correcto = desdeString(esperado);
    const Num clasica = multiplicacionClasica(x, y);
    const Num rapida = karatsuba(x, y);

    ++casos;
    if (clasica != correcto || rapida != correcto) {
        ++fallos;
        std::cout << "FALLA " << a << " x " << b << ": esperado " << esperado
                  << ", clasica " << aString(clasica) << " (" << clasica.size() << " digitos)"
                  << ", karatsuba " << aString(rapida) << " (" << rapida.size() << " digitos)\n";
    }
}

// Caso base de Karatsuba: los 100 productos posibles de dos digitos, incluidos 0 x 9 y 9 x 9.
void probarUnDigito() {
    for (int a = 0; a <= 9; ++a) {
        for (int b = 0; b <= 9; ++b) {
            comprobar(std::to_string(a), std::to_string(b), std::to_string(a * b));
        }
    }
}

// Los ceros a la izquierda no cambian el valor: la entrada debe normalizarse.
void probarCerosIzquierda() {
    comprobar("000123", "45", "5535");
    comprobar("000123", "000456", "56088");
    comprobar("000", "000123", "0");
    comprobar("0000", "0000", "0");
    comprobar("0009", "0009", "81");
}

// Los ceros en medio del numero si tienen valor: no se pueden perder al partir.
void probarCerosIntercalados() {
    comprobar("100200300", "4005", "401302201500");
    comprobar("1000", "1000", "1000000");
    comprobar("10000001", "10000001", "100000020000001");
    comprobar("1010101", "101", "102020201");
    comprobar("90009", "90009", "8101620081");
}

}

int main() {
    probarUnDigito();
    probarCerosIzquierda();
    probarCerosIntercalados();

    std::cout << casos << " casos, " << fallos << " fallos\n";
    return fallos == 0 ? 0 : 1;
}
