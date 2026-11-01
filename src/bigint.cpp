#include "bigint.h"

// a * 10^k: como el digito menos significativo esta en [0], basta con poner
// k ceros al inicio. No normaliza: desplazar({0}, 2) da {0,0,0}.
Num desplazar(const Num& a, int k) {
    Num r(k, 0);
    r.insert(r.end(), a.begin(), a.end());
    return r;
}
