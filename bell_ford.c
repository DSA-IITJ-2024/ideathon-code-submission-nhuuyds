#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_FRIENDS 100

// Function to find minimum of two integers
int min(int a, int b)
{
    return a < b ? a : b;
}

// Function to find negative-cost cycles using Bellman-Ford algorithm
bool bellmanFord(int graph[MAX_FRIENDS][MAX_FRIENDS], int n, int parent[])
{
    int dist[MAX_FRIENDS];
    // Initialize distances to infinity
    for (int i = 0; i < n; ++i)
    {
        dist[i] = INT_MAX;
        parent[i] = -1;
    }
    // Set distance to source as 0
    dist[0] = 0;

    // Relax edges |V| - 1 times
    for (int i = 0; i < n - 1; ++i)
    {
        for (int u = 0; u < n; ++u)
        {
            for (int v = 0; v < n; ++v)
            {
                if (graph[u][v] > 0 && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
                {
                    dist[v] = dist[u] + graph[u][v];
                    parent[v] = u;
                }
            }
        }
    }

    // Check for negative-cost cycles
    for (int u = 0; u < n; ++u)
    {
        for (int v = 0; v < n; ++v)
        {
            if (graph[u][v] > 0 && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
            {
                return true; // Negative-cost cycle found
            }
        }
    }
    return false; // No negative-cost cycle found
}

// Function to minimize cash flow using Cycle-Canceling Algorithm
void minCashFlow(int graph[MAX_FRIENDS][MAX_FRIENDS], int n)
{
    int parent[MAX_FRIENDS]; // Array to store parent of each node in negative-cost cycles

    // Find negative-cost cycles and augment flow until no negative-cost cycles exist
    while (bellmanFord(graph, n, parent))
    {
        // Find the minimum capacity along the negative-cost cycle
        int minCapacity = INT_MAX;
        for (int v = 0; v < n; ++v)
        {
            int u = parent[v];
            minCapacity = min(minCapacity, graph[u][v]);
        }

        // Augment flow along the negative-cost cycle
        for (int v = 0; v < n; ++v)
        {
            int u = parent[v];
            graph[u][v] -= minCapacity;
            graph[v][u] += minCapacity;
        }
    }

    // Print the final flow network
    printf("Minimum cash flow:\n");
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (graph[i][j] > 0)
            {
                printf("Person %d pays %d to Person %d\n", i, graph[i][j], j);
            }
        }
    }
}

int main()
{
    int n;
    printf("Enter the number of friends participating in the cash flow: ");
    scanf("%d", &n);

    // Define the initial flow network representing the debts among friends
    int graph[MAX_FRIENDS][MAX_FRIENDS];
    printf("Enter the debts among friends:\n");
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            printf("Debt from Person %d to Person %d: ", i, j);
            scanf("%d", &graph[i][j]);
        }
    }

    // Call the function to minimize cash flow using Cycle-Canceling Algorithm
    minCashFlow(graph, n);

    return 0;
}