#pragma once
typedef int Rank;
#define ListNodePosi(T) ListNode<T> * //�б�ڵ�λ��

template <typename T> struct ListNode { //�б�ڵ�ģ���ࣨ��˫��������ʽʵ�֣�
// ��Ա
	T data; ListNodePosi(T) pred; ListNodePosi(T) succ; //��ֵ��ǰ�������
// ���캯��
	ListNode() {} //���header��trailer�m����
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
		: data(e), pred(p), succ(s) {} //Ĭ��������
//�����ӿ�
	ListNodePosi(T) insertAsPred(T const& e); //������ǰ�ڵ�Tǰ�I���½ڵ�
	ListNodePosi(T) insertAsSucc(T const& e); //������ǰ�ڵ�T��I���½ڵ�
};

template <typename T> //��e������ǰ�ڵ�Tǰ�I��ء��ǰ�ڵ������б�ک���ڱ�ͷ�ڵ�header��
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e) {
	ListNodePosi(T) x = new ListNode(e, pred, this); //�����½ڵ�
	pred->succ = x; pred = x; //ک����������
	return x; //ކ���½ڵ�mλ��
}
template < typename T> //��e������ǰ�ڵ�T��I��ء��ǰ�ڵ������б�ک���ڱ�β�ڵ�trailer��
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e) {
	ListNodePosi(T) x = new ListNode(e, this, succ); //�����½ڵ�
	succ->pred = x; succ = x; //ک����������
	return x; //ކ���½ڵ�mλ��
}