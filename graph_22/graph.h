#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <queue>
#include <stack>
#include <set>
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
	cout << "time : " << time << " at: " << nodes[s].name << endl;
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
	cout << "time : " << time << " at: " << nodes[s].name << endl;
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
	for (int i = 0; i < num; i++) {
		cout << i << " first visit: " << nodes[i].m_d << ", last visit: " << nodes[i].m_f << endl;
	}
	idx.clear();
	std::vector<int> tmp;
	for (int i = 0; i <= time; i++) tmp.push_back(-1);
	for (int i = 0; i < num; i++) {
		tmp[nodes[i].m_f] = i;
	}
	for (int i = tmp.size()-1; i >= 0; i--)
		if(tmp[i] >= 0)
			idx.push_back(tmp[i]);
}



// 强连通分量
// 在G中可联通的强连通分量，在G^T中仍然是强连通分量,从最大的f开始遍历是为找到最大的联通

static void DFS_CUR_CONN(GRAPH& g, NODE* nodes, std::vector<int>& topo_order, int s, std::vector<int>& conn_v) {
	conn_v.push_back(s);
	nodes[s].m_color = COLOR::GRAY;
	if (s >= 0 && s < topo_order.size()) {
		for (int i = 0; i < topo_order.size(); i++) {
			if (g[s][topo_order[i]] < INFINIETE && COLOR::WHITE == nodes[topo_order[i]].m_color) {
				DFS_CUR_CONN(g, nodes, topo_order, topo_order[i], conn_v);
			}
		}
	}
	nodes[s].m_color = COLOR::BLACK;
}
/*
强连通分量： 
1、 构建拓扑排序（深搜）；第一个访问的，离开此节点的编号>所有依赖此节点的编号
2、 清除所有的状态标志位，并转置图（图的连接方向反转）
3、 再次，对转置图深搜，优先考虑拓扑排序编号大的节点，按照拓扑排序（离开节点时的编号）（从某个节点>到它最深的依赖节点）进行深搜，
找到的所有的连通的节点即为连通分量。
为什么可行：
	拓扑排序：展示了从一个节点开始，到它能访问的所有节点的顺序。对图转置，此时图的连接方向反转，若这个时候仍然能够访问到未转置前的节点，
	说明起始点与当前节点存在双向的连通的，因此访问到的点都是连通分量
*/
static void StrongConnectedComponent(GRAPH& g, NODE* nodes, int num, std::vector<std::vector<int>>& conn_comp) {
	std::vector<int> idx;
	TOPOLOGY_SORT(g, nodes, num, idx);
	for (int i = 0; i < idx.size(); i++)
		cout << idx[i] << " ";
	cout << endl;
	// compute G^t
	GRAPH g_t = g;
	for (int i = 0; i < g.size(); i++) {
		for (int j = 0; j < i; j++)
			std::swap(g_t[i][j], g_t[j][i]);
		nodes[i].m_color = WHITE;
	}
	conn_comp.clear();
	for (int i = idx.size() - 1; i >= 0; i--) {
		std::vector<int> v;
		if (COLOR::WHITE == nodes[idx[i]].m_color) {
			DFS_CUR_CONN(g_t, nodes, idx, idx[i], v);
		}
		if(v.size() > 0)
			conn_comp.push_back(v);
	}
}
///////////////////////////////////////////////
//最小生成树////////////////////////////////
// kruskal 每次选择最小边

void MST_KRUSKAL(GRAPH& g, NODE* nodes, int num, std::vector<std::pair<int, int>> A) {
	std::set<int> trees;
	for (int i = 0; i < num; i++) {
		trees.insert(i);
	}

}


#endif
