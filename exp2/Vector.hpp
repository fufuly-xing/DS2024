#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
using namespace std;
using Rank = unsigned int; //秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）


template <typename T> class Vector { //向量模板类
protected:
	Rank _size; Rank _capacity;  T* _elem; //规模、容量、数据区
	void copyFrom(T const* A, Rank lo, Rank hi); //复制数组区间A[lo, hi)
	void expand(); //空间不足时扩容
	void shrink(); //装填因子过小时压缩
	bool bubble(Rank lo, Rank hi); //扫描交换
	void bubbleSort(Rank lo, Rank hi); //起泡排序算法
	Rank maxItem(Rank lo, Rank hi); //选取最大元素
	void selectionSort(Rank lo, Rank hi); //选择排序算法
	void merge(Rank lo, Rank mi, Rank hi); //归并算法
	void mergeSort(Rank lo, Rank hi); //归并排序算法
	void heapSort(Rank lo, Rank hi); //堆排序（稍后结合完全堆讲解）
	Rank partition(Rank lo, Rank hi); //轴点构造算法
	void quickSort(Rank lo, Rank hi); //快速排序算法
	void shellSort(Rank lo, Rank hi); //希尔排序算法
public:
	// 构造方法
	Vector(Rank c = DEFAULT_CAPACITY) //容量为c的空向量
	{
		_elem = new T[_capacity = c]; _size = 0;
	}
	Vector(Rank c, Rank s, T v) //容量为c、规模为s、所有元素初始为v；s<=c
	{
		_elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //数组整体复制
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //区间
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //向量整体复制
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //区间
	// 析构方法
	~Vector() { delete[] _elem; } //释放内部空间
	// 只读访问接口
	Rank size() const { return _size; } //规模
	bool empty() const { return !_size; } //判空
	Rank find(T const& e) const { return find(e, 0, _size); } //无序向量整体查找
	Rank find(T const& e, Rank lo, Rank hi) const; //无序向量区间查找
	Rank select(Rank k) { return quickSelect(_elem, _size, k); } //从无序向量中找到第k大的元素
	Rank search(T const& e) const //有序向量整体查找
	{
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}
	Rank search(T* A, T const& e, Rank lo, Rank hi) const; //有序向量区间查找
	// 可写访问接口
	T& operator[] (Rank r); //重载下标操作符，可以类似于数组形式引用各元素
	const T& operator[] (Rank r) const; //仅限于做右值的重载版本
	Vector<T>& operator= (Vector<T> const&); //重载赋值操作符，以便直接克隆向量
	T remove(Rank r); //删除秩为r的元素
	int remove(Rank lo, Rank hi); //删除秩在区间[lo, hi)之内的元素
	Rank insert(Rank r, T const& e); //插入元素
	Rank insert(T const& e) { return insert(_size, e); } //默认作为末元素插入
	void sort(Rank lo, Rank hi); //对[lo, hi)排序
	void sort() { sort(0, _size); } //整体排序
	void unsort(Rank lo, Rank hi); //对[lo, hi)置乱
	void unsort() { unsort(0, _size); } //整体置乱
	int deduplicate(); //无序去重
	int uniquify(); //有序去重
	void print();
	void reverse();
	// 遍历
	void traverse(void (*) (T&)); //遍历（使用函数指针，只读或局部性修改）
	template <typename VST>
	void traverse(VST&);// 遍历（使用函数对象，可全局性修改）
}; //Vector
//复制的构造方法
template <typename T> void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
	_elem = new T[_capacity = 2 * (hi - lo)]; _size = 0;
	while (lo < hi)
		_elem[_size++] = A[lo++];
}
//重载赋值操作符，以便直接克隆向量
template <typename T> Vector<T>& Vector<T>:: operator= (Vector<T> const& V) {
	if (_elem) delete[] _elem;
	copyFrom(V._elem, 0, V.size());
	return *this;
}
//扩容
template <typename T> void Vector<T>::expand() {
	if (_size < _capacity) return;
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
	T* oldElem = _elem; _elem = new T[_capacity <<= 1];
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i];
	delete[] oldElem;
}
// 缩容
template <typename T> void Vector<T>::shrink() {
	if (_capacity < DEFAULT_CAPACITY << 1)
		return;
	if (_size << 2 > _capacity)
		return;
	T* oldElem = _elem;
	_elem = new T[_capacity >>= 1];
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i];
	delete[] oldElem;
}
// 重载下标操作符，可以类似于数组形式引用各元素
template <typename T> T& Vector<T>::operator[] (Rank r) {
	return _elem[r];
}
// 整体置乱
template <typename T> void permute(Vector<T>& V) {
	for (int i = V.size(); i > 0; i--)
		swap(V[i - 1], V[rand() % i]);
}
//对[lo, hi)置乱
template <typename T> void Vector<T>::unsort(Rank lo, Rank hi) {
	T* V = _elem + lo;
	for (Rank i = hi - lo; i > 0; i--)
		swap(V[i - 1], V[rand() % i]);
}
//无序向量区间查找
template <typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
	while ((lo < hi--) && (e != _elem[hi]));
	return hi;
}
//插入元素
template <typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
	expand();
	for (int i = _size; i > r; i--)_elem[i] = _elem[i - 1];
	_elem[r] = e; _size++;
	return r;
}
// 区间删除
template <typename T> int Vector<T>::remove(Rank lo, Rank hi) {
	if (lo == hi)
		return 0;
	while (hi < _size)
		_elem[lo++] = _elem[hi++];
	_size = lo;
	shrink();
	return hi - lo;
}
// 单元素区间删除
template <typename T> T Vector<T>::remove(Rank r) {
	T e = _elem[r];
	remove(r, r + 1);
	return e;
}
//无序去重
template <typename T> int Vector<T>::deduplicate() {
	int oldSize = _size;
	Rank i = 1;
	while (i < _size)
	{
		find(_elem[i], 0, i) < 0 ? i++ : remove(i);
	}
	return oldSize - _size;
}
// 遍历
template <typename T> void Vector<T>::traverse(void(*vist)(T&)) {
	for (int i = 0; i < _size; i++) visit(_elem[i]);
}
template <typename T> template <typename VST>
void Vector<T>::traverse(VST& visit) {
	for (int i = 0; i < _size; i++)visit(_elem[i]);
}
//基于遍历实现increase()功能
template <typename T> struct Increase {
	void operator()(T& e)
	{
		e++;
	}
};
template <typename T> void increase(Vector<T>& V) {
	V.traverse(Increase<T>());
}
//唯一化（有序高效版）
template <typename T> int Vector<T>::uniquify() {
	Rank i = 0, j = 0;
	while (++j < _size)
	{
		if (_elem[i] != _elem[j]) _elem[++i] = _elem[j];
	}
	_size = ++i;
	shrink();
	return j - i;
}
//扫描交换
template <typename T> bool Vector<T>::bubble(Rank lo, Rank hi) {
	bool sorted = true;
	while (++lo < hi)
		if (_elem[lo - 1] > _elem[lo]) {
			sorted = false;
			swap(_elem[lo - 1], _elem[lo]);
		}
	return sorted;
}
//查找
#include "Fib.h"
#include <iostream>
template <typename T> static Rank search(T* A, T const& e, Rank lo, Rank hi) {
	Fib fib(hi - lo);
	while (lo < hi)
	{
		while (hi - lo < fib.get())
			fib.prev();
		Rank mi = lo + fib.get() - 1;
		if (e < A[mi])
			hi = mi;
		else if (e > A[mi])
			lo = mi + 1;
		else
			return mi;
	}
	return -1;
}
//起泡排序算法
template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
	while (!bubble(lo, hi--));
}
//归并排序算法
template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
	if (hi - lo < 2) return;
	int mi = (lo + hi) / 2;
	mergeSort(lo, hi); mergeSort(mi, hi);
	merge(lo, mi, hi);
}
//归并算法
template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
	T* A = _elem + lo;
	int lb = mi - lo;
	T* B = new T[lb];
	for (Rank i = 0; i < lb; B[i] = A[i++]);
	int lc = hi - mi;
	T* C = _elem + mi;
	for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);)
	{
		if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
		if ((k < lc) && (!(j < lb) || (C[k] < C[j]))) A[i++] = C[k++];
	}
}
template <typename T> void Vector<T>::sort(Rank lo, Rank hi) {
	switch (rand() % 2) {
	case 1: bubbleSort(lo, hi); break; //起泡排序
		//case 2: selectionSort(lo, hi); break; //选择排序（习题）
	default: mergeSort(lo, hi); break; //归并排序
		//case 4: heapSort(lo, hi); break; //堆排序（第12章）
		//case 5: quickSort(lo, hi); break; //快速排序（第14章）
		//default: shellSort(lo, hi); break; //希尔排序（第14章）
	}
}
template <typename T> void Vector<T>::reverse() {
	size_t left = 0;
	size_t right = _size - 1;
	while (left < right) {
		std::swap(_elem[left], _elem[right]);
		++left;
		--right;
	}
}