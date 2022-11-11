// CS-302 Analysis and design of algorithm project

// A new Karzanov-type algorith O(n^3) max-flow algorithm
// Submitted By -
/*
    Raghav Patidar - 2020CSB1115
    Raj            - 2020CSB1116
    Rohan          - 2020CSB1117
    Janmeet Singh  - 2020CSB1175
    Jyoti Dhiman   - 2020CSB1092
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define mn 1000 // Maximum nodes

int n;                  // Total Number of Nodes in Graph
int cap[mn][mn];        // Capacity of each edge
int flow[mn][mn];       // Flow through particular edge
int level[mn];          // Level of nodes after making of layered network Acyclic Structure

struct node {          // Node description
    int data;
    node* next;
    node(int d)
    {
        data = d;
        next = NULL;
    }
};

struct Queue {         // Queue data Structure from Scratch
    node *front, *rear;
    Queue() { front = rear = NULL; }

    void enQueue(int x)      // Enqueue Function
    {
        node* temp = new node(x);
        if (rear == NULL) {
            front = rear = temp;
            return;
        }
        rear->next = temp;
        rear = temp;
    }
    void deQueue()        // Dequeue Function
    {
        if (front == NULL)
            return;
        node* temp = front;
        front = front->next;
        if (front == NULL)
            rear = NULL;
        delete (temp);
    }
};

struct pairs{     // Pair data structure From scratch
    int lvl;
    int node;
    pairs(int l, int n){
        lvl = l;
        node = n;
    }
};

void initialize()   // Initializing capacity matrix (cap) and flow matrix (flow) and level
{
    for (int i = 0; i < mn; i++)
    {
        level[i] = -1;
        for (int j = 0; j < mn; j++)
        {
            cap[i][j] = 0;
            flow[i][j] = 0;
        }
}

    }

void assign_level(int src)       // Creating layered acyclic network and assigning levels to each node
{
    Queue q ;
    level[src] = 0;
    q.enQueue(src);
    while (q.front != NULL)
    {
        int x = (q.front)->data;
        q.deQueue();
        for (int j = 1; j <= n; j++)
        {
            if (cap[x][j]>0)
            {
                if (level[j] < level[x] + 1)
                    level[j] = level[x] + 1;
                q.enQueue(j);
            }
        }
    }
}

int main()
{
    initialize();            // calling initializing function
    cout << "Enter number of vertices: ";
    cin >> n;

    int incoming_cap[n + 1];          // sum of total incoming flow of a particular node
    int outgoing_cap[n + 1];          // sum of total outgoing flow of a particular node

    for (int i = 0; i <= n ; i++)  // initializing to zero
    {
        incoming_cap[i] = 0;
        outgoing_cap[i] = 0;
    }

    printf("Enter edges and capacity as three spaced integers : ");
    printf("\n\nNote That : If you inter x y z then it means egdes goes from x to y i.e x->y , having weight z \n\n");
    printf("\n\nMake sure you use 1-based indexing for graph nodes.\n\n");
    printf("Once you are done, enter -1 to quit.\n");

    while (1)
    {
        int left;  // left vertex
        int right; // right vertex
        int w;     // capacity of an edge
        cin >> left;
        if (left == -1)
        {
            break;
        }
        cin >> right >> w;
        cap[left][right] = w;
        cap[right][left] = -w; // for parent identification

        cout << "Inserted Edge !!\n";
    }
    struct pairs *pr[n]; // array of pairs for storing {level,node}

    int source, sink;
    cout << "Enter source vertex : ";
    cin >> source;
    cout << "Enter sink vertex : ";
    cin >> sink;
    assign_level(source);
    struct pairs *tt = new pairs(0, 0);
    pr[0] = tt ;

    int k = 1 ;
    for(int i = -1; i <= n ; i++){
        for(int j = 1 ; j <= n; j++){
            if(level[j]==i){
                struct pairs *temp = new pairs(i, j);
                pr[k] = temp;
                ++k;
            }
        }
    }

    int balanced[n + 1];       // check for balanced node
    int blocked[n + 1];        // check for blocked node

    for (int i = 0; i <= n; i++) // initializing to zero
    {
        balanced[i] = 0;
        blocked[i]=0;
    }

    int is_balanced = 0; // check for balanced condition of every node
    int f = 1;     // f==1 means algorithm is in forward step else it is in backward step

    balanced[source]=1; // source is always balanced
    balanced[sink]=1;    // sink is always balanced
    blocked[source]=0;   // source is always unblocked
    blocked[sink]=0;     // sink is always unblocked

    while (!is_balanced)
    {
        // Forward step
        if (f == 1)
        {
            // step 1 : Maximizing flow to its capacity of an edge except edge between source node and blocked node

            for (int i = 1; i <= n; i++)
            {
                if (cap[source][i] > 0 && blocked[i] == 0)
                {
                    int increment = 0;
                    int oldflow = flow[source][i];
                    flow[source][i] = cap[source][i];
                    increment = flow[source][i] - oldflow;
                    incoming_cap[i] +=increment;
                    outgoing_cap[source] += increment;
                }
            }

            // step 2 : level 1 onwards

            for (int i = 1; i <= n; i++)
            {
                int lv = pr[i]->lvl;
                int nd = pr[i]->node;
                if (nd==source || nd==sink)
                    continue;
                if(lv==-1)
                {
                    balanced[nd]=1;
                    blocked[nd]=0;
                    continue;
                }

                if (incoming_cap[nd] > outgoing_cap[nd])
                {
                    int to_increase = incoming_cap[nd] - outgoing_cap[nd];

                    for (int j = 0; j <= n; j++)
                    {
                        if (cap[nd][j] > 0 && blocked[j] == 0)
                        {
                            int child = j;

                            if (to_increase > 0)
                            {
                                if (cap[nd][child]-flow[nd][child] >= to_increase)
                                {

                                    int incremented = 0;
                                    int oldflow = flow[nd][child];
                                    flow[nd][child] = flow[nd][child] + to_increase;
                                    incremented = flow[nd][child] - oldflow;
                                    outgoing_cap[nd] = outgoing_cap[nd] + incremented;
                                    incoming_cap[child] += incremented;
                                    to_increase = to_increase - incremented;

                                }
                                else
                                {
                                    int incremented=0;
                                    int oldflow = flow[nd][child];
                                    flow[nd][child]=cap[nd][child];
                                    incremented = flow[nd][child] - oldflow;
                                    outgoing_cap[nd] = outgoing_cap[nd] + incremented;
                                    incoming_cap[child] += incremented;
                                    to_increase = to_increase - incremented;
                                }
                            }
                            else
                                break;
                        }
                    }

                    if(incoming_cap[nd]!=outgoing_cap[nd])
                    {
                        blocked[nd]=1;
                        balanced[nd]=0;
                    }
                    else balanced[nd]=1;

                }
                else if (incoming_cap[nd] < outgoing_cap[nd])
                {
                    int to_decrease = outgoing_cap[nd] - incoming_cap[nd];

                    for (int j = 0; j <= n; j++)
                    {
                        if (cap[nd][j] > 0 && blocked[j] == 0)
                        {
                            int child = j;

                            if (to_decrease > 0)
                            {
                                if (flow[nd][child] >= to_decrease)
                                {
                                    int decremented =0;
                                    int oldflow = flow[nd][child];
                                    flow[nd][child] = flow[nd][child] - to_decrease;
                                    decremented = oldflow - flow[nd][child];
                                    outgoing_cap[nd]-=decremented;
                                    incoming_cap[child]-=decremented;

                                }
                                else
                                {
                                    int decremented =0;
                                    int oldflow = flow[nd][child];
                                    decremented = flow[nd][child];
                                    flow[nd][child]=0;
                                    outgoing_cap[nd]-=decremented;
                                    incoming_cap[child]-=decremented;
                                }
                            }
                            else break;
                        }
                    }

                    if(incoming_cap[nd]!=outgoing_cap[nd])
                    {
                        blocked[nd]=1;
                        balanced[nd]=0;
                    }
                    else balanced[nd]=1;

                }
                else
                {
                    balanced[nd]=1;
                }
            }

            f=0;  // making forward to zero it means now our algorithm goes in backward direction
        }

        ///// Backward Direction
        else
        {
            // step 1 : Maximizing flow to its capacity of an edge except edge between blocked node and sink node
            for (int i = 1; i <= n; i++)
            {
                if (cap[sink][i] < 0 && blocked[i]==0)
                {
                    int increment =0;
                    int oldflow = flow[i][sink];
                    flow[i][sink] = cap[i][sink];
                    increment = flow[i][sink] - oldflow;
                    outgoing_cap[i] += increment;
                    incoming_cap[sink]+=increment;
                }
            }

            // step 2 : from second last level to level 1

            for (int i = n; i >= 1; i--)
            {
                int nd = pr[i]->node;
                int lv = pr[i]->lvl;
                if(nd==source || nd==sink) continue;
                if(lv==-1)
                {
                    balanced[nd]=1;
                    blocked[nd]=0;
                    continue;
                }

                if (incoming_cap[nd] < outgoing_cap[nd])
                {
                    int to_increase = outgoing_cap[nd] - incoming_cap[nd];

                    for (int j = 0; j <= n; j++)
                    {

                        if(cap[nd][j]<0 && blocked[j]==0)
                        {
                            int parent = j ;

                            if(to_increase>0)
                            {
                                if(cap[parent][nd]-flow[parent][nd]>=to_increase)
                                {
                                    int incremented =0 ;
                                    int oldflow = flow[parent][nd];
                                    flow[parent][nd] += to_increase;
                                    to_increase=0;
                                    incremented = flow[parent][nd] - oldflow;
                                    outgoing_cap[parent]+=incremented;
                                    incoming_cap[nd]+=incremented;
                                }
                                else
                                {
                                    int incremented =0 ;
                                    int oldflow = flow[parent][nd];
                                    flow[parent][nd] = cap[parent][nd];
                                    incremented = flow[parent][nd] - oldflow;
                                    outgoing_cap[parent]+=incremented;
                                    incoming_cap[nd]+=incremented;
                                    to_increase-=incremented;
                                }
                            }
                            else break;

                        }

                    }
                    if (incoming_cap[nd] != outgoing_cap[nd]) // even now if node is not balanced we marked it blocked and unbalanced
                    {
                        blocked[nd] = 1;
                        balanced[nd]=0;
                    }
                    else balanced[nd]=1;

                }
                else if (incoming_cap[nd] > outgoing_cap[nd])
                {
                    int to_decrease = incoming_cap[nd]-outgoing_cap[nd];

                    for(int j = n ; j>=0 ;j--)
                    {

                        if(cap[nd][j]<0 && blocked[j]==0)
                        {
                            int parent = j;
                            if(to_decrease>0)
                            {
                                if(flow[parent][nd]>=to_decrease)
                                {
                                    int decrement = 0;
                                    int oldflow = flow[parent][nd];
                                    flow[parent][nd] = flow[parent][nd] - to_decrease;
                                    decrement = oldflow - flow[parent][nd];
                                    outgoing_cap[parent]-=decrement;
                                    incoming_cap[nd]-=decrement;
                                    to_decrease=0;
                                }
                                else
                                {
                                    int decrement = 0;
                                    int oldflow = flow[parent][nd];
                                    flow[parent][nd]=0;
                                    decrement = oldflow - flow[parent][nd];
                                    outgoing_cap[parent]-=decrement;
                                    incoming_cap[nd]-=decrement;
                                    to_decrease = to_decrease - decrement;
                                }
                            }
                            else break;
                        }
                    }

                    if(incoming_cap[nd]!=outgoing_cap[nd])   // even now if node is not balanced we marked it blocked and unbalanced
                    {
                        blocked[nd]=1;
                        balanced[nd]=0;
                    }
                    else
                    {
                        balanced[nd]=1;
                    }

                }
            }
            f=1;

        }

        // Check for all balance nodes

        int c = 0;
        for (int i = 1; i <= n; i++)
        {
            if (balanced[i] == 0)
            {
                c = 1;
                break;
            }
        }
        if (c == 0) is_balanced = 1;  // if all nodes are balanced we come out of loop and our job is done
    }

    int max_flow = 0 ;
    for(int i = 1 ; i <= n ; i++){
        max_flow += flow[i][sink];
    }
    cout<<endl;
    cout<<" MAXIMUM FLOW IS : " << max_flow <<endl;

}
