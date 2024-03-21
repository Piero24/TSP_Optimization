

## Menu parser (Pietrobon)
Build a menu for selecting the different operations and algo to apply. Also inclue the possibility to choose if auto generate file or note


Schermate:

1:
    Choose input:
    1 - Insert input file name
    2 - Generate random input

    9 - Show help
    0 - Exit

2:
    Choose algorithm:
    1 - Random
    2 - NN

    9 - Show help
    0 - Exit

3:
    Choose optimization:
    1 - None
    2 - 2opt
    3 - Tabù (+ 2opt)

    9 - Show help
    0 - Exit

4:
    Options add one or more of the following opt:
    - Verbose from 0 to 100 (es: -v 50)
    - Time limit (es: -t 10000)
    - Random seed (es: -r 34567)
    - Show with gnuplot (es: "-s 0" to show without slowing down, "-s 1" to show with slowing down)
    
    Warning: you have to put all the options one after the other, the algorithm will start after you press ENTER
    

Input: -v 1000 -t 10000 -s 1234



## Fix tabù + fix read input (Felline)


































## Separated function to call NN on every possible firstNode

## Add the time limit as input parameter and create the function that stop when the time limit is reached

## Create the parser for the optimization parameter

TODO:
 - Only (2opt) optimization 
 - (2opt) optimization + Tabu search

 Implement this on the algoSelector ???

## Make the txt file with the explanation of the homework asked in class && Start preparing the Latex file with the solution proposed

## Add the fact that if the code use an opt method it must save the image and print the solution with and without the optimization method so it's possible to check both the results || Add multiple solutions on the instance for the final print and images. 

## Check free for each alloc







## Fix includes

## Comment the code

## Add the possibility that the algorithm generate a input file itself

## Profiler
Used to check the time that the algorithm takes to complete a certain task.

## Menù