#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream> 
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge {
	int benefit;
	int cost;
	int origin;
	int destination;

	bool operator> (const Edge &other) const {
		return benefit - cost > other.benefit - other.cost;
	}
};

struct Node {
	int id;
	set<Edge, greater<Edge>> edges;

	Node (int i) : id(i) { }

	bool operator< (const Node &other) const {
		return id < other.id;
	}
};

struct Graph {
	set<Node> nodes;
};

Graph prim(Graph g) {
	struct Graph forest;	//Forest to give as an answer
	set<Edge, greater<Edge>> edges;	//Edges to iterate through to construct the solution
	set<Node>::iterator nit;	//Node iterator
	set<Edge, greater<Edge>>::iterator eit1;	//Need two edge iterators
	set<Edge, greater<Edge>>::iterator eit2;
	
	nit = g.nodes.begin();	//Get the first node to initialize the forest
	Node auxn = *nit;
	for (eit1 = auxn.edges.begin(); eit1 != auxn.edges.end(); eit1++) {	//Add all of its edges to iterate through
		Edge auxe = *eit1;
		edges.insert(auxe);
	}
	auxn.edges.clear();	//Clear its edges to add to the forest
	forest.nodes.insert(auxn);	//Finally, add it

	eit1 = edges.begin();	//Get the first edge, notice they're already ordered
	do {
		Edge auxe = *eit1;	//First edge
		nit = forest.nodes.find(Node(auxe.destination));	//Search if its destination is already is in the forest
		if (nit == forest.nodes.end()) {	//Let's say it is
			nit = forest.nodes.find(auxe.origin);	//Find the origin node
			auxn = *nit;	
			auxn.edges.insert(auxe);	//Add the new edge to it
			forest.nodes.erase(nit);	//Erase and replace with the new node
			forest.nodes.insert(auxn);

			auxn.id = auxe.destination;	//Then, create the new node to add to the forest
			auxn.edges.clear();	//Clear its edges again
			swap(auxe.origin, auxe.destination);	//Swap its origins and destination
			auxn.edges.insert(auxe);	//Add the modified edge
			forest.nodes.insert(auxn);	//Add the second node to the forest

			nit = g.nodes.find(Node(auxe.origin));	//Find the new node in the graph
			auxn = *nit;	
			for (eit2 = auxn.edges.begin(); eit2 != auxn.edges.end(); eit2++) {	//Add all of its edges to iterate through
				Edge auxe = *eit2;
				edges.insert(auxe);
			}
		}

		edges.erase(eit1);	//Remove the edge from the set
		eit1 = edges.begin();	//Get another one
	} while(eit1 != edges.end());	//Repeat this till we run out of edges
	return forest;
}

//vector<int> fleury(Graph g) {	//To generate the eulerian trail
//}


int main(int argc, char* argv[]) {
	struct Graph graph;	//The graph
	ifstream file(argv[1]); //Input file as argument
	string edge_string; //To read lines on the file
	string subString; //The split the line into tokens
	int i; //Need this for iterators
	set<Node>::iterator nit; //To iterate and find on the set
	set<Edge, greater<Edge>>::iterator eit; //To iterate as well
	int first_node; //Nodes to use and construct while reading edges
	int second_node;
	int cost; //Benefit and cost to use while reading edges
	int benefit;	

	getline(file, edge_string); //Getting the first line, but number of nodes is not relevant
	getline(file, edge_string); //Getting the second line, number of edges also irrelevant
	istringstream it_sub_string(edge_string); //Initializing the string stream

 	while (getline(file, edge_string)) { //Now reading the edges
		it_sub_string.str(edge_string);
		it_sub_string.clear();

		if (isdigit(edge_string[0])) { //First we check if the first letter is a digit
			it_sub_string >> subString; //If it isn't we read the edges
			first_node = stoi(subString); //First node goes here
			it_sub_string >> subString;
			second_node = stoi(subString); //Second node goes here
			
			Edge auxe; //Create the actual edge
			it_sub_string >> subString; 
			auxe.cost = stoi(subString); //Cost first
			it_sub_string >> subString;
			auxe.benefit = stoi(subString); //Benefit second

			auxe.origin = first_node; //First origin
			auxe.destination = second_node; //Then, we create the edge for the first node, clearly goes to the second node

			nit = graph.nodes.find(Node(first_node)); //Find if the node is already on the graph
			if (nit == graph.nodes.end()) { //Let's say it isn't
				Node auxn = Node(first_node); //So we just create it
				auxn.edges.insert(auxe); //Add the edge to its adjacency list
				graph.nodes.insert(auxn); //Add the node to the graph
			}
			else { //But if it is
				Node auxn = *nit; //We copy it somewhere safe to edit
				auxn.edges.insert(auxe); //We add the edge to its adjacency list
				graph.nodes.erase(nit); //Erase the one we found
				graph.nodes.insert(auxn); //Replace with the new one
			}

			auxe.origin = second_node; //Now we do the same process with the other node
			auxe.destination = first_node;
			nit = graph.nodes.find(Node(second_node)); //This is because it is a non-directed graph
			if (nit == graph.nodes.end()) { //So technically every edge goes in both nodes it connects
				Node auxn = Node(second_node);
				auxn.edges.insert(auxe);
				graph.nodes.insert(auxn);
			}
			else {
				Node auxn = *nit;
				auxn.edges.insert(auxe);
				graph.nodes.erase(nit);
				graph.nodes.insert(auxn);
			}
		}
	}

	i = 0;
	for (nit = graph.nodes.begin(); nit != graph.nodes.end(); nit++) {
		Node auxn = *nit;
		for (eit = auxn.edges.begin(); eit != auxn.edges.end(); eit++) {
			Edge auxe = *eit;
			cout << i++ << ' ' << auxn.id << ' ' << auxe.destination << ' ' << auxe.cost << ' ' << auxe.benefit << ' ' << auxe.benefit - auxe.cost << '\n';
		}
	}

	struct Graph mst = prim(graph);

	i = 0;
	for (nit = mst.nodes.begin(); nit != mst.nodes.end(); nit++) {
		Node auxn = *nit;
		for (eit = auxn.edges.begin(); eit != auxn.edges.end(); eit++) {
			Edge auxe = *eit;
			cout << i++ << ' ' << auxn.id << ' ' << auxe.destination << ' ' << auxe.cost << ' ' << auxe.benefit << ' ' << auxe.benefit - auxe.cost << '\n';
		}
	}
}