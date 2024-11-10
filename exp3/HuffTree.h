#include "BinTree.h"
#include "List.cpp"
#include "Bitmap.h"
#include <iostream>

#define N_CHAR (0x80 - 0x20)
struct HuffChar {
    char ch;         // 字符
    int weight;      // 频率
    // 构造函数
    HuffChar(char c = '^', int w = 0) : ch(c), weight(w) {};
    // 比较器，重载 `<` 运算符
    bool operator< (HuffChar const& hc) {
        return weight > hc.weight;
    }
    // 重载 `==` 运算符
    bool operator== (HuffChar const& hc) {
        return weight == hc.weight;
    }
};

#define HuffTree BinTree<HuffChar> //Huffman树，由BinTree派生，节点类型为HuffChar
typedef List<HuffTree*> HuffForest;//Huffman森林
typedef Bitmap HuffCode;//Huffman二进制编码

HuffForest* initForest(int* freq) { // 根据频率统计表，为每个字符创建一棵树
    HuffForest* forest = new HuffForest;  // 创建一个新的 Huffman 森林，假设 HuffForest 是 List 类型
    for (int i = 0; i < N_CHAR; i++) {  // 为每个字符创建 Huffman 树
        if (freq[i] > 0) {  // 只为频率大于零的字符创建树
            HuffTree* tree = new HuffTree;  // 创建一个新的 Huffman 树
            tree->insertAsRoot(HuffChar(0x20 + i, freq[i]));  // 插入字符和其频率作为树的根节点
            forest->insertAsLast(tree);  // 将树插入到 HuffForest 中
        }
    }
    return forest;
}


HuffTree* minHChar(HuffForest* forest) {
    ListNodePosi(HuffTree*) p = forest->first();  // 从森林的首节点开始
    ListNodePosi(HuffTree*) minChar = p;  // 初始为首节点
    int minWeight = p->data->root()->data.weight;  // 当前最小权重

    // 遍历森林中的每个节点，寻找权重最小的 Huffman 树
    while (forest->valid(p = p->succ)) {  // 遍历所有节点
        // 如果当前树的权重小于已知的最小权重
        if (minWeight > p->data->root()->data.weight) {
            minWeight = p->data->root()->data.weight;  // 更新最小权重
            minChar = p;  // 更新最小权重树所在节点
        }
    }

    // 从森林中移除并返回找到的最小权重 Huffman 树
    return forest->remove(minChar);
}


HuffTree* generateTree(HuffForest* forest) {
    // 构建 Huffman 树直到森林中只剩一棵树
    while (forest->size() > 1) {
        // 取出权重最小的两棵树
        HuffTree* T1 = minHChar(forest);
        HuffTree* T2 = minHChar(forest);

        // 创建新树，作为这两棵树的父节点
        HuffTree* S = new HuffTree();
        S->insertAsRoot(HuffChar('^', T1->root()->data.weight + T2->root()->data.weight));  // 根节点权重是两棵树的权重之和

        // 将两棵树作为新树的左子树和右子树
        S->attachAsLC(S->root(), T1);
        S->attachAsRC(S->root(), T2);

        // 将合并后的树插入森林
        forest->insertAsLast(S);
    }
    // 返回森林中唯一剩下的树，即 Huffman 编码树
    return forest->first()->data;
}



