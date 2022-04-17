#define NAME_SIZE 10
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef enum{FAILURE, SUCCESS} status_code;

typedef struct free_Node_tag
{
    int address;
	int size;
    struct free_Node_tag* next;
    struct free_Node_tag* prev;
} free_Node;

typedef struct alloc_Node_tag
{
    char name[NAME_SIZE];
    int address;
	int size;
    struct alloc_Node_tag* next;
    struct alloc_Node_tag* prev;
} Allocated_Node;

typedef struct Heap_tag
{
    int size;
    free_Node* freelist;
    Allocated_Node* varlist;
} Heap;

free_Node* free_node(int address, int size);    //Creates free_Node
Allocated_Node* var_node(char* name, int address, int size);  //Creates Allocated_Node
void intializeHeap(Heap *hptr, int size);   //Initialize heap 

void free_sorted_insert(free_Node **headptr, free_Node *nptr);   //Inserts free_Node into freelist in a sorted manner
void alloc_sorted_insert(Allocated_Node **headptr, Allocated_Node *nptr);   //Inserts Allocated_Node into varlist in a sorted manner
status_code free_deleteNode(free_Node** lpptr, free_Node* p);   //Deletes Node "p" from freelist
status_code alloc_deleteNode(Allocated_Node** lpptr, Allocated_Node* p);  //Deletes Node "p" from varlist

status_code firstFitMalloc(Heap *hptr, char* varName, int varSize); //Allocates memory of size "varSize" in the heap and alots that to variable name "varName"
status_code mfree(Heap *hptr, char *varName);   //Frees memory of space pointed by the variable name "varName" 
void free_pList(free_Node *free_ptr);   //Prints the freelist
void allocated_pList(Allocated_Node *var_lptr); //Prints the varlist
void pHeap(Heap *hptr); //Prints all the heap blocks (allocated and free)
void freeHeap(Heap * hptr); //Frees both freelist and varlist completely
