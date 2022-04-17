#include<stdio.h>
#include<stdlib.h>
#define SIZE 100
typedef enum {false, true} bool;
typedef struct node
{
    bool mark;
    int value;
    struct node* pointers[SIZE];
    int fields;
}Node;

typedef struct node_2
{
    Node* next;
}Root;

Node* initialise_Node(int value)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->mark = false;
    node->value = value;
    node->fields = 0;
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
    return root1;
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
    ptr->fields += 1;
    ptr->pointers[i] = *node2;
    *node1 = ptr;   
} 

void mark_node(Node *ptr)
{
    int done = 0;
    int ind = 0;
    Node* prev = NULL;
    Node *curr = ptr;
    while (!done)
    {
        ptr->mark = true;
        if (ptr->pointers[0] != NULL && ind < ptr->fields)
        {
            curr = ptr->pointers[ind];
            ptr->pointers[ind] = prev;
            prev = ptr;
            ptr = ptr->pointers[ind];
            ptr = curr;
        }
        else
        {
            Node *temp = prev;
            prev = prev->pointers[ind-1];
            temp->pointers[ind] = curr;
            curr = temp;
            ptr = curr;
            ind++;
            if (prev == NULL && ind > ptr->fields)
            {
                done = 1;
            }
        }
    }
}

void print_ref(Node *node[8])
{
    for (int i = 0; i < 8; i++)
    {
        printf("The node has value %d.has mark %d\n",node[i]->value, node[i]->mark);
    }
}


void sweep_node(Node *n[8])
{
    for (int i = 0; i < 8; i++)
    {
        if (n[i]->mark == 0)
        {
            printf("node with value %d is freed.\n", n[i]->value);
            free(n[i]);
        }
    }
}

int main()
{
      Node *n[8], *n1[8];
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
    print_ref(n);
    mark_node(root1->next);
    //print_ref(n);
    sweep_node(n);

    n1[0] = initialise_Node(5);
    n1[1] = initialise_Node(1);
    root2 = initialise_root(n1[1]);
    n1[1]->mark = true;
    n1[2] = initialise_Node(7);
    n1[3] = initialise_Node(3);
    n1[4] = initialise_Node(8);
    n1[5] = initialise_Node(2);
    n1[6] = initialise_Node(9);
    n1[7] = initialise_Node(10);
    Create_ref(&n1[0], &n1[1]);
    Create_ref(&n1[2], &n1[1]);
    Create_ref(&n1[2], &n1[4]);
    Create_ref(&n1[3], &n1[4]);
    Create_ref(&n1[3], &n1[7]);
    Create_ref(&n1[1], &n1[5]);
    Create_ref(&n1[1], &n1[6]);
    Create_ref(&n1[1], &n1[7]);
    Create_ref(&n1[4], &n1[6]);
    print_ref(n1);
    printf("%d\n",root2->next->fields);
    mark_node(root2->next);
    sweep_node(n1);
}