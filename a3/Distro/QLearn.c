/*
	CSC D84 - Unit 3 - Reinforcement Learning
	
	This file contains stubs for implementing the Q-Learning method
	for reinforcement learning as discussed in lecture. You have to
	complete two versions of Q-Learning.
	
	* Standard Q-Learning, based on a full-state representation and
	  a large Q-Table
	* Feature based Q-Learning to handle problems too big to allow
	  for a full-state representation
	    
	Read the assignment handout carefully, then implement the
	required functions below. Sections where you have to add code
	are marked

	**************
	*** TO DO:
	**************

	If you add any helper functions, make sure you document them
	properly and indicate in the report.txt file what you added.
	
	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Jan. 16
*/

#include "QLearn.h"

void QLearn_update(int s, int a, double r, int s_new, double *QTable)
{
  /*
   This function implementes the Q-Learning update as stated in Lecture. It 
   receives as input a <s,a,r,s'> tuple, and updates the Q-table accordingly.
   
   Your work here is to calculate the required update for the Q-table entry
   for state s, and apply it to the Q-table
     
   The update involves two constants, alpha and lambda, which are defined in QLearn.h - you should not 
   have to change their values. Use them as they are.
     
   Details on how states are used for indexing into the QTable are shown
   below, in the comments for QLearn_action. Be sure to read those as well!
 */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
}

int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, double *QTable, int size_X, int graph_size)
{
  /*
     This function decides the action the mouse will take. It receives as inputs
     - The graph - so you can check for walls! The mouse must never move through walls
     - The mouse position
     - The cat position
     - The chees position
     - A 'pct' value in [0,1] indicating the amount of time the mouse uses the QTable to decide its action,
       for example, if pct=.25, then 25% of the time the mouse uses the QTable to choose its action,
       the remaining 75% of the time it chooses randomly among the available actions.
       
     Remember that the training process involves random exploration initially, but as training
     proceeds we use the QTable more and more, in order to improve our QTable values around promising
     actions.
     
     The value of pct is controlled by QLearn_core_GL, and increases with each round of training.
     
     This function *must return* an action index in [0,3] where
        0 - move up
        1 - move right
        2 - move down
        3 - move left
        
     QLearn_core_GL will print a warning if your action makes the mouse cross a wall, or if it makes
     the mouse leave the map - this should not happen. If you see a warning, fix the code in this
     function!
     
   The Q-table has been pre-allocated and initialized to 0. The Q-table has
   a size of
   
        graph_size^3 x 4
        
   This is because the table requires one entry for each possible state, and
   the state is comprised of the position of the mouse, cat, and cheese. 
   Since each of these agents can be in one of graph_size positions, all
   possible combinations yield graph_size^3 states.
   
   Now, for each state, the mouse has up to 4 possible moves (up, right,
   down, and left). We ignore here the fact that some moves are not possible
   from some states (due to walls) - it is up to the QLearn_action() function
   to make sure the mouse never crosses a wall. 
   
   So all in all, you have a big table.
        
   For example, on an 8x8 maze, the Q-table will have a size of
   
       64^3 x 4  entries
       
       with 
       
       size_X = 8		<--- size of one side of the maze
       graph_size = 64		<--- Total number of nodes in the graph
       
   Indexing within the Q-table works as follows:
   
     say the mouse is at   i,j
         the cat is at     k,l
         the cheese is at  m,n
         
     state = (i+(j*size_X)) + ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size)
     ** Make sure you undestand the state encoding above!
     
     Entries in the Q-table for this state are

     *(QTable+(4*state)+a)      <-- here a is the action in [0,3]
     
     (yes, it's a linear array, no shorcuts with brackets!)
     
     NOTE: There is only one cat and once cheese, so you only need to use cats[0][:] and cheeses[0][:]
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/

  return (0); // <--- of course, you will change this!
}

double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function computes and returns a reward for the state represented by the input mouse, cat, and
    cheese position. 
    
    You can make this function as simple or as complex as you like. But it should return positive values
    for states that are favorable to the mouse, and negative values for states that are bad for the 
    mouse.
    
    I am providing you with the graph, in case you want to do some processing on the maze in order to
    decide the reward. 
        
    This function should return a maximim/minimum reward when the mouse eats/gets eaten respectively.      
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/

  return (0); // <--- of course, you will change this as well!
}

void feat_QLearn_update(double gr[max_graph_size][4], double weights[25], double reward, int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function performs the Q-learning adjustment to all the weights associated with your
    features. Unlike standard Q-learning, you don't receive a <s,a,r,s'> tuple, instead,
    you receive the current state (mouse, cats, and cheese potisions), and the reward 
    associated with this action (this is called immediately after the mouse makes a move,
    so implicit in this is the mouse having selected some action)
    
    Your code must then evaluate the update and apply it to the weights in the weight array.    
   */
  double current_features[25];
  evaluateFeatures(gr, current_features, mouse_pos, cats, cheeses, size_X, graph_size);
  double current_state = Qsa(weights, current_features);

  double next_state;
  int action;
  maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &next_state, &action);
  
  for (int i = 0; i < numFeatures; i++) {
    weights[i] += alpha * (reward + ( lambda * next_state) - current_state) * current_features[i];
  }
  
  // w_i = alpha * (reward + (lambda * Q(s'' )) - Q(s)) * feature_i(s)
}

