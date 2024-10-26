#define _CRT_SECURE_NO_WARNINGS
#ifndef M_PI  
#define M_PI 3.14159265358979323846  
#endif
#include "Vector.hpp"
#include <stdio.h> 
#include <string.h> 
#include <iostream>  
#include <string>  
#include <cctype>  
#include <cmath>  
#include <stdexcept>  
#include <sstream> 
#include <unordered_map>

template <typename T> class Stack : public Vector<T> {
public:
    void push(T const& e) { Vector<T>::insert(Vector<T>::size(), e); }
    T pop() { return Vector<T>::remove(Vector<T>::size() - 1); }
    T& top() { return(*this)[Vector<T>::size() - 1]; }
};

#define N_OPTR 11 //���������
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE, SIN, LOG,} Operator; //���������
//�ӡ������ˡ������˷����׳ˡ������š������š���ʼ������ֹ��

const char pri[N_OPTR][N_OPTR] = { //��������ȵȼ� [ջ��] [��ǰ]
    /* |-------------------- �� ǰ �� �� �� --------------------| */
             /* +     -    *    /    ^    !    (    )   \0   sin  log*/
    /* --  + */ '>', '>', '<', '<', '<', '<', '<', '>', '>', '<', '<',
    /* |   - */ '>', '>', '<', '<', '<', '<', '<', '>', '>', '<', '<',
    /* ջ  * */ '>', '>', '>', '>', '<', '<', '<', '>', '>', '<', '<',
    /* ��  / */ '>', '>', '>', '>', '<', '<', '<', '>', '>', '<', '<',
    /* ��  ^ */ '>', '>', '>', '>', '>', '<', '<', '>', '>', '>', '>',
    /* ��  ! */ '>', '>', '>', '>', '>', '>', ' ', '>', '>', '>', '>',
    /* ��  ( */ '<', '<', '<', '<', '<', '<', '<', '=', ' ', '<', '<',
    /* |   ) */ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    /* -- \0 */ '<', '<', '<', '<', '<', '<', '<', ' ', '=', '<', '<',
    /* --sin */ '>', '>', '>', '>', '>', '>', '<', '>', '>', '>', '<',
    /* --log */ '>', '>', '>', '>', '>', '>', '<', '>', '>', '>', '<'
};

Operator optr2rank(char op)
{
    if (op == '+') return ADD;
    else if (op == '-') return SUB;
    else if (op == '*') return MUL;
    else if (op == '/') return DIV;
    else if (op == '^') return POW;
    else if (op == '!') return FAC;
    else if (op == '(') return L_P;
    else if (op == ')') return R_P;
    else if (op == 's' || op == 'c' || op == 't') return SIN; // ��������ַ�ӳ�䵽 SIN  
    else if (op == 'l') return LOG;
    else if (op == '\0') return EOE;
    else  exit(-1); // δ֪�ַ�  
   
} 

char orderBetween(char op1, char op2) //�Ƚ�����������T��m���ȼ�
{
    return pri[optr2rank(op1)][optr2rank(op2)];
}

