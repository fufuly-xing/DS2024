#include <iostream>
#include <ctime>
#include <iomanip>
#include <cmath>
#include <random>
#include <algorithm> 
#include "Vector.cpp"
using namespace std;

class Complex {
public:
    double real;
    double imag;

    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    double getmodulus() const { return sqrt(real * real + imag * imag); }
};
bool operator>(const Complex a, const Complex b)
{
    return ((a.getmodulus() > b.getmodulus()) ||(a.getmodulus() == b.getmodulus() &&a.real > b.real)); // 优先比较向量模，若模相同比较实部
}
bool operator==(const Complex a, const Complex b) {
    return (a.real==b.real&&a.imag==b.imag);
}
bool operator<(const Complex a, const Complex b)
{
    return ((a.getmodulus() < b.getmodulus()) ||(a.getmodulus() == b.getmodulus() && a.real < b.real));
}
bool operator!=(const Complex a, const Complex b)
{

    return (a.real != b.real) || (a.imag != b.imag);
}
bool operator<=(const Complex a, const Complex b)
{
    return ((a.getmodulus() <= b.getmodulus()) ||
        (a.getmodulus() == b.getmodulus() && a.real <= b.real));
}
bool operator>=(const Complex a, const Complex b)
{

    return ((a.getmodulus() >= b.getmodulus()) ||
        (a.getmodulus() == b.getmodulus() && a.real >= b.real));
}
std::ostream& operator<<(std::ostream& os, const Complex& cn) {
    os << std::fixed << std::setprecision(2) << cn.real << (cn.imag >= 0 ? "+" : "") << cn.imag << "i";
    return os;
}
Complex randomcomp(double min, double max) {
    static std::default_random_engine generator(std::random_device{}()); 
    static std::uniform_real_distribution<double> distribution(min, max);
    double real = distribution(generator);
    double imag = distribution(generator);
    return Complex(real, imag);
}
template <typename T> void Vector<T>:: print() {
    for(int i=0;i<10;i++)
        cout << _elem[i]<< endl;
}
// 测试排序效率
void test_time(Vector<Complex>& v)
{
    Vector<Complex> v1 = v;
    Vector<Complex> v2 = v;
    clock_t start, end;
    double spendtime;
    // 冒泡排序
    start = clock();
    Vector<Complex> bubbleSort(v1, 0, v1.size());
    end = clock();
    spendtime = ((double)(end - start) / CLOCKS_PER_SEC);
    cout << "Bubble Sort took " << spendtime << " seconds" << endl;
    // 归并排序
    start = clock();
    Vector<Complex> mergeSort(v2, 0, v2.size());
    end = clock();
    spendtime = ((double)(end - start) / CLOCKS_PER_SEC);
    cout << "Merge Sort took " << spendtime << " seconds" << endl;
}
// 斐波那契查找
int fibsearch(Vector<Complex>& A, double e)
{
    int lo = 0, hi = A.size();
    Fib fib(hi - lo);
    while (lo < hi)
    {
        while (hi - lo < fib.get())
            fib.prev();
        Rank mi = lo + fib.get() - 1;
        if (e < A[mi].getmodulus())
            hi = mi;
        else if (e > A[mi].getmodulus())
            lo = mi + 1;
        else
            return mi;
    }
    return -1;
}
// 区间查找
Vector<Complex> findInRange(Vector<Complex>& v, double m1, double m2)
{
    Vector<Complex> res;
    int lo = fibsearch(v, m1);
    // cout << lo << endl;
    int hi = fibsearch(v, m2);
    // cout << hi << endl;
    for (int i = lo; i < hi + 1; i++)
    {
        res.insert(v[i]);
    }
    return res;
}
int main()
{
    const int SAMPLE_NUMBER = 10;
    Vector<Complex> cv;
    // 生成一个无序的复数向量  
    for (int i = 0; i < SAMPLE_NUMBER; ++i) {
        Complex rc = randomcomp(-10.0, 10.0);
        cv.insert(rc); // 使用 push_back 而不是 insert  
    }
    cout << "The original complex vector is :" << endl;
    cv.print();
    cout << endl;
    // 置乱
    Vector<Complex> uncv = cv;
    uncv.unsort();
    cout << "置乱 :" << endl;
    uncv.print();
    cout << endl;
    // 查找
    cout << "查找 :" << endl;
    cout<<cv.find(Complex(1.02,2.12))<<endl;
    cout << endl;
    //插入
    cout << "插入 :" << endl;
    Vector<Complex> icv = cv;
    icv.insert(3, Complex(1, 4));
    icv.print();
    cout << endl;
    //删除
    cout << "删除区间元素 :" << endl;
    Vector<Complex> rcv2 = cv;
    rcv2.remove(2,9);
    rcv2.print();
    cout << endl;
    //唯一化
    cout << "唯一化 :" << endl;
    Vector<Complex> ucv2 = cv;
    ucv2.uniquify();
    ucv2.print();
    cout << endl;
    //乱序
    cout << "乱序的时间 :" << endl;
    Vector<Complex> lcv = cv;
    test_time(lcv);
    cout << endl;
    //顺序
    cout << "顺序的时间 :" << endl;
    Vector<Complex> scv = cv;
    test_time(scv);
    cout << endl;
    //逆序
    cout << "逆序的时间 :" << endl;
    Vector<Complex> ncv = cv;
    ncv.reverse();
    test_time(ncv);
    cout << endl;
    //区间查找
    cout << "区间查找 :" << endl;
    Vector<Complex> mcv = cv;
    mcv.sort();
    double m1 = mcv[3].getmodulus();
    double m2 = mcv[8].getmodulus();
    Vector<Complex> res = findInRange(mcv, m1, m2);
    cout << "The range complex vector is :" << endl;
    for (int i = m1; i <= m2; i++) {
        cout <<res[i]<< endl;
    }

    return 0;
}
