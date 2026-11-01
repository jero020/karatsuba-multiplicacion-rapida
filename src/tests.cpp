#include "bigint.h"

#include <iostream>
#include <string>

namespace {

int casos = 0;
int fallos = 0;

// Multiplica a * b con los dos algoritmos y exige que ambos den el valor esperado.
void comprobar(const std::string& a, const std::string& b, const std::string& esperado) {
    const Num x = desdeString(a);
    const Num y = desdeString(b);
    const std::string clasica = aString(multiplicacionClasica(x, y));
    const std::string rapida = aString(karatsuba(x, y));

    ++casos;
    if (clasica != esperado || rapida != esperado) {
        ++fallos;
        std::cout << "FALLA " << a << " x " << b << ": esperado " << esperado
                  << ", clasica " << clasica << ", karatsuba " << rapida << "\n";
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

}

int main() {
    probarUnDigito();

    std::cout << casos << " casos, " << fallos << " fallos\n";
    return fallos == 0 ? 0 : 1;
}