int feat_QLearn_action(double gr[max_graph_size][4], double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, int size_X, int graph_size) {
  /*
    Similar to its counterpart for standard Q-learning, this function returns the index of the next
    action to be taken by the mouse.
    
    Once more, the 'pct' value controls the percent of time that the function chooses an optimal
    action given the current policy.
    
    E.g. if 'pct' is .15, then 15% of the time the function uses the current weights and chooses
    the optimal action. The remaining 85% of the time, a random action is chosen.
    
    As before, the mouse must never select an action that causes it to walk through walls or leave
    the maze.    
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  int random_action;
  // Draw a random number c in [0,1]
  double c = (double)rand() / (double)RAND_MAX;

  if (c > pct) {
    random_action = rand() % 4;
    int index = mouse_pos[0][0] + (mouse_pos[0][1] * size_X);
    while (gr[index][random_action] < 1) {
      random_action = rand() % 4;
    }
  }
  else {
    double trash;
    maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &trash, &random_action);
  }

  printf("returning random actin: %d", random_action);
  return (random_action); // <--- replace this while you're at it!
}

void evaluateFeatures(double gr[max_graph_size][4], double features[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size) {
  /*
   This function evaluates all the features you defined for the game configuration given by the input
   mouse, cats, and cheese positions. You are free to define up to 25 features. This function will
   evaluate each, and return all the feature values in the features[] array.
   
   Take some time to think about what features would be useful to have, the better your features, the
   smarter your mouse!
   
   Note that instead of passing down the number of cats and the number of cheese chunks (too many parms!)
   the arrays themselves will tell you what are valid cat/cheese locations.
   
   You can have up to 5 cats and up to 5 cheese chunks, and array entries for the remaining cats/cheese
   will have a value of -1 - check this when evaluating your features!
  */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/

  // basically just go through and fill in the values for features i = 0 to i = numFeatures
  // sum of distances from mouse to all cheeses
  double temp = 0;

  // FEATURE 1 MOUSE DISTANCE TO CHEESES
  int cheese_count = 0;
  double cheese_dist = 0;
  while (cheeses[cheese_count][0] != -1) {
    temp = sqrt(pow(mouse_pos[0][0] - cheeses[cheese_count][0], 2) + pow(mouse_pos[0][1] - cheeses[cheese_count][1], 2));
    cheese_dist += temp;
    cheese_count += 1;
  }
  features[0] = cheese_dist;

  // FEATURE 2 MOUSE DISTANCE TO CATS
  int cat_count = 0;
  double cat_dist = 0;
  while (cats[cat_count][0] != -1) {
    temp = sqrt(pow(mouse_pos[0][0] - cats[cat_count][0], 2) + pow(mouse_pos[0][1] - cats[cat_count][1], 2));
    cat_dist += temp;
    cat_count += 1;
  }
  features[1] = cat_dist;

  printf("calculated features,    mouse_cheese f1:%f    mouse_cats f2:%f", features[0], features[1]);
}

