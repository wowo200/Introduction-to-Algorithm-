#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <algorithm>
#include <stdio.h>

#define NIL -1

#define INFINIETE 1e+6

enum COLOR{
    WHITE,
    GRAY,
    BLACK
};

struct NODE{
    COLOR m_color;
    std::string name;
    int m_d; // the rank of the begin of visiting this node
    int m_f; // the rank of the end of visiting this node
    int m_pie;
    NODE(){
        m_color = WHITE;
        m_pie = NIL;
        m_d = 0;
    }
};

typedef vector<vector<float>> GRAPH;

static void BFS(GRAPH& g, NODE* nodes, int num, int s){
    std::queue<int> que;
    nodes[s].m_d = 0;
    nodes[s].m_pie = NIL;
    nodes[s].m_color = GRAY;
    que.push(s);
//     for(int i = 0; i < num; i++){
//         if(nodes[i].m_color == WHITE)
//         {
//             que.push(i);
//             nodes[i].m_color = GRAY;
//             nodes[i].m_d = 0;
//             nodes[i].m_pie = NIL;
//         }
    while(!que.empty()){
        int cur_i = que.front();
        cout << cur_i << "-node: " << nodes[cur_i].name << " depth: "<< nodes[cur_i].m_d << " pre node: " << nodes[cur_i].m_pie << endl;
        for(int j = 0; j < num; j++){
            if(g[cur_i][j] < INFINIETE && COLOR::WHITE == nodes[j].m_color ){
                nodes[j].m_color = GRAY;
                nodes[j].m_d = nodes[cur_i].m_d + 1;
                nodes[j].m_pie = cur_i;
                que.push(j);
            }
        }
        cout << "queue size : " << que.size() << endl;
        nodes[cur_i].m_color = BLACK;
        que.pop();
    }
//     }
}

static void print_path(/*GRAPH g, */NODE* nodes, int s, int v){
    if(s == v){
        printf("%s", nodes[v].name.c_str());
    }
    else if(nodes[v].m_pie == NIL){
        printf("NIL");
    }
    else{
        print_path(nodes, s, nodes[v].m_pie);
        printf("->%s", nodes[v].name.c_str());
    }
}

static void DFS_CUR(GRAPH& g, NODE* nodes, int num, int s, int& time){
    time += 1;
    nodes[s].m_color = COLOR::GRAY;
    nodes[s].m_d = time;
    if( s >= 0 && s < num){
        for(int i = 0; i < num; i++){
            if(g[s][i] < INFINIETE && COLOR::WHITE == nodes[i].m_color){
                // nodes[i].m_d = nodes[s].m_d + 1;
                nodes[i].m_pie = s;
                DFS_CUR(g, nodes, num, i, time);
            }
        }
    }
    nodes[s].m_color = COLOR::BLACK;
    time += 1;
    nodes[s].m_f = time;
}

static void DFS_STACK(GRAPH& g, NODE* nodes, int num, int s, int& time){
    std::stack<int> sta;
    sta.push(s);
    while(!sta.empty()){
        int cur_i = sta.top();
        time += 1;
        nodes[cur_i].m_d = time;
        nodes[cur_i].m_color = COLOR::GRAY;
        bool has_node_unvisit = false;
        for(int i = num -1 ; i >= 0; i--){
            if(g[cur_i][i] < INFINIETE && COLOR::WHITE == nodes[i].m_color){
                sta.push(i);
                nodes[i].m_pie = cur_i;
                has_node_unvisit = true;
                // break;
            }
        }
        if(!has_node_unvisit){
            cur_i = sta.top();
            sta.pop();
            time += 1;
            nodes[cur_i].m_f = time;
            nodes[cur_i].m_color = COLOR::BLACK;
            
        }
    }
}

// static void DFS(GRAPH& g, NODE* nodes, int num, int s, int& time){
//     nodes[s].m_d = 0;
//     nodes[s].m_pie = NIL;
//     DFS_STACK(g, nodes, num, s, time);
// }

static void TOPOLOGY_SORT(GRAPH& g, NODE* nodes, int num, std::vector<int>& idx){
    int time = 0;
    for(int i = 0; i < num; i++){
        if(nodes[i].m_color == COLOR::WHITE){
            DFS_CUR(g, nodes, num, i, time);
        }
    }
    idx.resize(num);
    for(int i = 0; i < num; i++)
        idx[i] = i;
        
    struct cmp{
        bool operator ()(int i, int j){
            return nodes[i].m_f < nodes[j].m_f;
        }
    }
    std::sort(idx.begin(), idx.end(), cmp);
}

#endif
