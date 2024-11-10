#include "BinNode.h" //����������ڵ���
#include <ranges>
#include <algorithm>
template <typename T> class BinTree { //������ģ����
protected:
	int _size; BinNodePosi(T) _root; //��ģ�����ڵ�
	virtual int updateHeight(BinNodePosi(T) x); //���½ڵ�x�ĸ߶�
	void updateHeightAbove(BinNodePosi(T) x); //���½ڵ�x�������ȵĸ߶�
public:
	BinTree() : _size(0), _root(nullptr) { } //���캯��
	~BinTree() { if (0 < _size) remove(_root); } //��������
	int size() const { return _size; } //��ģ
	bool empty() const { return !_root; } //�п�
	BinNodePosi(T) root() const { return _root; } //����
	BinNodePosi(T) insertAsRoot(T const& e); //������ڵ�
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e); //e��Ϊx�����ӣ�ԭ�ޣ�����
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e); //e��Ϊx���Һ��ӣ�ԭ�ޣ�����
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>*&T); //T��Ϊx����������
	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>*&T); //T��Ϊx����������
	int remove(BinNodePosi(T) x); //ɾ����λ��x���ڵ�Ϊ�������������ظ�����ԭ�ȵĹ�ģ
	BinTree<T>*secede(BinNodePosi(T) x); //������x�ӵ�ǰ����ժ����������ת��Ϊһ�ö�������
	template < typename VST> //������
		void travLevel(VST & visit) { if (_root) _root->travLevel(visit); } //�ִα���
	template < typename VST> //������
		void travPre(VST & visit) { if (_root) _root->travPre(visit); } //�������
	template < typename VST> //������
		void travIn(VST & visit) { if (_root) _root->travIn(visit); } //�������
	template < typename VST> //������
		void travPost(VST & visit) { if (_root) _root->travPost(visit); } //�������
		bool operator< (BinTree<T> const& t) //�Ƚ������������в��䣩
			{ return _root && t._root && lt(_root, t._root); }
		bool operator== (BinTree<T> const& t) //�е���
			{ return _root && t._root && (_root == t._root); }
}; //BinTree
//���½ڵ�x�ĸ߶�
template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) // ���½ڵ� x �߶�
{
	return x->height = 1 + std::max(stature(x->lc), stature(x->rc));// �������������ĸ߶ȣ�����ڵ� x �ĸ߶�
}
//���½ڵ�x�������ȵĸ߶�
template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) // ���½ڵ� x �������ȵĸ߶�
{
	while (x) {
		updateHeight(x);
		x = x->parent;
	}
}
//������ڵ�
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e)
{
	_size = 1;
	return _root = new BinNode<T>(e); // �� e ��Ϊ���ڵ����յĶ�����
}
//e��Ϊx�����ӣ�ԭ�ޣ�����
template <typename T> 
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e) 
{
    _size++; 
    x->insertAsLC(e); 
    updateHeightAbove(x); 
    return x->lc; 
}
//e��Ϊx���Һ��ӣ�ԭ�ޣ�����
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)
{
	_size++;
	x->insertAsRC(e);
	updateHeightAbove(x);
	return x->rc;
} // e ����Ϊ x ���Һ���
//T��Ϊx����������
template <typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>*& S) { // x->lc == NULL
	if (x->lc = S->_root) x->lc->parent = x; // ����
	_size += S->_size;
	updateHeightAbove(x); // ����ȫ����ģ�� x �������ȵĸ߶�
	S->_root = NULL;
	S->_size = 0;
	S = NULL;
	return x; // ���ؽ���λ��
}
//T��Ϊx����������
template <typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>*& S) { // x->rc == NULL
	if (x->rc = S->_root) x->rc->parent = x; // ����
	_size += S->_size;
	updateHeightAbove(x); // ����ȫ����ģ�� x �������ȵĸ߶�
	S->_root = NULL;
	S->_size = 0;
	S = NULL;
	return x; // ���ؽ���λ��
}
//ɾ����λ��x���ڵ�Ϊ�������������ظ�����ԭ�ȵĹ�ģ
template <typename T>
int BinTree<T>::remove(BinNodePosi(T) x) {
	// assert: xΪ�������еĺϷ�λ��
	FromParentTo(*x) = NULL; // �ж����Ը��ڵ��ָ��
	updateHeightAbove(x->parent); // �������ȵĸ߶�
	int n = removeAt(x); _size -= n; return n; // ɾ������x�����¹�ģ������ɾ���Ľڵ���
}
template <typename T>
static int removeAt(BinNodePosi(T) x) {
	// assert: xΪ�������еĺϷ�λ��
	if (!x) return 0; // �ݹ�����սڵ�
	int n = 1 + removeAt(x->lc) + removeAt(x->rc); // �ݹ�ɾ����������
	release(x->data); release(x); return n; // �ͷŽڵ�����ݣ�����ɾ���Ľڵ���
}
//������x�ӵ�ǰ����ժ����������ת��Ϊһ�ö�������
template <typename T>
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) {
	// assert: xΪ�������еĺϷ�λ��
	FromParentTo(*x) = NULL; // �ж����Ը��ڵ��ָ��
	updateHeightAbove(x->parent); // ����ԭ�����������ȵĸ߶�
	BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = NULL; // ������xΪ��
	S->_size = x->size(); _size -= S->_size; return S; // ���¹�ģ�����ط������������
}