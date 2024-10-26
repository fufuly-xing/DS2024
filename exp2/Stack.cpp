#include "Stack.hpp"
#include "heights.hpp"

int main() {
    char input[100];
    char* RPN = nullptr;

    cout << "请输入表达式: ";
    cin.getline(input, sizeof(input));

    try {
        float result = evaluate(input, RPN);
        cout << "计算结果: " << result << endl;
        delete[] RPN; // 释放动态分配的内存  
    }
    catch (const std::exception& e) {
        cout << "式子无效: " << e.what() << endl;
    }

    int n;
    std::cout << "请输入高度数组的长度: ";
    std::cin >> n; // 读取数组长度  

    std::vector<int> heights(n);
    std::cout << "请输入高度数组的元素: ";
    for (int i = 0; i < n; ++i) {
        std::cin >> heights[i]; // 读取数组元素  
    }

    int maxArea = largestRectangleArea(heights); // 计算最大矩形面积  
    std::cout << "最大矩形面积是: " << maxArea << std::endl; // 输出结果  

    for (int i = 0; i < 10; ++i) {
        int n = std::rand() % 10 + 1; // 生成1到10之间的随机数作为数组长度  
        std::vector<int> heights(n);

        for (int j = 0; j < n; ++j) {
            heights[j] = std::rand() % 105; // 生成0到104之间的随机数作为柱子高度  
        }

        int maxArea = largestRectangleArea(heights);
        std::cout << "Test case " << i + 1 << ": Max area = " << maxArea << std::endl;
    }
    return 0;
}