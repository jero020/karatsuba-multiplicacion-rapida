#include "bigint.h"

#include <algorithm>

namespace {

bool esCero(const Num& a) {
    return std::all_of(a.begin(), a.end(), [](int digito) {
        return digito == 0;
    });
}

Num karatsubaRec(Num a, Num b) {
    if (esCero(a) || esCero(b)) {
        return Num{0};
    }

    const std::size_t n = std::max(a.size(), b.size());
    if (n == 1) {
        const int producto = a[0] * b[0];
        Num resultado{producto % 10};
        if (producto >= 10) {
            resultado.push_back(producto / 10);
        }
        return resultado;
    }

    a.resize(n, 0);
    b.resize(n, 0);
    const int m = static_cast<int>(n / 2);
    Num x1, x0, y1, y0;
    partir(a, m, x1, x0);
    partir(b, m, y1, y0);

    const Num p1 = karatsubaRec(x1, y1);
    const Num p2 = karatsubaRec(x0, y0);
    const Num p3 = karatsubaRec(sumar(x1, x0), sumar(y1, y0));
    const Num centro = restar(restar(p3, p1), p2);

    return sumar(sumar(desplazar(p1, 2 * m), desplazar(centro, m)), p2);
}

}

Num karatsuba(const Num& a, const Num& b) {
    Num x = a;
    Num y = b;
    normalizar(x);
    normalizar(y);
    if (esCero(x) || esCero(y)) {
        return Num{0};
    }

    const std::size_t mayor = std::max(x.size(), y.size());
    std::size_t n = 1;
    while (n < mayor) {
        n *= 2;
    }
    x.resize(n, 0);
    y.resize(n, 0);

    Num resultado = karatsubaRec(x, y);
    normalizar(resultado);
    return resultado;
}

// Metodo de la escuela: cada digito de a por cada digito de b, Theta(n^2).
// El digito a[i] * b[j] aporta en la posicion i + j, y el producto de un numero
// de n digitos por uno de m digitos tiene a lo sumo n + m digitos.
Num multiplicacionClasica(const Num& a, const Num& b) {
    Num resultado(a.size() + b.size(), 0);

    for (std::size_t i = 0; i < a.size(); ++i) {
        int acarreo = 0;
        for (std::size_t j = 0; j < b.size(); ++j) {
            const int total = resultado[i + j] + a[i] * b[j] + acarreo;
            resultado[i + j] = total % 10;
            acarreo = total / 10;
        }
        resultado[i + b.size()] += acarreo;
    }
    normalizar(resultado);
    return resultado;
}
