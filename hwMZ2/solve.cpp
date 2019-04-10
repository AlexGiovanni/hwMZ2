//Alexis Santiago  November 17, 2018


#include <vector>
#include <string>
#include <unordered_set>
#include <queue>
#include <unordered_map>
#include <iostream>
#include "vertex.h"
#include "MinPriorityQueue.h"

using namespace std;
//hash table under the hood. list of vertices(pointers to Vertex objects)
unordered_map<string, Vertex*> vertexSet;
//Algorithm tools
unordered_map<Vertex*, Vertex*> breadCrumbs;
int rc[2];//stores the number of rows and columns in array
unordered_map<int, Vertex*> portals;//list of portals weights, and vertices*

//finds the number of columns and rows and returns an array containing those values row,col
void findRowCol(string maze) {
	rc[0] = 0; rc[1] = 0;
	for (int i = 0; i < maze.size() - 1; i++) {//find #of row and col
		if (maze[i] == '\n') {
			rc[1] = i + 1;//find # of col
			break;
		}
	}
	
	rc[0] = (maze.size() / rc[1]) - 1;//#of rows
	rc[1] = rc[1] - 2;//# of columns
}



//checks if im on the border of the maze
bool inBorder(string maze, int r, int c) {
	if (r == 0 || c == 0 || r == rc[0] || c == rc[1]) {
		return true;
	}
	return false;
}

//check if there is a vertex on top
//if the vertex i'm serching for exists in vertexset return true
bool checkTop(int r, int c) {
	string key = to_string(r - 1) + "," + to_string(c);
	if (vertexSet.find(key) == vertexSet.end()) {//not a vertex
		return false;
	}
	return true;
}

//check if there is a vertex on its left
//given coordinates calculates left coordinates
bool checkLeft(int r, int c) {
	string key = to_string(r) + "," + to_string(c - 1);
	if (vertexSet.find(key) == vertexSet.end()) {//not a vertex
		return false;
	}
	return true;
}


//Add a basic (bidirectional) edge connecting a and b
//each string is a coordinate x,y w is the weight
void addEdge(string a, string b, int w)
{
	Vertex * aVert = vertexSet[a];//find object to a vertex create a pointer to it
	Vertex * bVert = vertexSet[b];
	//avert point to vertex a //bvert points to vertex b
	aVert->neighs.push_back(make_pair(bVert, w));//add b to the list of a's neighbors
	bVert->neighs.push_back(make_pair(aVert, w));//add a to the list of b's neighbors
}

//dijkstra
void dijkstra(string source)
{
	//cout << "in dijkstra " << endl;
	//create Q and insert 
	MinPriorityQueue<Vertex*> Q;
	//insert all vertecies with "infinity" weight
	for (auto it = vertexSet.begin(); it != vertexSet.end(); ++it) {
		Q.push(it->second, 90000);
	}
	Vertex * s = vertexSet[source];//creates a pointer to a vertex that's the start
	Q.decrease_key(s, 0);
	//step 2: 
	while (Q.size()!=0)
	{
		//cout << "Q.size " << Q.size() << endl;
		//2.1 Remove smallest item from Q
		Vertex * x = Q.front().first;
		int priority = Q.front().second;
		Q.pop();

		//2.2 For each neighbor y of x,
		//relax
		for (int i = 0; i < x->neighs.size(); i++)//traversing the list of neighbours
		{
			Vertex * y = x->neighs[i].first;//y is neighbor vertex
			//relax if priority + weight < priority y
			if ( priority < Q.getPriority(y).second  ) {
			//relax
				Q.decrease_key(y, priority + x->neighs[i].second);
			//breadcrumb
				breadCrumbs[y] = x;
			}
			
		}
	}

}//end of dijkstra


//computes the index of the maze string given its coordinates
int findIndex( int r, int c) {
	return (r * (rc[1] + 2)) + c;
}

//checks if there is a number [0-9] on the given coordinatess
bool isNumber(string maze, int r, int c) {
	for (int i = 0; i <= 9; i++) {
		if ((int)maze[findIndex(r, c)]-'0' == i) {
			return true;
		}
	}
	return false;
}