double Qsa(double weights[25], double features[25])
{
  /*
    Compute and return the Qsa value given the input features and current weights
   */

  /***********************************************************************************************
  * TO DO: Complete this function
  ***********************************************************************************************/
  double sum = 0;
  for (int i = 0; i < numFeatures; i++) {
    sum += weights[i] * features[i];
  }

  printf("reutrning sum from Qsa:%f", sum);
  return (sum); // <--- stub! compute and return the Qsa value
}

void maxQsa(double gr[max_graph_size][4], double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size, double *maxU, int *maxA) {
  /*
   Given the state represented by the input positions for mouse, cats, and cheese, this function evaluates
   the Q-value at all possible neighbour states and returns the max. The maximum value is returned in maxU
   and the index of the action corresponding to this value is returned in maxA.
   
   You should make sure the function does not evaluate moves that would make the mouse walk through a
   wall. 
  */

  int location_index = mouse_pos[0][0] + (mouse_pos[0][1] * size_X);
  double action_rewards[4];
  double feats[25];

  //set the indexes of the children arrays
  int top_neighbour[1][2];
	int right_neighbour[1][2];
	int bottom_neighbour[1][2];
	int left_neighbour[1][2];
  // top neighbour
  top_neighbour[0][0] = mouse_pos[0][0]; // x coord 
  top_neighbour[0][1] = mouse_pos[0][1] - 1;  // y coord
  // right neighbour
  right_neighbour[0][0] = mouse_pos[0][0] + 1 ; // x coord 
  right_neighbour[0][1] = mouse_pos[0][1];  // y coord
  // bottom neighbour
  bottom_neighbour[0][0] = mouse_pos[0][0]; // x coord 
  bottom_neighbour[0][1] = mouse_pos[0][1] + 1;  // y coord
  // left neighbour
  left_neighbour[0][0] = mouse_pos[0][0] - 1; // x coord 
  left_neighbour[0][1] = mouse_pos[0][1];  // y coord



  // check if the top neighbour is connected
  if (gr[location_index][0] == 1.0) {
    // call evaluateFeatures() which will update the values inside the features array
    evaluateFeatures(gr, feats, top_neighbour, cats, cheeses, size_X, graph_size);
    // call Qsa() which will return to us the features* wieght / reward value for taking action to move above
    action_rewards[0] = Qsa(weights, feats);
  } else {
    action_rewards[0] = -2147483648;
  }

  // check if the right neighbour is connected
  if (gr[location_index][1] == 1.0) {
    // call evaluateFeatures() which will update the values inside the features array
    evaluateFeatures(gr, feats, right_neighbour, cats, cheeses, size_X, graph_size);
    // call Qsa() which will return to us the features* wieght / reward value for taking action to move above
    action_rewards[1] = Qsa(weights, feats);
  } else {
    action_rewards[1] = -2147483648;
  }

  // check if the bottom neighbour is connected
  if (gr[location_index][2] == 1.0) {
    // call evaluateFeatures() which will update the values inside the features array
    evaluateFeatures(gr, feats, bottom_neighbour, cats, cheeses, size_X, graph_size);
    // call Qsa() which will return to us the features* wieght / reward value for taking action to move above
    action_rewards[2] = Qsa(weights, feats);
  } else {
    action_rewards[2] = -2147483648;
  }

  // check if the left neighbour is connected
  if (gr[location_index][3] == 1.0) {
    // call evaluateFeatures() which will update the values inside the features array
    evaluateFeatures(gr, feats, left_neighbour, cats, cheeses, size_X, graph_size);
    // call Qsa() which will return to us the features* wieght / reward value for taking action to move above
    action_rewards[3] = Qsa(weights, feats);
  } else {
    action_rewards[3] = -2147483648;
  }

  // we have the utility value for each child, so noowwwww
  // we need to find the max utility cost among the children
  int max_index = 0;
  for (int x = 1; x < 4; x++){
    if (action_rewards[max_index] < action_rewards[x]) {
      max_index = x;
    }
  }

  *maxU = action_rewards[max_index]; // <--- stubs! your code will compute actual values for these two variables!
  *maxA = max_index;
  return;
}

/***************************************************************************************************
 *  Add any functions needed to compute your features below 
 *                 ---->  THIS BOX <-----
 * *************************************************************************************************/