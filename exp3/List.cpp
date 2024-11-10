#pragma once
#include "ListNode.cpp"
template <typename T>
class List {
private:
	int _size;                       // ����Ľڵ���
	ListNodePosi(T) header;          // ͷ�ڱ��ڵ㣨���洢ʵ�����ݣ�
	ListNodePosi(T) trailer;         // β�ڱ��ڵ㣨���洢ʵ�����ݣ�

protected:
	void init();                                         // ��ʼ������
	int clear();                                         // ������нڵ�
	void copyNodes(ListNodePosi(T) p, int n);                // ���ƴ�ָ���ڵ㿪ʼ�� n ���ڵ�
	void merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m); // �鲢����������
	void mergeSort(ListNodePosi(T)& p, int n);               // �鲢����
	void selectionSort(ListNodePosi(T) p, int n);            // ѡ������
	void insertionSort(ListNodePosi(T) p, int n);            // ��������

public:
	// ���췽��
	List() { init(); }//Ĭ��
	List(List<T>const& L);//���帴���б�L
	List(List<T> const& L, Rank r, int n);//�����б�L���Ե�r�����n��
	List(ListNodePosi(T) p, int n);//�����б�����λ��p���n��
	// ��������
	~List();//�ͷ�(����ͷ��β�ڱ����ڵ�)���нڵ�
	//ֻ�����ʽӿ�
	int size() const { return _size; }//��ģ
	bool empty() const { return _size <= 0; }//�п�
	ListNodePosi(T)operator[](Rank r)const;//����,֧��ѭ�ȷ���(Ч�ʵ�)
	ListNodePosi(T) first()const { return header->succ; }//�׽ڵ�λ��
	ListNodePosi(T) last() const { return trailer->pred; }//ĩ�ڵ�λ��
	bool valid(ListNodePosi(T) p)//�ж�λ��p�Ƿ����Ϸ�
	{
		return (p != nullptr) && (trailer != p) && (header != p);
	}//��ͷ��β�ڵ��ͬ��NULL
	int disordered() const; // �ж��б��Ƿ�������
	ListNodePosi(T) find(T const& e) const
	{
		return find(e, _size, trailer);
	} // �����������в���Ԫ�� e
	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const; // ���������������
	ListNodePosi(T) search(T const& e) const
	{
		return search(e, _size, trailer);
	} // �����������в���Ԫ�� e
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const; // ���������������
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n); // �� p ���� n-1 �������ѡ�������
	ListNodePosi(T) selectMax()
	{
		return selectMax(header->succ, _size);
	} // ������������ѡ�������
	// ��д���ʽӿ�
	ListNodePosi(T) insertAsFirst(T const& e); // �� e ��Ϊ�׽ڵ����
	ListNodePosi(T) insertAsLast(T const& e); // �� e ��Ϊβ�ڵ����
	ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e); // �� e ��Ϊ p �ĺ�̲���
	ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e); // �� e ��Ϊ p ��ǰ������
	T remove( ListNodePosi(T) p);
	void merge(List<T>& L)
	{
		merge(first(), _size, L, L.first(), L._size);
	} // �ϲ���������
	void sort(ListNodePosi(T) p, int n); // �Դ� p ��ʼ�� n ���ڵ��������
	void sort()
	{
		sort(first(), _size);
	} // �����������������
	int deduplicate(); // ����ȥ�أ�����ɾ��Ԫ�صĸ���
	int uniquify(); // ����ȥ�أ�����ɾ��Ԫ�صĸ���
	void reverse(); // ǰ���ã�ϰ�⣩
	// ����
	void traverse(void (*visit)(T&)); // ʹ�ú���ָ����б���
	template <typename VST>
	void traverse(VST& visit); // ʹ�ú���������б���
};//List

template <typename T>
void List<T>::init() {
	// �б��ʼ�����ڴ����б����ʱͳһ����
	header = new ListNode<T>;   // ����ͷ�ڱ��ڵ�
	trailer = new ListNode<T>;  // ����β�ڱ��ڵ�

	header->succ = trailer;     // ͷ�ڱ��ĺ��ָ��β�ڱ�
	header->pred = nullptr;        // ͷ�ڱ���ǰ��Ϊ��
	trailer->pred = header;     // β�ڱ���ǰ��ָ��ͷ�ڱ�
	trailer->succ = nullptr;       // β�ڱ��ĺ��Ϊ��

	_size = 0;                  // ��ʼ�������СΪ 0
}