//computes the shortest pathe from start to end
string shortestPath(string * startEnd, string maze)
{
	//cout << "Shortest Path" << endl;
	//cout << "start: " << startEnd[0] << " end: " << startEnd[1] << endl;
	Vertex * s = vertexSet[startEnd[0]];//creates a pointer to a vertex that's the start
	Vertex * t = vertexSet[startEnd[1]];//creates a pointer to the end vertex
	//cout << "Found the start and end vertecies " << endl;
	//step 1:
	dijkstra(startEnd[0]);

	//step 2: Use the breadcrumbs computed in bfs
	//to trace back a path from s to t:

	Vertex * current = t;//pointer to the end (follow the breadcrumns back to start)
	while (current != s)//while not at the beggining
	{
		//output = current->data + " " + output;//concatonate the string into output
		//use coordinates to find index in maze string
		maze[findIndex( current->row, current->col)] = 'o';
		current = breadCrumbs[current];//move down the list of breadcrumbs
	}

	maze[findIndex( s->row, s->col)] = 'o';
	cout << maze;

	return maze;
}


//creates the graph from the maze string
//creates vertecies and stores them in vertexset
string * createGraph(string maze) {
	//Step 1: create the graph
	static string startEnd[2]; startEnd[0] = ""; startEnd[1] = "";
	 
	//cout << maze << endl;
	int borderCounter = 0;
	int r = 0; int c = 0;
	for (int i = 0; i < maze.size() - 1; i++) {//traverse the string
		if (maze[i] == ' ' || isNumber(maze, r, c)) {
			Vertex * babyvertex = new Vertex(r, c);//create a pointer to vertex object
			string coor = to_string(r) + "," + to_string(c);//key is string of coordinates
			vertexSet[coor] = babyvertex;//add vertex to list of vertecies
			if (isNumber(maze, r, c)) {
				if (portals.find((int)maze[findIndex(r, c)] - '0') != portals.end()) {//its in the list of portals
					Vertex * p = portals[(int)maze[findIndex(r, c)] - '0'];//finds the vertice at that number
					string ptwo = to_string(p->row) + "," + to_string(p->col);;
					addEdge(coor, ptwo, (int)maze[findIndex(r, c)] - '0');//creates edge between portals
				}
				else {
					portals[(int)maze[findIndex(r, c)] - '0'] = babyvertex; //add to list of portals
				}
			}
			
			string coorTop = to_string(r - 1) + "," + to_string(c);
			string coorLeft = to_string(r) + "," + to_string(c - 1);
			if (inBorder(maze, r, c) && borderCounter == 0) {//if it's first vertex on the border
				startEnd[0] = coor;//save coordinates to start vertex
				//check if there should be an edge to top or left if so add it
				if (checkTop(r, c)) 
					addEdge(coor, coorTop, 1);
				if (checkLeft(r, c)) 
					addEdge(coor, coorLeft, 1);
				borderCounter++;
			}
			else if (inBorder(maze, r, c) && borderCounter == 1) {//if it's the second vertex on the border	
				startEnd[1] = coor;//save coordinates to start vertex
				//check if there should be an edge to top or left if so add it
				if (checkTop(r, c)) 
					addEdge(coor, coorTop, 1);
				if (checkLeft(r, c)) 
					addEdge(coor, coorLeft, 1);
			}
			else {//add to list of vertices 
				//check if there should be an edge to top or left
				if (checkTop(r, c))
					addEdge(coor, coorTop, 1);
				if (checkLeft(r, c))
					addEdge(coor, coorLeft, 1);
			}
			

		}
		
		if (maze[i] == '\n') { r++; c = 0; }
		else { c++; }
	}
		//cout << "\nFinished creating the graph" << endl;
		//cout << "start: " << startEnd[0] << " end: " << startEnd[1] << endl;
	return startEnd;

}//end of create graph




// For the mandatory running time, assume that the time for
// operations of queue, unordered_set, and map are O(1). 
// (They are for average-case, but not worst-case).
//
// For the mandatory running time below, s is the length of 
// the input string representing the maze.
// 
// For a complete description of the maze string 
// and maze solution formats, see the assignment pdf.


// Returns a string representing a shortest solution to the maze.
// Has undefined behavior if the maze is not valid or has no solution.
//
// Must run in O(s) time.
string solve(string maze) {
	vertexSet.clear();
	breadCrumbs.clear();
	portals.clear();
	findRowCol(maze);
	return shortestPath(createGraph(maze), maze);
}

