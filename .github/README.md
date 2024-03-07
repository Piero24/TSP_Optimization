# TSP_Optimization

### Command to build with Cmake:
(Use these command from the main folder to build the project with Cmake.)

To build the project:
```bash
rm -r build && mkdir build && cmake -S . -B build
```
To run the project:
```bash
make -C build && clear && ./TSP_Optimization -file Resource/pr10.tsp -model 1
```