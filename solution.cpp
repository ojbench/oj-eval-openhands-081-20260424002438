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
int parent[MAXN];
int parent_edge[MAXN];
bool visited[MAXN];

void add_edge(int from, int to, int cap) {
    adj[from].push_back({to, cap, 0, (int)adj[to].size()});
    adj[to].push_back({from, cap, 0, (int)adj[from].size() - 1});
}

bool bfs(int s, int t) {
    fill(visited, visited + n + 1, false);
    fill(parent, parent + n + 1, -1);
    queue<int> q;
    q.push(s);
    visited[s] = true;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int i = 0; i < adj[u].size(); i++) {
            Edge& e = adj[u][i];
            if (!visited[e.to] && e.cap > e.flow) {
                visited[e.to] = true;
                parent[e.to] = u;
                parent_edge[e.to] = i;
                if (e.to == t) return true;
                q.push(e.to);
            }
        }
    }
    
    return false;
}

int max_flow(int s, int t) {
    // Reset all flows
    for (int i = 1; i <= n; i++) {
        for (auto& e : adj[i]) {
            e.flow = 0;
        }
    }
    
    int total_flow = 0;
    
    while (bfs(s, t)) {
        int path_flow = INF;
        
        // Find minimum capacity along the path
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            int edge_idx = parent_edge[v];
            path_flow = min(path_flow, adj[u][edge_idx].cap - adj[u][edge_idx].flow);
        }
        
        // Update flow along the path
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            int edge_idx = parent_edge[v];
            adj[u][edge_idx].flow += path_flow;
            adj[v][adj[u][edge_idx].rev].flow -= path_flow;
        }
        
        total_flow += path_flow;
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
