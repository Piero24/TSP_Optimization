# TSP_Optimization

### Command to build with Cmake:
(Use these command from the main folder to build the project with Cmake.)


#### MacOS

To build the project:

```bash
rm -r build && mkdir build && cmake -S . -B build
```

To run the project:

(Single Launch)
```bash
make -C build && clear && ./TSP_Optimization -file Resource/pr10v1.tsp -model 1 -opt 1 -v 50
```

(Multiple Launch)
```bash
make -C build && clear && ./TSP_Optimization -launcher Resource/Launcher/launcher.txt
```


#### Windows

To build the project:

```bash
cmake . -B build --fresh && cmake --build build --clean-first
```

To run the project:

```bash
Debug\TSP_Optimization.exe -g 100 -model 1 -opt 3 -s 0 -v 60 -tl 10
for /L %i in (1,1,10) do @echo. & if %i==10 (Debug\TSP_Optimization.exe -g 100 -model 3 -v 95)
Debug\TSP_Optimization.exe -g 100 -model 3 -v 60
```

#### Linux

To build the project:

```bash
rm -r build && mkdir build && cmake -S . -DCPLEXDIR="C:/Program Files/IBM/ILOG/CPLEX_Studio_Community2211/cplex/" -B build
```

To run the project:

```bash
make -C build && clear && ./TSP_Optimization -file Resource/pr10v1.tsp -model 1 -opt 1 -v 50
```


### Available Algorithms:

- **Random**
- **Nearest Neighbor**
- **CPLEX**
- **Benders' Loop**
- **Glued Benders' Loop**

### Available Optimization Methods:

- **2-Opt**
- **Tabu Search**
- **Variable Neighborhood Search**

### Available Parameters:

- **-file** or **-input** or **-f**: Input file
- **-time_limit** or **-tl** or **-t**: Total time limit
- **-model_type** or **-model** or **-alg**: Model type
- **-opt** or **-2opt** or **-optimization**: Optimization type
- **-seed** or **-r**: Random seed
- **-verbose** or **-v**: Verbose
- **-generate** or **-g**: Generated file
- **-cplexOpt** or **-c**: CPLEX options
- **-show** or **-plot** or **-gnuplot** or **-s**: Plot with gnuplot
- **-help** or **--help** or **-h**: Help
