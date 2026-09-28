/*Name: Hala Khalil
  ID: 1231019
  Sec: 1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 60
#define NAME_LEN 50
#define INF 999999

typedef struct
{
    char name[NAME_LEN];
} City;

// Edge node for adjacency list
typedef struct EdgeNode
{
    int dest;           // Destination vertex
    int weight;         // Edge weight (distance)
    struct EdgeNode *next;
} EdgeNode;

// Adjacency list
typedef struct
{
    EdgeNode *head;     // Head of linked list
} AdjList;

// MIN HEAP FOR DIJKSTRA
typedef struct
{
    int vertex;
    int distance; // key (dist)
} HeapNode;

typedef struct
{
    HeapNode *array;// heap array
    int size;//number of element into heap
    int capacity;// max elements
} MinHeap;

MinHeap* createMinHeap(int capacity)
{
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));// allocate heap
    if (!heap)
    {
        printf("Error: Memory allocation failed for heap!\n");
        return NULL;
    }

    heap->array = (HeapNode*)malloc(capacity * sizeof(HeapNode)); // allocate array
    if (!heap->array)
    {
        printf("Error: Memory allocation failed for heap array!\n");
        free(heap);
        return NULL;
    }

    heap->size = 0;// empty heap

    heap->capacity = capacity;
    return heap;
}

void swap(HeapNode *a, HeapNode *b)
{
    HeapNode temp = *a; // swap
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap *heap, int idx)
{
    int smallest = idx; // assume idx is smallest
    int left = 2 * idx + 1;// left child
    int right = 2 * idx + 2; // right child

    if (left < heap->size && heap->array[left].distance < heap->array[smallest].distance)
        smallest = left; // choose smaller child

    if (right < heap->size && heap->array[right].distance < heap->array[smallest].distance)
        smallest = right; // choose smaller child

    if (smallest != idx)
    {
        swap(&heap->array[smallest], &heap->array[idx]);
        minHeapify(heap, smallest); // continue down
    }
}

HeapNode extractMin(MinHeap *heap)
{
    if (heap->size == 0)
    {
        HeapNode empty = {-1, INF}; // invalid
        return empty;
    }

    HeapNode root = heap->array[0];  // min element
    heap->array[0] = heap->array[heap->size - 1]; // move last to root
    heap->size--; // shrink
    minHeapify(heap, 0); // restore heap

    return root;
}

// Used when a shorter path to an existing node is found in Dijkstra
void decreaseKey(MinHeap *heap, int vertex, int newDist)
{
    int i;
    for (i = 0; i < heap->size; i++) // find vertex
    {
        if (heap->array[i].vertex == vertex)
            break;
    }

    if (i == heap->size) // not found
        return;

    heap->array[i].distance = newDist; // update key

    while (i > 0 && heap->array[(i - 1) / 2].distance > heap->array[i].distance)
    {
        swap(&heap->array[i], &heap->array[(i - 1) / 2]); // bubble up
        i = (i - 1) / 2; // move to parent
    }
}

void insertHeap(MinHeap *heap, int vertex, int distance)
{
    if (heap->size == heap->capacity)
        return;

    heap->size++;
    int i = heap->size - 1;
    heap->array[i].vertex = vertex;
    heap->array[i].distance = distance;

    while (i > 0 && heap->array[(i - 1) / 2].distance > heap->array[i].distance)
    {
        swap(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void freeMinHeap(MinHeap *heap)
{
    if (!heap)
        return;

    if (heap->array)
        free(heap->array);

    free(heap);
}

//ADJACENCY LIST FUNCTIONS

// Create a new edge node
EdgeNode* createEdgeNode(int dest, int weight)
{
    EdgeNode *newNode = (EdgeNode*)malloc(sizeof(EdgeNode));
    if (!newNode)
    {
        printf("Error: Memory allocation failed for edge!\n");
        return NULL;
    }
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Add edge to adjacency list (undirected graph)
void addEdge(AdjList graph[], int src, int dest, int weight)
{
    // Add edge from src to dest
    EdgeNode *newNode = createEdgeNode(dest, weight);
    newNode->next = graph[src].head;
    graph[src].head = newNode;

    // Add edge from dest to src (undirected)
    newNode = createEdgeNode(src, weight);
    newNode->next = graph[dest].head;
    graph[dest].head = newNode;
}

// Free adjacency list
void freeGraph(AdjList graph[], int n)
{
    for (int i = 0; i < n; i++)
    {
        EdgeNode *current = graph[i].head;
        while (current)
        {
            EdgeNode *temp = current;
            current = current->next;
            free(temp);
        }
        graph[i].head = NULL;
    }
}

//GLOBAL

City cities[MAX];
AdjList graph[MAX];  // Changed from matrix to adjacency list
int cityCount = 0;

int dist[MAX];
int parent[MAX];

// Tracks whether each vertex is currently inside the heap (O(1) membership check)
int inHeap[MAX];

int bfsParent[MAX];
int bfsDistance[MAX];

char lastSource[NAME_LEN];
char lastDest[NAME_LEN];
int lastDistance = INF;
double dijTime = 0;
double bfsTime = 0;


//PROTOTYPES

void loadCities();
int getIndex(char name[]);
void dijkstra(int src);
void bfs(int src);
void printDijkstraPath(int dest);
void printBfsPath(int dest);

double getTimeMs()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec * 1000.0 + (double)t.tv_nsec / 1000000.0;
}

//MAIN
int main()
{
    // Initialize adjacency list
    for (int i = 0; i < MAX; i++)
        graph[i].head = NULL;

    int choice;
    char source[NAME_LEN], dest[NAME_LEN];
    int srcIndex = -1, destIndex = -1;

    do
    {

        printf("\n==== Graph Menu ====\n");
        printf("1. Load cities\n");
        printf("2. Enter source city\n");
        printf("3. Enter destination city\n");
        printf("4. Save & Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            loadCities();
            printf("Cities loaded successfully!\n");
        }
        else if (choice == 2)
        {
            printf("Enter source city: ");
            scanf("%s", source);

            srcIndex = getIndex(source);

            if (srcIndex == -1)
            {
                printf("City not found!\n");
            }
            else
            {


                double start, end;

                /* Run Dijkstra to compute shortest paths */
                start = getTimeMs();
                dijkstra(srcIndex);
                end = getTimeMs();
                dijTime = end - start;

                /* Run BFS for comparison */
                start = getTimeMs();
                bfs(srcIndex);
                end = getTimeMs();
                bfsTime = end - start;

                printf("\nShortest paths calculated successfully.\n");
                printf("You can now choose option 3 to display results.\n");
            }
        }

        else if (choice == 3)
        {

            if (srcIndex == -1)
            {
                printf("Please choose source city first!\n");
                continue;
            }

            printf("Enter destination city: ");
            scanf("%s", dest);

            destIndex = getIndex(dest);
            if (destIndex == -1)
            {
                printf("City not found!\n");
                continue;
            }



            if (dist[destIndex] == INF)
            {
                printf("\nNo path exists.\n");
                continue;
            }
            printf("\nDijkstra Result (Minimum Distance)\n");
            printf("Path: ");
            printDijkstraPath(destIndex);
            printf("\n");

            printf("\nFull Route Details:\n");

            // Build path array to print in correct order
            int path[MAX];
            int pathLen = 0;
            int v = destIndex;

            while (v != -1)
            {
                path[pathLen++] = v;
                v = parent[v];
            }

            // Print route from source to destination with weights
            int total = 0;
            for (int i = pathLen - 1; i > 0; i--)
            {
                int u = path[i];
                int nextCity = path[i-1];

                // Find weight in adjacency list
                int w = 0;
                EdgeNode *edge = graph[u].head;
                while (edge)
                {
                    if (edge->dest == nextCity)
                    {
                        w = edge->weight;
                        break;
                    }
                    edge = edge->next;
                }

                printf("- %s to %s (%d km)\n",
                       cities[u].name,
                       cities[nextCity].name,
                       w);
                total += w;
            }

            printf("Total Minimum Cost: %d km\n", dist[destIndex]);
            printf("Execution Time: %.3f ms\n", dijTime);


            //BFS

            printf("\nBFS Result (Shortest Hops)\n");
            printf("Path: ");
            printBfsPath(destIndex);
            printf("\n");

            int bfsTotal = 0;
            v = destIndex;
            while (bfsParent[v] != -1)
            {
                // Find weight in adjacency list
                EdgeNode *edge = graph[bfsParent[v]].head;
                while (edge)
                {
                    if (edge->dest == v)
                    {
                        bfsTotal += edge->weight;
                        break;
                    }
                    edge = edge->next;
                }
                v = bfsParent[v];
            }

            printf("\nTotal Path Distance: %d km\n", bfsTotal);
            printf("Number of Hops: %d\n", bfsDistance[destIndex]);
            printf("Execution Time: %.3f ms\n", bfsTime);


            strcpy(lastSource, cities[srcIndex].name);
            strcpy(lastDest, cities[destIndex].name);
            lastDistance = dist[destIndex];
        }

        else if (choice == 4)
        {
            if (lastDistance == INF)
            {
                printf("No results to save. Run a search first.\n");
                continue;
            }

            FILE *out = fopen("shortest_path.txt", "w");
            if (!out)
            {
                printf("Error opening output file!\n");
                continue;
            }

            fprintf(out, "==== Graph Results ====\n");
            fprintf(out, "Source: %s\n", lastSource);
            fprintf(out, "Destination: %s\n\n", lastDest);

            //Dijkstra
            fprintf(out, "Dijkstra Result (Minimum Distance)\n");
            fprintf(out, "Path: ");

            // Build path for correct order output
            int path[MAX];
            int pathLen = 0;
            int v = getIndex(lastDest);

            while (v != -1)
            {
                path[pathLen++] = v;
                v = parent[v];
            }

            // Print path from source to destination
            for (int i = pathLen - 1; i >= 0; i--)
            {
                fprintf(out, "%s", cities[path[i]].name);
                if (i > 0) fprintf(out, " -> ");
            }
            fprintf(out, "\n");

            fprintf(out, "Total Distance: %d km\n", lastDistance);
            fprintf(out, "Execution Time: %.3f ms\n\n", dijTime);

            //BFS
            fprintf(out, "BFS Result (Shortest Hops)\n");
            fprintf(out, "Path: ");

            // Build BFS path for correct order output
            pathLen = 0;
            v = getIndex(lastDest);

            while (v != -1)
            {
                path[pathLen++] = v;
                v = bfsParent[v];
            }

            // Print BFS path from source to destination
            int bfsTotal = 0;
            for (int i = pathLen - 1; i >= 0; i--)
            {
                fprintf(out, "%s", cities[path[i]].name);
                if (i > 0) fprintf(out, " -> ");
            }
            fprintf(out, "\n");

            // Calculate BFS total distance
            for (int i = pathLen - 1; i > 0; i--)
            {
                EdgeNode *edge = graph[path[i]].head;
                while (edge)
                {
                    if (edge->dest == path[i-1])
                    {
                        bfsTotal += edge->weight;
                        break;
                    }
                    edge = edge->next;
                }
            }
            fprintf(out, "Total Distance: %d km\n", bfsTotal);
            fprintf(out, "Number of Hops: %d\n", bfsDistance[getIndex(lastDest)]);
            fprintf(out, "Execution Time: %.3f ms\n", bfsTime);

            fclose(out);

            printf("Results saved successfully to shortest_path.txt\n");
        }

        else
        {
            printf("Invalid choice!\n");
        }

    }
    while (choice != 4);

    // Free graph memory before exit
    freeGraph(graph, cityCount);

    return 0;
}

