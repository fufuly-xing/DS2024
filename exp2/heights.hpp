#include <vector >
#include <stack>  
#include <algorithm>  

int largestRectangleArea(std::vector<int>& heights) {
    std::stack<int> stk; // ���ڴ洢�����ĵ���ջ  
    std::vector<int> left(heights.size(), 0);  // �洢ÿ��������ߵ�һ������С�����ӵ���������-1��  
    std::vector<int> right(heights.size(), heights.size()); // �洢ÿ�������ұߵ�һ������С�����ӵ���������n��  
    int maxArea = 0;

    // ����ÿ��������ߵ�һ������С�����ӵ�����  
    for (int i = 0; i < heights.size(); ++i) {
        while (!stk.empty() && heights[stk.top()] >= heights[i]) {
            stk.pop();
        }
        left[i] = stk.empty() ? -1 : stk.top();
        stk.push(i);
    }

    // ���ջ������ÿ�������ұߵ�һ������С�����ӵ�����  
    while (!stk.empty()) {
        stk.pop();
    }
    for (int i = heights.size() - 1; i >= 0; --i) {
        while (!stk.empty() && heights[stk.top()] >= heights[i]) {
            stk.pop();
        }
        right[i] = stk.empty() ? heights.size() : stk.top();
        stk.push(i);
    }

    // �������������  
    for (int i = 0; i < heights.size(); ++i) {
        int area = heights[i] * (right[i] - left[i] - 1);
        maxArea = std::max(maxArea, area);
    }

    return maxArea;
}