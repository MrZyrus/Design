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
	int explored;
	multiset<Edge, greater<Edge>> edges;

	Node (int i) : id(i), explored(0) { }

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
	set<Node> nodes_left;	//First, we need a set of all the nodes to explore
	set<Node>::iterator nit;	//Gotta iterate through them
	vector<Node> stack;	//Stack to use for DFS
	set<Graph> components;	//All Components so far found
	Graph connected;	//Current connected component found
	int new_component;	//Just to check if we found a new component or not

	for (set<Node>::iterator it = g.nodes.begin(); it != g.nodes.end(); it++) {	//First, add all the nodes of the graph to check
		Node n = *it;
		nodes_left.insert(n);
	}

	for (set<Node>::iterator it = nodes_left.begin(); it != nodes_left.end(); it++) {	//We iterate through them
		Node n = *it;
		nit = g.nodes.find(n);	//Find it in the graph, this is to check if it has been explored
		n = *nit;

		if (!n.explored) {	//If it hasn't been explored, add it to the stack to dfs
			stack.push_back(n);
		}

		while (!stack.empty()) {	//DFS through it
			new_component = 1;	//Since we're exploring it, we have found a new component
			n = stack.back();	//So we pop the first element from the stack
			stack.pop_back();
			connected.nodes.insert(n);	//We insert it into the connected component 

			for (multiset<Edge, greater<Edge>>::iterator eit = n.edges.begin(); eit != n.edges.end(); eit++) {	//Next we iterate through its edges
				Edge e = *eit;
				nit = g.nodes.find(Node(e.destination));	//Find it on the graph, check if it has been explored before
				Node n2 = *nit;
				if (!n2.explored) {	//If it hasn't, it goes to the stack, gotta DFS through as well
					stack.push_back(n2);
				}
			}
			nit = g.nodes.find(n);	//Once it's on the stack, have to mark it as explored
			n.explored = 1;
			g.nodes.erase(nit);	//Updating it on the graph
			g.nodes.insert(n);
		}

		if (new_component) {	//If we have found a new component, add it to the components set
			components.insert(connected);
			connected.nodes.clear();
			new_component = 0;	//Reset new component marker
		}	
	}

	return components;	//Once all connected components have been found, return it
}

int path_cost(vector<int> path, Graph g) {	//This one just takes a path and returns the gain from it
	int cost = 0;	//Initialize the cost
	for (int i = 0; i < path.size() - 1; i++) {	//Iterate through the path
		set<Node>::iterator nit = g.nodes.find(Node(path[i]));	//Gotta find the node
		Node n = *nit;
		multiset<Edge, greater<Edge>>::iterator eit = n.edges.begin();	//Then gotta find the edge to the next node
		Edge e = *eit;
		while (e.destination != path[i + 1]) {
			eit++;
			e = *eit;
		}
		cost += e.benefit - e.cost;	//Once we get it, update the cost
		e.benefit = 0;	//Update the edge, because the path might contain the same edge twice
		n.edges.erase(eit);
		n.edges.insert(e);
		g.nodes.erase(nit);
		g.nodes.insert(n);

		swap(e.origin, e.destination);	//Of course, non-directed graph, gotta update both edges in both nodes
		nit = g.nodes.find(Node(e.origin));
		n = *nit;
		eit = n.edges.begin();
		e = *eit;
		while (e.destination != path[i]) {
			eit++;
			e = *eit;
		}
		e.benefit = 0;
		n.edges.erase(eit);
		n.edges.insert(e);
		g.nodes.erase(nit);
		g.nodes.insert(n);
	}
	return cost;	//Once that's all done, return the cost of that path, it might be negative it's a loss
}

