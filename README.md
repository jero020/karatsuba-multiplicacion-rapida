# Multiplicación rápida de enteros: Karatsuba vs. método clásico

Práctica 2 — Análisis y Diseño de Algoritmos, Universidad EAFIT.

Implementación en C++ de la multiplicación de enteros grandes representados como dígitos en base 10, comparando el método clásico Θ(n²) con el algoritmo de Karatsuba (Divide y Vencerás), Θ(n^log₂3) ≈ Θ(n^1.585).

## Integrantes

- Javier Andres Sierra Machado
- Jeronimo Velez Acosta

## Estructura del repositorio

```
include/bigint.h        Interfaz compartida
src/bigint.cpp          Conversiones y operaciones auxiliares
src/multiplicacion.cpp  Multiplicación clásica y Karatsuba
src/tests.cpp           Validación de correctitud
src/benchmark.cpp       Medición de tiempos
resultados/             Tiempos (CSV), tabla, gráfica y script auxiliar de la gráfica
informe/                Informe de la práctica
Makefile                Objetivos tests y bench
```

## Estado

- [x] Representación y operaciones auxiliares
- [x] Multiplicación clásica
- [x] Karatsuba
- [x] Validación
- [x] Medición de tiempos y punto de cruce
- [x] Informe 

## Compilación y ejecución

Requisitos: compilador compatible con C++17 y GNU Make. Ejecutar los comandos desde la raíz del repositorio.

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

`make tests` compara Karatsuba y el método clásico en 134 casos e imprime el total de fallos; termina con error si alguno falla. Cubre números de 1 dígito, ceros a la izquierda, ceros intercalados, longitudes impares y productos de varios cientos de dígitos.

`make bench` mide ambos algoritmos de 2 a 4096 dígitos, tarda cerca de un minuto y **reemplaza** `resultados/tiempos.csv`. Los valores del repositorio son los de la corrida usada en el informe; para repetirla, conviene cerrar otros programas. Si se ejecuta solo para probar, se puede restaurar el archivo con `git restore resultados/tiempos.csv`.

## Tabla y gráfica

Los algoritmos y la medición están en C++. `resultados/graficar.py` es una herramienta auxiliar que solo presenta los tiempos medidos; no hace falta ejecutarla, porque la tabla y la gráfica ya están en `resultados/`. Para regenerarlas a partir de `resultados/tiempos.csv` se necesita Python 3 con matplotlib:

```sh
python3 resultados/graficar.py
```

## Representación

Usamos `Num`, un alias de `std::vector<int>`, en base 10. El índice 0 contiene las unidades: `123` se guarda como `{3, 2, 1}`. Cada elemento debe estar entre 0 y 9. Los números son no negativos.

Un número normalizado no tiene ceros al final del vector, excepto el cero, que se representa como `{0}`. `desdeString` acepta ceros a la izquierda y rechaza cadenas vacías o con caracteres distintos de los dígitos decimales.

## Operaciones

`restar(a, b)` requiere `a >= b`. Si no se cumple, lanza `std::invalid_argument`. Las operaciones no modifican los operandos, salvo `normalizar`, que recibe una referencia al vector que debe modificar.

`partir(a, m, alta, baja)` separa los primeros `m` dígitos como parte baja y los restantes como parte alta, con `0 < m < a.size()`. Conserva los ceros de las dos partes. `desplazar(a, k)` agrega `k` ceros en las posiciones bajas para multiplicar por `10^k`, con `k >= 0`; puede devolver un cero con varios dígitos. Las sumas de la recomposición y el resultado público de Karatsuba quedan normalizados.

## Reparto del trabajo

- **Parte A (Javier):** `normalizar`, `desdeString`, `aString`, `sumar`, `restar`, `karatsuba`, `Makefile` e `informe/parte_A.md`.
- **Parte B (Jeronimo):** `desplazar`, `partir`, `multiplicacionClasica`, `src/tests.cpp`, `src/benchmark.cpp` y `resultados/`.
