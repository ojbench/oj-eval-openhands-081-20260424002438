#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>

using namespace std;

const int MAXN = 3005;

int n, m;
vector<int> adj[MAXN];
int cap[MAXN][MAXN];
int flow[MAXN][MAXN];
int parent[MAXN];
vector<pair<int,int>> edges;

inline bool bfs(int s, int t) {
    fill(parent + 1, parent + n + 1, -1);
    parent[s] = s;
    queue<int> q;
    q.push(s);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        if (u == t) return true;
        
        for (int v : adj[u]) {
            if (parent[v] == -1 && cap[u][v] > flow[u][v]) {
                parent[v] = u;
                q.push(v);
            }
        }
    }
    
    return false;
}

int max_flow(int s, int t) {
    // Reset flows only for existing edges
    for (auto& e : edges) {
        flow[e.first][e.second] = flow[e.second][e.first] = 0;
    }
    
    int total = 0;
    
    while (bfs(s, t)) {
        // Find min capacity
        int path_flow = 1000000;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, cap[u][v] - flow[u][v]);
        }
        
        // Update flow
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            flow[u][v] += path_flow;
            flow[v][u] -= path_flow;
        }
        
        total += path_flow;
    }
    
    return total;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin >> n >> m;
    
    memset(cap, 0, sizeof(cap));
    memset(flow, 0, sizeof(flow));
    
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
        cap[a][b] = cap[b][a] = 1;
        edges.push_back({a, b});
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