vector<int> negative_cycle_elimination(vector<int> p, Graph g) {	//This looks for a cycle in a path and eliminates it if it has negative gain
	for (int i = 2; i < p.size() - 2; i++) {	//First, iterate through the path
		for (int j = i + 1; j < p.size() - 1; j++) {	//Then find the cycle in it 
			if (p[i] == p[j]) {	//Once it's found we check its cost
 				vector<int> cycle;
				cycle.clear();
				for (int k = i; k < j + 1; k++) {	//Gotta copy the cycle into another path
					cycle.push_back(p[k]);
				}
				int gain = path_cost(cycle, g);	//Checking its cost
				if (gain < 0) {	//If the cost is negative
					p.erase(p.begin()+i, p.begin()+j);	//We delete the cycle from the path
					p[0] -= gain;	//Update the new gain with the cycle deleted
					i = 2;	//Since we altered the main array, gotta start the iteration again
					j = i + 1;
				}
			}
		}
	}
	return p;
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
	Node n = *nit;	
	eit = n.edges.begin();	//And on the first edge of it, they're already sorted by gain
	Edge e = *eit;
	Graph old_g = g;

	do {	//Always happens at least once
		if (e.passes < 2) {	//Edges can only be travelled through at most twice
			cycle.push_back(e.destination);	//If it hasn't be travelled twice, we pick it, add it to the cycle
			cycle[0] += e.benefit - e.cost;	//Update the gain

			old_benefit = e.benefit;	//Save the old benefit
			e.benefit = 0;	//Update the benefit to 0, since it just got travelled, notice this happens regardless of passes
			e.passes++;	//Update the passes, notice this only happens at most twice with every edge

			n.edges.erase(eit);	//Update the edge in the node
			n.edges.insert(e);

			g.nodes.erase(nit);	//Update the node in the graph
			g.nodes.insert(n);

			swap(e.origin, e.destination);	//Swap the origin and destination, to update it on the other node

			nit = g.nodes.find(Node(e.origin));	//Find the other node
			n = *nit;

			old_destination = e.destination;	//We save the old destination to find the correct node, since
			e.benefit = old_benefit;	//Restore the original edge, this is for purposes of finding it
			eit = n.edges.find(e);	//Then once restored, we find it
			e = *eit;
			while (e.destination != old_destination) {	//Since the nodes are found using only benefit/cost
				eit++;	//We have to make sure we get the right node comparing the destination
				e = *eit;	//We conveniently saved previously
			}

			e.benefit = 0;	//We update the benefit again to 0
			e.passes++;	//Update the passes on it as well
			n.edges.erase(eit);	//We update the edge on the node
			n.edges.insert(e);

			g.nodes.erase(nit);	//Update the node in the grapph
			g.nodes.insert(n);

			nit = g.nodes.find(n);	//Find the new node, it was already updated
			n = *nit;
			eit = n.edges.begin();	//Find the next edge to follow through
			e = *eit;
		}
		
		else {
			eit++; //Of course, if it already has 2 passes, we just go to the next edge
			e = *eit;
		}
		
	} while (n.id != 1 || e.benefit - 2 * e.cost >= 0);
	//We do this till we return to the first node and it has no remaining edge with benefit - 2*cost > 0;
	
	if (cycle[0] < 0) {	//Of course, once we find a cycle, if it has negative gain
		cycle = negative_cycle_elimination(cycle, old_g);	//We gotta improve it, can't give a negative answer
		if (cycle[0] < 0) {	//If it's still negative, we just return a 0 as answer
			vector<int> no_path;
			no_path.push_back(0);
			no_path.push_back(1);
			return no_path;
		}
	}
	return cycle;
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
			Edge e; //Create the actual edge
			it_sub_string >> subString; //If it isn't we read the edges
			e.origin = stoi(subString); //First node goes here
			it_sub_string >> subString;
			e.destination = stoi(subString); //Second node goes here
			it_sub_string >> subString; 
			e.cost = stoi(subString); //Cost first
			it_sub_string >> subString;
			e.benefit = stoi(subString); //Benefit second
			e.passes = 0;

			if (e.benefit - 2 * e.cost >= 0) { //If the edge belongs to R then add it to the R graph
				r_graph = add_edge_to_graph(r_graph, e);
			}

			graph =	add_edge_to_graph(graph, e);
		}
	}

	vector<int> cycle = trail(graph);
	for (int i = 0; i < cycle.size(); i++) {
		cout << cycle[i] << ' ';
	}
	cout << '\n';

	cycle = negative_cycle_elimination(cycle, graph);

	for (int i = 0; i < cycle.size(); i++) {
		cout << cycle[i] << ' ';
	}
	cout << '\n';
}
