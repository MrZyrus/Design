#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream> 
#include <queue>

using namespace std;

struct Edges {
	int benefit;
	int cost;
	int origin;
	int destination;
};


struct Graph {
	struct Edges *edges;
	int vertex_counter;
	int edge_counter;
	int required_edges;
	int non_required_edges;
};

int djisktra(int origin, int destination, Graph graph) {

};

int main(int argc, char* argv[]) {
	struct Graph graph;
	ifstream file (argv[1]);
	string edge_string;
	string subString;
	int i = 0; //iterator

	getline(file, edge_string);
	istringstream it_sub_string(edge_string);

	while(it_sub_string) {
		it_sub_string >> subString;
	}
	graph.vertex_counter = stoi(subString);

	getline(file, edge_string);  //For getting the number of edges
	it_sub_string.str(edge_string);
	it_sub_string.clear();

	while(it_sub_string) {
		it_sub_string >> subString;
	}
	graph.required_edges = stoi(subString);
	graph.edge_counter = graph.required_edges;
	graph.edges = new Edges[graph.edge_counter];

 	while (getline(file, edge_string)) {
		it_sub_string.str(edge_string);
		it_sub_string.clear();
		if (isdigit(edge_string[0])) {
				it_sub_string >> subString;
				graph.edges[i].origin = stoi(subString);
				it_sub_string >> subString;
				graph.edges[i].destination = stoi(subString);
				it_sub_string >> subString;
				graph.edges[i].cost = stoi(subString);
				it_sub_string >> subString;
				graph.edges[i].benefit = stoi(subString);
				i++;
		}

		else {
			while(it_sub_string) {
				it_sub_string >> subString;
			}
			graph.non_required_edges = stoi(subString);
			graph.edge_counter += graph.non_required_edges;
			graph.edges = (Edges*)realloc(graph.edges, sizeof(Edges) * (graph.edge_counter));
		}
	}

	 required_nodes = new int[graph.required_edges*2];
	i = 0;
	while(i < graph.required_edges) {
		required_nodes[i] = graph.edges[i].origin;
		required_nodes
	}

	i = 0;
	while(i < graph.edge_counter){
		cout << i << ' ';
		cout << graph.edges[i].origin << ' ' << graph.edges[i].destination << ' ' << graph.edges[i].cost << ' ' << graph.edges[i].benefit << '\n';		
		i++;
	}	
}