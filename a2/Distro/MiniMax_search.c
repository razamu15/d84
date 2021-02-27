/*
	CSC D84 - Unit 2 - MiniMax search and adversarial games

	This file contains stubs for implementing a MiniMax search
        procedure with alpha-beta pruning. Please read the assignment
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

	Starter code: F.J.E. Sep. 15
*/

#include "MiniMax_search.h"
#include <unistd.h>

int times = 0;

double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta) {
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in MiniMax_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:

	Exactly the same as for Assignment 1 - have a look at your code if you need a reminder of how the adjacency
	list and agent positions are stored.	

	Note that in this case, the path will contain a single move - at the top level, this function will provide
	an agent with the 'optimal' mini-max move given the game state.


   IMPORTANT NOTE: Mini-max is a recursive procedure. This function will need to fill-in the mini-max values for 
	 	   all game states down to the maximum search depth specified by the user. In order to do that,
		   the function needs to be called with the correct state at each specific node in the mini-max
		   search tree.

		   The game state is composed of:

			* Mouse, cat, and cheese positions (and number of cats and cheeses)
			
		   At the top level (when this function is called by the mini-max driver code), the game state
		   correspond to the current situation of the game. But once you start recursively calling
		   this function for lower levels of the search tree the positions of agents will have changed.
		   
		   Therefore, you will need to define local variables to keep the game state at each node of the
		   mini-max search tree, and you will need to update this state when calling recursively so that
		   the search does the right thing.

		   This function *must check* whether:
			* A candidate move results in a terminal configuration (cat eats mouse, mouse eats cheese)
			  at which point it calls the utility function to get a value
	 		* Maximum search depth has been reached (depth==maxDepth), at which point it will also call
			  the utility function to get a value
			* Otherwise, call recursively using the candidate configuration to find out what happens
			  deeper into the mini-max tree.

   Arguments:
		gr[graph_size][4]   		- This is an adjacency list for the maze
		path[1][2] 			- Your function will return the optimal mini-max move in this array.
		minmax_cost[size_X][size_Y]	- An array in which your code will store the
						  minimax value for maze locations expanded by
						  the search *when called for the mouse, not
						  for the cats!*

						  This array will be used to provide a visual 
						  display of minimax values during the game.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- No alpha-beta pruning
					mode = 1	- Alpha-beta pruning

		(*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);
				- This is a pointer to the utility function which returns a value for a specific game configuration

				   NOTE: Unlike the search assignment, this utility function also gets access to the graph so you can do any processing 
				   that requires knowledge of the maze for computing the utility values.

				  * How to call the utility function from within this function : *
					- Like any other function:
						u = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
						
		agentId: Identifies which agent we are doing MiniMax for. agentId=0 for the mouse, agentId in [1, cats] for cats. Notice that recursive calls
                         to this function should increase the agentId to reflect the fact that the next level down corresponds to the next agent! For a game
                         with two cats and a mouse, the agentIds for the recursion should look like 0, 1, 2, 0, 1, 2, ...
	
		depth: Current search depth - whether this is a MIN or a MAX node depends both on depth and agentId.
		
		maxDepth: maximum desired search depth - once reached, your code should somehow return
			  a minimax utility value for this location.

		alpha. beta: alpha and beta values passed from the parent node to constrain search at this
			     level.

   Return values:
		Your search code will directly update data passed-in as arguments:
		
		- Mini-Max value	: Notice this function returns a double precision number. This is
					  the minimax value at this level of the tree. It will be used 
					  as the recursion backtracks filling-in the mini-max values back
					  from the leaves to the root of the search tree. 

		- path[1][2]		: Your MiniMax function will return the location for the agent's 
					  next location (i.e. the optimal move for the agent). 
		- minmax_cost[size_X][size_Y] 	:  Your search code will update this array to contain the
						   minimax value for locations that were expanded during
						   the search. This must be done *only* for the mouse.

						   Values in this array will be in the range returned by
						   your utility function.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO:	Implement code to perform a MiniMax search. This will involve a limited-depth BFS-like
 *              expansion. Once nodes below return values, your function will propagate minimax utilities
 *		as per the minimax algorithm.
 *	
 *		Note that if alpha-beta pruning is specified, you must keep track of alphas and betas
 *		along the path.
 *
 *		You can use helper functions if it seems reasonable. Add them to the MiniMax_search.h
 *		file and explain in your code why they are needed and how they are used.
 *
 *		Recursion should appear somewhere.
 *
 *		MiniMax cost: If the agentId=0 (Mouse), then once you have a MiniMax value for a location
 *		in the maze, you must update minmax_cost[][] for that location.
 *
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/

	if  (depth == 0) {
		times += 1;
	}

	printf("depth:%d, agentID:%d   mouse x:%d y:%d\n",depth, agentId, mouse_loc[0][0], mouse_loc[0][1]);

	// if (times >= 100 && depth == 0) {
	// 	sleep(600);
	// }
	

	// if (depth == 0){
	// 	sleep(5);
	// }

	// so we need to do a limited depth BFS (chigao, DFS)

	// when this function is called, we are at root.
	
	// before we do anything, we need to check if this is a terminal node
	if (depth + 1 == maxDepth || checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses)) {
		double value = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);

		
		// for (int a = 0; a < depth; a++) {
		// 	printf("    ");
		// }
		// printf("reached max depth: %d    for agent:%d   returning utility:%f\n", depth, agentId, value);
		printf("returning utility:%f\n", value);

		if (agentId == 0) {
			minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]] = value;
		}
		return value;
	}


	// delcare some vars
	double children_utilities[4]; // [top:0, right:1, bottom:2, left:3]
	int location_index;

	int top_neighbour[1][2];
	int right_neighbour[1][2];
	int bottom_neighbour[1][2];
	int left_neighbour[1][2];

	// mouse tings
	if (agentId == 0) {

		// we have a location for the mouse which is the root of the tree
		location_index = mouse_loc[0][0] + (mouse_loc[0][1] * size_X);


		printf("MOUSE    connections-> top:%f  right:%f  bottom:%f  left:%f\n", gr[location_index][0], gr[location_index][1], gr[location_index][2], gr[location_index][3]);


		//set the indexes of the children arrays
		// top neighbour
		top_neighbour[0][0] = mouse_loc[0][0]; // x coord 
		top_neighbour[0][1] = mouse_loc[0][1] - 1;  // y coord
		// right neighbour
		right_neighbour[0][0] = mouse_loc[0][0] + 1 ; // x coord 
		right_neighbour[0][1] = mouse_loc[0][1];  // y coord
		// bottom neighbour
		bottom_neighbour[0][0] = mouse_loc[0][0]; // x coord 
		bottom_neighbour[0][1] = mouse_loc[0][1] + 1;  // y coord
		// left neighbour
		left_neighbour[0][0] = mouse_loc[0][0] - 1; // x coord 
		left_neighbour[0][1] = mouse_loc[0][1];  // y coord

		
		// we need to get the utility values for each of the children of the mouse
		// we need to figure out what the cat would do if the mouse moved to each of these locations
			// so to find out we need to
				// update the mouse location to that specific child 
				// call the minimax search on the cat. to call the next minimax search we need to
					// calculate the next agentId
					// increment the depth counter

		// check top child is connected in the maze
		if (gr[location_index][0] == 1.0) {
			printf("updating the mouse to its top neighbour\n");
			
			// call the next minimax search and set the return value in the children_utilities array
			children_utilities[0] = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, top_neighbour, mode, utility, agentId + 1, depth + 1, maxDepth, alpha, beta);
			// update the minmax_cost array with the utility for this child
			minmax_cost[top_neighbour[0][0]][top_neighbour[0][1]] = children_utilities[0];

			// check for pruning
			if (mode == 1) {
				// set the alpha
				if (children_utilities[0] > alpha) {
					alpha = children_utilities[0];
				}
				// pruning condition
				if (alpha >= beta) {
					return (children_utilities[0]);
				}
			}

		} else {
			children_utilities[0] = -2147483648;
		}

		// check right child is connected in the maze
		if (gr[location_index][1] == 1.0) {
			printf("updating the mouse to its right neighbour\n");

			// call the next minimax search and set the return value in the children_utilities array
			children_utilities[1] = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, right_neighbour, mode, utility, agentId + 1, depth + 1, maxDepth, alpha, beta);
			// update the minmax_cost array with the utility for this child
			minmax_cost[right_neighbour[0][0]][right_neighbour[0][1]] = children_utilities[1];

			// check for pruning
			if (mode == 1) {
				// set the alpha
				if (children_utilities[1] > alpha) {
					alpha = children_utilities[1];
				}
				// pruning condition
				if (alpha >= beta) {
					return (children_utilities[1]);
				}
			}

		} else {
			children_utilities[1] = -2147483648;
		}

		// check bottom child is connected in the maze
		if (gr[location_index][2] == 1.0) {
			printf("updating the mouse to its bottom neighbour\n");
			
			// call the next minimax search and set the return value in the children_utilities array
			children_utilities[2] = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, bottom_neighbour, mode, utility, agentId + 1, depth + 1, maxDepth, alpha, beta);
			// update the minmax_cost array with the utility for this child
			minmax_cost[bottom_neighbour[0][0]][bottom_neighbour[0][1]] = children_utilities[2];

			// check for pruning
			if (mode == 1) {
				// set the alpha
				if (children_utilities[2] > alpha) {
					alpha = children_utilities[2];
				}
				// pruning condition
				if (alpha >= beta) {
					return (children_utilities[2]);
				}
			}

		} else {
			children_utilities[2] = -2147483648;
		}

		// check left child is connected in the maze
		if (gr[location_index][3] == 1.0) {
			printf("updating the mouse to its left neighbour\n");

			// call the next minimax search and set the return value in the children_utilities array
			children_utilities[3] = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, left_neighbour, mode, utility, agentId + 1, depth + 1, maxDepth, alpha, beta);
			// update the minmax_cost array with the utility for this child
			minmax_cost[left_neighbour[0][0]][left_neighbour[0][1]] = children_utilities[3];

			// check for pruning
			if (mode == 1) {
				// set the alpha
				if (children_utilities[3] > alpha) {
					alpha = children_utilities[3];
				}
				// pruning condition
				if (alpha >= beta) {
					return (children_utilities[3]);
				}
			}

		} else {
			children_utilities[3] = -2147483648;
		}
		
		// we have the utility value for each child, so noowwwww
		// we need to find the max utility cost among the children
		int max_index = 0;
		for (int x = 1; x < 4; x++){
			if (children_utilities[max_index] < children_utilities[x]) {
				max_index = x;
			}
		}

		// set the max utility child as the next optimal move in the path array and set the utility in the minmax_cost
		if (max_index == 0) {
			// top neightbour is best option
			path[0][0] = top_neighbour[0][0];
			path[0][1] = top_neighbour[0][1];
		}
		else if (max_index == 1) {
			// right neightbour is best option
			path[0][0] = right_neighbour[0][0];
			path[0][1] = right_neighbour[0][1];
		}
		else if (max_index == 2) {
			// bottom neightbour is best option
			path[0][0] = bottom_neighbour[0][0];
			path[0][1] = bottom_neighbour[0][1];
		}
		else if (max_index == 3) {
			// left neightbour is best option
			path[0][0] = left_neighbour[0][0];
			path[0][1] = left_neighbour[0][1];
		}
		
		
		printf("MOUSE  top_utility:%f  right_utility:%f  bottom_utility:%f  left_utility:%f\n", children_utilities[0], children_utilities[1], children_utilities[2], children_utilities[3]);
		printf("MOUSE  chosen move:%d\n", max_index);
		
		// return max utility cost
		return(children_utilities[max_index]);

	} 
	else { // cat tings
		
		// copy the cats given into local cats
		int local_cats[10][2];
		memcpy(local_cats, cat_loc, sizeof(int) * 10 * 2);
	
		// need to figure out which cat this function call was for
		int cat_index = agentId - 1;
		location_index = cat_loc[cat_index][0] + (cat_loc[cat_index][1] * size_X);
		int next_agent_id = (agentId + 1) % (cats + 1);
		int new_depth = depth;
		if (next_agent_id == 0) {
			new_depth += 1;
		}
		

		printf("CAT     connections-> top:%f  right:%f  bottom:%f  left:%f\n", gr[location_index][0], gr[location_index][1], gr[location_index][2], gr[location_index][3]);

		
		
		// now we need to get the utility for each child of THIS cat
		// we need to figure out what the mouse would do if the cat moved to each of these locations
		// so to find out we need to
			// update THIS cat's location to that specific child 
			// call the minimax search on the NEXT AGENT-ID. to call the next minimax search we need to
				// calculate the next agentId
				// increment the depth counter
		
		// check top child is connected in the maze
		if (gr[location_index][0] == 1.0) {
			printf("updating the cat to its top neighbour\n");


			// update this cats location to its top neighbour
			local_cats[cat_index][0] = cat_loc[cat_index][0];
			local_cats[cat_index][1] = cat_loc[cat_index][1] - 1;
			// call the next minimax search and set the return value in the children_utilities array
			children_utilities[0] = MiniMax(gr, path, minmax_cost, local_cats, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_agent_id, new_depth, maxDepth, alpha, beta);

			// check for pruning
			if (mode == 1) {
				// set the beta
				if (children_utilities[0] < beta) {
					beta = children_utilities[0];
				}
				// pruning condition
				if (alpha >= beta) {
					return (children_utilities[0]);
				}
			}

		} else {
			children_utilities[0] = 2147483647;
		}

		// check right child is connected in the maze
		if (gr[location_index][1] == 1.0) {
			printf("updating the cat to its right neighbour\n");


			// update this cats location to its right neighbour
			local_cats[cat_index][0] = cat_loc[cat_index][0] + 1;
			local_cats[cat_index][1] = cat_loc[cat_index][1];
			// call the next minimax search and set the return value in the children_utilities array
			children_utilities[1] = MiniMax(gr, path, minmax_cost, local_cats, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_agent_id, new_depth, maxDepth, alpha, beta);

			// check for pruning
			if (mode == 1) {
				// set the beta
				if (children_utilities[1] < beta) {
					beta = children_utilities[1];
				}
				// pruning condition
				if (alpha >= beta) {
					return (children_utilities[1]);
				}
			}

		} else {
			children_utilities[1] = 2147483647;
		}

		// check bottom child is connected in the maze
		if (gr[location_index][2] == 1.0) {
			printf("updating the cat to its bottom neighbour\n");


			// update this cats location to its bottom neighbour
			local_cats[cat_index][0] = cat_loc[cat_index][0];
			local_cats[cat_index][1] = cat_loc[cat_index][1] + 1;
			// call the next minimax search and set the return value in the children_utilities array
			children_utilities[2] = MiniMax(gr, path, minmax_cost, local_cats, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_agent_id, new_depth, maxDepth, alpha, beta);

			// check for pruning
			if (mode == 1) {
				// set the beta
				if (children_utilities[2] < beta) {
					beta = children_utilities[2];
				}
				// pruning condition
				if (alpha >= beta) {
					return (children_utilities[2]);
				}
			}

		} else {
			children_utilities[2] = 2147483647;
		}

		// check left child is connected in the maze
		if (gr[location_index][3] == 1.0) {
			printf("updating the cat to its left neighbour\n");



			// update this cats location to its left neighbour
			local_cats[cat_index][0] = cat_loc[cat_index][0] - 1;
			local_cats[cat_index][1] = cat_loc[cat_index][1];
			// call the next minimax search and set the return value in the children_utilities array
			children_utilities[3] = MiniMax(gr, path, minmax_cost, local_cats, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_agent_id, new_depth, maxDepth, alpha, beta);

			// check for pruning
			if (mode == 1) {
				// set the beta
				if (children_utilities[3] < beta) {
					beta = children_utilities[3];
				}
				// pruning condition
				if (alpha >= beta) {
					return (children_utilities[3]);
				}
			}

		} else {
			children_utilities[3] = 2147483647;
		}

		// we have the utility value for each child, so noowwwww
		// we need to find the min utility cost among the children
		int min_index = 0;
		for (int x = 1; x < 4; x++){
			if (children_utilities[min_index] > children_utilities[x]) {
				min_index = x;
			}
		}

		printf("CAT  top_utility:%f  right_utility:%f  bottom_utility:%f  left_utility:%f\n", children_utilities[0], children_utilities[1], children_utilities[2], children_utilities[3]);
		printf("CAT  chosen move:%d\n", min_index);
		return(children_utilities[min_index]);
	}

}

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{
	/*
	This function computes and returns the utility value for a given game configuration.
	As discussed in lecture, this should return a positive value for configurations that are 'good'
	for the mouse, and a negative value for locations that are 'bad' for the mouse.

	How to define 'good' and 'bad' is up to you. Note that you can write a utility function
	that favours your mouse or favours the cats, but that would be a bad idea... (why?)

	Input arguments:

		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		depth - current search depth
		gr - The graph's adjacency list for the maze

		These arguments are as described in A1. Do have a look at your solution!
 */

	// the further the mouse if from the cats the better the position

	// the closer the mouse is to the cheese the better the position

	// first calculate the distance of the mouse to all the cheeses
	double temp;
	double util = 0;

	if (cheese_exists(cheese_loc, cheeses, mouse_loc[0][0], mouse_loc[0][1])) {
		util = 100;
	}
	
	if (cat_exists(cat_loc, cats, mouse_loc[0][0], mouse_loc[0][1])) {
		util = -100;
	}

	// double mouse_cheese_dist[10];
	// double mouse_cheese_sum = 0;
	// for (int i=0; i < cheeses; i++) {
	// 	temp = sqrt(pow(mouse_loc[0][0] - cheese_loc[i][0], 2) + pow(mouse_loc[0][1] - cheese_loc[i][1], 2));
	// 	mouse_cheese_dist[i] = temp;
	// 	mouse_cheese_sum += temp;
	// }

	int bfs_mouse_cheese = bfs(gr, cat_loc, cats, cheese_loc, cheeses, mouse_loc);
	printf("bfs_mouse_cheese: %d\n", bfs_mouse_cheese);
	util -= bfs_mouse_cheese * 1.1;
	int bfs_mouse_cat = bfs(gr, cheese_loc, cheeses, cat_loc, cats, mouse_loc);
	printf("bfs_mouse_cat %d\n", bfs_mouse_cat);
	util += bfs_mouse_cat * 0.9;

	int num_walls = 0;
	int mouse_index = mouse_loc[0][0] + (mouse_loc[0][1] * size_X);
	double penalty = 0;
	for (int j = 0; j < 4; j++) {
		if (gr[mouse_index][j] == 0.0) {
			num_walls++;
		}
	}
	if (num_walls == 3) {
		penalty = 25;
	}
	else {
		penalty = num_walls * 0.3;
	}
	util -= penalty;


	// find a function that gives a value combining the distance to the cats and the cheeses
	// printf("starto\n");
	// for(int loop = 0; loop < cats; loop++) {
	// 	printf("mouse to cheese [%d]: %f \n",loop, mouse_cheese_dist[loop]);
	// 	printf("mouse to cat [%d]: %f \n",loop, mouse_cat_dist[loop]);
	// 	printf("----loopo-----\n");
	// }
	// printf("sum of mouse cheese distances: %f\n", mouse_cheese_sum);
	// printf("sum of mouse cat distances: %f\n", mouse_cat_sum);
	printf("utility value: %f\n", util);

	return (util); // <--- Obviously, this will be replaced by your computer utilities
}

