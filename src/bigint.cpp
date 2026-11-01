#include "bigint.h"

#include <algorithm>
#include <stdexcept>

void normalizar(Num& a) {
    while (a.size() > 1 && a.back() == 0) {
        a.pop_back();
    }
    if (a.empty()) {
        a.push_back(0);
    }
}

Num desdeString(const std::string& s) {
    if (s.empty()) {
        throw std::invalid_argument("El numero no puede estar vacio");
    }

    Num resultado;
    resultado.reserve(s.size());
    for (auto it = s.rbegin(); it != s.rend(); ++it) {
        if (*it < '0' || *it > '9') {
            throw std::invalid_argument("Solo se admiten digitos de 0 a 9");
        }
        resultado.push_back(*it - '0');
    }
    normalizar(resultado);
    return resultado;
}

std::string aString(const Num& a) {
    Num copia = a;
    normalizar(copia);
    std::string resultado;
    resultado.reserve(copia.size());
    for (auto it = copia.rbegin(); it != copia.rend(); ++it) {
        resultado.push_back(static_cast<char>('0' + *it));
    }
    return resultado;
}

Num sumar(const Num& a, const Num& b) {
    const std::size_t n = std::max(a.size(), b.size());
    Num resultado;
    resultado.reserve(n + 1);
    int acarreo = 0;

    for (std::size_t i = 0; i < n; ++i) {
        const int digitoA = i < a.size() ? a[i] : 0;
        const int digitoB = i < b.size() ? b[i] : 0;
        const int total = digitoA + digitoB + acarreo;
        resultado.push_back(total % 10);
        acarreo = total / 10;
    }
    if (acarreo != 0) {
        resultado.push_back(acarreo);
    }
    normalizar(resultado);
    return resultado;
}

Num restar(const Num& a, const Num& b) {
    const std::size_t n = std::max(a.size(), b.size());
    Num resultado;
    resultado.reserve(n);
    int prestamo = 0;

    for (std::size_t i = 0; i < n; ++i) {
        const int digitoA = i < a.size() ? a[i] : 0;
        const int digitoB = i < b.size() ? b[i] : 0;
        int diferencia = digitoA - digitoB - prestamo;
        if (diferencia < 0) {
            diferencia += 10;
            prestamo = 1;
        } else {
            prestamo = 0;
        }
        resultado.push_back(diferencia);
    }
    if (prestamo != 0) {
        throw std::invalid_argument("La resta requiere que a sea mayor o igual que b");
    }
    normalizar(resultado);
    return resultado;
}
