#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 3005;
const int INF = 1e9;

struct Edge {
    int to, cap, flow, rev;
};

int n, m;
vector<Edge> adj[MAXN];
int level[MAXN];
int iter[MAXN];
vector<pair<int, int>> modified_edges;

void add_edge(int from, int to, int cap) {
    adj[from].push_back({to, cap, 0, (int)adj[to].size()});
    adj[to].push_back({from, cap, 0, (int)adj[from].size() - 1});
}

bool bfs(int s, int t) {
    fill(level, level + n + 1, -1);
    queue<int> q;
    level[s] = 0;
    q.push(s);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int i = 0; i < adj[u].size(); i++) {
            Edge& e = adj[u][i];
            if (level[e.to] < 0 && e.cap > e.flow) {
                level[e.to] = level[u] + 1;
                q.push(e.to);
            }
        }
    }
    
    return level[t] >= 0;
}

int dfs(int u, int t, int pushed) {
    if (u == t || pushed == 0) return pushed;
    
    for (int& i = iter[u]; i < adj[u].size(); i++) {
        Edge& e = adj[u][i];
        
        if (level[u] + 1 != level[e.to] || e.cap <= e.flow)
            continue;
        
        int tr = dfs(e.to, t, min(pushed, e.cap - e.flow));
        if (tr > 0) {
            e.flow += tr;
            adj[e.to][e.rev].flow -= tr;
            modified_edges.push_back({u, i});
            modified_edges.push_back({e.to, e.rev});
            return tr;
        }
    }
    
    return 0;
}

int max_flow(int s, int t) {
    // Reset only modified flows
    for (auto& p : modified_edges) {
        adj[p.first][p.second].flow = 0;
    }
    modified_edges.clear();
    
    int total_flow = 0;
    
    while (bfs(s, t)) {
        fill(iter, iter + n + 1, 0);
        while (int pushed = dfs(s, t, INF)) {
            total_flow += pushed;
        }
    }
    
    return total_flow;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin >> n >> m;
    
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        add_edge(a, b, 1);
    }
    
    long long total = 0;
    
    for (int a = 1; a <= n; a++) {
        for (int b = a + 1; b <= n; b++) {
            total += max_flow(a, b);
        }
    }
    
    cout << total << endl;
    
    return 0;
}
