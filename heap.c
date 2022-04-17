//Function defintions
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"heap.h"

//Function to create free_Node.
//returns pointer to free_Node.
free_Node* free_node(int address, int size)
{
    free_Node* nptr = (free_Node*)malloc(sizeof(free_Node));
	//making free node pointer
    nptr->size = size;
    nptr->address = address;
    nptr->next = NULL;
    nptr->prev = NULL;

    return nptr;
}

//Function to create Allocated_Node.
//returns pointer to Allocated_Node.
Allocated_Node* var_node(char* name, int address, int size)
{
    Allocated_Node* nptr = (Allocated_Node*)malloc(sizeof(Allocated_Node));

    strcpy(nptr->name, name); // a pointer that would point to the allocated node
    nptr->size = size;//side of the allocated node
    nptr->address = address;//address of the allocated node
    nptr->next = NULL; 
    nptr->prev = NULL;
	//This node is a single node where the next and prev pointer points to null.
    return nptr;
}

//Function to initialize a Heap with number of bytes = size
void intializeHeap(Heap *hptr, int size)
{
    hptr->size = size;
    hptr->freelist = free_node(0, hptr->size); //Implies that the heap is initially all free.
    hptr->varlist = NULL;
}

//Function to insert "nptr" into the free list, where llptr is dereferenced to get the head of the free list
//and chage it to poit a node if the node is first in the order
void free_sorted_insert(free_Node **llptr, free_Node *nptr)
{
    free_Node *ptr, *prev_node, *head;
	head = *llptr;
	nptr->next = nptr->prev = NULL;

	if(head == NULL)	//Empty list case
	{
	    head = nptr;
	}
	else
	{
	    ptr = head;
	    while(ptr->next != NULL && ptr->address < nptr->address)	//Skip all nodes having address lesser than nptr->address
	    {
	        ptr = ptr->next;
	    }
	    
	    if(ptr->next == NULL && ptr->address < nptr->address)  	//If inserted node address is more than address of all nodes in the list
	    {
	        ptr->next = nptr;
	        nptr->prev = ptr;
	    }
	    else
	    {
	        if(ptr == head) 	//If inserted node address is less than address of all nodes in the list
	        {
			    head = nptr;
	        }    
	        prev_node = ptr->prev;

			//Joining all links
	        if(prev_node != NULL)
	        {
	            prev_node->next = nptr;
	        }
	        nptr->prev = prev_node;
	        nptr->next = ptr;
	        ptr->prev = nptr;
	    }
	}
	*llptr = head;
}

//Function to insert "nptr" into the allocated list, where llptr is dereferenced to get the head of the variable list.
void alloc_sorted_insert(Allocated_Node **llptr, Allocated_Node *nptr)
{
    Allocated_Node *ptr, *prev_node, *head;
	head = *llptr;
	nptr->next = nptr->prev = NULL;

	if(head == NULL)	//Empty list case
	{
	    head = nptr;
	}
	else
	{
	    ptr = head;
	    while(ptr->next != NULL && ptr->address < nptr->address)	//Skip all nodes having address lesser than nptr->address
	    {
	        ptr = ptr->next;
	    }
	    
	    if(ptr->next == NULL && ptr->address < nptr->address)  	//If inserted node address is more than address of all nodes in the list
	    {
	        ptr->next = nptr;
	        nptr->prev = ptr;
	    }
	    else
	    {
	        if(ptr == head) 	//If inserted node address is less than address of all nodes in the list
	        {
			    head = nptr;
	        }    
	        prev_node = ptr->prev;

			//Joining all links
	        if(prev_node != NULL)
	        {
	            prev_node->next = nptr;
	        }
	        nptr->prev = prev_node;
	        nptr->next = ptr;
	        ptr->prev = nptr;
	    }
	}
	*llptr = head;
}

