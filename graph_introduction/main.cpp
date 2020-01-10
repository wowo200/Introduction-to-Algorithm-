#include <iostream>
#include <vector>
using namespace std;
#include "graph.h"

#define NODE_NUM 8

int main(int argc, char **argv) {
    NODE nodes[NODE_NUM];
    GRAPH g;
    for(int i = 0; i < NODE_NUM; i++)
        g.push_back(vector<float>(NODE_NUM, INFINIETE));
    nodes[0].name = "a";
    nodes[1].name = "b";
    nodes[2].name = "c";
    nodes[3].name = "d";
    nodes[4].name = "e";
    nodes[5].name = "f";
    nodes[6].name = "g";
    nodes[7].name = "h";
    int ralations[13][2] = {
        {0,1},
        {1,2},
		{1,4},
        {2,3},
        {2,6},
        {3,2},
        {3,7},
        {4,0},
        {4,5},
        {5,6},
        {6,5},
		{6,7},
		{7,7}
    };
	for (int i = 0; i < 13; i++)
		g[ralations[i][0]][ralations[i][1]] = 1;
    /*for(int i = 0; i < g.size(); i++){
        for(int j = 0; j < g[i].size(); j++){
            cout << g[i][j] << " ";
        }
        cout << endl;
    }
    int time = 0;
    DFS_STACK(g, nodes, 8, 1, time);
    print_path(nodes, 1, 7);*/
	std::vector<std::vector<int>> component;
	StrongConnectedComponent(g, nodes, 8, component);
	for (int i = 0; i < component.size(); i++) {
		cout << "component " << i << ": ";
		for (int j = 0; j < component[i].size(); j++)
			cout << component[i][j] << " ";
		cout << endl;
	}
	char c;
	cin >> c;
    return 0;
}