//LOAD
void loadCities()
{


    FILE *f = fopen("cities.txt", "r");
    if (!f)
    {
        printf("Error: cities.txt not found!\n");
        return;
    }

    char line[150];
    char *token;
    char c1[NAME_LEN], c2[NAME_LEN];
    int d;

    // Free old graph data in case Load is called again
    freeGraph(graph, cityCount);

// Reset graph heads
    for (int i = 0; i < MAX; i++)
        graph[i].head = NULL;

    cityCount = 0; // reset after freeing


    while (fgets(line, sizeof(line), f))
    {

        line[strcspn(line, "\n")] = '\0';

        token = strtok(line, "#");
        if (!token)
        {
            printf("Invalid line format.\n");
            continue;
        }
        strcpy(c1, token);

        token = strtok(NULL, "#");
        if (!token)
        {
            printf("Invalid line format.\n");
            continue;
        }
        strcpy(c2, token);

        token = strtok(NULL, "#");
        if (!token)
        {
            printf("Invalid line format.\n");
            continue;
        }

        d = atoi(token);
        if (d < 0)
        {
            printf("Invalid distance detected: %s -> %s (%d)\n", c1, c2, d);
            continue;
        }

        int i = getIndex(c1);
        if (i == -1)
        {
            if (cityCount >= MAX) break;
            strcpy(cities[cityCount].name, c1);
            i = cityCount++;
        }

        int j = getIndex(c2);
        if (j == -1)
        {
            if (cityCount >= MAX) break;
            strcpy(cities[cityCount].name, c2);
            j = cityCount++;
        }

        // Add edge to adjacency list
        addEdge(graph, i, j, d);
    }

    fclose(f);
}

