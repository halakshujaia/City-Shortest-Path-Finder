# City Shortest Path Finder

A Data Structures and Algorithms project implemented in **C** for finding
routes between cities using graph algorithms.

The system loads city connections and distances from a file, constructs a
weighted undirected graph, and compares **Dijkstra's Algorithm** and
**Breadth-First Search (BFS)** for route finding.

---

## Project Overview

The program represents cities as vertices in a graph and the roads between
them as weighted edges.

Each connection contains:

- Source city
- Destination city
- Distance in kilometers

The user selects a source city and a destination city, and the program
calculates routes using:

1. **Dijkstra's Algorithm**
2. **Breadth-First Search (BFS)**

The two algorithms are then compared based on:

- Selected route
- Total path distance
- Number of hops
- Execution time

---

## Graph Representation

The graph is implemented using an **Adjacency List**.

Each city is represented as a vertex.

Each edge stores:

- Destination vertex
- Distance to the destination
- Pointer to the next edge

The graph is undirected, so every connection is stored in both directions.

---

## Dijkstra's Algorithm

Dijkstra's Algorithm is used to calculate the route with the
**minimum total distance** from the source city.

The implementation uses a **Min-Heap** to efficiently select the next city
with the smallest known distance.

The algorithm maintains:

- Minimum distance for each city
- Parent of each city
- Min-Heap of candidate vertices

When a shorter route is found, the distance is updated and the heap is
adjusted.

---

## Min-Heap

The project implements a custom Min-Heap for Dijkstra's Algorithm.

The heap supports:

- Insert
- Extract minimum
- Decrease key
- Heapify
- Dynamic memory allocation

Each heap node stores:

```text
Vertex
Distance
```

---

## Breadth-First Search (BFS)

BFS is also performed from the selected source city.

Unlike Dijkstra, BFS focuses on finding the route with the
**minimum number of hops** rather than the minimum weighted distance.

The BFS implementation uses:

- Queue
- Visited array
- Parent array
- Hop-distance array

---

## Dijkstra vs BFS

The program compares the two algorithms.

### Dijkstra

Optimizes:

```text
Minimum total distance
```

### BFS

Optimizes:

```text
Minimum number of hops
```

For each algorithm, the program displays:

- Route
- Total path distance
- Execution time

BFS additionally displays:

- Number of hops

---

## Input File

The program reads city information from:

```text
cities.txt
```

Each line follows the format:

```text
SourceCity#DestinationCity#Distance
```

Example:

```text
Jerusalem#Gaza#41
Akka#Haifa#35
Jenin#Qalqilya#23
Gaza#Hebron#32
Bethlehem#Jerusalem#9
```

Distances are represented in kilometers.

---

## Main Menu

The application provides the following menu:

```text
==== Graph Menu ====

1. Load cities
2. Enter source city
3. Enter destination city
4. Save & Exit
```

---

## Load Cities

Option 1 reads:

```text
cities.txt
```

and constructs the graph.

The program:

- Reads each city connection
- Creates new cities when necessary
- Adds weighted edges
- Builds the adjacency lists

---

## Select Source City

Option 2 allows the user to enter the source city.

After selecting a valid source city, the program runs both:

- Dijkstra
- BFS

The execution time of each algorithm is measured separately.

---

## Select Destination City

Option 3 asks the user for the destination city.

The program then displays the Dijkstra result.

Example:

```text
Dijkstra Result (Minimum Distance)

Path:
CityA -> CityB -> CityC

Full Route Details:
- CityA to CityB (20 km)
- CityB to CityC (15 km)

Total Minimum Cost: 35 km
Execution Time: ...
```

It also displays the BFS result:

```text
BFS Result (Shortest Hops)

Path:
CityA -> CityD -> CityC

Total Path Distance: ...
Number of Hops: ...
Execution Time: ...
```

---

## Output File

When the user selects:

```text
4. Save & Exit
```

the most recent route results are written to:

```text
shortest_path.txt
```

The file contains information such as:

- Source city
- Destination city
- Dijkstra path
- Dijkstra total distance
- Dijkstra execution time
- BFS path
- BFS total distance
- BFS number of hops
- BFS execution time

---

## Performance Measurement

The implementation measures algorithm execution time using a monotonic clock.

Execution time is reported in:

```text
milliseconds (ms)
```

This allows the program to compare the practical execution time of
Dijkstra and BFS.

---

## Data Structures and Algorithms

The project demonstrates the use of:

- Graphs
- Adjacency Lists
- Linked Lists
- Min-Heaps
- Priority-based processing
- Queues
- Dijkstra's Algorithm
- Breadth-First Search
- Parent Arrays
- Dynamic Memory Allocation
- File Handling
- Path Reconstruction
- Weighted Graphs
- Execution-Time Measurement

---

## Constants

The implementation supports up to:

```text
60 cities
```

Important constants include:

```text
MAX = 60
NAME_LEN = 50
INF = 999999
```

---

## Memory Management

The program dynamically allocates memory for:

- Graph edges
- Min-Heap
- Heap array

Allocated graph and heap memory is released when it is no longer needed.

---

## Project Files

Main implementation:

```text
main.c
```

Input file:

```text
cities.txt
```

Generated output file:

```text
shortest_path.txt
```

---

## Author

**Hala Khalil**  
Student ID: **1231019**  
Section: **1**

---

## Course Information

**Course:** COMP2421 – Data Structures and Algorithms  
**Project:** Project No. 3  
**Semester:** Fall 2025/2026  
**Department:** Computer Science  
**University:** Birzeit University

---

## Project Purpose

The purpose of this project is to apply graph data structures and shortest-path
algorithms to a real-world route-finding problem.

The project demonstrates the difference between two graph traversal approaches:

- **Dijkstra's Algorithm** finds the path with the minimum weighted distance.
- **BFS** finds a path with the minimum number of edges or hops.

The results allow the behavior and execution time of both algorithms to be
compared within the same graph.
