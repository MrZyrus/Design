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
	int passes;

	bool operator> (const Edge &other) const {
		return benefit - cost > other.benefit - other.cost;
	}
};

struct Node {
	int id;
	multiset<Edge, greater<Edge>> edges;

	Node (int i) : id(i) { }

	bool operator< (const Node &other) const {
		return id < other.id;
	}
};

struct Graph {
	set<Node> nodes;

	bool operator< (const Graph &other) const {
		set<Node>::iterator it = nodes.begin();
		Node a = *it;
		it = other.nodes.begin();
		Node b = *it;
		return a < b;
	}
};

struct Graph add_edge_to_graph(struct Graph g, Edge e) {
	set<Node>::iterator it = g.nodes.find(Node(e.origin)); //Find if the node is already on the graph
	if (it == g.nodes.end()) { //Let's say it isn't
		Node n = Node(e.origin); //So we just create it
		n.edges.insert(e); //Add the edge to its adjacency list
		g.nodes.insert(n); //Add the node to the graph
	}
	else { //But if it is
		Node n = *it; //We copy it somewhere safe to edit
		n.edges.insert(e); //We add the edge to its adjacency list
		g.nodes.erase(it); //Erase the one we found
		g.nodes.insert(n); //Replace with the new one
	}

	swap(e.origin, e.destination); //Now we do the same process with the other node
	it = g.nodes.find(e.origin); //This is because it is a non-directed graph
	if (it == g.nodes.end()) { //So technically every edge goes in both nodes it connects
		Node n = Node(e.origin);
		n.edges.insert(e);
		g.nodes.insert(n);
	}
	else {
		Node n = *it;
		n.edges.insert(e);
		g.nodes.erase(it);
		g.nodes.insert(n);
	}
	return g;
}

set<Graph> r_components(Graph g) {
	set<Node> nodes_left;
	set<Node>::iterator nit;
	multiset<Edge, greater<Edge>>::iterator eit;
	Node auxn = Node(1);
	Edge auxe;
	vector<Node> stack;

	for (nit = g.nodes.begin(); nit != g.nodes.end(); nit++) {
		auxn = *nit;
		nodes_left.insert(auxn);
	}

	nit = nodes_left.begin();
	while (nit != nodes_left.end()) {
		auxn = *nit;
		stack.push_back(auxn);
		while (!stack.empty()) {

		}
	} 
}

vector<int> trail(Graph g) {
	vector<int> cycle;	//Cycle to return, just using a dynamic array
	cycle.push_back(0);	//The first element is the total gain so far of this cycle
	cycle.push_back(1);	//We always start from node 1

	int old_benefit;	//Gotta save the old benefit to update the edges
	int old_destination;	//Saving the old destination to find the edges to update
	set<Node>::iterator nit;	//Need to iterate through nodes as we build the cycle
	multiset<Edge, greater<Edge>>::iterator eit;	//Also need to iterate through the edges
	nit = g.nodes.begin();	//We begin on the first node, the depot
	Node auxn = *nit;	
	eit = auxn.edges.begin();	//And on the first edge of it, they're already sorted by gain
	Edge auxe = *eit;

	do {	//Always happens at least once
		if (auxe.passes < 2) {	//Edges can only be travelled through at most twice
			cycle.push_back(auxe.destination);	//If it hasn't be travelled twice, we pick it, add it to the cycle
			cycle[0] += auxe.benefit - auxe.cost;	//Update the gain

			old_benefit = auxe.benefit;	//Save the old benefit
			auxe.benefit = 0;	//Update the benefit to 0, since it just got travelled, notice this happens regardless of passes
			auxe.passes++;	//Update the passes, notice this only happens at most twice with every edge

			auxn.edges.erase(eit);	//Update the edge in the node
			auxn.edges.insert(auxe);

			g.nodes.erase(nit);	//Update the node in the graph
			g.nodes.insert(auxn);

			swap(auxe.origin, auxe.destination);	//Swap the origin and destination, to update it on the other node

			nit = g.nodes.find(Node(auxe.origin));	//Find the other node
			auxn = *nit;

			old_destination = auxe.destination;	//We save the old destination to find the correct node, since
			auxe.benefit = old_benefit;	//Restore the original edge, this is for purposes of finding it
			eit = auxn.edges.find(auxe);	//Then once restored, we find it
			auxe = *eit;
			while (auxe.destination != old_destination) {	//Since the nodes are found using only benefit/cost
				eit++;	//We have to make sure we get the right node comparing the destination
				auxe = *eit;	//We conveniently saved previously
			}

			auxe.benefit = 0;	//We update the benefit again to 0
			auxe.passes++;	//Update the passes on it as well
			auxn.edges.erase(eit);	//We update the edge on the node
			auxn.edges.insert(auxe);

			g.nodes.erase(nit);	//Update the node in the grapph
			g.nodes.insert(auxn);

			nit = g.nodes.find(auxn);	//Find the new node, it was already updated
			auxn = *nit;
			eit = auxn.edges.begin();	//Find the next edge to follow through
			auxe = *eit;
		}
		
		else {
			eit++; //Of course, if it already has 2 passes, we just go to the next edge
			auxe = *eit;
		}
	} while (auxn.id != 1 || auxe.benefit - 2 * auxe.cost > 0);
	//We do this till we return to the first node and it has no remaining edge with benefit - 2*cost > 0;
	
	if (cycle[0] < 0) {	//Of course, once we find a cycle, if it has negative gain
		vector<int> no_path;	//We just construct a path where it never leaves and give a 0 gain as answer
		no_path.push_back(0);
		no_path.push_back(1);
		return no_path;
	}
	
	else {
		return cycle;
	}
}


