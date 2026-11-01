#include "bigint.h"

#include <exception>
#include <iostream>
#include <string>

namespace {

int casos = 0;
int fallos = 0;

// Multiplica a * b con los dos algoritmos y exige que ambos den el valor esperado.
// Compara vectores y no textos porque aString normaliza y esconderia un resultado
// con ceros de sobra.
void comprobar(const std::string& a, const std::string& b, const std::string& esperado) {
    ++casos;
    try {
        const Num x = desdeString(a);
        const Num y = desdeString(b);
        const Num correcto = desdeString(esperado);
        const Num clasica = multiplicacionClasica(x, y);
        const Num rapida = karatsuba(x, y);

        if (clasica != correcto || rapida != correcto) {
            ++fallos;
            std::cout << "FALLA " << a << " x " << b << ": esperado " << esperado
                      << ", clasica " << aString(clasica) << " (" << clasica.size() << " digitos)"
                      << ", karatsuba " << aString(rapida) << " (" << rapida.size() << " digitos)\n";
        }
    } catch (const std::exception& e) {
        // restar lanza una excepcion si a < b: cuenta como fallo y las pruebas siguen.
        ++fallos;
        std::cout << "FALLA " << a << " x " << b << ": excepcion: " << e.what() << "\n";
    }
}

// La interfaz promete que la entrada queda normalizada, asi que se revisa el vector directamente.
void comprobarConversion(const std::string& texto, const Num& vector, const std::string& salida) {
    ++casos;
    if (desdeString(texto) != vector || aString(vector) != salida) {
        ++fallos;
        std::cout << "FALLA conversion de \"" << texto << "\"\n";
    }
}

void probarConversiones() {
    comprobarConversion("123", Num{3, 2, 1}, "123");
    comprobarConversion("00123", Num{3, 2, 1}, "123");
    comprobarConversion("0", Num{0}, "0");
    comprobarConversion("0000", Num{0}, "0");
    comprobarConversion("1002", Num{2, 0, 0, 1}, "1002");
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

// Longitudes que no son potencia de 2: obligan a rellenar antes de la recursion.
// Los nueves hacen que x1 + x0 crezca un digito.
void probarLongitudesImpares() {
    comprobar("123", "456", "56088");
    comprobar("12345", "67890", "838102050");
    comprobar("1234567", "7654321", "9449772114007");
    comprobar("999", "999", "998001");
    comprobar("99999", "99999", "9999800001");
    comprobar("9999999", "9999999", "99999980000001");
    comprobar("123", "4567890", "561850470");
    comprobar("7654321", "123", "941481483");
    comprobar("12345", "7", "86415");
}

// Texto formado por repeticiones de un patron: "12" y 3 da "121212".
std::string repetir(const std::string& patron, int veces) {
    std::string resultado;
    for (int i = 0; i < veces; ++i) {
        resultado += patron;
    }
    return resultado;
}

// Productos de varios cientos de digitos. Los resultados de nueves y de potencias
// de 10 salen de una formula, asi que no dependen de ninguno de los dos algoritmos:
// (10^n - 1)^2 = 99..9 8 00..0 1, con n - 1 nueves y n - 1 ceros.
void probarProductosGrandes() {
    for (int n : {300, 512, 513, 777}) {
        const std::string nueves(n, '9');
        comprobar(nueves, nueves, std::string(n - 1, '9') + "8" + std::string(n - 1, '0') + "1");

        const std::string potencia = "1" + std::string(n, '0');
        comprobar(potencia, potencia, "1" + std::string(2 * n, '0'));
    }

    // Longitudes muy distintas: (10^500 - 1) * 999 = 998 99..9 001, con 497 nueves.
    comprobar(std::string(500, '9'), "999", "998" + std::string(497, '9') + "001");

    // Sin formula sencilla: aqui se compara Karatsuba contra el clasico, que es la referencia.
    const std::string a = repetir("1234567890", 30);
    const std::string b = repetir("9876543210", 20);
    comprobar(a, b, aString(multiplicacionClasica(desdeString(a), desdeString(b))));
}

}

int main() {
    probarConversiones();
    probarUnDigito();
    probarCerosIzquierda();
    probarCerosIntercalados();
    probarLongitudesImpares();
    probarProductosGrandes();

    std::cout << casos << " casos, " << fallos << " fallos\n";
    return fallos == 0 ? 0 : 1;
}
