
# Available Parameters:

Qusta è una lista dei parametri che si possono utilizzare per eseguire il programma.
Per una più completa visione sul loro funzionamento compreso lo pseudocodice e le varie performance che si possono ottenere si può consultare il [» paper »](https://github.com/Piero24/TSP_Optimization/blob/main/Thesis.pdf) .

## Input File
#### Commands: `-file`, `-input`, `-f`
Se è gia disponibile un file `.tsp` contenete i punti per i quali si vogliono usare gli algoritmi e possibile utilizzarlo specificando la il percorso del file con il parametro.

Un esempio del suo utilizzo è il seguente:
```sh
./TSP_Optimization -file Resource/pr10v1.tsp -model 2
```


## Time Limit
#### Commands: `-time_limit`, `-tl`, `-t`
Alcuni algoritmi come Tabu Search continuano a cercare la soluzione migliore all'infinito. Per questo motivo è presente un limite di tempo per l'esecuzione dell'algoritmo. Questo parametro permette di specificare il tempo massimo di esecuzione in secondi.

Un esempio del suo utilizzo è il seguente:
```sh
./TSP_Optimization -file Resource/pr240.tsp -model 2 -opt 2 -tl 120
```


## Algorithm
#### Commands: `-model_type`, `-model`, `-alg`
Questo parametro permette di specificare l'algoritmo che si vuole utilizzare per risolvere il problema. I valori possibili sono:

<table>
    <tr  align="center">
        <th>Parameter</th> 
        <th>Algorithm</th> 
    </tr>
    <tr  align="center">
        <th><strong>1</strong></th>
        <th>Random</th>
    </tr>
    <tr  align="center">
        <th><strong>2</strong></th>
        <th>Nearest Neighbour</th>
    </tr>
    <tr  align="center">
        <th><strong>3</strong></th>
        <th>CPLEX</th>
    </tr>
    <tr  align="center">
        <th><strong>4</strong></th>
        <th>Benders' Loop</th>
    </tr>
    <tr  align="center">
        <th><strong>5</strong></th>
        <th>Glued Benders' Loop</th>
    </tr>
    <tr  align="center">
        <th><strong>6</strong></th>
        <th>Diving</th>
    </tr>
    <tr  align="center">
        <th><strong>7</strong></th>
        <th>Local Branching</th>
    </tr>
</table>

Un esempio del suo utilizzo è il seguente:
```sh
./TSP_Optimization -file Resource/pr50.tsp -model 6 -tl 300 -r 200 -s 1
```


## Optimizer
#### Commands: `-opt`, `-2opt`, `-optimization`
Questo parametro permette di specificare l'ottimizzatore che si vuole utilizzare per migliorare la soluzione trovata dall'algoritmo. Per esempio si puù applicare 2-opt per migliorare le prestazioni ottenute da Neirest Neighbor. I valori possibili sono:

<table>
    <tr  align="center">
        <th>Parameter</th> 
        <th>Optimizator</th> 
    </tr>
    <tr  align="center">
        <th> - </th> 
        <th>No Optimizaton</th> 
    </tr>
    <tr  align="center">
        <th><strong>1</strong></th>
        <th>2-Opt</th>
    </tr>
    <tr  align="center">
        <th><strong>2</strong></th>
        <th>Tabu Search</th>
    </tr>
    <tr  align="center">
        <th><strong>3</strong></th>
        <th>Variable Neighbourhood Search (VNS)</th>
    </tr>
</table>

Un esempio del suo utilizzo è il seguente:
```sh
./TSP_Optimization -g 100 -model 2 -opt 2 -tl 300 -r 52
```


## Seed
#### Commands: `-seed`, `-r`
Il seed permette di ricreare i file generati in modo casuale in modo che siano sempre i medesimi. Così da comparare i risultati ottenuti dai differenti algoritmi sugli stessi dati.
Di default è impostato il seed `93846529`.

Un esempio del suo utilizzo è il seguente:
```sh
./TSP_Optimization -g 100 -model 1 -r 52
```


## Verbose
#### Commands: `-verbose`, `-v`
Il verbose permette di visualizzare i dettagli dell'esecuzione dell'algoritmo. Di default è impostato a `0` e non mostra nessun dettaglio. Incrementando il valore del seed di 10 alla volta da `0` a `100` si possono visualizzare i dettagli dell'esecuzione. Più il valore del verbose è alto più dettagli verranno mostrati.

Un esempio del suo utilizzo è il seguente:
```sh
./TSP_Optimization -g 100 -model 2 -v 60
```


## File Generator
#### Commands: `-generate`, `-g`
Questo parametro permette di generare un file `.tsp` con un numero di punti specificato.

Un esempio del suo utilizzo è il seguente:
```sh
./TSP_Optimization -g 345 -model 3
```


## CPLEX Optimization
#### Commands: `-cplexOpt`, `-c`
Questo parametro permette di specificare il tipo di ottimizzazione che si vuole fare con CPLEX. I valori possibili sono:

<table>
    <tr  align="center">
        <th>Parameter</th> 
        <th>Optimizator</th> 
    </tr>
    <tr  align="center">
        <th><strong>1</strong></th>
        <th>CPLEX</th>
    </tr>
    <tr  align="center">
        <th><strong>2</strong></th>
        <th>CPLEX with 2-Opt</th>
    </tr>
    <tr  align="center">
        <th><strong>3</strong></th>
        <th>CPLEX with Tabu Search</th>
    </tr>
    <tr  align="center">
        <th><strong>4</strong></th>
        <th>CPLEX with VNS</th>
    </tr>
</table>

Un esempio del suo utilizzo è il seguente:
```sh
./TSP_Optimization -file Resource/pr222.tsp -model 3 -c 1
```


## Show Plot
#### Commands: `-show`, `-plot`, `-gnuplot`, `-s`
Questo parametro permette di visualizzare in tempo reale l'algoritmo nella ricerca della soluzione migliore. Di default è nullo e non verrà mostrato nessun plot (verranno solamente salvati nell'archivio in formato `.png` e `.svg`). Impostando il valore a `0` sarà possibile avere l'output finale con la soluzione migliore trovata. Aumentando il valore si potrà vedere l'evoluzione dell'algoritmo. **ATTENZIONE:** Ogni unità corrisponde a un secondo di tempo di esecuzione. Quindi impostando il valore ad 10 per esempio si vedrà una modifica ogni 10 secondi. Si consiglia pertanto di utilizzare solo valori molto bassi come 3, 2 o 1 e di utilizzarlo per file molto piccoli.
```sh
./TSP_Optimization -file Resource/pr45.tsp -model 2 -opt 2 -s 2
```


## Plot Cost
#### Commands: `-showCost`, `-plotCost`, `-sC`

```sh
./TSP_Optimization -file Resource/pr45.tsp -model 2 -opt 2 -s 2
```

- **-help** or **--help** or **-h**: Help


## Help
#### Commands: `-help`, `--help`, `-h` (Not completed yet)
Per visualizzare l'help con tutti i parametri disponibili è possibile utilizzare il seguente comando:

```sh
./TSP_Optimization -help
```