int main(int argc, char* argv[]) {
	struct Graph graph;	//The graph
	struct Graph r_graph; //Graph with ONLY edges that comply with benefit - 2 * cost > 0
	ifstream file(argv[1]); //Input file as argument
	string edge_string; //To read lines on the file
	string subString; //The split the line into tokens

	getline(file, edge_string); //Getting the first line, but number of nodes is not relevant
	getline(file, edge_string); //Getting the second line, number of edges also irrelevant
	istringstream it_sub_string(edge_string); //Initializing the string stream

 	while (getline(file, edge_string)) { //Now reading the edges
		it_sub_string.str(edge_string);
		it_sub_string.clear();

		if (isdigit(edge_string[0])) { //First we check if the first letter is a digit
			Edge auxe; //Create the actual edge
			it_sub_string >> subString; //If it isn't we read the edges
			auxe.origin = stoi(subString); //First node goes here
			it_sub_string >> subString;
			auxe.destination = stoi(subString); //Second node goes here
			it_sub_string >> subString; 
			auxe.cost = stoi(subString); //Cost first
			it_sub_string >> subString;
			auxe.benefit = stoi(subString); //Benefit second
			auxe.passes = 0;

			if (auxe.benefit - 2 * auxe.cost >= 0) { //If the edge belongs to R then add it to the R graph
				r_graph = add_edge_to_graph(r_graph, auxe);
			}

			graph =	add_edge_to_graph(graph, auxe);
		}
	}


	for (set<Node>::iterator it = graph.nodes.begin(); it != graph.nodes.end(); it++) {
		Node n = *it;
		for (multiset<Edge, greater<Edge>>::iterator eit = n.edges.begin(); eit != n.edges.end(); eit++) {
			Edge e = *eit;
			cout << e.origin << ' ' << e.destination << ' ' << e.cost << ' ' << e.benefit << ' ' << e.passes << '\n';
		}	
	}

	cout << '\n';

	for (set<Node>::iterator it = r_graph.nodes.begin(); it != r_graph.nodes.end(); it++) {
		Node n = *it;
		for (multiset<Edge, greater<Edge>>::iterator eit = n.edges.begin(); eit != n.edges.end(); eit++) {
			Edge e = *eit;
			cout << e.origin << ' ' << e.destination << ' ' << e.cost << ' ' << e.benefit << ' ' << e.passes << '\n';
		}	
	}

	vector<int> cycle = trail(graph);
	for (int i = 0; i < cycle.size(); i++) {
		cout << cycle[i++] << ' ';
	}
	cout << '\n';
}