template<typename T>//�����±������,��ͨ����ֱ�ӷ����б�ڵ�(0(r)Ч��,�䷽��,�����)
ListNodePosi(T) List<T>:: operator[] (int r) const { // 0 <= r < size
	ListNodePosi(T)p = first();//���׽ڵ����
	while (0 < r--)p = p->succ;//˳����r���ڵ㼴��
	return p;//Ŀ��ڵ�
}

template<typename T>//�������б��ڽڵ�p(������trailer)��n��(��)ǰ����,�ҵ�����e�������
ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) const {
	while (0 < n--)//(0 <= n <= int(p) <_size)����p�������n��ǰ��,��������
		if (e == (p = p->pred)->data)return p; //����ȶ�,ֱ�����л�ΧԽ��
	return nullptr;//pԽ����߽���ζ�������ڲ���e,����ʧ��
}//ʧ��ʱ,����NULL

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
	// p �Ϸ����������� n-1 �����̽ڵ�
	init(); // ����ͷ��β�ڱ��ڵ㲢����ʼ��
	while (n--) {
		insertAsLast(p->data); // �� p �ڵ��������Ϊĩ�ڵ����
		p = p->succ; // �ƶ�����һ���ڵ�
	}
}

template<typename T>//�����б�����λ��p���n��(assert:pΪ�Ϸ�λ��,��������n-1����̽ڵ�)
List<T>::List(ListNodePosi(T) p, int n) { copyNodes(p, n); }

template <typename T>//���帴���б�L
List<T>::List(List<T> const& L) { copyNodes(L.first(), L._size); }

template<typename T>//����L���Ե�r�����n��(assert: r+n <= L._size)
List<T>::List(List<T> const& L, int r, int n) {
	ListNodePosi(T) p = L.first();
	while (0 < r--) p = p->succ;
	copyNodes(p, n);
}

template <typename T>
T List<T>::remove(ListNodePosi(T) p) { // ɾ���Ϸ��ڵ� p������������ֵ
	T e = p->data; // ���ݴ�ɾ���ڵ����ֵ���ٶ� T ���Ϳ�ֱ�Ӹ�ֵ��
	p->pred->succ = p->succ; // �� p ��ǰ���ĺ��ָ�� p �ĺ��
	p->succ->pred = p->pred; // �� p �ĺ�̵�ǰ��ָ�� p ��ǰ��
	delete p; // �ͷŽڵ� p
	_size--; // ���������ģ
	return e; // ���ر��ݵ���ֵ
}

template <typename T>
List<T>::~List() { // �б���������
	clear(); // ����б��е����нڵ�
	delete header; // �ͷ�ͷ�ڱ��ڵ�
	delete trailer; // �ͷ�β�ڱ��ڵ�
}

template <typename T>
int List<T>::clear() { // ����б�
	int oldSize = _size; // ���浱ǰ�б��С
	while (0 < _size) remove(header->succ); // ����ɾ���׽ڵ㣬ֱ���б�Ϊ��
	return oldSize; // �����б�ԭ�д�С
}

