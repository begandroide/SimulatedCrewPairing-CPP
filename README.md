# Airline Crew Pairing - Modelado y Programación

Para dejar en logs los output con evaluacion de tiempo:
```bash
  make test1 2>&1 | tee logs/Output0.txt
```

## TODO Section
- [x] Generation Initial.
- [x] Selection Function.
- [x] Fitness function. 
 - !Dudas! comenzando (cant. de vuelos tomados, cantidad de vuelos repetidos) ¡Pensar en fitness Function!.
 - Fitness seteada. a medida de evolucion del programa setear estos valores dependiendo de la data.
- [ ] Mutation. Profe recomendó 2 tipos, con probabilidades distintas de aparición; una que acepte cosas mejores que la solucion actual y otro que seleccione sin importar si es mejor o no.
- [ ] Elitism.

**Técnica asignada :** Algoritmos Evolutivos + Greedy

- Greedy: Usado para generar instancias iniciales (i.e una población de individuos iniciales).
     - Problematica: Los individuos iniciales serán iguales.
     - Soluciones:
          - [x] **Usar distintos puntos de partida, si es posible.** -- recomendada por M.C.Riff
          - [ ] Agregar un toque de aleatoriedad en lsa construcciones con el greedy.
               - Ejemplo: En cada movimiento haya una posibilidad de elegir un vuelo al azar en la solución, ignorando la función miope.
          - [ ] Usar Greedy con funciones miopes diferentes.
          - [x] Subproblema: Infactibilidad de las soluciones.
               - Podemos movernos el lo infactible y reparar al final.
               - O solo permitir movimientos en los factible.
               - pd: intentar ambos.
- Algoritmos evolutivos: Algoritmos genéticos de optimización; Basados en heurísticas y en la idea del proceso de selección natural.

## Presentación del Modelo para el problema

Cosas interesantes en:
- ElenaKhmeleva-Evolutionary algorithms for scheduling operations.pdf
- evol.pdf - evol.dvi
- podemos usar este enfoque en greedy: https://www.inf.utfsm.cl/~mcriff/IA-avanzada/NN.html
### Greedy function
- Representación: En **ElenaKhmeleva-Evolutionary algorithms for scheduling operations.pdf** podemos encontrar un analisis de las representaciones para el problema, en donde se muestan las siguientes:
  1. Enfoquede programación lineal entera en el enfoque de generacion por columnas. La ventaja de usar EA es la capacidad de devolver mas de una columna en una misma iteración; ya que trabaja con varias soluciones simultaneamente.
  2. Llegamos a una decisión, vectores en C++; intentemos usar C+-

### Evolutionary Algorithms
 En esta etapa debemos decidir un par de transformaciones y algoritmos de selección: **initialization, selection, genetic operators, termination**.

 **Contexto**
 Deseamos encontrar la mejor combinación de elementos que maximize alguna **Fitnes Function**, y vamos a acpetar una solucion final una vez  mediante algún criterio de término como el numero de iteraciones, o hemos alcanzado algún tipo de aptitud física (en la fitness function).
threshold:
  swipe: 0.5
  pinch: 0.2

interval:
  swipe: 0.2
  pinch: 0.2
 **Initialization**
 Para comenzar, debemos crear una población inicial de soluciones; esto lo realizamos con Greedy. La pobalcion contendrá un número posible de soluciones iniciales máxima, llamados **individuos**. 
 - Los individuos son generados inicialmente de acuerdo a ciertas reglas que podemos configurar o centrarnos en alguna creencia.threshold:
  swipe: 0.5
  pinch: 0.2

interval:
  swipe: 0.2
  pinch: 0.2
 - Es **importante** que la población inicial abarque una gama de soluciones, ya que es el *patrimonio genético*. **Cuidado**; depende de lo que estemos buscando, en el caso de exploración necesitamos un variado patrimonio genético, por otro lado para explotar, ..¿?

**Selection**
Una vez la población fué creada, miembros de la población deben ser evaluados de acuerdo a una **fitness function**; traducida como función de aptitud.
- Una Fitness Function es una funcion que toma las caracteristicas(genes) de un individuo, y da como output una representación numérica de que tan **viable** es la solución.
- Fitness function es muy especifica del problema, hace falta pensar bien ésta.
Luego de obtener los datos cualitativos de los individuos, seleccionamos una porción de los miembos, dados los criterios adecuados al problema.
*Spoiler Progamming*: Pueden existir múltiples funciones objetivos, es decir múltiple fitness function. Ésto complica el proceso, por que en vez de ser capaz de identificar un solo punto óptimo, en cambio con múltiples fitness function obtener un conjunto de puntos óptimos. El conjunto de óptimas soluciones se llama **Pareto Frontier** , y se caracteriza por tener soluciones no dominantes sobre las otras en la frontera.