//Function to delete a node "p" which is present in the free list, where lpptr is dereferenced to get the head of the free list.
//return SUCCESS or FAILURE
status_code free_deleteNode(free_Node** lpptr, free_Node* p)
{
    status_code ret_val = SUCCESS;
    free_Node *prev_node, *next_node;
    
    if(p == NULL)
    {
        ret_val = FAILURE;
    }
    else
    {
        prev_node = p->prev;
        next_node = p->next;

        if(prev_node != NULL)   //If we are not deleting the first node
        {
            prev_node->next = next_node;
        }
        else    //If we are deleting the first node
        {
            *lpptr = next_node;
        }

        if(next_node != NULL)   //If we are not deleting the last node
        {
            next_node->prev = prev_node;
        }

        free(p);
    }

    return ret_val;
}

//Function to delete a node "p" which is present in the allocated list, where lpptr is dereferenced to get the head of the variable list.
//return SUCCESS or FAILURE
status_code alloc_deleteNode(Allocated_Node** lpptr, Allocated_Node* p)
{
    status_code ret_val = SUCCESS;
    Allocated_Node *prev_node, *next_node;
    
    if(p == NULL)
    {
        ret_val = FAILURE;
    }
    else
    {
        prev_node = p->prev;
        next_node = p->next;

        if(prev_node != NULL)   //If we are not deleting the first node
        {
            prev_node->next = next_node;
        }
        else    //If we are deleting the first node
        {
            *lpptr = next_node;
        }

        if(next_node != NULL)   //If we are not deleting the last node
        {
            next_node->prev = prev_node;
        }

        free(p);
    }

    return ret_val;
}

//Function to allocate memory according to first fit strategy.
//return SUCCESS or FAILURE
status_code firstFitMalloc(Heap *hptr, char* varName, int varSize)
{
    status_code sc = SUCCESS;
    free_Node *free_lptr, *fptr;
    Allocated_Node *var_lptr, *vptr;

    free_lptr = hptr->freelist;
    fptr = free_lptr;
    var_lptr = hptr->varlist;
    vptr = var_lptr;

    int found = 0;

    while(fptr != NULL && !found)	//Search for the first fit block in the free list.
    {
        if(fptr->size >= varSize)
        {
            found = 1;
        }
        else
        {
            fptr = fptr->next;
        }
    }

    if(!found || varSize == 0)	// If not found any block of that size then, failed to allocate memory
    {
        sc = FAILURE;
    }
    else	// If found
	{
		Allocated_Node *var_nptr = var_node(varName, fptr->address, varSize);		// make a new Allocated_Node of same size and address, and given name
		alloc_sorted_insert(&var_lptr, var_nptr);	// and sorted insert into the varlist so that it finds its right place

		fptr->address = fptr->address + varSize;	//Updating the free block after partition
		fptr->size = fptr->size - varSize;

		if(fptr->size == 0)		//If size becomes 0, delete from free list
		{
			free_deleteNode(&free_lptr, fptr);
		}
	} 

    hptr->freelist = free_lptr;
    hptr->varlist = var_lptr;
    return sc;
}

//Function to free memory
//return SUCCESS or FAILURE
status_code mfree(Heap *hptr, char *varName)
{
	status_code sc = SUCCESS;
	free_Node *free_lptr, *fptr, *free_prev_node;
    Allocated_Node *var_lptr, *vptr;
	int found = 0;

    free_lptr = hptr->freelist;
    fptr = free_lptr;
    var_lptr = hptr->varlist;
    vptr = var_lptr;

	while(vptr != NULL && !found)	//Search the varlist for the Allocated_Node of given name
	{
		if(strcmp(varName, vptr->name) == 0)
		{
			found = 1;
		}
		else
		{
			vptr = vptr->next;
		}
	}

	if(!found)	//If not found, failed to free memory
	{
		sc = FAILURE;
	}
	else	//If found
	{
		free_Node *free_nptr = free_node(vptr->address, vptr->size);	// make a new free_Node of same size and address.
		free_sorted_insert(&free_lptr, free_nptr);	//and sorted insert into free list
		alloc_deleteNode(&var_lptr, vptr);	//delete the Allocated_Node from varlist

		//Merging free nodes
		free_Node *free_next_node, *free_prev_node;
		free_next_node = free_nptr->next;
		free_prev_node = free_nptr->prev;

		if(free_next_node != NULL)
		{
			if(free_nptr->address + free_nptr->size == free_next_node->address)	//If next free_Node is adjacent to the newly inserted free_Node
			{
				free_nptr->size = free_nptr->size + free_next_node->size;
				free_deleteNode(&free_lptr, free_next_node);
			}
		}
		if(free_prev_node != NULL)
		{
			if(free_prev_node->address + free_prev_node->size == free_nptr->address)	//If previous free_Node is adjacent to the newly inserted free_Node
			{
				free_prev_node->size = free_prev_node->size + free_nptr->size;
				free_deleteNode(&free_lptr, free_nptr);
			}
		}
	}

	hptr->freelist = free_lptr;
    hptr->varlist = var_lptr;
	return sc;
}

