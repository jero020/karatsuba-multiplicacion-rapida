# Multiplicación rápida de enteros: Karatsuba vs. método clásico

Práctica 2 — Análisis y Diseño de Algoritmos, Universidad EAFIT.

Implementación en C++ de la multiplicación de enteros grandes representados como dígitos en base 10, comparando el método clásico Θ(n²) con el algoritmo de Karatsuba (Divide y Vencerás), Θ(n^log₂3) ≈ Θ(n^1.585).

## Integrantes

Javier Andres Sierra Machado
Jeronimo Velez Acosta


## Estructura del repositorio

```
include/      Interfaz compartida (bigint.h)
src/          Código fuente: auxiliares, algoritmos, pruebas y benchmark
resultados/   Tiempos medidos (CSV), script de gráfica y gráficas
informe/      Informe de la práctica
```

## Estado

- [x] Representación y operaciones auxiliares
- [x] Multiplicación clásica
- [x] Karatsuba
- [ ] Validación
- [ ] Medición de tiempos y punto de cruce
- [ ] Informe

## Compilación y ejecución

Requisitos: compilador compatible con C++17 y GNU Make. Ejecutar los comandos desde la raíz del repositorio, después de integrar la parte B.

```sh
make tests
make bench
```

En Windows con MinGW, el comando puede llamarse `mingw32-make`:

```powershell
mingw32-make tests
mingw32-make bench
```

Para compilar sin Make:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -Iinclude src/bigint.cpp src/multiplicacion.cpp src/tests.cpp -o karatsuba_tests
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -Iinclude src/bigint.cpp src/multiplicacion.cpp src/benchmark.cpp -o karatsuba_bench
```

En Linux o macOS se ejecutan con `./karatsuba_tests` y `./karatsuba_bench`. En Windows, con `./karatsuba_tests.exe` y `./karatsuba_bench.exe` desde PowerShell.

## Representación

Usamos `Num`, un alias de `std::vector<int>`, en base 10. El índice 0 contiene las unidades: `123` se guarda como `{3, 2, 1}`. Cada elemento debe estar entre 0 y 9. Los números son no negativos.

Un número normalizado no tiene ceros al final del vector, excepto el cero, que se representa como `{0}`. `desdeString` acepta ceros a la izquierda y rechaza cadenas vacías o con caracteres distintos de los dígitos decimales.

## Parte A

- `include/bigint.h`: interfaz compartida del plan de trabajo.
- `src/bigint.cpp`: normalización, conversiones, suma y resta.
- `src/multiplicacion.cpp`: Karatsuba y su función recursiva interna.
- `informe/parte_A.md`: sección prevista de deducción de la identidad, Teorema Maestro y decisiones de implementación.
- `Makefile`: objetivos para compilar y ejecutar las pruebas y el benchmark al integrar la parte B.

`restar(a, b)` requiere `a >= b`. Si no se cumple, lanza `std::invalid_argument`. Las operaciones no modifican los operandos, salvo `normalizar`, que recibe una referencia al vector que debe modificar.

## Integración con la parte B

Las funciones `desplazar` y `partir` de la parte B ya están integradas en `src/bigint.cpp`. `multiplicacionClasica` ya está en `src/multiplicacion.cpp`. Queda incorporar `src/tests.cpp` y `src/benchmark.cpp`. Al unir los cambios hay que conservar las funciones de ambas partes.

`partir(a, m, alta, baja)` separa los primeros `m` dígitos como parte baja y los restantes como parte alta, con `0 < m < a.size()`. Conserva los ceros de las dos partes. `desplazar(a, k)` agrega `k` ceros en las posiciones bajas para multiplicar por `10^k`, con `k >= 0`; puede devolver un cero con varios dígitos. Las sumas de la recomposición y el resultado público de Karatsuba quedan normalizados.

Los objetivos `tests` y `bench` requieren los archivos de prueba y medición de la parte B. Las instrucciones de compilación anteriores se aplican cuando esos archivos estén incorporados. La sección del informe de A se incorpora por separado antes de unir el informe final.
