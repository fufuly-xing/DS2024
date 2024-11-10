#pragma once
#include "ListNode.cpp"
template <typename T>
class List {
private:
	int _size;                       // 链表的节点数
	ListNodePosi(T) header;          // 头哨兵节点（不存储实际数据）
	ListNodePosi(T) trailer;         // 尾哨兵节点（不存储实际数据）

protected:
	void init();                                         // 初始化链表
	int clear();                                         // 清除所有节点
	void copyNodes(ListNodePosi(T) p, int n);                // 复制从指定节点开始的 n 个节点
	void merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m); // 归并两个子链表
	void mergeSort(ListNodePosi(T)& p, int n);               // 归并排序
	void selectionSort(ListNodePosi(T) p, int n);            // 选择排序
	void insertionSort(ListNodePosi(T) p, int n);            // 插入排序

public:
	// 构造方法
	List() { init(); }//默认
	List(List<T>const& L);//整体复制列表L
	List(List<T> const& L, Rank r, int n);//复制列表L中自第r项起的n项
	List(ListNodePosi(T) p, int n);//复制列表中自位置p起的n项
	// 析构方法
	~List();//释放(包含头、尾哨兵在内的)所有节点
	//只读访问接口
	int size() const { return _size; }//规模
	bool empty() const { return _size <= 0; }//判空
	ListNodePosi(T)operator[](Rank r)const;//重载,支持循秩访问(效率低)
	ListNodePosi(T) first()const { return header->succ; }//首节点位置
	ListNodePosi(T) last() const { return trailer->pred; }//末节点位置
	bool valid(ListNodePosi(T) p)//判断位置p是否对外合法
	{
		return (p != nullptr) && (trailer != p) && (header != p);
	}//将头、尾节点等同于NULL
	int disordered() const; // 判断列表是否已排序
	ListNodePosi(T) find(T const& e) const
	{
		return find(e, _size, trailer);
	} // 在无序链表中查找元素 e
	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const; // 在无序子区间查找
	ListNodePosi(T) search(T const& e) const
	{
		return search(e, _size, trailer);
	} // 在有序链表中查找元素 e
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const; // 在有序子区间查找
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n); // 在 p 及其 n-1 个后继中选出最大者
	ListNodePosi(T) selectMax()
	{
		return selectMax(header->succ, _size);
	} // 在整个链表中选出最大者
	// 可写访问接口
	ListNodePosi(T) insertAsFirst(T const& e); // 将 e 作为首节点插入
	ListNodePosi(T) insertAsLast(T const& e); // 将 e 作为尾节点插入
	ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e); // 将 e 作为 p 的后继插入
	ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e); // 将 e 作为 p 的前驱插入
	T remove( ListNodePosi(T) p);
	void merge(List<T>& L)
	{
		merge(first(), _size, L, L.first(), L._size);
	} // 合并两个链表
	void sort(ListNodePosi(T) p, int n); // 对从 p 开始的 n 个节点进行排序
	void sort()
	{
		sort(first(), _size);
	} // 对整个链表进行排序
	int deduplicate(); // 无序去重，返回删除元素的个数
	int uniquify(); // 有序去重，返回删除元素的个数
	void reverse(); // 前后倒置（习题）
	// 遍历
	void traverse(void (*visit)(T&)); // 使用函数指针进行遍历
	template <typename VST>
	void traverse(VST& visit); // 使用函数对象进行遍历
};//List

template <typename T>
void List<T>::init() {
	// 列表初始化，在创建列表对象时统一调用
	header = new ListNode<T>;   // 创建头哨兵节点
	trailer = new ListNode<T>;  // 创建尾哨兵节点

	header->succ = trailer;     // 头哨兵的后继指向尾哨兵
	header->pred = nullptr;        // 头哨兵的前驱为空
	trailer->pred = header;     // 尾哨兵的前驱指向头哨兵
	trailer->succ = nullptr;       // 尾哨兵的后继为空

	_size = 0;                  // 初始化链表大小为 0
}

template<typename T>//重载下标操作符,以通过秩直接访问列表节点(0(r)效率,虽方便,勿多用)
ListNodePosi(T) List<T>:: operator[] (int r) const { // 0 <= r < size
	ListNodePosi(T)p = first();//从首节点出发
	while (0 < r--)p = p->succ;//顺数第r个节点即是
	return p;//目标节点
}