// DIJKSTRA WITH MIN-HEAP
void dijkstra(int src)
{
    MinHeap *heap = createMinHeap(MAX);
    if (!heap)
    {
        printf("Error: Failed to create min heap!\n");
        return;
    }

    for (int i = 0; i < cityCount; i++)
    {
        dist[i] = INF;
        parent[i] = -1;
        inHeap[i] = 0;  // Initialize tracking array
    }

    dist[src] = 0;
    insertHeap(heap, src, 0);
    inHeap[src] = 1;  // Mark source as in heap

    while (heap->size > 0)
    {
        HeapNode minNode = extractMin(heap);
        int u = minNode.vertex;

        inHeap[u] = 0;  // Mark as removed from heap

        if (minNode.distance > dist[u])
            continue;

        // Traverse adjacency list
        EdgeNode *edge = graph[u].head;
        while (edge)
        {
            int v = edge->dest;
            int weight = edge->weight;

            int newDist = dist[u] + weight;

            if (newDist < dist[v])
            {
                dist[v] = newDist;
                parent[v] = u;

                if (inHeap[v])  // O(1)
                    decreaseKey(heap, v, newDist);
                else
                {
                    insertHeap(heap, v, newDist);
                    inHeap[v] = 1;  // Mark as inserted
                }
            }

            edge = edge->next;
        }
    }

    freeMinHeap(heap);
}

