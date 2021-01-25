/*
	CSC D84 - Unit 1 - Search

	This file contains stubs for implementing the different search
	algorithms covered in the course. Please read the assignment
	handout carefully - it describes the game, the data you will
	have to handle, and the search functions you must provide.

	Once you have read the handout carefully, implement your search
	code in the sections below marked with

	**************
	*** TO DO:
	**************

	Make sure to add it to your report.txt file - it will be marked!

	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E., Jul. 15
	Updated: F.J.E., Jan. 18
*/

/**********************************************************************
% COMPLETE THIS TEXT BOX:
%
% 1) Student Name:		
% 2) Student Name:		
%
% 1) Student number:
% 2) Student number:
% 
% 1) UtorID
% 2) UtorID
% 
% We hereby certify that the work contained here is our own
%
% ____________________             _____________________
% (sign with your name)            (sign with your name)
***********************************************************************/

#include "AI_search.h"

/* lines 49 to 115; queue implementation taken from https://www.geeksforgeeks.org/queue-linked-list-implementation/ 
   adaptions made to also make a stack work out of the same structures*/
struct Node
{
	int x;
	int y;
	struct Node *next;
};

// The queue, front stores the front node of LL and rear stores the
// last node of LL
struct Queue
{
	struct Node *front, *rear;
};

// A utility function to create a new linked list node.
struct Node *newNode(int x, int y)
{
	struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
	temp->x = x;
	temp->y = y;
	temp->next = NULL;
	return temp;
}

// A utility function to create an empty queue
struct Queue *createQ()
{
	struct Queue *q = (struct Queue *)malloc(sizeof(struct Queue));
	q->front = q->rear = NULL;
	return q;
}

// The function to add a key k to q
void enQ(struct Queue *q, int x, int y)
{
	// Create a new LL node
	struct Node *temp = newNode(x, y);

	// If queue is empty, then new node is front and rear both
	if (q->rear == NULL)
	{
		q->front = q->rear = temp;
		return;
	}

	// Add the new node at the end of queue and change rear
	q->rear->next = temp;
	q->rear = temp;
}

// Function to remove a key from given queue q
struct Node *deQ(struct Queue *q)
{
	// If queue is empty, return NULL.
	if (q->front == NULL)
		return (NULL);

	// Store previous front and move front one node ahead
	struct Node *result = q->front;

	// free(q->front);
	q->front = q->front->next;
	// If front becomes NULL, then change rear also as NULL
	if (q->front == NULL)
		q->rear = NULL;

	return (result);
}

struct Stack
{
	struct Node *top;
};

struct Stack *createStack()
{
	struct Stack *s = (struct Stack *)malloc(sizeof(struct Stack));
	s->top = NULL;
	return s;
}

void push(struct Stack *s, int x, int y)
{
	// Create a new LL node
	struct Node *temp = newNode(x, y);
	temp->next = s->top;
	s->top = temp;
}

struct Node *pop(struct Stack *s) {
	if (s->top == NULL) {
		return (NULL);
	} else {
		struct Node *result = s->top;
		s->top = s->top->next;
		return (result);
	}
};

int cat_exists(int cat_loc[10][2], int cats, int x, int y) {
	int i;
	for (i = 0; i < cats; i++) {
		if ((cat_loc[i][0] == x) && (cat_loc[i][1] == y)) {
			return 1;
		}
	}
	return 0;
}

int cheese_exists(int cheese_loc[10][2], int cheeses, int x, int y) {
	int i;
	for (i = 0; i < cheeses; i++) {
		if ((cheese_loc[i][0] == x) && (cheese_loc[i][1] == y)) {
			return 1;
		}
	}
	return 0;
}