template<typename T>//在无序列表内节点p(可能是trailer)的n个(真)前驱中,找到等于e的最后者
ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) const {
	while (0 < n--)//(0 <= n <= int(p) <_size)对于p的最近的n个前驱,从右向左
		if (e == (p = p->pred)->data)return p; //逐个比对,直至命中或范围越界
	return nullptr;//p越出左边界意味着区间内不含e,查找失败
}//失败时,返回NULL

template <typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const& e) {
	_size++;
	return header->insertAsSucc(e);
}

template <typename T>
ListNodePosi(T) List<T>::insertAsLast(T const& e) {
	_size++;
	return trailer->insertAsPred(e);
}

template <typename T>
ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p, T const& e) {
	_size++;
	return p->insertAsSucc(e);
}

template <typename T>
ListNodePosi(T) List<T>::insertB(ListNodePosi(T) p, T const& e) {
	_size++;
	return p->insertAsPred(e);
}

template <typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n) {
	// p 合法，且至少有 n-1 个真后继节点
	init(); // 创建头、尾哨兵节点并做初始化
	while (n--) {
		insertAsLast(p->data); // 将 p 节点的数据作为末节点插入
		p = p->succ; // 移动到下一个节点
	}
}

template<typename T>//复制列表中自位置p起的n项(assert:p为合法位置,且至少有n-1个后继节点)
List<T>::List(ListNodePosi(T) p, int n) { copyNodes(p, n); }

template <typename T>//整体复制列表L
List<T>::List(List<T> const& L) { copyNodes(L.first(), L._size); }

template<typename T>//复制L中自第r项起的n项(assert: r+n <= L._size)
List<T>::List(List<T> const& L, int r, int n) {
	ListNodePosi(T) p = L.first();
	while (0 < r--) p = p->succ;
	copyNodes(p, n);
}

template <typename T>
T List<T>::remove(ListNodePosi(T) p) { // 删除合法节点 p，并返回其数值
	T e = p->data; // 备份待删除节点的数值（假定 T 类型可直接赋值）
	p->pred->succ = p->succ; // 将 p 的前驱的后继指向 p 的后继
	p->succ->pred = p->pred; // 将 p 的后继的前驱指向 p 的前驱
	delete p; // 释放节点 p
	_size--; // 更新链表规模
	return e; // 返回备份的数值
}

template <typename T>
List<T>::~List() { // 列表析构函数
	clear(); // 清空列表中的所有节点
	delete header; // 释放头哨兵节点
	delete trailer; // 释放尾哨兵节点
}

template <typename T>
int List<T>::clear() { // 清空列表
	int oldSize = _size; // 保存当前列表大小
	while (0 < _size) remove(header->succ); // 反复删除首节点，直到列表为空
	return oldSize; // 返回列表原有大小
}

template <typename T>
int List<T>::deduplicate() { // 剔除无序列表中的重复节点
	if (_size < 2) return 0; // 平凡列表自然无重复
	int oldSize = _size; // 记下原列表大小
	ListNodePosi(T) p = header; Rank r = 0; // p 从首节点开始，r 记录当前节点的秩
	while (trailer != (p = p->succ)) { // 依次遍历列表直到尾节点
		ListNodePosi(T) q = find(p->data, r, p); // 在 p 的前 r 个节点中查找相同的数据
		q ? remove(q) : r++; // 如果找到重复元素，则删除；否则，r 增加
	} // assert: 循环过程中，p 的所有前驱互不相同
	return oldSize - _size; // 返回被删除的元素数
}

template <typename T>
void List<T>::traverse(void (*visit)(T&)) {
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
		visit(p->data);
}

template <typename T>
template <typename VST>
void List<T>::traverse(VST& visit) {
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
		visit(p->data);
}

