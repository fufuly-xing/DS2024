#include "BinTree.h"
#include "List.cpp"
#include "Bitmap.h"
#include <iostream>

#define N_CHAR (0x80 - 0x20)
struct HuffChar {
    char ch;         // �ַ�
    int weight;      // Ƶ��
    // ���캯��
    HuffChar(char c = '^', int w = 0) : ch(c), weight(w) {};
    // �Ƚ��������� `<` �����
    bool operator< (HuffChar const& hc) {
        return weight > hc.weight;
    }
    // ���� `==` �����
    bool operator== (HuffChar const& hc) {
        return weight == hc.weight;
    }
};

#define HuffTree BinTree<HuffChar> //Huffman������BinTree�������ڵ�����ΪHuffChar
typedef List<HuffTree*> HuffForest;//Huffmanɭ��
typedef Bitmap HuffCode;//Huffman�����Ʊ���

HuffForest* initForest(int* freq) { // ����Ƶ��ͳ�Ʊ�Ϊÿ���ַ�����һ����
    HuffForest* forest = new HuffForest;  // ����һ���µ� Huffman ɭ�֣����� HuffForest �� List ����
    for (int i = 0; i < N_CHAR; i++) {  // Ϊÿ���ַ����� Huffman ��
        if (freq[i] > 0) {  // ֻΪƵ�ʴ�������ַ�������
            HuffTree* tree = new HuffTree;  // ����һ���µ� Huffman ��
            tree->insertAsRoot(HuffChar(0x20 + i, freq[i]));  // �����ַ�����Ƶ����Ϊ���ĸ��ڵ�
            forest->insertAsLast(tree);  // �������뵽 HuffForest ��
        }
    }
    return forest;
}


HuffTree* minHChar(HuffForest* forest) {
    ListNodePosi(HuffTree*) p = forest->first();  // ��ɭ�ֵ��׽ڵ㿪ʼ
    ListNodePosi(HuffTree*) minChar = p;  // ��ʼΪ�׽ڵ�
    int minWeight = p->data->root()->data.weight;  // ��ǰ��СȨ��

    // ����ɭ���е�ÿ���ڵ㣬Ѱ��Ȩ����С�� Huffman ��
    while (forest->valid(p = p->succ)) {  // �������нڵ�
        // �����ǰ����Ȩ��С����֪����СȨ��
        if (minWeight > p->data->root()->data.weight) {
            minWeight = p->data->root()->data.weight;  // ������СȨ��
            minChar = p;  // ������СȨ�������ڽڵ�
        }
    }

    // ��ɭ�����Ƴ��������ҵ�����СȨ�� Huffman ��
    return forest->remove(minChar);
}


HuffTree* generateTree(HuffForest* forest) {
    // ���� Huffman ��ֱ��ɭ����ֻʣһ����
    while (forest->size() > 1) {
        // ȡ��Ȩ����С��������
        HuffTree* T1 = minHChar(forest);
        HuffTree* T2 = minHChar(forest);

        // ������������Ϊ���������ĸ��ڵ�
        HuffTree* S = new HuffTree();
        S->insertAsRoot(HuffChar('^', T1->root()->data.weight + T2->root()->data.weight));  // ���ڵ�Ȩ������������Ȩ��֮��

        // ����������Ϊ��������������������
        S->attachAsLC(S->root(), T1);
        S->attachAsRC(S->root(), T2);

        // ���ϲ����������ɭ��
        forest->insertAsLast(S);
    }
    // ����ɭ����Ψһʣ�µ������� Huffman ������
    return forest->first()->data;
}



