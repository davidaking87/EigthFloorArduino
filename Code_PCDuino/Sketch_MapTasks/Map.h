/*
 * map.h
 *
 *  Created on: Apr 29, 2017
 *      Author: admin
 *  COPIED FROM pathfinder code!
 */

#ifndef MAP_H_
#define MAP_H_

typedef struct NODE *vertex;
typedef struct GRAPHHEAD *graph;

//North = N, South = S, east = E, west = W, X = none
typedef enum FACING {N,S,E,W, X} dir;
typedef enum GRAPHVISIT {G, B, C} vis;
typedef enum MOVE{FORWARD, LEFT, RIGHT, REVERSE} move;
void openNodes(struct GRAPHHEAD* g);
void openNode(struct GRAPHHEAD* g, int node);
void blackNode (struct GRAPHHEAD g, int node);
void countEdges (struct GRAPHHEAD *g);
int countAt (struct GRAPHHEAD g, int node);
int pathOpen(int n);
void clearWeight(struct GRAPHHEAD* g, int node);
void clearWeights(struct GRAPHHEAD* g);
void setWeights (graph g, int node, dir initial_heading);



struct NODE {
  vis visited;
  //where the car will try to go relative to it's facing
  char north;
  char south;
  char east;
  char west;
  //where it will enter the next node;
  dir enternorth;
  dir entersouth;
  dir entereast;
  dir enterwest;
  //weights
  char wn, ws, we, ww;
};

struct GRAPHHEAD{
  int edge_count;
  int vertex_count;
  struct NODE vertices[18];
};




#endif /* MAP_H_ */
