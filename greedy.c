#include <stdio.h>
#include <stdbool.h>

#define MAX_FRIENDS 100

// Structure to represent a person
struct Person
{
    int id;
    int cash;
};

// Structure to represent a priority queue
struct PriorityQueue
{
    struct Person array[MAX_FRIENDS];
    int size;
};

// Function to initialize a priority queue
void initializePriorityQueue(struct PriorityQueue *pq)
{
    pq->size = 0;
}

// Function to compare two persons for max heap
bool compareAsc(struct Person p1, struct Person p2)
{
    return p1.cash < p2.cash;
}

// Function to compare two persons for min heap
bool compareDesc(struct Person p1, struct Person p2)
{
    return p1.cash > p2.cash;
}

// Function to swap two persons
void swap(struct Person *p1, struct Person *p2)
{
    struct Person temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

// Function to push a person into the priority queue
void push(struct PriorityQueue *pq, struct Person person, bool (*compare)(struct Person, struct Person))
{
    int index = pq->size++;
    pq->array[index] = person;

    while (index > 0 && compare(pq->array[(index - 1) / 2], pq->array[index]))
    {
        swap(&pq->array[(index - 1) / 2], &pq->array[index]);
        index = (index - 1) / 2;
    }
}

// Function to pop a person from the priority queue
struct Person pop(struct PriorityQueue *pq, bool (*compare)(struct Person, struct Person))
{
    struct Person popped = pq->array[0];
    pq->array[0] = pq->array[--pq->size];

    int index = 0;
    while (2 * index + 1 < pq->size)
    {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int swapIndex = leftChild;

        if (rightChild < pq->size && compare(pq->array[leftChild], pq->array[rightChild]))
        {
            swapIndex = rightChild;
        }

        if (compare(pq->array[index], pq->array[swapIndex]))
        {
            swap(&pq->array[index], &pq->array[swapIndex]);
        }
        else
        {
            break;
        }

        index = swapIndex;
    }

    return popped;
}

// Function to minimize cash flow between friends
void minCashFlow(int (*graph)[MAX_FRIENDS], int n)
{
    struct PriorityQueue minQ, maxQ;
    initializePriorityQueue(&minQ);
    initializePriorityQueue(&maxQ);

    int amount[MAX_FRIENDS] = {0};

    // Calculate the cash to be credited/debited to/from each person
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            int diff = graph[j][i] - graph[i][j];
            amount[i] += diff;
        }
    }

    // Fill the minQ and maxQ queues
    for (int i = 0; i < n; ++i)
    {
        if (amount[i] > 0)
        {
            struct Person p = {i, amount[i]};
            push(&maxQ, p, compareDesc);
        }
        else if (amount[i] < 0)
        {
            struct Person p = {i, amount[i]};
            push(&minQ, p, compareAsc);
        }
    }

    // Solve the transaction
    while (minQ.size && maxQ.size)
    {
        struct Person maxCreditEntry = pop(&maxQ, compareDesc);
        struct Person maxDebitEntry = pop(&minQ, compareAsc);

        int transaction_val = maxCreditEntry.cash + maxDebitEntry.cash;

        int debtor = maxDebitEntry.id;
        int creditor = maxCreditEntry.id;
        int owed_amount;

        if (transaction_val == 0)
        {
            owed_amount = maxCreditEntry.cash;
        }
        else if (transaction_val < 0)
        {
            owed_amount = maxCreditEntry.cash;
            struct Person p = {debtor, transaction_val};
            push(&minQ, p, compareAsc);
        }
        else
        {
            owed_amount = -(maxDebitEntry.cash);
            struct Person p = {creditor, transaction_val};
            push(&maxQ, p, compareDesc);
        }

        // Print result
        printf("Person %d pays %d to Person %d\n", debtor, owed_amount, creditor);
    }
}

int main()
{
    // Test case 1
    int graph1[MAX_FRIENDS][MAX_FRIENDS] = {
        {0, 1000, 2000},
        {0, 0, 5000},
        {0, 0, 0},
    };

    printf("Solution 1:\n");
    minCashFlow(graph1, 3);

    // Test case 2
    int graph2[MAX_FRIENDS][MAX_FRIENDS] = {
        {2, 63, 0, 85, 49},
        {0, 76, 0, 0, 27},
        {0, 0, 0, 17, 0},
        {73, 32, 50, 6, 71},
        {0, 86, 0, 0, 10}};

    printf("\nSolution 2:\n");
    minCashFlow(graph2, 5);

    return 0;
}