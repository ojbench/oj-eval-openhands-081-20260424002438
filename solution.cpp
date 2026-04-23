#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 3005;
const int INF = 1e9;

struct Edge {
    int to, cap, flow;
};

class MaxFlow {
private:
    vector<Edge> edges;
    vector<int> graph[MAXN];
    int level[MAXN];
    int iter[MAXN];
    int n;
    
    void add_edge_internal(int from, int to, int cap) {
        graph[from].push_back(edges.size());
        edges.push_back({to, cap, 0});
        graph[to].push_back(edges.size());
        edges.push_back({from, 0, 0});
    }
    
    bool bfs(int s, int t) {
        memset(level, -1, sizeof(level));
        queue<int> q;
        level[s] = 0;
        q.push(s);
        
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int id : graph[v]) {
                Edge& e = edges[id];
                if (level[e.to] < 0 && e.cap > e.flow) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        
        return level[t] >= 0;
    }
    
    int dfs(int v, int t, int pushed) {
        if (v == t || pushed == 0) return pushed;
        
        for (int& cid = iter[v]; cid < (int)graph[v].size(); cid++) {
            int id = graph[v][cid];
            Edge& e = edges[id];
            
            if (level[v] + 1 != level[e.to] || e.cap <= e.flow)
                continue;
            
            int tr = dfs(e.to, t, min(pushed, e.cap - e.flow));
            if (tr > 0) {
                e.flow += tr;
                edges[id ^ 1].flow -= tr;
                return tr;
            }
        }
        
        return 0;
    }
    
public:
    MaxFlow(int n) : n(n) {}
    
    void add_edge(int from, int to, int cap) {
        add_edge_internal(from, to, cap);
    }
    
    long long max_flow(int s, int t) {
        // Reset flows
        for (auto& e : edges) {
            e.flow = 0;
        }
        
        long long flow = 0;
        while (bfs(s, t)) {
            memset(iter, 0, sizeof(iter));
            while (int pushed = dfs(s, t, INF)) {
                flow += pushed;
            }
        }
        return flow;
    }
    
    void clear() {
        edges.clear();
        for (int i = 0; i <= n; i++) {
            graph[i].clear();
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<pair<int, int>> edges;
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        edges.push_back({a, b});
    }
    
    long long total = 0;
    
    for (int a = 1; a <= n; a++) {
        for (int b = a + 1; b <= n; b++) {
            MaxFlow mf(n);
            for (auto& e : edges) {
                mf.add_edge(e.first, e.second, 1);
                mf.add_edge(e.second, e.first, 1);
            }
            long long flow = mf.max_flow(a, b);
            total += flow;
        }
    }
    
    cout << total << endl;
    
    return 0;
}