void bfs(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2]) {
	// create the predecessros array and set ii all to -1
	int predecessors[graph_size];
	int i;
	for (i = 0; i < graph_size; i++) {
		predecessors[i] = -1;
	}

	// create and enqueue the mouse's location
	struct Queue *Q = createQ();
	enQ(Q, mouse_loc[0][0], mouse_loc[0][1]);
	predecessors[mouse_loc[0][0] + (mouse_loc[0][1] * size_X)] = mouse_loc[0][0] + (mouse_loc[0][1] * size_X);

	// declare variables for the loop
	int order_counter = 1;
	struct Node *cur_node;
	int child_index;
	int current_index = 0;
	int cheese_index[2] = {-99, -99};

	// loop through the queue
	while (cheese_index[0] == -99)
	{
		// dequeue the next node
		cur_node = deQ(Q);
		if (cur_node == NULL) {
			break;
		}

		// check if the current node is a cheese node
		if (cheese_exists(cheese_loc, cheeses, cur_node->x, cur_node->y) == 1) {
			cheese_index[0] = cur_node->x;
			cheese_index[1] = cur_node->y;
		}

		current_index = cur_node->x + ((cur_node->y) * size_X);
		// mark node as visited and set its predecessor
		visit_order[cur_node->x][cur_node->y] = order_counter;
		order_counter++;
		// now go through the 4 children and add them to the queue if they qualify

		// top neighbour
		child_index = cur_node->x + ((cur_node->y - 1) * size_X);
		if ((gr[current_index][0] == 1.0) && ((predecessors[child_index] == -1) && (cat_exists(cat_loc, cats, cur_node->x, (cur_node->y - 1)) == 0))) {
			predecessors[child_index] = current_index;
			enQ(Q, cur_node->x, (cur_node->y - 1));
		}

		// right neighbour
		child_index = (cur_node->x + 1) + ((cur_node->y) * size_X);
		if ((gr[current_index][1] == 1.0) && ((predecessors[child_index] == -1) && (cat_exists(cat_loc, cats, cur_node->x + 1, (cur_node->y)) == 0))) {
			predecessors[child_index] = current_index;
			enQ(Q, cur_node->x + 1, (cur_node->y));
		}

		// bottom neighbour
		child_index = cur_node->x + ((cur_node->y + 1) * size_X);
		if ((gr[current_index][2] == 1.0) && ((predecessors[child_index] == -1) && (cat_exists(cat_loc, cats, cur_node->x, (cur_node->y + 1)) == 0))) {
			predecessors[child_index] = current_index;
			enQ(Q, cur_node->x, (cur_node->y + 1));
		}

		// left neighbour
		child_index = (cur_node->x - 1) + (cur_node->y) * 32;
		if ((gr[current_index][3] == 1.0) && ((predecessors[child_index] == -1) && (cat_exists(cat_loc, cats, cur_node->x - 1, (cur_node->y)) == 0))) {
			predecessors[child_index] = current_index;
			enQ(Q, cur_node->x - 1, (cur_node->y));
		}

		free(cur_node);
	}

	// now build the path in reverse from the cheese to the mouse
	int reverse_path[graph_size];
	int mouse = mouse_loc[0][0] + mouse_loc[0][1] * 32;
	int counter = 0;
	reverse_path[0] = cheese_index[0] + cheese_index[1] * 32;
	// first loop to build the path backwards
	while (reverse_path[counter] != mouse) {
		reverse_path[counter + 1] = predecessors[reverse_path[counter]];
		counter++;
	}
	// second loop to flip the reverse path and change from linear index to coordinates along the way
	int j = 0;
	for (counter; counter >= 0; counter--) {
		path[j][0] = reverse_path[counter] % size_X;
		path[j][1] = reverse_path[counter] / size_X;
		j++;
	}
	return;
}