void convert(Stack<char>& S, __int64 n, int base)
{
    static char digit[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    while (n > 0)
    {
        int remainder = (int)(n % base);
        S.push(digit[remainder]);
        n /= base;
    }
}

bool paren(const char exp[], Rank lo, Rank hi) { //����ʽ����ƥ���飬�ɼ����������
    Stack<char> S; //ʹ��ջ��¼�ѷ��ֵ���δƥ���������
    for (Rank i = lo; i <= hi; i++) /* ��һ��鵱ǰ�ַ� */
        switch (exp[i]) { //������ֱ�ӽ�ջ������������ջ��ʧ�䣬�����ʽ�ز�ƥ��
        case '(': case '[': case '{': S.push(exp[i]); break;
        case ')': if ((S.empty()) || ('(' != S.pop())) return false; break;
        case ']': if ((S.empty()) || ('[' != S.pop())) return false; break;
        case '}': if ((S.empty()) || ('{' != S.pop())) return false; break;
        default: break; //�������ַ�һ�ɺ���
        }
    return S.empty(); //����ջ�գ����ҽ���ƥ��
}

bool paren(const char exp[])
{
    return paren(exp, 0, strlen(exp) - 1);
}

// �ж��ַ��Ƿ�Ϊ����  
bool isDigit(char c) {
    return isdigit(c);
}

void append(char*& R, float a)
{
    if (R == nullptr)
    {
        R = new char[1];
        R[0] = '\0';
    }
    char* b = new char[50];
    sprintf(b, "%.2f", a); // ��float����ת��Ϊchar��
    size_t ol = strlen(R);
    size_t nl = ol + strlen(b) + 1;
    char* nS = new char[nl];
    strcpy(nS, R);
    strcat(nS, b);
    R = nS;
}
// ���ַ�׷�ӵ��ַ����У������ַ�����������  
void append(char*& RPN, char c) {
    static int len = 0; // ��̬���������ڸ��ٵ�ǰRPN�ַ����ĳ���  
    static char* buffer = nullptr; // ��̬�����������ڴ洢RPN�ַ���  
    buffer = (char*)realloc(buffer, len + 2); // ���·����ڴ棬+2��Ϊ��'\0'�Ϳ��ܵ���һ���ַ�  
    buffer[len++] = c;
    buffer[len] = '\0';
    RPN = buffer;
}

void readNumber(char*& p, Stack<float>& stk) {
    std::ostringstream oss;

    // ��ȡ��������  
    while (isdigit(*p)) {
        oss << *p++;
    }

    // ��ȡС������  
    if (*p == '.') {
        oss << '.';
        while (isdigit(*(++p))) {
            oss << *p;
        }
    }

    // ���Խ��ַ���ת��Ϊ������  
    try {
        stk.push(std::stof(oss.str()));
    }
    catch (const std::invalid_argument&) {
        throw std::runtime_error("Invalid number format.");
    }
    catch (const std::out_of_range&) {
        throw std::runtime_error("Number out of range.");
    }

    // ����Ƿ�Ϊ����  
    if (*p == 's' || *p == 'c' || *p == 't' || *p == 'l') {
        std::string funcName;
        while (isalnum(*(p + 1))) { // �����������ĵ�һ���ַ�����Ϊ���Ѿ�������  
            funcName += *(++p);
        }

        // ��������  
        if (funcName == "sin") {
            if (stk.empty()) throw std::runtime_error("Stack is empty for sin.");
            stk.push(sin(stk.pop()));
        }
        else if (funcName == "cos") {
            if (stk.empty()) throw std::runtime_error("Stack is empty for cos.");
            stk.push(cos(stk.pop()));
        }
        else if (funcName == "tan") {
            if (stk.empty()) throw std::runtime_error("Stack is empty for tan.");
            stk.push(tan(stk.pop()));
        }
        else if (funcName == "log") {
            if (stk.empty()) throw std::runtime_error("Stack is empty for log.");
            float operand = stk.pop();
            if (operand <= 0) throw std::runtime_error("Logarithm of non-positive number.");
            stk.push(log(operand));
        }
        else {
            throw std::runtime_error("Unknown function name.");
        }
    }
}

float d_transform(float x) { return x * (M_PI / 180.00); }

float calcu(float operand1, char op, float operand2) {
    switch (op) {
    case '+': return operand1 + operand2;
    case '-': return operand1 - operand2;
    case '*': return operand1 * operand2;
    case '/': return operand2 != 0.0f ? operand1 / operand2 : INFINITY; // ע��ʹ�� 0.0f ��ȷ������Ƚ�  
    case '^': return pow(operand1, operand2);
    default: cout << "δ֪������: " << op << endl; exit(-1);
    }
}

float calcu(char op, float operand) {
    switch (op)
    {
    case ('!'):
    {
        float r = 1.0;
        while (operand > 1)
        {
            r *= operand--;
        }
        return r;
    }
    case ('s'):
        return sin(d_transform(operand));
    case ('c'):
        return cos(d_transform(operand));
    case ('t'):
        return tan(d_transform(operand));
    case ('l'):
        return logf(operand);
    default:
        cout << "δ֪������: " << op << endl; exit(-1);
    }
}



float evaluate(char* S, char*& RPN)
{
    if (!paren(S)) { cerr << "�������Ų�ƥ��" << endl; exit(-1); }
    Stack<float> opnd;
    Stack<char> optr;
    optr.push('\0');
    // cout<<opnd.top()<<endl;
    while (!optr.empty())
    {
        if (isdigit(*S))
        {
            readNumber(S, opnd);
            append(RPN, opnd.top());
        }
        else
            switch (orderBetween(optr.top(), *S))
            {
            case '<':
                optr.push(*S);
                if (isalpha(static_cast<Rank>(*S)))
                {
                    while (isalpha(static_cast<Rank>(*S)))// ���Sָ�����Ǻ�������log
                        S++;
                }
                else
                    S++;
                break;
            case '=':
                optr.pop();
                if (*S) S++;
                break;
            case '>':
            {
                char op = optr.pop();
                append(RPN, op);
                if ('!' == op|| 's' == op || 'c' == op || 't' == op || 'l' == op)
                {
                    float p0pnd = opnd.pop();
                    opnd.push(calcu(op, p0pnd));
                }
                else
                {
                    float p0pnd2 = opnd.pop(), p0pnd1 = opnd.pop();
                    opnd.push(calcu(p0pnd1, op, p0pnd2));
                }
                break;
            }
            default:exit(-1);
            }
    }
    return opnd.pop();
}

