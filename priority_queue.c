#include <stdio.h>
#include <stdlib.h>

#define MAX_FRIENDS 100

// Function to minimize cash flow among friends
void min_cash_flow(int graph[][MAX_FRIENDS], int n)
{
    // Compute net amount for each person
    int net_amounts[MAX_FRIENDS] = {0};
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            net_amounts[i] += graph[j][i] - graph[i][j];
        }
    }

    // Priority queue represented using arrays
    int pq[MAX_FRIENDS];
    int pq_size = 0;

    // Insert net amounts into the priority queue
    for (int i = 0; i < n; i++)
    {
        if (net_amounts[i] != 0)
        {
            pq[pq_size++] = i;
        }
    }

    // Process transactions until the priority queue is empty
    while (pq_size > 1)
    {
        // Find maximum creditor and maximum debtor
        int max_creditor = pq[0];
        int max_debtor = pq[pq_size - 1];

        // Find minimum of maxDebit and maxCredit
        int settle_amount = (net_amounts[max_creditor] > -net_amounts[max_debtor]) ? -net_amounts[max_debtor] : net_amounts[max_creditor];

        // Settle the transaction
        graph[max_debtor][max_creditor] += settle_amount;
        net_amounts[max_creditor] -= settle_amount;
        net_amounts[max_debtor] += settle_amount;

        // Update priority queue
        if (net_amounts[max_creditor] == 0)
        {
            pq_size--;
        }
        if (net_amounts[max_debtor] == 0)
        {
            pq_size--;
        }

        // Print transaction
        if (settle_amount < 0)
        {
            printf("Person %d pays %d to Person %d\n", max_creditor + 1, (-1*settle_amount), max_debtor + 1);
        }
        else
        {

            printf("Person %d pays %d to Person %d\n", max_debtor + 1, settle_amount, max_creditor + 1);
        }
    }
}

int main()
{
    int n;
    printf("Enter the number of friends: ");
    scanf("%d", &n);

    int graph[MAX_FRIENDS][MAX_FRIENDS];
    printf("Enter the amount each person owes or is owed to others (use 0 for no transaction):\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("Debt from Person %d to Person %d: ", (i + 1), (j + 1));
            scanf("%d", &graph[i][j]);
        }
    }

    min_cash_flow(graph, n);

    return 0;
}