void dfs(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2]) {
	// create the predecessros array and set ii all to -1
	int predecessors[graph_size];
	int i;
	for (i = 0; i < graph_size; i++) {
		predecessors[i] = -1;
	}

	// create and enqueue the mouse's location
	struct Stack *stack = createStack();
	push(stack, mouse_loc[0][0], mouse_loc[0][1]);
	predecessors[mouse_loc[0][0] + (mouse_loc[0][1] * size_X)] = mouse_loc[0][0] + (mouse_loc[0][1] * size_X);

	// declare variables for the loop
	int order_counter = 1;
	struct Node *cur_node;
	int child_index;
	int current_index = 0;
	int cheese_index[2] = {-99, -99};

	// loop through the queue
	while (cheese_index[0] == -99) {
		// dequeue the next node
		cur_node = pop(stack);
		if (cur_node == NULL) {
			break;
		}

		// check if the current node is a cheese node
		if (cheese_exists(cheese_loc, cheeses, cur_node->x, cur_node->y) == 1) {
			cheese_index[0] = cur_node->x;
			cheese_index[1] = cur_node->y;
		}

		current_index = cur_node->x + ((cur_node->y) * size_X);
		// mark node as visited and set its predecessor
		visit_order[cur_node->x][cur_node->y] = order_counter;
		order_counter++;
		// now go through the 4 children and add them to the queue if they qualify

		// top neighbour
		child_index = cur_node->x + ((cur_node->y - 1) * size_X);
		if ((gr[current_index][0] == 1.0) && ((predecessors[child_index] == -1) && (cat_exists(cat_loc, cats, cur_node->x, (cur_node->y - 1)) == 0))) {
			predecessors[child_index] = current_index;
			push(stack, cur_node->x, (cur_node->y - 1));
		}

		// right neighbour
		child_index = (cur_node->x + 1) + ((cur_node->y) * size_X);
		if ((gr[current_index][1] == 1.0) && ((predecessors[child_index] == -1) && (cat_exists(cat_loc, cats, cur_node->x + 1, (cur_node->y)) == 0))) {
			predecessors[child_index] = current_index;
			push(stack, cur_node->x + 1, (cur_node->y));
		}

		// bottom neighbour
		child_index = cur_node->x + ((cur_node->y + 1) * size_X);
		if ((gr[current_index][2] == 1.0) && ((predecessors[child_index] == -1) && (cat_exists(cat_loc, cats, cur_node->x, (cur_node->y + 1)) == 0))) {
			predecessors[child_index] = current_index;
			push(stack, cur_node->x, (cur_node->y + 1));
		}

		// left neighbour
		child_index = (cur_node->x - 1) + (cur_node->y) * 32;
		if ((gr[current_index][3] == 1.0) && ((predecessors[child_index] == -1) && (cat_exists(cat_loc, cats, cur_node->x - 1, (cur_node->y)) == 0))) {
			predecessors[child_index] = current_index;
			push(stack, cur_node->x - 1, (cur_node->y));
		}

		free(cur_node);
	}

	// now build the path in reverse from the cheese to the mouse
	int reverse_path[graph_size];
	int mouse = mouse_loc[0][0] + mouse_loc[0][1] * 32;
	int counter = 0;
	reverse_path[0] = cheese_index[0] + cheese_index[1] * 32;
	// first loop to build the path backwards
	while (reverse_path[counter] != mouse) {
		reverse_path[counter + 1] = predecessors[reverse_path[counter]];
		counter++;
	}
	// second loop to flip the reverse path and change from linear index to coordinates along the way
	int j = 0;
	for (counter; counter >= 0; counter--) {
		path[j][0] = reverse_path[counter] % size_X;
		path[j][1] = reverse_path[counter] / size_X;
		j++;
	}
	return;
}


struct PQNode { 
    int data; 
 
    // Lower values indicate higher priority 
    int priority; 
    struct PQNode* next;
}; 
 
// Function to Create A New Node 
struct PQNode* newPQNode(int d, int p) 
{ 
    struct PQNode* temp =(struct PQNode*)malloc(sizeof(struct PQNode)); 
    temp->data = d; 
    temp->priority = p; 
    temp->next = NULL; 
 
    return temp; 
} 
 
// Return the value at head 
int peek(struct PQNode** head) 
{ 
    return (*head)->data; 
} 
 
// Removes the element with the 
// highest priority form the list 
void PQpop(struct PQNode** head) 
{ 
    struct PQNode* temp = *head; 
    (*head) = (*head)->next; 
    free(temp); 
} 
 
// Function to push according to priority 
void PQpush(struct PQNode** head, int d, int p) 
{ 
    struct PQNode* start = (*head); 
 
    // Create new Node 
    struct PQNode* temp = newPQNode(d, p); 
 
    // Special Case: The head of list has lesser 
    // priority than new node. So insert new 
    // node before head node and change head node. 
    if ((*head)->priority > p) { 
 
        // Insert New Node before head 
        temp->next = *head; 
        (*head) = temp; 
    } 
    else { 
 
        // Traverse the list and find a 
        // position to insert new node 
        while (start->next != NULL && start->next->priority < p) { 
            start = start->next; 
        } 
 
        // Either at the ends of the list 
        // or at required position 
        temp->next = start->next; 
        start->next = temp; 
    } 
} 
 