int checkForTerminal(int mouse_loc[1][2],int cat_loc[10][2],int cheese_loc[10][2],int cats,int cheeses) {
 /* 
   This function determines whether a given configuration constitutes a terminal node.
   Terminal nodes are those for which:
     - A cat eats the mouse
     or
     - The mouse eats a cheese
   
   If the node is a terminal, the function returns 1, else it returns 0
 */

 // Check for cats having lunch
 for (int i=0; i<cats; i++)
  if (mouse_loc[0][0]==cat_loc[i][0]&&mouse_loc[0][1]==cat_loc[i][1]) return(1);

 // Check for mouse having lunch
 for (int i=0; i<cheeses; i++)
  if (mouse_loc[0][0]==cheese_loc[i][0]&&mouse_loc[0][1]==cheese_loc[i][1]) return(1);

 return(0);

}

/* lines 622 to 689; queue implementation taken from https://www.geeksforgeeks.org/queue-linked-list-implementation/ 
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

int cat_exists(int cat_loc[10][2], int cats, int x, int y)
{
	int i;
	for (i = 0; i < cats; i++)
	{
		if ((cat_loc[i][0] == x) && (cat_loc[i][1] == y))
		{
			return 1;
		}
	}
	return 0;
}

int cheese_exists(int cheese_loc[10][2], int cheeses, int x, int y)
{
	int i;
	for (i = 0; i < cheeses; i++)
	{
		if ((cheese_loc[i][0] == x) && (cheese_loc[i][1] == y))
		{
			return 1;
		}
	}
	return 0;
}

int bfs(double gr[graph_size][4], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2])
{
	// create the predecessros array and set ii all to -1
	int predecessors[graph_size];
	int i;
	for (i = 0; i < graph_size; i++)
	{
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
	int cat_found = 0;

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
		// now go through the 4 children and add them to the queue if they qualify

		// top neighbour
		child_index = cur_node->x + ((cur_node->y - 1) * size_X);
		if ((gr[current_index][0] == 1.0) && (predecessors[child_index] == -1))
		{
			predecessors[child_index] = current_index;
			enQ(Q, cur_node->x, (cur_node->y - 1));
		}

		// right neighbour
		child_index = (cur_node->x + 1) + ((cur_node->y) * size_X);
		if ((gr[current_index][1] == 1.0) && (predecessors[child_index] == -1))
		{
			predecessors[child_index] = current_index;
			enQ(Q, cur_node->x + 1, (cur_node->y));
		}

		// bottom neighbour
		child_index = cur_node->x + ((cur_node->y + 1) * size_X);
		if ((gr[current_index][2] == 1.0) && (predecessors[child_index] == -1))
		{
			predecessors[child_index] = current_index;
			enQ(Q, cur_node->x, (cur_node->y + 1));
		}

		// left neighbour
		child_index = (cur_node->x - 1) + (cur_node->y) * 32;
		if ((gr[current_index][3] == 1.0) && (predecessors[child_index] == -1))
		{
			predecessors[child_index] = current_index;
			enQ(Q, cur_node->x - 1, (cur_node->y));
		}
		free(cur_node);
	}
	free(Q);

	// now build the path in reverse from the cheese to the mouse
	int reverse_path[graph_size];
	int mouse = mouse_loc[0][0] + mouse_loc[0][1] * 32;
	int counter = 0;
	reverse_path[0] = cheese_index[0] + cheese_index[1] * 32;
	// first loop to build the path backwards
	while (reverse_path[counter] != mouse)
	{
		reverse_path[counter + 1] = predecessors[reverse_path[counter]];
		counter++;
	}
	return counter;
}
