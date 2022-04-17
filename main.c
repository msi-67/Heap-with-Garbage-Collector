#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"heap.h"

int main()
{
	int c;
    Heap myHeap;
    intializeHeap(&myHeap, 200);

	char varName[NAME_SIZE];
	int size;
	status_code sc;
	c = 1;
	while(c != 0)
	{
		printf("\n___________________________________________________________________________________________________________\n");
		printf("0					//Exit terminal. For Ex: \"0\"\n");
		printf("1					//Print Heap and lists. For Ex: \"1\"\n");
		printf("2 [VariableName] [Size]			//Memory allocation with first fit strategy. For Ex: \"2 a 40\"\n");
		printf("3 [VariableName]			//Memory free. For Ex: \"3 a\"\n");
		printf("\nEnter the proper syntax for the function you want to execute : ");
		scanf(" %d",&c);
		printf("\n");
		switch(c) 
		{
			case 0:		//Exit terminal
			{
				printf("Terminal exited\n");
				break;
			}
			case 1:		//Print Heap, free list and allocated list
			{
				pHeap(&myHeap);
				free_pList(myHeap.freelist);
				allocated_pList(myHeap.varlist);
				break;
			}
			case 2:		//Memory allocation with first fit strategy
			{
				scanf("%s", varName);
				scanf("%d", &size);
				sc = firstFitMalloc(&myHeap, varName, size);
				if(sc == SUCCESS)
					printf("Malloc Successful\n");
				else
					printf("Malloc unsuccessful\n");
				break;
			}
			case 3:		//Memory free
			{
				scanf("%s", varName);
				sc = mfree(&myHeap, varName);
				if(sc == SUCCESS)
					printf("Memory free Successful\n");
				else
					printf("Memory free unsuccessful\n");
				break;
			}
			default:
			{
				printf("Syntax error\n");
				break;
			}
		}
	}
	freeHeap(&myHeap);
    return 0;
}