// Function to check is list is empty 
int isEmpty(struct PQNode** head) 
{ 
    return (*head) == NULL; 
} 


void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]))
{
	/*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in AI_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:
	
	The game takes place on a grid of size 32x32, the file board_layout.h specifies the size and defines two
	constants 'size_X' and 'size_Y' for the horizontal and vertical size of the board, respectively. For our
	purposes, the grid of locations is represented by a graph with one node per grid location, so given
	the 32x32 cells, the graph has 1024 nodes. 

	To create a maze, we connect cell locations in the grid in such a way that a) there is a path from any
	grid location to any other grid location (i.e. there are no disconnected subsets of nodes in the graph),
	and b) there are loops.

	Since each node represents a grid location, each node can be connected to up to 4 neighbours in the
	top, right, bottom, and left directions respectively:

			node at (i,j-1)
				^
				|
	(node at i-1, j) <- node at (i,j) -> node at (i+1, j)
				|
				v
			node at (i,j+1)

	The graph is theredore stored as an adjacency list with size 1024 x 4, with one row per node in the
	graph, and 4 columns corresponding to the weight of an edge linking the node with each of its 4
	possible neighbours in the order towp, right, bottom, left (clockwise from top).

	Since all we care is whether nodes are connected. Weights will be either 0 or 1, if the weight is
	1, then the neighbouring nodes are connected, if the weight is 0, they are not. For example, if

	graph[i][0] = 0 
	graph[i][1] = 1
	graph[i][2] = 0
	graph[i][3] = 1

	then node i is connected to the right and left neighbours, but not to top or bottom.

	The index in the graph for the node corresponding to grid location (x,y) is

	index = x + (y*size_X) 		or in this case		index = x + (y*32)

	Conversely, if you have the index and want to figure out the grid location,

	x = index % size_X		or in this case		x = index % 32
	y = index / size_Y		or in this case		y = index / 32

	(all of the above are *integer* operations!)

	A path is a sequence of (x,y) grid locations. We store it using an array of size	
	1024 x 2 (since there are 1024 locations, this is the maximum length of any
	path that visits locations only once).

	Agent locations are coordinate pairs (x,y)	

   Arguments:
		gr[graph_size][4]   - This is an adjacency list for the maze
		path[graph_size][2] - An initially empty path for your code to fill.
				      In this case, empty means all entries are initially -1
		visit_order[size_X][size_Y]	- An array in which your code will store the
						  *order* in which grid locations were 
						  visited during search. For example, while
						  doing BFS, the initial location is the
						  start location, it's visit order is 1.
						  Then the search would expand the immediate
						  neighbours of the initial node in some order,
						  these would get a visit order of 2, 3, 4, and
						  5 respectively, and so on.
					
						  This array will be used to display the search
						  pattern generated by each search method.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- BFS
					mode = 1	- DFS
					mode = 2	- A*

		(*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[10][2], int cats, int cheeses)
				- This is a pointer to one of the heuristic functions you will implement, either H_cost()
				  or H_cost_nokitty(). The driver in AI_search_core_GL will pass the appropriate pointer
				  depending on what search the user wants to run.
			
				  If the mode is 0 or 1, this pointer is NULL

				  * How to call the heuristic function from within this function : *
					- Like any other function:
						h = heuristic( x, y, cat_loc, cheese_loc, mouse_loc, cats, cheeses);

   Return values:
		Your search code will directly update data passed-in as arguments:

		- path[graph_size][2]	: Your search code will update this array to contain the path from
					  the mouse to one of the cheese chunks. The order matters, so
					  path[0][:] must be the mouse's current location, path[1][:]
					  is the next move for the mouse. Each successive row will contain
					  the next move toward the cheese, and the path ends at a location
					  whose coordinates correspond to one of the cheese chunks.
					  Any entries beyond that must remain set to -1
		- visit_order[size_X][size_Y] 	:  Your search code will update this array to contain the
						   order in which each location in the grid was expanded
						   during search. This means, when that particular location
						   was checked for being a goal, and if not a goal, had its
						   neighbours added as candidates for future expansion (in
						   whatever order is dictated by the search mode).

						   Note that since there are 1024 locations, the values in 
						   this array must always be in [0, 1023]. The driver code
						   will then display search order as a yellow-colored
						   brightness map where nodes expanded earlier will look
						   brighter.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

	/********************************************************************************************************
 * 
 * TO DO:	Implement code to carry out the different types of search depending on the
 *		mode.
 *
 *		You can do this by writing code within this single function (and being clever about it!)
 *		Or, you can use this function as a wrapper that calls a different search function
 *		 (BFS, DFS, A*) depending on the mode. Note that in the latter case, you will have
 *	    	  to inform your A* function somehow of what heuristic it's supposed to use.
 *
 *		Visiting Order: When adding the neighbours of a node to your list of candidates for
 *				expansion, do so in the order top, right, bottom, left.
 *
 *		NOTE: Your search functions should be smart enough to not choose a path that goes
 *		      through a cat! this is easily done without any heuristics.
 * 
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/

	// Stub so that the code compiles/runs - The code below will be removed and replaced by your code!

	path[0][0] = mouse_loc[0][0];
	path[0][1] = mouse_loc[0][1];
	path[1][0] = mouse_loc[0][0];
	path[1][1] = mouse_loc[0][1];

	// call the right function based on the mode
	if (mode == 0)
	{
		bfs(gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc);
	}
	else if (mode == 1)
	{
		dfs(gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc);
	}
	else if (mode == 2)
	{
		//  implementing A* here
 
		// Create a Priority Queue 
		// 7->4->5->6 
		// struct PQNode* pq = newPQNode(4, 1); 
		// PQpush(&pq, 5, 2); 
		// PQpush(&pq, 6, 3); 
		// PQpush(&pq, 7, 0);
	
		// while (!isEmpty(&pq)) { 
		// 	printf("%d ", peek(&pq)); 
		// 	PQpop(&pq); 
		// } 
		// printf("end");


		// create the predecessros array and include cost to get to predecessor in the second index 
		// set it all to -1 and 0 respectively
		int predecessors[graph_size][2];
		int i;
		for (i = 0; i < graph_size; i++)
		{
			predecessors[i][0] = -1;
			predecessors[i][1] = 0;
		}

		// create and enqueue the mouse's location
		struct Queue *Q = createQ();
		enQ(Q, mouse_loc[0][0], mouse_loc[0][1]);
		predecessors[mouse_loc[0][0] + (mouse_loc[0][1] * size_X)][0] = mouse_loc[0][0] + (mouse_loc[0][1] * size_X);
		predecessors[mouse_loc[0][0] + (mouse_loc[0][1] * size_X)][1] = 0;

		// declare variables for the loop
		int order_counter = 1;
		struct Node *cur_node;
		int child_index;
		int current_index = 0;
		int cheese_index[2] = {-99, -99};

		// loop through the queue
		while (cheese_index[0] == -99)
		{
			// dequeue the next node
			cur_node = deQ(Q);
			if (cur_node == NULL)
			{
				break;
			}

			// check if the current node is a cheese node
			if (cheese_exists(cheese_loc, cheeses, cur_node->x, cur_node->y) == 1)
			{
				cheese_index[0] = cur_node->x;
				cheese_index[1] = cur_node->y;
			}

			current_index = cur_node->x + ((cur_node->y) * size_X);
			// mark node as visited and set its predecessor
			visit_order[cur_node->x][cur_node->y] = order_counter;
			order_counter++;
			// now go through the 4 children and add them to the queue if they qualify

			// top neighbour
			child_index = cur_node->x + ((cur_node->y - 1) * size_X);
			if ((gr[current_index][0] == 1.0) && ((predecessors[child_index][0] == -1) && (cat_exists(cat_loc, cats, cur_node->x, (cur_node->y - 1)) == 0))) {
				predecessors[child_index][0] = current_index;
				predecessors[child_index][1] = predecessors[current_index][1] + 1;
				//h = heuristic(cur_node->x, (cur_node->y - 1), cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
				//enPQ(&PQ, predecessors[child_index][1] + h, cur_node->x, (cur_node->y - 1));
				// enPQ(PQ, h, cur_node->x, (cur_node->y - 1));
				enQ(Q, cur_node->x, (cur_node->y - 1));
			}

			// right neighbour
			child_index = (cur_node->x + 1) + ((cur_node->y) * size_X);
			if ((gr[current_index][1] == 1.0) && ((predecessors[child_index][0] == -1) && (cat_exists(cat_loc, cats, cur_node->x + 1, (cur_node->y)) == 0))) {
				predecessors[child_index][0] = current_index;
				predecessors[child_index][1] = predecessors[current_index][1] + 1;
				//h = heuristic(cur_node->x + 1, cur_node->y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
				//enPQ(&PQ, predecessors[child_index][1] + h, cur_node->x + 1, (cur_node->y));
				// enPQ(PQ, h, cur_node->x + 1, (cur_node->y));
				enQ(Q, cur_node->x + 1, (cur_node->y));

			}

			// bottom neighbour
			child_index = cur_node->x + ((cur_node->y + 1) * size_X);
			if ((gr[current_index][2] == 1.0) && ((predecessors[child_index][0] == -1) && (cat_exists(cat_loc, cats, cur_node->x, (cur_node->y + 1)) == 0))) {
				predecessors[child_index][0] = current_index;
				predecessors[child_index][1] = predecessors[current_index][1] + 1;
				//h = heuristic(cur_node->x, (cur_node->y + 1), cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
				//enPQ(&PQ, predecessors[child_index][1] + h, cur_node->x, (cur_node->y + 1));
				// enPQ(PQ, h, cur_node->x, (cur_node->y + 1));
				enQ(Q, cur_node->x, (cur_node->y + 1));
			}

			// left neighbour
			child_index = (cur_node->x - 1) + (cur_node->y) * 32;
			if ((gr[current_index][3] == 1.0) && ((predecessors[child_index][0] == -1) && (cat_exists(cat_loc, cats, cur_node->x - 1, (cur_node->y)) == 0))) {
				predecessors[child_index][0] = current_index;
				predecessors[child_index][1] = predecessors[current_index][1] + 1;
				//h = heuristic(cur_node->x - 1, cur_node->y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
				//enPQ(&PQ, predecessors[child_index][1] + h, cur_node->x - 1, (cur_node->y));
				// enPQ(PQ, h, cur_node->x - 1, (cur_node->y));
				enQ(Q, cur_node->x - 1, (cur_node->y));
			}

			free(cur_node);
		}

		// now build the path in reverse from the cheese to the mouse
		int reverse_path[graph_size];
		int mouse = mouse_loc[0][0] + mouse_loc[0][1] * 32;
		int counter = 0;
		reverse_path[0] = cheese_index[0] + cheese_index[1] * 32;
		// first loop to build the path backwards
		while (reverse_path[counter] != mouse)
		{
			reverse_path[counter + 1] = predecessors[reverse_path[counter]][0];
			counter++;
		}
		// second loop to flip the reverse path and change from linear index to coordinates along the way
		int j = 0;
		for (counter; counter >= 0; counter--)
		{
			path[j][0] = reverse_path[counter] % size_X;
			path[j][1] = reverse_path[counter] / size_X;
			j++;
		}
		return;
	}

	return;
}

int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
	/*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. Which cheese is up to you.
	Whatever you code here, your heuristic must be admissible.

	Input arguments:

		x,y - Location for which this function will compute a heuristic search cost
		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		gr - The graph's adjacency list for the maze

		These arguments are as described in the search() function above
 */

	return (1); // <-- Evidently you will need to update this.
}

int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
	/*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. 

	However - this time you want your heuristic function to help the mouse avoid being eaten.
	Therefore: You have to somehow incorporate knowledge of the cats' locations into your
	heuristic cost estimate. How well you do this will determine how well your mouse behaves
	and how good it is at escaping kitties.

	This heuristic *does not have to* be admissible.

	Input arguments have the same meaning as in the H_cost() function above.
 */

	return (1); // <-- Evidently you will need to update this.
}