//Function to print the free list in address sorted manner.
void free_pList(free_Node *free_ptr)
{
	free_Node *ptr = free_ptr;
	printf("Free list display:\n");
	if(free_ptr == NULL)
	{
		printf("No free space\n");
	}
	else
	{
		printf("Start_Address\tEnd_Address\tSize\n");
		while(ptr != NULL)
		{
			printf("%d\t\t%d\t\t%d\n", ptr->address, ptr->address + ptr->size - 1, ptr->size);
			ptr = ptr->next;
		}
	}
	printf("\n");
}

//Function to print the variable list in address sorted manner.
void allocated_pList(Allocated_Node *var_lptr)
{
	Allocated_Node *ptr = var_lptr;
	printf("Variable list display:\n");
	if(var_lptr == NULL)
	{
		printf("No allocated space\n");
	}
	else
	{
		printf("Start_Address\tEnd_Address\tSize\tVariable_name\n");
		while(ptr != NULL)
		{
			printf("%d\t\t%d\t\t%d\t%s\n", ptr->address, ptr->address + ptr->size - 1, ptr->size, ptr->name);
			ptr = ptr->next;
		}
	}
	printf("\n");
}

//Function to print the heap blocks (Both allocated and free) in address sorted manner.
void pHeap(Heap *hptr)
{
	Allocated_Node *vptr = hptr->varlist;
	free_Node *fptr = hptr->freelist;
	printf("Total heap space = %d bytes\n", hptr->size);
	printf("Heap memory display:\n");
	printf("Start_Address\tEnd_Address\tSize\tAllocation_Status\n");
	while(vptr != NULL && fptr != NULL)
	{
		if(vptr->address < fptr->address)
		{
			printf("%d\t\t%d\t\t%d\tAllocated\n", vptr->address, vptr->address + vptr->size - 1 ,vptr->size);
			vptr = vptr->next;
		}
		else
		{
			printf("%d\t\t%d\t\t%d\tFree\n", fptr->address, fptr->address + fptr->size - 1 ,fptr->size);
			fptr = fptr->next;
		}
	}

	if(vptr != NULL)
	{
		while(vptr != NULL)
		{
			printf("%d\t\t%d\t\t%d\tAllocated\n", vptr->address, vptr->address + vptr->size - 1 ,vptr->size);
			vptr = vptr->next;
		}
	}
	else
	{
		while(fptr != NULL)
		{
			printf("%d\t\t%d\t\t%d\tFree\n", fptr->address, fptr->address + fptr->size - 1 ,fptr->size);
			fptr = fptr->next;
		}
	}
	printf("\n");
}

//Function to free both freelist and varlist completely
void freeHeap(Heap * hptr)
{
	free_Node *free_lptr = hptr->freelist;
	free_Node *fptr = free_lptr;
	Allocated_Node *var_lptr = hptr->varlist;
	Allocated_Node *vptr = var_lptr;

	while(free_lptr != NULL)
	{
		fptr = free_lptr;
		free_lptr = free_lptr->next;
		free(fptr);
	}

	while(var_lptr != NULL)
	{
		vptr = var_lptr;
		var_lptr = var_lptr->next;
		free(vptr);
	}

	hptr->freelist = NULL;
	hptr->varlist = NULL;
}