# Airline Crew Pairing - Modelado y Programación

## Make & Run

1. Clonar repositorio
2. Entrar a la carpeta creada
3. Ejecutar **make**
4. Ejecutar:
```bash
./out/fly ./instances/instancia.csv
```

## TODO Section
- [x] Generation Initial.
- [x] Selection Function. Roulette Wheel selection.
- [x] Fitness function. Price and constraint violations.
- [X] Mutation. 1.Acepta cosas mejores que la solucion actual. 2.Selecciona sin importar si es mejor o no.
- [X] Elitism.

**Técnica Empleada :** Algoritmos Evolutivos + Greedy

- Greedy: Usado para generar instancias iniciales (i.e una población de individuos iniciales).
     - Problematica: Los individuos iniciales serán iguales.
     - Soluciones:
          - [x] **Usar distintos puntos de partida, si es posible.** 
          - [X] Agregar un toque de aleatoriedad en lsa construcciones con el greedy.
               - Usamos números aleatorios para elegir el vuelo del que parte un duty.
          - [x] Subproblema: Infactibilidad de las soluciones.
               - Podemos movernos el lo infactible y reparar al final.
               - O solo permitir movimientos en los factible.
               - pd: Ambos implementados.
- Algoritmos evolutivos: Algoritmos genéticos de optimización; Basados en heurísticas y en la idea del proceso de selección natural.

## Presentación del Modelo para el problema

### Greedy function
- Representación: vectores en C++; Vectores de vectores; un vector se componer de pairings, un pairing (vector) se compone de dutyes(vector).

### Evolutionary Algorithms
 En esta etapa debemos decidir un par de transformaciones y algoritmos de selección: **initialization, selection, genetic operators, termination**.

## Useful

Para dejar en logs los output con evaluacion de tiempo:
```bash
  make test1 2>&1 | tee logs/Output0.txt
```


