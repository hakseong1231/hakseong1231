//Singly Linked List

#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp

// User structure type definition
typedef struct
{
	char	*token;
	int		freq;
} tTOKEN;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	tTOKEN		*dataPtr;
	struct node	*link;
} NODE;

typedef struct
{
	int		count;
	NODE	*pos;
	NODE	*head;
	NODE	*rear;
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST *createList( void)
{
	LIST *list = (LIST*)malloc(sizeof(LIST));
	list->count = 0;
	list->pos = NULL;
	list->head = NULL;
	list->rear = NULL;
	return list;
}

/* Deletes all data in list and recycles memory
	return	NULL head pointer
*/
LIST *destroyList( LIST *pList)
{
	NODE *cur = pList->head;
	while (cur != NULL)
	{
		NODE *next = cur->link;
		free(cur);
		cur = next;
		pList->head = cur;
	}
	return pList;
}

/* Inserts data into list
	return	-1 if overflow
			0 if successful
			1 if duplicated key
*/
int addNode( LIST *pList, tTOKEN *dataInPtr)
{
	NODE *node = (NODE*)malloc(sizeof(NODE));
	node->dataPtr = dataInPtr;
	NODE *cur = pList->head;
	if (pList->count == 0)
	{
		pList->head = node;
		pList->rear = node;
		pList->count++;
		return 0;
	}
	else if (pList->count == 1)
	{
		if (dataInPtr == cur->dataPtr)
		{
			return 1;
		}
		else if (dataInPtr < cur->dataPtr)
		{
			pList->head = node;
			node->link = cur;
			pList->count++;
			return 0;
		}
		else
		{
			pList->rear = node;
			cur->link = node;
			pList->count++;
			return 0;
		}
	}
	return 1;
}

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode( LIST *pList, char *keyPtr, tTOKEN **dataOut)
{
	if (pList->count == 0)
	{
		return 0;
	}
	NODE *cur = pList->head;
	if (*cur->dataPtr->token == *keyPtr)
	{
		*dataOut = cur;
		pList->head = cur->link;
		free(cur);
		pList->count--;
		return 1;
	}
	while (*cur->link->dataPtr->token < *keyPtr)
	{
		cur = cur->link;
		if (cur->link == NULL)
		{
			return 0;
		}
	}
	*dataOut = cur->dataPtr;
	NODE *next = cur->link;
	cur->link = next->link;
	free(next);
	pList->count--;
	return 1;
}

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList( LIST *pList, char *pArgu, tTOKEN **pDataOut)
{
	if (pList->count == 0)
	{
		return 0;
	}
	NODE *cur = pList->head;
	while (cur->dataPtr->token != pArgu)
	{
		cur = cur->link;
		if (cur->link == NULL)
		{
			return 0;
		}
	}
	*pDataOut = cur;
	return 1;
}

/* returns number of nodes in list
*/
int listCount( LIST *pList)
{
	if (pList->head == NULL)
	{
		return 0;
	}
	else if (pList->head == pList->rear)
	{
		return 1;
	}
	NODE *cur = pList->head;
	int count = 1;
	while (cur->link != NULL)
	{
		cur = cur->link;
		count++;
	}
	return count;
}

/* returns	1 empty
			0 list has data
*/
int emptyList( LIST *pList)
{
	if (pList->head != NULL)
	{
		return 0;
	}
	return 1;
}

//int fullList( LIST *pList);

/* prints data from list
*/
void printList( LIST *pList)
{
	NODE *cur = pList->head;
	while (cur != NULL)
	{
		printf(cur->dataPtr->token, "t");
		cur = cur->link;
	}
	printf("\n");
}

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, tTOKEN *dataInPtr)
{
	NODE *cur = pList->head;
	NODE *node = (NODE*)malloc(sizeof(NODE));
	node->dataPtr = dataInPtr;
	while (cur->link == NULL||cur->link->dataPtr->token < dataInPtr->token)
	{
		if (cur->link == NULL)
		{
			cur->link = node;
			node->link = pList->rear;
			node->dataPtr->freq++;
			return 0;
		}
		cur = cur->link;
	}
	if (cur->dataPtr->token == dataInPtr->token)
	{
		cur->dataPtr->freq++;
		return 1;
	}
	node->link = cur->link;
	cur->link = node;
	node->dataPtr->freq++;
	return 0;
}

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr)
{
	NODE *cur = pList->head;
	while (cur->link != pPre)
	{
		if (cur->link == NULL)
		{
			return;
		}
		cur = cur->link;
	}
	cur->link = cur->link->link;
	*dataOutPtr = cur->link->dataPtr;
	free(cur->link);
	return;
}

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu)
{
	NODE *cur = pList->head;
	while (cur->link != *pPre)
	{
		cur = cur->link;
		if (cur->link == NULL)
		{
			return 0;
		}
	}
	pArgu = cur->link->dataPtr->token;
	return 1;
}

/* Allocates dynamic memory for a token structure, initialize fields(token, freq) and returns its address to caller
	return	token structure pointer
			NULL if overflow
*/
tTOKEN *createToken( char *str)
{
	tTOKEN *token = (tTOKEN*)malloc(sizeof(tTOKEN));
	token->freq = 0;
	token->token = NULL;
	return token;
}

/* Deletes all data in token structure and recycles memory
	return	NULL head pointer
*/
tTOKEN *destroyToken( tTOKEN *pToken)
{
	free(pToken);
	return pToken;
}

////////////////////////////////////////////////////////////////////////////////
int main( void)
{
	LIST *list;
	char str[1024];
	tTOKEN *pToken;
	int ret;

	// creates a null list
	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}

	while(scanf( "%s", str) == 1)
	{
		pToken = createToken( str);

		// insert function call
		ret = addNode( list, pToken);

		if (ret == 1) // duplicated
			destroyToken( pToken);
	}
	// print function call
	printList( list);

	destroyList( list);

	return 0;
}
