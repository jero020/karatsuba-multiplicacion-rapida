"""Herramienta auxiliar: dibuja la tabla y la grafica de tiempos.csv.

Los algoritmos y la medicion estan en C++; este script no multiplica nada,
solo presenta lo que midio benchmark.cpp. Requiere matplotlib.
"""

import csv
import math
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt

CARPETA = Path(__file__).resolve().parent

# Las pendientes se calculan desde aqui: en tamanos menores domina el costo fijo
# de cada llamada, no el crecimiento asintotico.
DESDE_DIGITOS = 64

AZUL = "#2a78d6"
NARANJA = "#eb6834"
TEXTO = "#0b0b0b"
TEXTO_SECUNDARIO = "#52514e"
REJILLA = "#e4e3df"


def leer_tiempos(ruta):
    """Devuelve las filas del CSV con cada columna convertida a numero."""
    with open(ruta, newline="") as archivo:
        return [
            {
                "digitos": int(fila["digitos"]),
                "clasica": float(fila["clasica_us"]),
                "karatsuba": float(fila["karatsuba_us"]),
            }
            for fila in csv.DictReader(archivo)
        ]


def pendiente(filas, columna):
    """Minimos cuadrados sobre log2(tiempo) contra log2(digitos).

    Si T(n) = c * n^k, entonces log T = log c + k * log n: una recta de pendiente k.
    """
    puntos = [
        (math.log2(f["digitos"]), math.log2(f[columna]))
        for f in filas
        if f["digitos"] >= DESDE_DIGITOS
    ]
    media_x = sum(x for x, _ in puntos) / len(puntos)
    media_y = sum(y for _, y in puntos) / len(puntos)
    numerador = sum((x - media_x) * (y - media_y) for x, y in puntos)
    denominador = sum((x - media_x) ** 2 for x, _ in puntos)
    return numerador / denominador


def guardar_tabla(filas, ruta):
    """Escribe la tabla en Markdown, lista para llevar al informe."""
    lineas = [
        "| Dígitos | Clásica (µs) | Karatsuba (µs) | Karatsuba / Clásica |",
        "|---:|---:|---:|---:|",
    ]
    for f in filas:
        razon = f["karatsuba"] / f["clasica"]
        lineas.append(
            f"| {f['digitos']} | {f['clasica']:.3f} | {f['karatsuba']:.3f} | {razon:.0f} |"
        )
    ruta.write_text("\n".join(lineas) + "\n", encoding="utf-8")


def dibujar(filas, ruta):
    """Grafica log-log: en esta escala cada algoritmo se ve como una recta."""
    digitos = [f["digitos"] for f in filas]
    k_clasica = pendiente(filas, "clasica")
    k_karatsuba = pendiente(filas, "karatsuba")

    figura, ejes = plt.subplots(figsize=(8, 5), dpi=200)
    # Linea y marcador distintos para que se distingan sin color, tambien impresas.
    ejes.plot(digitos, [f["karatsuba"] for f in filas], color=NARANJA, linewidth=2,
              marker="s", markersize=6, label=f"Karatsuba (pendiente {k_karatsuba:.2f})")
    ejes.plot(digitos, [f["clasica"] for f in filas], color=AZUL, linewidth=2,
              linestyle="--", marker="o", markersize=6, label=f"Clásica (pendiente {k_clasica:.2f})")

    ejes.set_xscale("log", base=2)
    ejes.set_yscale("log", base=10)
    ejes.set_xticks(digitos)
    ejes.set_xticklabels([str(n) for n in digitos])
    ejes.minorticks_off()

    ejes.set_title("Sin punto de cruce hasta 4096 dígitos", loc="left",
                   color=TEXTO, fontsize=13, fontweight="bold")
    ejes.set_xlabel("Dígitos de cada factor (escala log₂)", color=TEXTO_SECUNDARIO)
    ejes.set_ylabel("Tiempo por multiplicación en µs (escala log₁₀)", color=TEXTO_SECUNDARIO)
    ejes.tick_params(colors=TEXTO_SECUNDARIO, length=0)
    ejes.grid(True, color=REJILLA, linewidth=0.8)
    ejes.set_axisbelow(True)
    for lado in ("top", "right", "left"):
        ejes.spines[lado].set_visible(False)
    ejes.spines["bottom"].set_color(TEXTO_SECUNDARIO)

    ejes.legend(loc="upper left", frameon=False, labelcolor=TEXTO)
    figura.text(0.01, 0.01, f"Pendientes ajustadas desde {DESDE_DIGITOS} dígitos. Teoría: 2 y log₂3 ≈ 1.585.",
                color=TEXTO_SECUNDARIO, fontsize=8)
    figura.tight_layout(rect=(0, 0.03, 1, 1))
    figura.savefig(ruta, facecolor="white")
    plt.close(figura)


def main():
    filas = leer_tiempos(CARPETA / "tiempos.csv")
    guardar_tabla(filas, CARPETA / "tabla_tiempos.md")
    dibujar(filas, CARPETA / "grafica.png")
    print(f"Pendiente clasica:   {pendiente(filas, 'clasica'):.3f}")
    print(f"Pendiente karatsuba: {pendiente(filas, 'karatsuba'):.3f}")
    print("Generados: resultados/tabla_tiempos.md y resultados/grafica.png")


if __name__ == "__main__":
    main()
