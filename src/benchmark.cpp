#include "bigint.h"

#include <chrono>
#include <iostream>
#include <random>

namespace {

using Multiplicacion = Num (*)(const Num&, const Num&);

// Se imprime al final: como el resultado se usa, el compilador no puede eliminar
// las multiplicaciones al optimizar con -O2.
unsigned long long checksum = 0;

// Numero de exactamente n digitos: el mas significativo nunca es 0.
Num numeroAleatorio(int digitos, std::mt19937& generador) {
    std::uniform_int_distribution<int> digito(0, 9);
    std::uniform_int_distribution<int> primero(1, 9);

    Num resultado(digitos);
    for (int& d : resultado) {
        d = digito(generador);
    }
    resultado.back() = primero(generador);
    return resultado;
}

// Tiempo promedio de una multiplicacion. Duplica el lote hasta que dure al menos
// 100 ms y promedia solo ese lote, asi el reloj no se lee en cada multiplicacion.
double medirMicrosegundos(Multiplicacion algoritmo, const Num& a, const Num& b) {
    using Reloj = std::chrono::steady_clock;
    const std::chrono::milliseconds minimo(100);

    for (long long lote = 1;; lote *= 2) {
        const auto inicio = Reloj::now();
        for (long long i = 0; i < lote; ++i) {
            const Num resultado = algoritmo(a, b);
            checksum += resultado[0] + resultado.size();
        }
        const auto duracion = Reloj::now() - inicio;

        if (duracion >= minimo) {
            return std::chrono::duration<double, std::micro>(duracion).count() / lote;
        }
    }
}

}

int main() {
    std::mt19937 generador(20261101);

    std::cout << "digitos  clasica_us  karatsuba_us\n";
    for (int n = 2; n <= 4096; n *= 2) {
        const Num a = numeroAleatorio(n, generador);
        const Num b = numeroAleatorio(n, generador);
        const double clasica = medirMicrosegundos(multiplicacionClasica, a, b);
        const double rapida = medirMicrosegundos(karatsuba, a, b);
        std::cout << n << "  " << clasica << "  " << rapida << "\n";
    }
    std::cout << "checksum: " << checksum << "\n";
    return 0;
}