template <typename T>
int List<T>::uniquify() { // 成批剔除重复元素，效率更高
	if (_size < 2) return 0; // 平凡列表自然无重复
	int oldSize = _size; // 记住原来的大小
	ListNodePosi(T) p = first(); // p 为列表的第一个节点
	ListNodePosi(T) q; // q 为 p 的后继节点

	while (trailer != (q = p->succ)) { // 反复考查紧邻的节点对 (p, q)
		if (p->data != q->data) {
			p = q; // 如果 p 和 q 不相同，则转向下一对
		}
		else {
			remove(q); // 否则（p 和 q 相同），删除 q 节点
		}
	}
	return oldSize - _size; // 返回列表的大小变化量，即被删除的元素个数
}

template <typename T>
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const {
	// assert: 0 <= n <= rank(p) < _size
	while (0 <= n--) { // 对于p的最近的n个前驱，从右向左逐个比较
		if (((p = p->pred)->data) <= e) break; // 直至命中、数值越界或范围越界
	}
	// assert: 至此位置p必符合输出语义约定——尽管此前最后一次键码比较可能没有意义（等效于不-inf比较）
	return p; // 返回查找终止的位置
}

template <typename T>
void List<T>::sort(ListNodePosi(T) p, int n) { // 列表区间排序
	switch (rand() % 3) { // 随机选择排序算法。可根据具体问题的特点灵活选择或扩展
	case 1: insertionSort(p, n); break;  // 插入排序
	case 2: selectionSort(p, n); break;  // 选择排序
	default: mergeSort(p, n); break;     // 归并排序
	}
}

template <typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n) { // valid(p) && rank(p) + n <= size
	for (int r = 0; r < n; r++) { // 逐一为各节点
		insertA(search(p->data, r, p), p->data); // 查找适合的位置并插入
		p = p->succ;
		remove(p->pred); // 转向下一节点
	}
}

template <typename T>
void List<T>::selectionSort(ListNodePosi(T) p, int n) { // valid(p) && rank(p) + n <= size
	ListNodePosi(T) head = p->pred;
	ListNodePosi(T) tail = p;

	for (int i = 0; i < n; i++)
		tail = tail->succ; // 待排序区间为(head, tail)

	while (1 < n) { // 在至少剩下两个节点之前，在待排序区间内
		ListNodePosi(T) max = selectMax(head->succ, n); // 找出最大者（默认时后者优先）
		insertB(tail, remove(max)); // 将其移至无序区间末尾（作为有序区间新首元素）
		tail = tail->pred;
		n--;
	}
}

template <typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n) {
	ListNodePosi(T) max = p; // 初始最大元素为p
	for (ListNodePosi(T) cur = p; 1 < n; n--) { // 遍历n个节点
		if (!lt((cur = cur->succ)->data, max->data)) // 若当前节点不小于最大节点，则更新最大节点
			max = cur;
	}
	return max; // 返回最大元素位置
}

template <typename T>
void List<T>::merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m) {
	// assert: this.valid(p) && rank(p) + n <= size && this.sorted(p, n)
	// L.valid(q) && rank(q) + m <= L._size && L.sorted(q, m)
	// 注意：在归并排序的场合，有可能 this == L && rank(p) + n = rank(q)

	ListNodePosi(T) pp = p->pred; // 备份p的前驱（可能是header），以便后续操作
	while (0 < m) { // 当q部分仍有元素时
		if ((0 < n) && (p->data <= q->data)) { // 如果p部分还有元素，并且p的值 <= q的值
			if (q == (p = p->succ)) break; // p继续前进，直到它超过原有部分
			n--; // p的元素被合并进来，n减小
		}
		else { // 如果p已超出或q的值 < p的值
			insertB(p, L.remove((q = q->succ)->pred)); // 将q移至p的前面
			m--; // q的元素被合并进来，m减小
		}
		p = pp->succ; // 将p重置为合并后的新起点
	}
}

template <typename T>
void List<T>::mergeSort(ListNodePosi(T)& p, int n) { //valid(p) && rank(p) + n <= size
	if (n < 2) return; // 若待排序范围已足够小，则直接返回；否则...

	int m = n >> 1; // 以中点为界，n的一半
	ListNodePosi(T) q = p;
	for (int i = 0; i < m; i++) q = q->succ; // 均分列表

	mergeSort(p, m);  // 对前半部分递归排序
	mergeSort(q, n - m);  // 对后半部分递归排序

	merge(p, m, *this, q, n - m); // 合并两个已排序的部分
}