**Generic Operators**
Luego de seleccionar los individuos, nos quedan dos pasos pendientes: *Crossover y mutation*.
- Objetivo: crear nuevas generaciones, usando las caracteristicas de los padres seleccionados.
- (Crossover) Nuevos hijos son creados con una mezcla de las caracteristicas de las calidades de los padres.
- (Mutation) También se hace necesario introducir nuevo material genético a las generaciones (si no lo hacemos, podemos estancarnos en extremos locales muy rapido, no obteniendo resultados óptimos). Se hace:
     - Cambiamos una pequeña porcion de las generaciones hijas de tal manera que no reflejen perfectamente los genes de los padres.
     - Tipicamente se debe usar probabilidades.
  
  **Terminación**
  El algoritmo debe terminar en algun momento. Hay dos casos usualmente usados:
- Tiempo de ejecución.
- Alcanzar algún umbral de rendimiento.
Mejor solucion la seleccionamos y retornamos.

-------------------------------------------------------------------------------
# Aportes Interesantes y dudas por Correo
- Primera duda sobre problema: ¿Existe pre-procesamiento? Función del Greedy vs EA
  - ***mcRiff***: 
    - Idea de usar Greedy: Generar población inicial. Incluir algunas soluciones obtenidas con el.
    - Lo malo de un Greedy: No sirve que todas las soluciones vengan de un Greedy, pero si puede aportar con un buen material genético, ¿como? tomando instancias iniciales partiendo de ciudades o puntos distintos.
- Segunda: Representación del problema. De las tres que le mandé por correo: 
  1. <span style="color:red;">Representación por String, tipo reresentación basada en columnas. Un string de largo igual a la cantidad de vuelos (dependiendo de la instancia), con dominio binario en cada casilla (para saber si se usa o no). Én esta representación me aprobleman dos cosas: el string para una instancia muy grande, será muy largo; y por otro lado será un string con demasiados ceros y pocos unos; por lo que no lo encuentro muy eficiente.</span>
  2. <span style="color:blue;" > Representación por String, con la diferencia del anterior que no se usa representación binaria, si no que entera. Lo bueno de esta es que no se necesita tener un string tan largo y lleno de 0's que no se usarán; eso si que es necesario que el largo del string sea variable. Lo malo de ésta es que debemos tener algun "marcador" para diferenciar los id's de vuelos; (e.g: "123" -> "1" y "23" o "123" ->"12" y "3",...etc; con  marcador -> "1*23").</span>
  3. <span style="color:green;"> Representación mediante un vector: type_ vectores<Individuo>, en donde Individuo es una clase que tiene distintos atributos (i.e: cromosomas, tamaño, puntuación de fitness; y sus métodos respectivos). Lo bueno de ésta es que tenemos una librería desarrollada en c++ para hacer operaciones (se supone estan optimizadas para el compilador)</span> 
  - ***mcRiff:*** Descarta Nº1 inmediatamente. Tiene preferencia por la 3º; pero antes de completarla saber los operadores:
    - No estamos obligados a usar crossover, es muy complicado definirlo cuando debemos manejar muchas restricciones.
    - Pensar en operador asexual único, con dos versiones:
      - Version1: Tipo Hill-climbing -> solo acepte algo que mejore.
      - Version2: El cambio sin preguntar si quedó mejor o no.
      - Ambos con una probabilidad diferente de ser aplicados.
      - pd(***bgautier***). Lo veo como combinación de HC, TS, HC.
    - Dejó la interrogante:
      - Si piensas en un movimiento tipo swap por ejemplo, cual de las dos representaciones te ayudaría mejor a hacerlo?
        - Hacer swap en elementos de vectores toma un tiempo computacional muy pequeño. vamos por esa.
- ***Sobre horas máximas por día***: La restricción de horas de vuelo es el tiempo máximo de vuelo para un día, es una restricción legal que asegura que no se abuse de la tripulación, es para darle un poco más de realismo al problema.; entonces, puede durar 12 horas un pairing o duty pero las horas trabajadas arriba del avión deben ser 8?
 - Duda a ***Rodolfo***: <span style"color:blue">en cuanto a uso de recursos y eficiencia de calculo, ¿vale la pena implementar vectores en c?, ¿mejorará el rendimiento/manejo de memoria?, o ¿vectores en c++ ya está bastante optimizado?</span>
  - stdlib de c++ está optimizada.
  - Respuesta es siempre: **depende del caso de uso**
    - Caso de uso: problema combinatorio, la idea es usar vectores de estructuras o clases. Pregunta dirigida a manejar instancia muy grande.
    -  ***Consejos***: 
      -  Implementa y si funciona no lo arregles.
      -  Claramente es mas facil de hacer con C++.
      -  <span style="color:red;">**Un paso adelante**: Usar C+-, es decir, solo usar vectores o lo que sea esencial en c++; el resto programarlo en C. Si funciona estas listo, si no debes hacer pocos cambios.</span>
      -  **OJO**: Para hacer una comparación objetiva, compilar con optimizaciones (-O3).
      -  Usar compilador g++
    - **Propuesta interesante:** Combinar lenguajes; C y C++; investigar: extern "C" {...}. 

# Vectores en c++
```C
swap(vector[position],vector[otherPosition]);
```
- Array vs vectors: https://hackernoon.com/c-investigation-arrays-vs-vectors-e9ba864468b6
- uso de ~ : *"Destructors are usually used to deallocate memory and do other cleanup for a class object and its class members when the object is destroyed. A destructor is called for a class object when that object passes out of scope or is explicitly deleted."*
- 
