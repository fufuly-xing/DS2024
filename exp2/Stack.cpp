#include "Stack.hpp"
#include "heights.hpp"

int main() {
    char input[100];
    char* RPN = nullptr;

    cout << "��������ʽ: ";
    cin.getline(input, sizeof(input));

    try {
        float result = evaluate(input, RPN);
        cout << "������: " << result << endl;
        delete[] RPN; // �ͷŶ�̬������ڴ�  
    }
    catch (const std::exception& e) {
        cout << "ʽ����Ч: " << e.what() << endl;
    }

    int n;
    std::cout << "������߶�����ĳ���: ";
    std::cin >> n; // ��ȡ���鳤��  

    std::vector<int> heights(n);
    std::cout << "������߶������Ԫ��: ";
    for (int i = 0; i < n; ++i) {
        std::cin >> heights[i]; // ��ȡ����Ԫ��  
    }

    int maxArea = largestRectangleArea(heights); // �������������  
    std::cout << "�����������: " << maxArea << std::endl; // ������  

    for (int i = 0; i < 10; ++i) {
        int n = std::rand() % 10 + 1; // ����1��10֮����������Ϊ���鳤��  
        std::vector<int> heights(n);

        for (int j = 0; j < n; ++j) {
            heights[j] = std::rand() % 105; // ����0��104֮����������Ϊ���Ӹ߶�  
        }

        int maxArea = largestRectangleArea(heights);
        std::cout << "Test case " << i + 1 << ": Max area = " << maxArea << std::endl;
    }
    return 0;
}