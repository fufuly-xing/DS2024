#include "HuffTree.h"
#include <iostream>
#include <unordered_map>
#include <cstring>

// 假设我们的字符集是 26 个字母，大小写不区分
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

    // 叶子节点，存储编码
    if (!x->lc && !x->rc) {
        huffmanTable[x->data.ch] = code;
        return;
    }

    // 继续递归遍历左子树和右子树
    if (x->lc) travPre_R(x->lc, code + '0');
    if (x->rc) travPre_R(x->rc, code + '1');
}

int main() {
    char file[45] = "dream.txt";  // 假设 dream.txt 存在
    int* freq = statistics(file);  // 统计文件中各字符的出现频率
    HuffForest* forest = initForest(freq); delete freq;
    HuffTree* tree = generateTree(forest); delete forest;

    // 初始化编码表
    travPre_R(tree->root());  // 根据 Huffman 树生成编码表

    std::cout << "哈夫曼编码表为:" << std::endl;
    for (const auto& i : huffmanTable) {
        std::cout << i.first << ": " << i.second << std::endl;
    }

    // 对 "dream" 和 "my" 进行编码
    std::string words[] = { "dream", "my" };
    for (const auto& word : words) {
        std::cout << word << " 的哈夫曼编码为: ";
        for (char ch : word) {
            ch = tolower(ch);  // 转换为小写字母
            std::cout << huffmanTable[ch] << " ";  // 输出每个字符的哈夫曼编码
        }
        std::cout << std::endl;
    }

    return 0;
}
