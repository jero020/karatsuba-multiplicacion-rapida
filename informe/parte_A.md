# Multiplicación de enteros grandes: fundamento e implementación de Karatsuba

## Deducción de la identidad

Para multiplicar dos enteros de muchos dígitos, dividimos cada número en una parte alta y una parte baja. Si la parte baja ocupa m posiciones decimales, escribimos:

$$x = x_1 10^m + x_0, \qquad y = y_1 10^m + y_0.$$

Al desarrollar el producto obtenemos:

$$xy = x_1y_1 10^{2m} + (x_1y_0 + x_0y_1)10^m + x_0y_0.$$

Una implementación directa de esta expresión necesita cuatro multiplicaciones entre las partes: las dos de los extremos y las dos del término central. Karatsuba calcula estos tres productos:

$$P_1=x_1y_1, \qquad P_2=x_0y_0, \qquad P_3=(x_1+x_0)(y_1+y_0).$$

Desarrollamos el tercero para recuperar los términos que faltan:

$$P_3 = x_1y_1+x_1y_0+x_0y_1+x_0y_0.$$

Por tanto,

$$P_3-P_1-P_2=x_1y_0+x_0y_1.$$

La recomposición queda:

$$xy=P_1 10^{2m}+(P_3-P_1-P_2)10^m+P_2.$$

Con esta identidad calculamos tres productos recursivos. A cambio, hacemos sumas y restas sobre los dígitos. Por ejemplo, para 12 por 34, con m = 1, tenemos P1 = 3, P2 = 8 y P3 = 21. El término central vale 21 - 3 - 8 = 10; al recomponer obtenemos 300 + 100 + 8 = 408.

## Recurrencia y Teorema Maestro

Tomamos n como la cantidad de dígitos del operando más largo. En el modelo de partición por mitades, cada llamada calcula tres productos de tamaño cercano a n/2. Las sumas, restas, copias y desplazamientos recorren una cantidad de dígitos proporcional a n. La recurrencia que usamos para el análisis es:

$$T(n)=3T(n/2)+\Theta(n), \qquad T(1)=\Theta(1).$$

En el Teorema Maestro identificamos a = 3, b = 2 y f(n) = Θ(n). Comparamos el trabajo fuera de la recursión con:

$$n^{\log_b a}=n^{\log_2 3}\approx n^{1.585}.$$

Como 1 < log₂3, el costo lineal crece más despacio por un factor polinómico. Podemos escoger ε = log₂3 - 1 > 0 para escribir:

$$f(n)=\Theta(n)=O\left(n^{\log_2 3-\varepsilon}\right).$$

Aplicamos el caso 1 del Teorema Maestro y concluimos:

$$T(n)=\Theta\left(n^{\log_2 3}\right).$$

El exponente log₂3 relaciona los tres subproblemas con la reducción a la mitad de su tamaño. Con cuatro productos tendríamos T(n) = 4T(n/2) + Θ(n), cuyo resultado sería Θ(n²), el mismo orden del método clásico.

La recurrencia anterior simplifica los redondeos y el acarreo de las sumas. En el código, las partes pueden tener tamaños distintos y una suma de mitades puede crecer un dígito. Por eso no afirmamos que cada llamada trabaje sobre exactamente n/2 dígitos: algunos operandos alcanzan ⌈n/2⌉ + 1. Ese incremento acotado no cambia el exponente asintótico; para aplicarlo al código distinguimos este detalle del modelo ideal por mitades.

## Representación y operaciones auxiliares

Representamos cada entero no negativo con un `std::vector<int>` en base 10. Guardamos las unidades en la posición 0. Por ejemplo, 1203 corresponde a `{3, 0, 2, 1}`. Así recorremos el vector desde el inicio para propagar acarreos y préstamos.

`normalizar` elimina ceros sobrantes del final del vector y conserva `{0}` para el cero. `desdeString` recorre la cadena desde el último carácter, verifica que contenga dígitos y normaliza el resultado. `aString` recorre los dígitos en el orden contrario para producir la escritura decimal habitual.

En `sumar` recorremos la longitud del operando más largo. Cuando un operando ya no tiene dígitos, usamos cero en esa posición. Guardamos el residuo módulo 10 y llevamos el cociente como acarreo. Si queda un acarreo al terminar, agregamos otro dígito. Esto permite, por ejemplo, sumar 999 + 1 y obtener 1000.

`restar` requiere que el primer operando sea mayor o igual que el segundo. En cada posición descontamos el préstamo anterior. Si la diferencia es negativa, sumamos 10 y trasladamos un préstamo a la siguiente posición. Al terminar normalizamos el resultado. La función lanza una excepción si queda un préstamo final, porque eso indica que se incumplió la precondición. La suma y la resta tienen costo lineal en la longitud del operando más largo.

## Implementación de Karatsuba

La función pública `karatsuba` copia y normaliza los operandos. Si uno es cero, devuelve `{0}`. Después busca la menor potencia de 2 que cubre ambas longitudes y rellena con ceros hasta igualarlas. Como usamos los dígitos en orden inverso, agrega esos ceros al final del vector.

La función interna `karatsubaRec` recibe copias que puede redimensionar sin modificar los operandos originales. Toma n como la mayor longitud de la llamada y completa el operando más corto con ceros. Si n = 1, multiplica los dos dígitos: el producto está entre 0 y 81 y cabe en un `int`. Lo convierte a uno o dos dígitos. No usamos tipos nativos para representar el producto de números grandes.

Para n mayor que 1, tomamos m = ⌊n/2⌋ y usamos `partir` para obtener las partes altas y bajas. Calculamos P1, P2 y P3 con llamadas recursivas. Las sumas de las partes pueden tener un dígito adicional; conservamos ese dígito y dejamos que la llamada siguiente adapte las longitudes. No truncamos los resultados para forzarlos al tamaño inicial.

Calculamos el término central como `restar(restar(p3, p1), p2)`. Este orden respeta la precondición de la resta, pues:

$$P_3-P_1=P_2+x_1y_0+x_0y_1\geq P_2.$$

Así, las dos restas producen valores no negativos. Por último, desplazamos P1 en 2m posiciones y el término central en m posiciones, y sumamos ambos con P2. La función pública normaliza el producto antes de devolverlo.

La implementación usa un caso base de un dígito y no delega productos grandes en otra biblioteca. Las copias y las reservas de memoria agregan costos que deben considerarse al interpretar las mediciones. Tener un exponente menor no implica que Karatsuba gane para todos los tamaños; la comparación de tiempos corresponde al análisis experimental del informe conjunto.
