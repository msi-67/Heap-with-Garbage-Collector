#include<stdio.h>
#include<stdlib.h>
#define src 11
#define des 11
#define SIZE 100
int arr[src][des];
int arr2[src][des];
    
typedef struct node
{
    unsigned int refcount;
    int value;
    struct node* pointers[SIZE];
}Node;

typedef struct node_2
{
    Node* next;
}Root;

Node* initialise_Node(int value)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->refcount = 0;
    node->value = value;
    for (int i = 0; i < SIZE; i++)
    {
        node->pointers[i] = NULL;
    }
    return node;
}

Root* initialise_root(Node* node)
{
    Root* root1 = (Root*)malloc(sizeof(Root));
    root1->next = node;
    node->refcount++;
    return root1;
}

void inc_ref(Node **node)
{
    Node *ptr = *node;
    if (ptr != NULL)
    {
        ptr->refcount += 1;
        *node = ptr;
    }
}

void Create_ref(Node** node1, Node** node2)
{
    int i = 0;
    Node *ptr = *node1;
    Node* ptr2 = *node2;
    while (i < SIZE && ptr->pointers[i] != NULL)
    {
        i++;
    }
    ptr->pointers[i] = *node2;
    *node1 = ptr;
    inc_ref(node2);   
} 

void dec_ref(Node *node)
{
    int i = 0;
    if (node != NULL)
    {
        if (node->refcount >= 1)
        {
            node->refcount -= 1;
        }
        else
        {
            while (i < SIZE && node->pointers[i] != NULL)
            {
                dec_ref(node->pointers[i]);
                i += 1;
            }
        }
    }
}

void Adjacency_matrix(Node* ptr, int arr[11][11])
{
    for (int i = 0; ptr->pointers[i] != NULL; i++)
    {
        arr[ptr->value][ptr->pointers[i]->value] = 1;
        printf("%d %d\n",ptr->value,ptr->pointers[i]->value);
        Adjacency_matrix(ptr->pointers[i], arr);
    }   
}

void remove_ref(Node* node1, Node* node2)
{
    dec_ref(node2);
}

void print_ref(Node *node[8], int garbage[8])
{
    int yes = 0;
    for (int i = 0; i < 8; i++)
    {
        yes = 0;
        for (int j = 1; j <= garbage[0]; j++)
        {
            if (garbage[j] == i)
            {
                yes = 1;
                printf("This node is garbage.\n");
                free(node[i]);
            }
        }
        if (yes == 0)
        {
            printf("The node with value %d has reference count %d.\n",node[i]->value,node[i]->refcount);
        }
    }
}

void run_gc(Node* n[8], int garbage[8])
{
    int j = 1, count = 0;
    for (int i = 0; i < 8; i++)
    {
        if (n[i]->refcount == 0)
        {
            garbage[j] = i;
            dec_ref(n[i]);
            j++;
            count++;
        }
    }
    garbage[0] = count;
}

int main()
{
    int garbage[8];
    int garbage2[8] = {0};
    Node *n[8];
    Root *root1, *root2;
    n[0] = initialise_Node(5);
    n[1] = initialise_Node(1);
    root1 = initialise_root(n[0]);
    n[2] = initialise_Node(7);
    n[3] = initialise_Node(3);
    n[4] = initialise_Node(8);
    n[5] = initialise_Node(2);
    n[6] = initialise_Node(9);
    n[7] = initialise_Node(10);
    Create_ref(&n[0], &n[1]);
    Create_ref(&n[2], &n[1]);
    Create_ref(&n[2], &n[4]);
    Create_ref(&n[3], &n[4]);
    Create_ref(&n[3], &n[7]);
    Create_ref(&n[1], &n[5]);
    Create_ref(&n[1], &n[6]);
    Create_ref(&n[1], &n[7]);
    Create_ref(&n[4], &n[6]);
    printf("\n FOR ROOT 1:\n");
    printf("\nAdjacency matrix is : \n");
    Adjacency_matrix(root1->next,arr);
    
    for (int i = 1; i < src; i++)
    {
        for (int j = 1; j < des; j++)
        {
            printf("%d ",arr[i][j]);
        }
        printf("\n");
    }
    print_ref(n,garbage); 
    printf("\nAFTER RUNNING GARBAGE COLLECTOR ONCE.\n");
    run_gc(n, garbage);
    print_ref(n, garbage);
    n[0] = initialise_Node(5);
    n[1] = initialise_Node(1);
    root2 = initialise_root(n[1]);
    n[2] = initialise_Node(7);
    n[3] = initialise_Node(3);
    n[4] = initialise_Node(8);
    n[5] = initialise_Node(2);
    n[6] = initialise_Node(9);
    n[7] = initialise_Node(10);
    Create_ref(&n[0], &n[1]);
    Create_ref(&n[2], &n[1]);
    Create_ref(&n[2], &n[4]);
    Create_ref(&n[3], &n[4]);
    Create_ref(&n[3], &n[7]);
    Create_ref(&n[1], &n[5]);
    Create_ref(&n[1], &n[6]);
    Create_ref(&n[1], &n[7]);
    Create_ref(&n[4], &n[6]);
    printf("\n FOR ROOT 2:\n");
    print_ref(n,garbage2); 
    printf("\nAdjacency matrix is : \n");
    Adjacency_matrix(root2->next,arr2);
    for (int i = 1; i < src; i++)
    {
        for (int j = 1; j < des; j++)
        {
            printf("%d ",arr2[i][j]);
        }
        printf("\n");
    }
    printf("\nAFTER RUNNING GARBAGE COLLECTOR ONCE.\n");
    run_gc(n, garbage2);
    print_ref(n, garbage2);
}