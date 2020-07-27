**Autor:** Sebastián González Morales

**Correo institucional:** Sebastian.gonzalez@alumnos.uv.cl

**Diseño de la solución**


El problema está compuesto en dos módulos, el primero consiste en llenar un arreglo unidimensional de números aleatorios del tipo uint32_t y el segundo debe sumar el contenido del arreglo. Posteriormente se implementará y luego se realizarán pruebas de desempeño para ver el comportamiento del tiempo de ejecución de ambos módulos según el tamaño del arreglo, la cantidad de threads utilizados y el rango de números aleatorios. La implementación será en C++ versión 2017.
Tanto el tamaño del arreglo, la cantidad de threads utilizados y el rango de números aleatorios serán en forma dinámica, configurable a través de parámetros de entrada del programa y es global al proceso. Esto quiere decir que es visible para el número de threas creados dentro de él. La solución de la primera parte del módulo se denomina “Etapa de llenado”. Cada thread conoce los índices de inicio y fin donde debe almacenar un número aleatorio.