// BFS
void bfs(int src)
{
    int visited[MAX] = {0};
    int queue[MAX];
    int front = 0, rear = 0;

    for (int i = 0; i < cityCount; i++)
    {
        bfsParent[i] = -1;
        bfsDistance[i] = 0;
    }

    visited[src] = 1;
    queue[rear++] = src;

    while (front < rear)
    {
        int u = queue[front++];

        // Traverse adjacency list
        EdgeNode *edge = graph[u].head;
        while (edge)
        {
            int v = edge->dest;

            if (!visited[v])
            {
                visited[v] = 1;
                bfsParent[v] = u;
                bfsDistance[v] = bfsDistance[u] + 1;
                queue[rear++] = v;
            }

            edge = edge->next;
        }
    }
}

//PRINT

void printDijkstraPath(int dest)
{
    if (parent[dest] == -1)
    {
        printf("%s", cities[dest].name);
        return;
    }
    printDijkstraPath(parent[dest]);
    printf(" -> %s", cities[dest].name);
}

void printBfsPath(int dest)
{
    if (bfsParent[dest] == -1)
    {
        printf("%s", cities[dest].name);
        return;
    }
    printBfsPath(bfsParent[dest]);
    printf(" -> %s", cities[dest].name);
}

int getIndex(char name[])
{
    for (int i = 0; i < cityCount; i++)
        if (strcmp(cities[i].name, name) == 0)
            return i;
    return -1;
}
