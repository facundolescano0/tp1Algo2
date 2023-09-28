

# TP1

## Repositorio de Facundo Lescano - 110784 - flescano@fi.uba.ar

- Para compilar:

```bash
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g

```

- Para ejecutar:

```bash

```

- Para ejecutar con valgrind:
```bash
valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yess

```
---
##  Funcionamiento

El TP funciona en base a dos estructuras(ademas de struct ataque). Estrucuta pokemon asigné nombre, tipo, un vector de ataques y un "tope". Estructura info_pokemon contiene un puntero a pokemones,que será tratado como vector y un topede cantidad de pokemones. 

El programa funciona abriendo un archivo que contiene pokemones el cual será validado y parseado linea por linea para poder llenar la informacion de cada pokemon, si es que lo hay(con formato correcto). Para poder cargar los datos y luego poder tener acceso a ellos se reservó bloques de memoria en el heap para las dos estructuras y para un pokemon auxiliar.

![](https://github.com/facundolescano0/tp1/blob/entrega/diagramas/Diagrama-funcionalidad.png)

el pokemon auxiliar fue usado para ir cargando los datos de las lineas y en caso de ser validos, plasmarlos en la memoria del vector de pokemones(previamente reservado) ubicado en info_pokemon.
Como se ve en la imagen, poke_aux será un recipiente que se le copiará el contenido en el tope de info->pokemon, se lo liberará y se lo volverá a iniciar para cargar el proximo pokemon.

![](https://github.com/facundolescano0/tp1/blob/entrega/diagramas/info-y-poke_aux.png)

para cargar un pokemon, se reservará nueva memoria a el puntero de pokemones que esta dentro de info_pokemon, invocando a la fúncion agrandar poke que contiene un realloc, siempre teniendo en cuenta los posibles casos nulos y la liberación de la memoria posible a perder.

![](https://github.com/facundolescano0/tp1/blob/entrega/diagramas/info.png)



## Respuestas a las preguntas teóricas
Incluír acá las respuestas a las preguntas del enunciado (si aplica).
El metodo de ordenamiento del vector aplicado, llamado por la funcion ordenar_pokemones, es una doble iteracion en la que se itera segun el largo del vector de pokemones y utiliza dos iteradores. El primero, j, va desde 0 hasta n-1. El segundo, i, va desde 0 hasta n-1-j. Es decir, si analizamos las acciones del codigo vemos que por dentro de los bucles hay 3 funciones constantes, luego el iterador i que hará n-1 iteraciones hasta llegar a 0 y reptira el ciclo hasta n-1-1 (j=1). Es decir que hara una sumatoria desde j=1 hasta j=n-1 de (n-i)... esta sumatoria es igual a n*(n-1)/2 y tomando el peor de los casos, siguiendo la regal big O este metodo sera O(n²)

