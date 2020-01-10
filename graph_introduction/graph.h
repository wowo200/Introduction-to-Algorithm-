#ifndef GRAPH_H
#define GRAPH_H

#include <list>
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
/*
GENERIC-MST(G, w)
1. A = empty
2. while A does not form a spanning tree
3.    find an edge (u,v) that is safe for A
4.    A = A + (u,v)
5. return A

kruskal prim 的技巧在于选择安全边
*/

// kruskal 选择安全边 的办法是：在所有连接森林中，两颗不同树的里面，找到权重最小的（u，v）。
// 如果返回 false， 则表示无法构成一颗树

static float MST_KRUSKAL(GRAPH& g, NODE* nodes, int num, std::vector<std::pair<int, int>>& A) {
    A.clear();
    struct W{
        int i, j; // i -> j
        float weight;
        W(int _i, int _j, float _w):
            i(_i), j(_j), weight(_w){}
    };
    struct cmp_W{
        bool operator()(const W& w1, const W& w2){
            return w1.weight < w2.weight;
            
        }
    };
    std::priority_queue<W, std::vector<W>, cmp_W> w_bt_trees;
    std::vector<int> idx_trees;
	for (int i = 0; i < num; i++) {
		idx_trees.push_back(i);
	}
	float sum = 0;
    for(int i = 0; i < g.size(); i++)
        for(int j = 0; j < g[i].size(); j++)
            if(g[i][j] < INFINIETE)
                w_bt_trees.push(W(i,j, g[i][j]));
    while(idx_trees.size() > 1){
        W edge = w_bt_trees.top();
        if( idx_trees[edge.i] != idx_trees[edge.j] ){
            int tmp_idx = edge.i;
            // do union
            while(idx_trees[tmp_idx] != tmp_idx)
                tmp_idx = idx_trees[tmp_idx];
            idx_trees[edge.j] = tmp_idx;
            A.push_back(std::make_pair (edge.i, edge.j));
            sum += edge.weight;
        }
        w_bt_trees.pop();
    }
    return sum;
}


// prim 算法保证集合A中的边总是构成一棵树。 这棵树从任意节点开始，一直生长到覆盖所有节点为止。
//      选择安全边： 选择一条集合A中的结点和集合A之外的结点构成的边

static float MST_PRIM ( GRAPH& g, NODE* nodes, int num, std::vector<std::pair<int, int>>& A )
{
    A.clear();
    float sum = 0;
    std::set<int> _A_set;
    _A_set.insert ( 0 );
    struct W {
        int i, j; // i is in A; j is not in A
        float weight;
        W ( int _i, int _j, float _w ) :
            i ( _i ), j ( _j ), weight ( _w ) {}
    };
    struct cmp_W {
        bool operator() ( const W& w1, const W& w2 )
        {
            return w1.weight < w2.weight;

        }
    };
    std::priority_queue<W, std::vector<W>, cmp_W> _q;
    for(int i = 0; i < g[0].size(); i++){
        if(g[0][i] < INFINIETE)
            _q.push(W(0, i, g[0][i]));
    }
    while ( !_q.empty() ) {
        W edge = _q.top();
        if( _A_set.find(edge.j) == _A_set.end()){
            _A_set.insert(edge.j);
            sum += edge.weight;
            A.push_back( std::make_pair (edge.i, edge.j));
            // 将edge.j的临接矩阵插入进去
            for(int i = 0; i < g[edge.j].size(); i++){
                if(_A_set.find(i) == _A_set.end())
                    _q.push(W(edge.j, i, g[edge.j][i]));
            }
        }
        _q.pop();
    }
}


/**
 * 单源最短路径
 * 
 * 最短路径最优子结构：设路径P=<v0, v1, v2, ..., vk>,从节点v0->vk的最短路径，vi和vj是P经过的某两个点，
 * 那么，子路径Pij也是从i->j的最短路径
 * 算法的区别在于：对每条边进行松弛的次数和松弛边的次序不同。
 *  Dijkstra， 用于有向无环图，对每条边松弛一次
 *  Bellman-Ford算法，对每条边松弛|V|-1次
*/
static void INITIAL_SINGLE_SOURCE(GRAPH& g, NODE* nodes, int num, int s){
    for(int i = 0; i < num; i++){
        nodes[i].m_d = INFINIETE; // distance
        nodes[i].m_pie = NIL; // pre node
    }
    nodes[s].m_d = 0;
}


static void RELAX( int u, int v, GRAPH& g, NODE* nodes, int num){
    if(nodes[v].m_d > nodes[u].m_d + g[u][v]){
        nodes[v].m_d = nodes[u].m_d + g[u][v];
        nodes[v].m_pie = u;
    }
}

