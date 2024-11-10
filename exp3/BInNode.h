#include <iostream>
#include <stack>
#include "Queue.h"
#define BinNodePosi(T) BinNode<T>* //�ڵ�λ��
#define stature(p) ((p) ? (p)->height : -1) //�ڵ�߶ȣ����������߶�Ϊ-1����Լ����ͳһ��
typedef enum { RB_RED, RB_BLACK } RBColor; //�ڵ���ɫ

template <typename T> struct BinNode { //�������ڵ�ģ����
// ��Ա��Ϊ���������ͳһ���ţ����߿ɸ�����Ҫ��һ����װ��
	T data; //��ֵ
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc; //���ڵ㼰���Һ���
	int height; //�߶ȣ�ͨ�ã�
	int npl; //Null Path Length����ʽ�ѣ�Ҳ��ֱ����height���棩
	RBColor color; //��ɫ���������
// ���캯��
	BinNode() :
		parent(nullptr), lc(nullptr), rc(nullptr), height(0), npl(1), color(RB_RED) { }
	BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lc = nullptr, BinNodePosi(T) rc = nullptr,
			int h = 0, int l = 1, RBColor c = RB_RED) :
		data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) { }
// �����ӿ�
	int size(); //ͳ�Ƶ�ǰ�ڵ����������༴����Ϊ���������Ĺ�ģ
	BinNodePosi(T) insertAsLC(T const&); //��Ϊ��ǰ�ڵ�����Ӳ����½ڵ�
	BinNodePosi(T) insertAsRC(T const&); //��Ϊ��ǰ�ڵ���Һ��Ӳ����½ڵ�
	BinNodePosi(T) succ(); //ȡ��ǰ�ڵ��ֱ�Ӻ��
	template <typename VST> void travLevel(VST&); //������α���
	template <typename VST> void travPre(BinNodePosi(T) x, VST&); //�����������
	template <typename VST> void travIn(BinNodePosi(T) x, VST&); //�����������
	template <typename VST> void travPost(BinNodePosi(T) x, VST&); //�����������
// �Ƚ������е�����������һ���������в��䣩
	bool operator< (BinNode const& bn) { return data < bn.data; } //С��
	bool operator== (BinNode const& bn) { return data == bn.data; } //����
};

#define IsRoot(x) (!(x.parent))// �ж��Ƿ�Ϊ���ڵ�  
#define IsLChild(x) (!IsRoot(x) && (&(x) == x.parent->lc))// �ж��Ƿ�Ϊ����    
#define IsRChild(x) (!IsRoot(x) && (&(x) == x.parent->rc))// �ж��Ƿ�Ϊ�Һ���    
#define HasParent(x) (!IsRoot(x))// �ж��Ƿ��и��ڵ�  
#define HasLChild(x) (x.lc != nullptr)// �ж��Ƿ�������   
#define HasRChild(x) (x.rc != nullptr)// �ж��Ƿ����Һ���    
#define HasChild(x) (HasLChild(x) || HasRChild(x))// �ж��Ƿ��к���    
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))// �ж��Ƿ�����������   
#define IsLeaf(x) (!HasChild(x))// �ж��Ƿ�ΪҶ�ӽڵ�    
#define sibling(p) (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->lc)// ��ȡ�ֵܽڵ�   
#define uncle(x) (IsLChild(*(x->parent)) ? (x)->parent->parent->rc : (x)->parent->parent->lc)// ��ȡ����ڵ�    
#define FromParentTo(x) ( IsRoot(x) ? _root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) )
//ͳ�Ƶ�ǰ�ڵ����������༴����Ϊ���������Ĺ�ģ
template <typename T> int BinNode<T>::size() {
		int count = 1;// ��ǰ�ڵ㱾������1���ڵ�
		if (lc != nullptr) count += lc->size(); //�ݹ������������ģ
		if (rc != nullptr) count += rc->size(); //�ݹ������������ģ
		return count;
}
//��e��Ϊ��ǰ�ڵ�����Ӳ��������
template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC(T const& e)
{
	lc = new BinNode<T>(e, this);
	return lc;
} 
//��e��Ϊ��ǰ�ڵ���Һ��Ӳ��������
template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC(T const& e)
{
	rc = new BinNode<T>(e, this);
	return rc;
} 
//ȡ��ǰ�ڵ��ֱ�Ӻ��
template <typename T>
BinNodePosi(T) BinNode<T>::succ() {
	BinNodePosi(T) s = this; // ��ʱ������ָ��ǰ�ڵ�
	if (rc) { // �����Һ���
		s = rc; // ��������
		while (HasLChild(*s)) s = s->lc; // �ҵ������������Ľڵ�
	}
	else { // ����ֱ�Ӻ������͵����Ƚڵ�
		while (IsRChild(*s)) s = s->parent; // �����������֧����
		s = s->parent; // ������Ƶ����ڵ�
	}
	return s;
}
//������α���
template <typename T> template <typename VST> 
void BinNode<T>::travLevel(VST& visit) {
	Queue<BinNodePosi(T)> Q; // �������У����ڴ洢��ǰ��Ľڵ�
	Q.enqueue(this); // ���ڵ����

	while (!Q.empty()) { // �ڶ��зǿ�֮ǰ�����ϱ���
		BinNodePosi(T) x = Q.dequeue(); // ȡ�����׽ڵ�
		visit(x->data); // ���ʽڵ������
		if (HasLChild(*x)) Q.enqueue(x->lc); // ��������ӣ����������
		if (HasRChild(*x)) Q.enqueue(x->rc); // ������Һ��ӣ����Һ������
	}
}
//�����������
template <typename T, typename VST>
void travPre(BinNodePosi(T) x, VST& visit) { // ��������������㷨��������#1��
	if (!x)
		return;
	travPre_R(x->lc, visit);
	visit(x->data);
	travPre_R(x->rc, visit);
}
//�����������
template <typename T, typename VST> // Ԫ�����͡�������
void travIn(BinNodePosi(T) x, VST& visit) { // ��������������㷨��������#3�����踨��ջ��
	bool backtrack = false; // ǰһ���Ƿ�մ����������ݣ�ʡȥջ������ O(1) �����ռ�
	while (true) {
		// 1. ����ǰ�ڵ㲻�Ǹմ���������������������
		if (!backtrack && HasLChild(*x)) {
			x = x->lc; // ����������
		}
		else {
			// 2. ���ʵ�ǰ�ڵ�
			visit(x->data);
			// 3. ���������ǿգ�������������������
			if (HasRChild(*x)) {
				x = x->rc;
				backtrack = false; // �رջ��ݱ�־
			}
			else {
				// 4. ����������Ϊ�գ����л���
				x = x->succ(); // ʹ�� `succ()` ����ֱ�Ӻ��
				if (x == nullptr) break; // ��û��ֱ�Ӻ�̣���������
				backtrack = true; // ���û��ݱ�־
			}
		}
	}
}
//�����������
template <typename T, typename VST>
void travPost(BinNodePosi(T) x, VST& visit) {
	if (!x)
		return;
	travPost_R(x->lc, visit);
	travPost_R(x->rc, visit);
	visit(x->data);
}