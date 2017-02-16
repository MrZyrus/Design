#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream> 


using namespace std;

struct Edges {
	int benefit;
	int cost;
	int origin;
	int destination;
};


struct Graph {
	struct Edges *edges;
};


int main(int argc, char* argv[]) {
	string edge_string;
	ifstream file (argv[1]);
	string subString;
	int i = 0; //iterator

	getline(file, edge_string);  //Number of vertices is not necessary
	getline(file, edge_string);  //For getting the number of edges
	istringstream it_sub_string(edge_string);

	while(it_sub_string) {
		it_sub_string >> subString;
	}

	int ed = stoi(subString); //Get the number of edges
	struct Graph graph;
	graph.edges = new Edges[ed];

	while (getline(file,edge_string)){
		if (isdigit(edge_string[0])){
				cout << edge_string;
				printf("\n");
				istringstream it_sub_string(edge_string);
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
	}
}