
# Available Parameters:

This is a list of parameters that can be used to run the program.
For a more complete view of their operation, including the pseudocode and the various performances that can be obtained, you can consult the [» paper »](https://github.com/Piero24/TSP_Optimization/blob/main/Thesis.pdf) .

## Input File
#### Commands: `-file`, `-input`, `-f`
If a `.tsp` file is already available containing the points for which you want to use the algorithms and you can use it by specifying the file path with the parameter.

An example of its use is the following:
```sh
./TSP_Optimization -file Resource/pr10v1.tsp -model 2
```


## Time Limit
#### Commands: `-time_limit`, `-tl`, `-t`
Some algorithms like Tabu Search keep searching for the best solution endlessly. For this reason there is a time limit for the execution of the algorithm. This parameter allows you to specify the maximum execution time in seconds.

An example of its use is the following:
```sh
./TSP_Optimization -file Resource/pr240.tsp -model 2 -opt 2 -tl 120
```


## Algorithm
#### Commands: `-model_type`, `-model`, `-alg`
This parameter allows you to specify the algorithm you want to use to solve the problem. Possible values ​​are:

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

An example of its use is the following:
```sh
./TSP_Optimization -file Resource/pr50.tsp -model 6 -tl 300 -r 200 -s 1
```


## Optimizer
#### Commands: `-opt`, `-2opt`, `-optimization`
This parameter allows you to specify the optimizer you want to use to improve the solution found by the algorithm. For example, you can apply 2-opt to improve the performance obtained from Neirest Neighbor. Possible values ​​are:

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

An example of its use is the following:
```sh
./TSP_Optimization -g 100 -model 2 -opt 2 -tl 300 -r 52
```


## Seed
#### Commands: `-seed`, `-r`
The seed allows you to recreate the randomly generated files so that they are always the same. So as to compare the results obtained by different algorithms on the same data.
By default the seed is set to `93846529`.

An example of its use is the following:
```sh
./TSP_Optimization -g 100 -model 1 -r 52
```


## Verbose
#### Commands: `-verbose`, `-v`
The verbose allows you to view the details of the algorithm's execution. By default it is set to `0` and does not show any details. Increasing the seed value 10 at a time from `0` to `100` allows you to view the execution details. The higher the verbose value, the more details will be shown.

An example of its use is the following:
```sh
./TSP_Optimization -g 100 -model 2 -v 60
```


## File Generator
#### Commands: `-generate`, `-g`
This parameter allows you to generate a `.tsp` file with a specified number of points.

An example of its use is the following:
```sh
./TSP_Optimization -g 345 -model 3
```


## CPLEX Optimization
#### Commands: `-cplexOpt`, `-c`
This parameter allows you to specify the type of optimization you want to do with CPLEX. Possible values ​​are:

<table>
    <tr  align="center">
        <th>Parameter</th> 
        <th>Optimizator</th> 
    </tr>
    <tr  align="center">
        <th><strong>1</strong></th>
        <th>Mipstart</th>
    </tr>
    <tr  align="center">
        <th><strong>2</strong></th>
        <th>Callback Base</th>
    </tr>
    <tr  align="center">
        <th><strong>3</strong></th>
        <th>Callback Relax</th>
    </tr>
    <tr  align="center">
        <th><strong>4</strong></th>
        <th>Posting Base</th>
    </tr>
    <tr  align="center">
        <th><strong>5</strong></th>
        <th>Posting Relax</th>
    </tr>
</table>

An example of its use is the following:
```sh
./TSP_Optimization -file Resource/pr222.tsp -model 3 -c 1
```

**NOTE:** These parameters can also be used simultaneously. Here are some examples:
```sh
./TSP_Optimization -file Resource/pr567.tsp -model 3 -c 124
./TSP_Optimization -g 379 -model 3 -c 25 -opt 2
```


## Show Plot
#### Commands: `-show`, `-plot`, `-gnuplot`, `-s`
This parameter allows you to view the algorithm in real time in the search for the best solution. By default it is null and no plots will be shown (they will only be saved in the archive in `.png` and `.svg` format). By setting the value to `0` it will be possible to have the final output with the best solution found. By increasing the value you will be able to see the evolution of the algorithm. **WARNING:** Each unit corresponds to one second of execution time. So by setting the value to 10 for example you will see a change every 10 seconds. It is therefore recommended to only use very low values ​​such as 3, 2 or 1 and to use it for very small files.

An example of its use is the following:
```sh
./TSP_Optimization -file Resource/pr45.tsp -model 2 -opt 2 -s 2
```


## Plot Cost
#### Commands: `-showCost`, `-plotCost`, `-sC`

An example of its use is the following:
```sh
./TSP_Optimization -file Resource/pr45.tsp -model 2 -opt 2 -s 2
```

- **-help** or **--help** or **-h**: Help


## Help
#### Commands: `-help`, `--help`, `-h` (Not completed yet)
To view the help with all the available parameters you can use the following command:

An example of its use is the following:
```sh
./TSP_Optimization -help
```