// Bellman-Ford算法  是一般情况下的单源最短路径
// 边的权重可以存在负值，但若存在负值的环路， 则会不满足：u->v, u.d < v.d
// Bellman-Ford算法执行|V|-1次循环：因为从S出发的到任意一点终点，最多经历 |V|-1 条边，最多迭代|V|-1 次
// 假定G 包含负值的环路，且该环路可以从源结点s到达，设该环路为c=<v_0, v_1, ... , v_k>，这里v_0=v_k，
// 则有Simga w(v_i-1, v_i) < 0, 则 v_i.d <= v_i-1.d +  w(v_i-1, v_i)

static bool BELLMAN_FORD(GRAPH& g, NODE* nodes, int num, int s){
    INITIAL_SINGLE_SOURCE(g, nodes, num, s);
    for(int i = 0; i < num -1; i++){
        for(int m = 0; m < g.size(); m++){
            for(int n = 0; n < g[m].size(); n ++){
                if(g[m][n] < INFINIETE){
                    RELAX(m, n, g, nodes, num);
                }
            }
        }
    }
    for(int m = 0; m < g.size(); m++){
        for(int n = 0; n < g[m].size(); n ++){
            if(g[m][n] < INFINIETE && nodes[n].m_d < nodes[m].m_d + g[m][n])
                return false;
        }
    }
    return true;
}


//有向无环图的单源最短路径
//Thita(V + E)
// 思路： 与Bellman方法相似，只是由于图中没有环路，因此可以构建拓扑排序（每节点之前的节点不存在入的边）
//       我们只需要按照次序，遍历一遍即可

static void DAG_SHORTTEST_PATHS(GRAPH& g, NODE* nodes, int num, int s){
    std::vector<int> idx;
    TOPOLOGY_SORT(g, nodes, num, idx);
    INITIAL_SINGLE_SOURCE(g, nodes, num, s);
    for (int i = 0; i < idx.size(); i++){
        for(int j = 0; j < g[i].size(); j++){
            if(g[i][j] < INFINIETE){
                RELAX(i,j, g, nodes, num);
            }
        }
    }
}


// Dijkstra 算法
// 思路： 算法维护一组结点集合S，算法重复从结点集合V-S中选择最短路径估计最小的节点u，
//       将u加入到集合S，然后对所有从u发出的边进行松弛。
static void DIJKSTRA(GRAPH& g, NODE* nodes, int num, int s){
    INITIAL_SINGLE_SOURCE(g, nodes, num, s);
    // std::set<int> S;
    // S.insert(s);
    struct W{
        NODE* node_ptr;
        int u;
        W(NODE* node, int _i):node_ptr(node), u(_i){}
    };
    struct CMP_W{
        bool operator()(const W& w1, const W& w2){
            return w1.node_ptr->m_d< w2.node_ptr->m_d;
        }
    } cmp_obj;
    std::list<W> _q;
    for(int i = 0; i < num; i++)
        _q.push_back(W(nodes + i, i));
    while(!_q.empty()){
        // find min dist
        std::list<W>::iterator it = std::min_element(_q.begin(), _q.end(), cmp_obj);
        for(int i = 0; i < num; i++){
            if(g[it->u][i] < INFINIETE  && nodes[i].m_d > nodes[it->u].m_d + g[it->u][i])
            {
                RELAX(it->u, i, g, nodes, num);
                // _q.push(W(nodes + , i)); // relax operation
            }
        }
    }
}


// 所有节点对的最短路径问题
// 从i到j的最短路径(k-1)，只考虑{v1,v2,...，vk-1}作为中间结点
//   因为节点K不是路径，并同过K连接两个路径p1,p2。
// 动态规划：
//    最优子结构：d_ij^(k) = w_ij  if k == 0
//              d_ij^(k) = min(d_ij^(k-1), d_ik^(k-1) + d_kj^(k-1))
void FLOYD_WARSHALL(GRAPH& g, NODE* nodes, int num){
    GRAPH W_i, W_i_1(g);
    std::vector<std::vector<int>> pies;
    for(int i = 0; i < g.size(); i++){
        pies.push_back(std::vector<int>());
        for(int j = 0; j < g[i].size(); j++){
            pies[i][j]=i;
        }
    }
    
    for(int k = 0; k < num; k++){
        for(int i = 0; i < g.size(); i ++)
        {
            for(int j = 0; j < g[i].size(); j++)
            {
                if(W_i_1[i][k] + W_i_1[k][j] < W_i_1[i][j]){
                    W_i[i][j] = W_i_1[i][k] + W_i_1[k][j];
                    pies[i][j] = k;
                }
                else{
                    W_i[i][j] = W_i_1[i][j];
                }
            }
        }
        W_i_1 = W_i;
    }
}




#endif
