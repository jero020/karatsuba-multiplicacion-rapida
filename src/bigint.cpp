#include "bigint.h"

// a * 10^k: como el digito menos significativo esta en [0], basta con poner
// k ceros al inicio. No normaliza: desplazar({0}, 2) da {0,0,0}.
Num desplazar(const Num& a, int k) {
    Num r(k, 0);
    r.insert(r.end(), a.begin(), a.end());
    return r;
}

// a = alta * 10^m + baja. PRECONDICION: 0 < m < a.size(). No normaliza: Karatsuba
// necesita conservar los ceros, 1002 con m = 2 da baja = {2,0} y alta = {0,1}.
void partir(const Num& a, int m, Num& alta, Num& baja) {
    baja.assign(a.begin(), a.begin() + m);
    alta.assign(a.begin() + m, a.end());
}
