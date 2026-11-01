#pragma once
#include <string>
#include <vector>

using Num = std::vector<int>; // base 10, digito menos significativo en [0]

// Conversion (para pruebas y salida)
Num desdeString(const std::string& s); // "00123" -> {3,2,1}
std::string aString(const Num& a);     // {3,2,1} -> "123"

// Auxiliares (seccion 4 del enunciado)
void normalizar(Num& a);                            // quita ceros a la izquierda; deja {0} si es cero
Num sumar(const Num& a, const Num& b);              // longitudes distintas; puede crecer 1 digito
Num restar(const Num& a, const Num& b);             // PRECONDICION: a >= b; resultado normalizado
Num desplazar(const Num& a, int k);                 // a * 10^k (inserta k ceros en las posiciones bajas)
void partir(const Num& a, int m, Num& alta, Num& baja); // baja = digitos [0, m), alta = digitos [m, n)

// Algoritmos
Num multiplicacionClasica(const Num& a, const Num& b); // Theta(n^2)
Num karatsuba(const Num& a, const Num& b);             // rellena a potencia de 2 y llama a la recursion
