## Neighbor algorithm for the Traveling Salesman Problem (TSP).

The nearest neighbor algorithm is a simple heuristic approach used to find a relatively 
good solution to the TSP. 

Despite its simplicity, the nearest neighbor algorithm is relatively efficient and easy to implement, making it a popular choice for quickly finding solutions, especially in situations where an exact optimal solution is not required or feasible within a reasonable amount of time.

It begins at any vertex and follows the edge of least weight from that vertex. At every subsequent vertex, it follows 
the edge of least weight that leads to a city not yet visited, until it returns to the 
starting point. The time complexity of the NN algorithm is O(n2). Although the nearest 
neighbor algorithm doesn't uarantee an optimal solution, it often provides a reasonably 
good solution, especially for small to moderate-sized instances of the TSP. However, 
it's important to note that the solution produced by this algorithm may not always be 
the shortest possible route. Some statistics show that the NNH produces tours that are 
on average 15% longer than the optimal tour.

Let \( G = (V, E) \) be a complete undirected graph, 
where \( V \) is the set of vertices (nodes) and \( E \) is the set of edges 
connecting these vertices. Each edge \( e_{ij} \) is associated with a 
non-negative weight \( w_{ij} \), representing the distance between 
node \( i \) and node \( j \).

The goal of the TSP is to find a Hamiltonian cycle, which is a cycle 
that visits each vertex exactly once and returns to the starting vertex, 
with the minimum total weight. The nearest neighbor algorithm provides 
an approximate solution to this problem.

Here's a step-by-step description of the algorithm:

1. **Initialization**: Choose an arbitrary starting vertex \( v_0 \) as the starting city.

2. **Greedy Selection**: At each step, select the nearest unvisited neighbor of the 
current city and add it to the tour. This can be mathematically expressed as follows:

   \[ v_{i+1} = \text{argmin}_{v_j \in V \setminus \{v_0, v_1, \ldots, v_i\}} \{w_{iv_j}\} \]

   Here, \( v_{i+1} \) is the next city to be visited, \( V \setminus \{v_0, v_1, 
   \ldots, v_i\} \) denotes the set of unvisited nodes, and \( w_{iv_j} \) represents 
   the distance between the current city \( v_i \) and an unvisited city \( v_j \).

3. **Termination**: Repeat step 2 until all vertices have been visited. Once all vertices 
have been visited, return to the starting vertex \( v_0 \) to complete the tour.

4. **Optimization (optional)**: After obtaining a feasible solution using the nearest 
neighbor algorithm, you may apply optimization techniques such as 2-opt or 3-opt to 
improve the solution further. These techniques involve iteratively swapping edges in 
the tour to reduce its total length while maintaining its feasibility.

It's important to note that while the nearest neighbor algorithm is simple and efficient, 
it may not always yield the optimal solution to the TSP. However, it often provides a 
good approximation, especially for small to medium-sized instances of the problem. 
Additionally, the nearest neighbor algorithm's time complexity is \( O(n^2) \), 
where \( n \) is the number of nodes, making it suitable for reasonably sized TSP instances.


The pseudocode for computing a single starting solution from any node is presented Here:
Nearest Neighbour implementation

Input : Starting node
Output: a valid tour.
tour <- empty tour
solution <- 0
visited <- {start node} /* visited nodes */ curr node <- start node
/* While there are nodes to visit */
while |visited| ≠ N do
closest node <- closest node to curr node ∈/ visited solution <- solution + dist(curr node,closest node) curr node <- closest node
add curr node to visited
add edge (curr node,closest node) to tour
end
add edge (curr node,start node) to tour
solution <- solution + dist(curr node,start node)


We also used two techniques:

########### NOT IMPLEMENTED ###############
• GRASP (greedy randomized adaptive search procedure): 
used in the nearest neighbor search, introducing randomization 
in the choice of the next node of the tour by choosing the second 
nearest node with probability of 10%:

• Multistart: initializes nearest neighbour search for each possible 
node and then returns the best solution.


Refinement Heuristics
This kind of heuristics start from a given solution and improves it making small changes. 
Their performances are strongly dependent by the construc- tion heuristic used.

2-OPT.
This heuristic proposed in 1958 by
G. A. Croes, A method for solving traveling salesman problems. Opera- tions Res. 6 (1958) , pp., 791-812.
first initializes the tour randomly or using some construction algorithm and then iteratively 
improves this tour by resolving crossing edges. This is done by selecting 2 edges that are 
crossing each other and then exchange them. More specifically, at each step it selects 
2 edges (a, b) and (c, d) from the tour and crosses them obtaining the edges (a, c) and (b, d) 
exploiting the triangle inequality. Thanks to the triangle inequality it’s possible to 
determine, when a solution contains two or more crossing edges, that this 
solution is not optimal. Given two edges (a,b) and (c,d) where a ̸= c, a ̸= d and b ̸= c the 
condition for exchanging two edges is the following:

∆(a,c)=cac +cbd −cab −ccd <0

where cij is the cost between node i and node j. When the condition 5.1 is satisfied, 
the edges (a,b) and (c,d) are replaced with (a,c) and (b,d) respectively, decreasing the 
total cost of the tour. The 2-opt move can be seen in fig. 5.2.
The 2-opt algorithm continues until there are no more crossing edges, i.e. when the 
condition 5.1 is not satisfied for all couple of edges, reaching a local optimum. 
The 2-OPT algorithm is shown in (Algorithm 7).

Figure: 2-Opt move: (a, b) and (c, d) are crossing, so replacing them 
with (a, c) and (b, d) decreases the tour length.

Comparison
We compared different versions of 2-opt refinement each with a different constructive 
heuristic as the initial solution. The results are shown in table 5.2 and Fig. 5.3. 
The best is greedy with multistart. This could be surprising since in comparison made 
with constructive heuristics in Fig. 5.1 the Extra Mileage was the winner. 
A reason on why applying 2-opt refinement the advantage of extra mileage is lost is 
due to the fact that extra mileage gen- erates a solution with fewer crossing edges 
compared with greedy and grasp. In terms of quality of the solution and running time, 
the best one is 2-opt in combination with basic greedy. As you can see in table 5.2, 
the solving time is in the order of fractions of second for the majority of instances 
tested, making this algorithm suitable for larger instances.