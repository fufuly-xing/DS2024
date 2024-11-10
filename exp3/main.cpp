#include "HuffTree.h"
#include <iostream>
#include <unordered_map>
#include <cstring>

// �������ǵ��ַ����� 26 ����ĸ����Сд������
#define N_CHAR 26

std::unordered_map<char, std::string> huffmanTable;

int* statistics(char* sample_text_file) {
    int* freq = new int[N_CHAR];
    memset(freq, 0, sizeof(int) * N_CHAR);

    FILE* fp = fopen(sample_text_file, "r");
    if (fp == nullptr) {
        std::cerr << "Error: Could not open file " << sample_text_file << std::endl;
        return nullptr;
    }

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch >= 'a' && ch <= 'z') {
            freq[ch - 'a']++;
        }
        if (ch >= 'A' && ch <= 'Z') {
            freq[ch - 'A']++;
        }
    }

    fclose(fp);
    return freq;
}

void travPre_R(BinNodePosi(HuffChar) x, std::string code = "") {
    if (!x) return;

    // Ҷ�ӽڵ㣬�洢����
    if (!x->lc && !x->rc) {
        huffmanTable[x->data.ch] = code;
        return;
    }

    // �����ݹ������������������
    if (x->lc) travPre_R(x->lc, code + '0');
    if (x->rc) travPre_R(x->rc, code + '1');
}

int main() {
    char file[45] = "dream.txt";  // ���� dream.txt ����
    int* freq = statistics(file);  // ͳ���ļ��и��ַ��ĳ���Ƶ��
    HuffForest* forest = initForest(freq); delete freq;
    HuffTree* tree = generateTree(forest); delete forest;

    // ��ʼ�������
    travPre_R(tree->root());  // ���� Huffman �����ɱ����

    std::cout << "�����������Ϊ:" << std::endl;
    for (const auto& i : huffmanTable) {
        std::cout << i.first << ": " << i.second << std::endl;
    }

    // �� "dream" �� "my" ���б���
    std::string words[] = { "dream", "my" };
    for (const auto& word : words) {
        std::cout << word << " �Ĺ���������Ϊ: ";
        for (char ch : word) {
            ch = tolower(ch);  // ת��ΪСд��ĸ
            std::cout << huffmanTable[ch] << " ";  // ���ÿ���ַ��Ĺ���������
        }
        std::cout << std::endl;
    }

    return 0;
}