template <typename T>
int List<T>::deduplicate() { // �޳������б��е��ظ��ڵ�
	if (_size < 2) return 0; // ƽ���б���Ȼ���ظ�
	int oldSize = _size; // ����ԭ�б��С
	ListNodePosi(T) p = header; Rank r = 0; // p ���׽ڵ㿪ʼ��r ��¼��ǰ�ڵ����
	while (trailer != (p = p->succ)) { // ���α����б�ֱ��β�ڵ�
		ListNodePosi(T) q = find(p->data, r, p); // �� p ��ǰ r ���ڵ��в�����ͬ������
		q ? remove(q) : r++; // ����ҵ��ظ�Ԫ�أ���ɾ��������r ����
	} // assert: ѭ�������У�p ������ǰ��������ͬ
	return oldSize - _size; // ���ر�ɾ����Ԫ����
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
int List<T>::uniquify() { // �����޳��ظ�Ԫ�أ�Ч�ʸ���
	if (_size < 2) return 0; // ƽ���б���Ȼ���ظ�
	int oldSize = _size; // ��סԭ���Ĵ�С
	ListNodePosi(T) p = first(); // p Ϊ�б�ĵ�һ���ڵ�
	ListNodePosi(T) q; // q Ϊ p �ĺ�̽ڵ�

	while (trailer != (q = p->succ)) { // ����������ڵĽڵ�� (p, q)
		if (p->data != q->data) {
			p = q; // ��� p �� q ����ͬ����ת����һ��
		}
		else {
			remove(q); // ����p �� q ��ͬ����ɾ�� q �ڵ�
		}
	}
	return oldSize - _size; // �����б�Ĵ�С�仯��������ɾ����Ԫ�ظ���
}

template <typename T>
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const {
	// assert: 0 <= n <= rank(p) < _size
	while (0 <= n--) { // ����p�������n��ǰ����������������Ƚ�
		if (((p = p->pred)->data) <= e) break; // ֱ�����С���ֵԽ���ΧԽ��
	}
	// assert: ����λ��p�ط����������Լ���������ܴ�ǰ���һ�μ���ȽϿ���û�����壨��Ч�ڲ�-inf�Ƚϣ�
	return p; // ���ز�����ֹ��λ��
}

template <typename T>
void List<T>::sort(ListNodePosi(T) p, int n) { // �б���������
	switch (rand() % 3) { // ���ѡ�������㷨���ɸ��ݾ���������ص����ѡ�����չ
	case 1: insertionSort(p, n); break;  // ��������
	case 2: selectionSort(p, n); break;  // ѡ������
	default: mergeSort(p, n); break;     // �鲢����
	}
}

template <typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n) { // valid(p) && rank(p) + n <= size
	for (int r = 0; r < n; r++) { // ��һΪ���ڵ�
		insertA(search(p->data, r, p), p->data); // �����ʺϵ�λ�ò�����
		p = p->succ;
		remove(p->pred); // ת����һ�ڵ�
	}
}

template <typename T>
void List<T>::selectionSort(ListNodePosi(T) p, int n) { // valid(p) && rank(p) + n <= size
	ListNodePosi(T) head = p->pred;
	ListNodePosi(T) tail = p;

	for (int i = 0; i < n; i++)
		tail = tail->succ; // ����������Ϊ(head, tail)

	while (1 < n) { // ������ʣ�������ڵ�֮ǰ���ڴ�����������
		ListNodePosi(T) max = selectMax(head->succ, n); // �ҳ�����ߣ�Ĭ��ʱ�������ȣ�
		insertB(tail, remove(max)); // ����������������ĩβ����Ϊ������������Ԫ�أ�
		tail = tail->pred;
		n--;
	}
}

template <typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n) {
	ListNodePosi(T) max = p; // ��ʼ���Ԫ��Ϊp
	for (ListNodePosi(T) cur = p; 1 < n; n--) { // ����n���ڵ�
		if (!lt((cur = cur->succ)->data, max->data)) // ����ǰ�ڵ㲻С�����ڵ㣬��������ڵ�
			max = cur;
	}
	return max; // �������Ԫ��λ��
}

template <typename T>
void List<T>::merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m) {
	// assert: this.valid(p) && rank(p) + n <= size && this.sorted(p, n)
	// L.valid(q) && rank(q) + m <= L._size && L.sorted(q, m)
	// ע�⣺�ڹ鲢����ĳ��ϣ��п��� this == L && rank(p) + n = rank(q)

	ListNodePosi(T) pp = p->pred; // ����p��ǰ����������header�����Ա��������
	while (0 < m) { // ��q��������Ԫ��ʱ
		if ((0 < n) && (p->data <= q->data)) { // ���p���ֻ���Ԫ�أ�����p��ֵ <= q��ֵ
			if (q == (p = p->succ)) break; // p����ǰ����ֱ��������ԭ�в���
			n--; // p��Ԫ�ر��ϲ�������n��С
		}
		else { // ���p�ѳ�����q��ֵ < p��ֵ
			insertB(p, L.remove((q = q->succ)->pred)); // ��q����p��ǰ��
			m--; // q��Ԫ�ر��ϲ�������m��С
		}
		p = pp->succ; // ��p����Ϊ�ϲ���������
	}
}

template <typename T>
void List<T>::mergeSort(ListNodePosi(T)& p, int n) { //valid(p) && rank(p) + n <= size
	if (n < 2) return; // ��������Χ���㹻С����ֱ�ӷ��أ�����...

	int m = n >> 1; // ���е�Ϊ�磬n��һ��
	ListNodePosi(T) q = p;
	for (int i = 0; i < m; i++) q = q->succ; // �����б�

	mergeSort(p, m);  // ��ǰ�벿�ֵݹ�����
	mergeSort(q, n - m);  // �Ժ�벿�ֵݹ�����

	merge(p, m, *this, q, n - m); // �ϲ�����������Ĳ���
}