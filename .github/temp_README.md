<!--**RELEASED-DATA**-->

<div id="top"></div>

<!-- Working ✅ Work in progress ⚠️ ~~Deprecate~~ ⛔️ -->
**Project Status:** Work in progress ⚠️ <a href="#maintenance">⇩</a>
<br/>
**Percentage Completely:** 40% 🔋 <a href="#roadmap">⇩</a>
<br/>
<br/>
<br/>
<br/>


<p align="center">
  <img src="https://github.com/Piero24/TSP_Optimization/blob/main/.github/path.png?raw=true" width="150" height="150">
</p>
<h1 align="center">
    <a href="https://github.com/Piero24/TSP_Optimization">Traveler Salesman Problem Optimization</a>
</h1>
<p align="center">
    <!-- BADGE -->
    <!--
        *** You can make other badges here
        *** [shields.io](https://shields.io/)
        *** or here
        *** [CircleCI](https://circleci.com/)
    -->
    <a href="https://github.com/Piero24/TSP_Optimization/commits/master">
    <img src="https://img.shields.io/github/last-commit/piero24/TSP_Optimization">
    </a>
    <a href="https://github.com/Piero24/TSP_Optimization">
    <img src="https://img.shields.io/badge/Maintained-yes-green.svg">
    </a>
    <!--<a href="https://github.com/Piero24/TSP_Optimization">
    <img src="https://img.shields.io/badge/Maintained%3F-no-red.svg">
    </a> -->
    <a href="https://github.com/Piero24/twitch-stream-viewer/issues">
    <img src="https://img.shields.io/github/issues/piero24/TSP_Optimization">
    </a>
    <a href="https://github.com/Piero24/TSP_Optimization/blob/master/LICENSE">
    <img src="https://img.shields.io/github/license/piero24/TSP_Optimization">
    </a>
</p>
<p align="center">
    A list of Heuristics, Metaheuristics and Matheuristic algorithms for solve the TSP.
    <br/>
    <a href="#index"><strong>Read the paper »</strong></a>
    <br/>
    <br/>
    <a href="https://github.com/Piero24/TSP_Optimization">View Demo</a>
    •
    <a href="https://github.com/Piero24/TSP_Optimization/issues">Report Bug</a>
    •
    <a href="https://github.com/Piero24/TSP_Optimization/issues">Request Feature</a>
</p>


---


<br/><br/>
<h2 id="itroduction">📔  Itroduction</h2>
<p> This project encompasses a comprehensive suite of algorithms designed to tackle the classic TSP, providing solutions via various heuristics, matheuristics, and exact optimization models. Leveraging the power of IBM’s CPLEX solver, our exact models deliver high-quality solutions with mathematical rigor and efficiency. </p>
<h4>Overview</h4>
<p>The TSP Solver offers a versatile and powerful framework to approach the Traveling Salesman Problem, which seeks the shortest possible route that visits each city once and returns to the origin city. Our solver is equipped with:</p>
    <ul>
        <li><strong>Heuristics:</strong> Fast, efficient algorithms that provide good solutions within a reasonable time frame. These include methods such as Nearest Neighbor and 2-Opt.</li>
        <li><strong>Matheuristics:</strong> Advanced hybrid techniques that combine heuristics with mathematical programming to enhance solution quality.</li>
        <li><strong>Exact Models:</strong> Robust formulations that guarantee optimal solutions using the CPLEX solver from IBM, ensuring precision and thoroughness in solving TSP instances.</li>
    </ul>
<br/>
<table  align="center">
  <tr  align="center">
    <th><strong>Neirest Neighbor + 2-Opt</strong></th>
    <th><strong>Neirest Neighbor + VNS</strong></th>
  </tr>
  <tr  align="center">
    <th><img src="Localhost.png" alt="Web Page"></th>
    <th><img src="example_img.jpeg" alt="Example Image"></th> 
  </tr>
  <tr  align="center">
    <th><strong>Benders' Loop</strong></th>
    <th><strong>Local Branching</strong></th> 
  </tr>
  <tr  align="center">
    <th><img src="Localhost.png" alt="Web Page"></th>
    <th><img src="example_img.jpeg" alt="Example Image"></th> 
  </tr>
</table>
<br/>
<br/>

> [!IMPORTANT]  
> 1. To run this program you need to have a valid CPLEX license. You can download the CPLEX solver from the IBM website.
> 2. 


<h2 id="made-in"><br/>🛠  Built in</h2>
<p>
    The entire program is developed using the following major frameworks and libraries. Leave any add-ons / plug-ins for the thanks section.
</p>
<br/>
<a href="https://www.cprogramming.com/">C Programming Language</a> • <a href="https://cmake.org/">CMake</a> • <a href="https://www.ibm.com/products/ilog-cplex-optimization-studio">IBM CPLEX Optimization Studio</a>

<p align="right"><a href="#top">⇧</a></p>

<h2 id="documentation"><br/><br/>📚  Documentation</h2>
<ul>
  <li><strong>Random</strong>: 
  </li>
  <li><strong>Heuristics</strong>
    <ul>
      <li><strong>Nearest Neighbor (NN)</strong>: This algorithm starts from an initial node and selects the nearest node to move to next. The process is repeated until all nodes have been visited. It is a simple and effective method to quickly obtain an acceptable solution for problems like the Traveling Salesman Problem (TSP).</li>
      <li><strong>2-Opt</strong>: The 2-Opt algorithm is used to iteratively improve a solution by exchanging pairs of edges. The idea is to reduce the overall path length by finding and removing "crosses" between the edges, replacing them with non-intersecting edges.</li>
    </ul>
  </li>
  <li><strong>Metaheuristics</strong>
    <ul>
      <li><strong>Variable Neighborhood Search (VNS)</strong>: This metaheuristic explores different neighborhood structures to avoid getting stuck in local minima. By changing the neighborhood structure, the algorithm can explore a larger part of the solution space, increasing the chances of finding the optimal solution.</li>
      <li><strong>Tabu Search (TS)</strong>: Tabu Search employs a tabu list to keep track of already visited solutions, preventing the algorithm from revisiting them. This mechanism helps avoid cycles and overcome local minima, promoting a more thorough search of the solution space.</li>
    </ul>
  </li>
  <li><strong>Exact Methods</strong>
    <ul>
      <li><strong>Benders' Loop</strong>: This algorithm solves complex problems by dividing them into a main problem and various subproblems. The iterative process alternates between solving the main problem and the subproblems, progressively refining the overall solution.</li>
      <li><strong>Glued Benders' Loop</strong>: Similar to Benders' Loop, this method addresses the main problem and subproblems iteratively but with the integration of techniques that improve convergence and process efficiency.</li>
    </ul>
  </li>
  <li><strong>Matheuristics</strong>
    <ul>
      <li><strong>Local Branching</strong>: This approach introduces local constraints to rapidly explore portions of the solution space, improving the effectiveness in searching for optimal solutions.</li>
      <li><strong>Hard Fixing (Diving)</strong>: This technique temporarily fixes some variables to reduce the problem size, allowing a deeper search in the remaining variables. It is particularly useful for tackling large-scale problems, improving the speed of convergence towards an optimal solution.</li>
    </ul>
  </li>
</ul>
<p>
    For a more complete explanation of how the algorithms work, please refer to the <a href="https://shields.io/">Paper »</a>
</p>
<p>
    There are different parameters and different ways to start the program. More details are available at the following link: <a href="https://shields.io/">Documentation »</a>
</p>

<p align="right"><a href="#top">⇧</a></p>


<h2 id="prerequisites"><br/>🧰  Prerequisites</h2>
<p>
    To use the actual algorithms, a CPLEX license is required. If you have a license, you can download the software from IBM's official website. An installation guide for MacOS is available at the following link: <a href="https://shields.io/">Installazione CPLEX su MacOS »</a>. Currently, there is no installation guide available for Windows or Linux.
</p>

NOTE: You also need to install `cmake` to compile the code.


<p align="right"><a href="#top">⇧</a></p>


<h2 id="how-to-start"><br/>⚙️  How to Start</h2>
<p>
   As mentioned earlier, there are different options:
    <ul>
    <li><strong>Single Command</strong>: Launch a single execution with command line parameters</li>
    <li><strong>Menu</strong>: Launch a single execution with parameters chosen from a menu</li>
    <li><strong>Launcher</strong>: Launch one or more executions with parameters present in a text file</li>
    </ul>
</p>
<h4><strong>Single Command</strong></h4>
<p>
    Once you have chosen the parameters and positioned yourself on the folder containing the code, you can launch the program with the following commands:
</p>

MacOS
```sh
    mkdir build && cmake -S . -DCPLEXDIR="/Applications/CPLEX_Studio2211/cplex" -B build
    make -C build && ./TSP_Optimization -g 100 -model 2 -opt 1
```

Windows
```sh
    cmake . -B build --fresh && cmake --build build --clean-first
    .\TSP_Optimization -f Resource/a280.tsp -model 2 -v 60
```

Linux
```sh
    mkdir build && cmake -S . -DCPLEXDIR="C:/Program Files/IBM/ILOG/CPLEX_Studio_Community2211/cplex/" -B build
    make -C build && ./TSP_Optimization -g 300 -model 3 -opt 12 -s 1 -v 60 -tl 120
```

<h4><strong>Menu</strong></h4>

If you want to use the menu after compiling the code as previously with `cmake`, you can launch the program with the following command:

MacOS/Linux
```sh
    make -C build && ./TSP_Optimization
```

Windows
```sh
    .\TSP_Optimization -g 300 -model 2 -v 10 -tl 60
```

<h4><strong>Launcher</strong></h4>

If you want to use the Launcher after compiling the code as previously with `cmake`, you can launch the program with the following command:

MacOS/Linux
```sh
    make -C build && clear && ./TSP_Optimization -launcher Resource/Launcher/launcher.txt
```

Windows
```sh
    .\TSP_Optimization -launcher Resource/Launcher/launcher.txt
```

We remind you that a list of available parameters is present at the following link: <a href="https://shields.io/">Parametri disponibili »</a>

Una volta lanciato il programma eseguirà l'algoritmo scelto monstrando i risultati a schermo con GNUPlot (Se selezionato come parametro). Inoltre, verranno salvati i file generati tramite il comando `-g` con le coordinate dei punti. In output verrà salvata anche una immagine `.svg` e `.png`con il percorso ottimo trovato.

<strong>Nota:</strong> Se si utilizza il loucher in output verrà generato un file `.csv` con i risultati ottenuti.così da poter utilizzare il `performance profiler`.















<p align="right"><a href="#top">⇧</a></p>


---

<h3 id="responsible-disclosure"><br/>📮  Responsible Disclosure</h3>
<p>
    We assume no responsibility for an improper use of this code and everything related to it. We do not assume any responsibility for damage caused to people and / or objects in the use of the code.
</p>
<strong>
    By using this code even in a small part, the developers are declined from any responsibility.
</strong>
<br/>
<br/>
<p>
    It is possible to have more information by viewing the following links: 
    <a href="#code-of-conduct"><strong>Code of conduct</strong></a>
     • 
    <a href="#license"><strong>License</strong></a>
</p>

<p align="right"><a href="#top">⇧</a></p>


<h3 id="report-a-bug"><br/>🐛  Bug and Feature</h3>
<p>
    To <strong>report a bug</strong> or to request the implementation of <strong>new features</strong>, it is strongly recommended to use the <a href="https://github.com/Piero24/TSP_Optimization/issues"><strong>ISSUES tool from Github »</strong></a>
</p>
<br/>
<p>
    Here you may already find the answer to the problem you have encountered, in case it has already happened to other people. Otherwise you can report the bugs found.
</p>
<br/>
<strong>
    ATTENTION: To speed up the resolution of problems, it is recommended to answer all the questions present in the request phase in an exhaustive manner.
</strong>
<br/>
<br/>
<p>
    (Even in the phase of requests for the implementation of new functions, we ask you to better specify the reasons for the request and what final result you want to obtain).
</p>
<br/>

<p align="right"><a href="#top">⇧</a></p>
  
 --- 

<h2 id="license"><br/>🔍  License</h2>
<strong>GNU GENERAL PUBLIC LICENSE</strong>
<br/>
<i>Version 3, 29 June 2007</i>
<br/>
<br/>
<i>Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/> Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.</i>
<br/>
<br/>
<i>Preamble</i>
<br/>
<i>The GNU General Public License is a free, copyleft license for software and other kinds of works.</i>
<br/>
<a href="https://github.com/Piero24/Template-README/blob/main/LICENSE"><strong>License Documentation »</strong></a>
<br/>
<br/>


<h3 id="authors-and-copyright"><br/>✏️  Authors and Copyright</h3>
<br/>
<p>
    <table  align="center">
    <tr  align="center">
        <th><strong>👨🏽‍💻</strong></th>
        <th><strong>Andrea Pietrobon</strong></th>
        <th><strong>Andrea Felline</strong></th>
    </tr>
    <tr  align="center">
        <th>🌐</th>
        <th><a href="https://www.pietrobonandrea.com">pietrobonandrea.com</a></th>
        <th><a href="https://www.pietrobonandrea.com">pietrobonandrea.com</a></th> 
    </tr>
    <tr  align="center">
        <th><img src="https://assets.stickpng.com/thumbs/580b57fcd9996e24bc43c53e.png" width="30" height="30" align="center"></th>
        <th><a href="https://twitter.com/pietrobonandrea">@PietrobonAndrea</a></th> 
        <th><a href="https://twitter.com/pietrobonandrea">@PietrobonAndrea</a></th> 
    </tr>
    <tr  align="center">
        <th>🗄</th>
        <th align="center"><a href="https://github.com/Piero24/TSP_Optimization">Traveler Salesman Problem Optimization</a></th>
        <th></th> 
    </tr>
    </table>

</p>
<p align="right"><a href="#top">⇧</a></p>


<h3 id="third-party-licenses"><br/>📌  Third Party Licenses</h3>
<br/>
<table  align="center">
  <tr  align="center">
    <th>Software</th>
    <th>License owner</th> 
    <th>License type</th> 
    <th>Link</th>
    <th>Note</th>
  </tr>
  <tr  align="center">
    <td>IBM CPLEX Optimization Studio</td>
    <td>IBM</td> 
    <td> - </td>
    <td><a href="https://www.ibm.com/products/ilog-cplex-optimization-studio">Link</a></td>
    <td></td>
  </tr>
  <tr  align="center">
  <td>Perfprof</td>
    <td>Proessor D. Salvagnin</td> 
    <td>Not Specified</td>
    <td><a href="https://www.dei.unipd.it/~fisch/ricop/OR2/PerfProf/read_me.txt" >link</a></td>
    <td></td>
  </tr>
</table>

<p align="right"><a href="#top">⇧</a></p>


---
> *<p align="center"> Copyrright (C) by Pietrobon Andrea <br/> Released date: **RELEASED-DATA***