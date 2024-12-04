#include"graph.hpp"
int main() {
    // 创建图实例，假设图有 5 个节点（0 到 4）
    Graph g(5);

    // 添加边
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(3, 4);

    // 打印图结构
    cout << "Graph adjacency list:" << endl;
    g.printGraph();

    // 广度优先搜索（BFS）
    cout << "BFS starting from vertex 0: ";
    g.bfs(0);

    // 深度优先搜索（DFS）
    cout << "DFS starting from vertex 0: ";
    g.dfsWrapper(0);

    // 最短路径算法（Dijkstra）
    cout << "Dijkstra's shortest path starting from vertex 0:" << endl;
    g.dijkstra(0);

    // 最小生成树（Prim's Algorithm）
    cout << "Prim's Minimum Spanning Tree:" << endl;
    g.prim();

    return 0;
}
