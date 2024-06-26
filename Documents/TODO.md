## Algorithms

## Features
- add checks (only if inst->debug == true)
    - done on NN
    - done on addMipstart
- add input flag for cost-itaration function plot + add cplex plot
- Add the fact that if the code use an opt method it must save the image and print the solution with and without the optimization method so it's possible to check both the results || Add multiple solutions on the instance for the final print and images.
- clean code


## Documentation
- Make the txt file with the explanation of the homework asked in class && Start preparing the Latex file with the solution proposed
- Comment the code

---------------------------------------------------

## Tools (to be used)
- Profiler: Used to check the time that the algorithm takes to complete a certain task.


## Official input - solutions
- Best solution of file "kroA100.tsp" has cost 21282 (100 nodes)
- Best solution of file "a280.tsp" has cost 2579 (280 nodes)
- Best solution of file "rat575.tsp" has cost 6773 (575 nodes)
- Best solution of file "pr1002.tsp" has cost 259045 (1002 nodes)


## Notes
- izero in cplex functions tells where the sections of the index and value arrays start for each constraint/cut
- CCcut_violated_cuts(inst->nnodes, ncols, elist, xstar, 1.9, add_cut, (void*) &user_handle);
    - 1.9 means that the cut added was violated by 0.1 (2 would means not violated)
- Orario tipico esame: 16-16.30
- ricordare di mettere foto nella tesina