#include <vector >
#include <stack>  
#include <algorithm>  

int largestRectangleArea(std::vector<int>& heights) {
    std::stack<int> stk; // 用于存储索引的单调栈  
    std::vector<int> left(heights.size(), 0);  // 存储每个柱子左边第一个比它小的柱子的索引（或-1）  
    std::vector<int> right(heights.size(), heights.size()); // 存储每个柱子右边第一个比它小的柱子的索引（或n）  
    int maxArea = 0;

    // 计算每个柱子左边第一个比它小的柱子的索引  
    for (int i = 0; i < heights.size(); ++i) {
        while (!stk.empty() && heights[stk.top()] >= heights[i]) {
            stk.pop();
        }
        left[i] = stk.empty() ? -1 : stk.top();
        stk.push(i);
    }

    // 清空栈并计算每个柱子右边第一个比它小的柱子的索引  
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

    // 计算最大矩形面积  
    for (int i = 0; i < heights.size(); ++i) {
        int area = heights[i] * (right[i] - left[i] - 1);
        maxArea = std::max(maxArea, area);
    }

    return maxArea;
}