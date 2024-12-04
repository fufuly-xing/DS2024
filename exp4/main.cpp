#include"graph.hpp"
int main() {
    // ����ͼʵ��������ͼ�� 5 ���ڵ㣨0 �� 4��
    Graph g(5);

    // ��ӱ�
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(3, 4);

    // ��ӡͼ�ṹ
    cout << "Graph adjacency list:" << endl;
    g.printGraph();

    // �������������BFS��
    cout << "BFS starting from vertex 0: ";
    g.bfs(0);

    // �������������DFS��
    cout << "DFS starting from vertex 0: ";
    g.dfsWrapper(0);

    // ���·���㷨��Dijkstra��
    cout << "Dijkstra's shortest path starting from vertex 0:" << endl;
    g.dijkstra(0);

    // ��С��������Prim's Algorithm��
    cout << "Prim's Minimum Spanning Tree:" << endl;
    g.prim();

    return 0;
}
