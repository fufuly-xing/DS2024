#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <climits>

using namespace std;

class Graph {
private:
    // 使用邻接表来存储图
    vector<list<int>> adjList;

public:
    Graph(int vertices) {
        adjList.resize(vertices);
    }

    // 添加边
    void addEdge(int v, int w) {
        adjList[v].push_back(w);
        adjList[w].push_back(v);  // 无向图的情况下
    }

    // 打印图（用于调试）
    void printGraph() {
        for (int i = 0; i < adjList.size(); ++i) {
            cout << "Vertex " << i << ": ";
            for (auto v : adjList[i]) {
                cout << v << " ";
            }
            cout << endl;
        }
    }

    void bfs(int start) {
        vector<bool> visited(adjList.size(), false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int vertex = q.front();
            cout << vertex << " ";
            q.pop();

            for (auto neighbor : adjList[vertex]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        cout << endl;
    }
    void dfs(int start, vector<bool>& visited) {
        visited[start] = true;
        cout << start << " ";

        for (auto neighbor : adjList[start]) {
            if (!visited[neighbor]) {
                dfs(neighbor, visited);
            }
        }
    }

    void dfsWrapper(int start) {
        vector<bool> visited(adjList.size(), false);
        dfs(start, visited);
        cout << endl;
    }
    void dijkstra(int start) {
        vector<int> dist(adjList.size(), INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        dist[start] = 0;
        pq.push({ 0, start });

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            // 遍历邻居节点
            for (auto neighbor : adjList[u]) {
                int weight = 1;  // 假设每条边的权重为 1
                if (dist[u] + weight < dist[neighbor]) {
                    dist[neighbor] = dist[u] + weight;
                    pq.push({ dist[neighbor], neighbor });
                }
            }
        }

        cout << "Shortest distances from node " << start << ": " << endl;
        for (int i = 0; i < dist.size(); ++i) {
            cout << "Node " << i << ": " << dist[i] << endl;
        }
    }
    void prim() {
        vector<int> key(adjList.size(), INT_MAX);
        vector<bool> inMST(adjList.size(), false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        key[0] = 0;  // 假设从第 0 个节点开始
        pq.push({ 0, 0 });

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            inMST[u] = true;

            for (auto neighbor : adjList[u]) {
                int weight = 1;  // 假设每条边的权重为 1
                if (!inMST[neighbor] && weight < key[neighbor]) {
                    key[neighbor] = weight;
                    pq.push({ key[neighbor], neighbor });
                }
            }
        }

        cout << "Minimum Spanning Tree: " << endl;
        for (int i = 0; i < key.size(); ++i) {
            cout << "Node " << i << ": " << key[i] << endl;
        }
    }
};