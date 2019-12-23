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
    nodes[0].name = "r";
    nodes[1].name = "s";
    nodes[2].name = "t";
    nodes[3].name = "u";
    nodes[4].name = "v";
    nodes[5].name = "w";
    nodes[6].name = "x";
    nodes[7].name = "y";
    int ralations[10][2] = {
        {0,4},
        {0,1},
        {1,5},
        {2,3},
        {2,5},
        {2,6},
        {3,6},
        {3,7},
        {5,6},
        {6,7},
    };
    for(int i = 0; i < 10; i++)
    {
        g[ralations[i][0]][ralations[i][1]] = 1;
        g[ralations[i][1]][ralations[i][0]] = 1;
    }
    for(int i = 0; i < g.size(); i++){
        for(int j = 0; j < g[i].size(); j++){
            cout << g[i][j] << " ";
        }
        cout << endl;
    }
    int time = 0;
    DFS_STACK(g, nodes, 8, 1, time);
    print_path(nodes, 1, 7);
    return 0;
}
