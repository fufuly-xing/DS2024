#include "BinNode.h" //引入二叉树节点类
#include <ranges>
#include <algorithm>
template <typename T> class BinTree { //二叉树模板类
protected:
	int _size; BinNodePosi(T) _root; //规模、根节点
	virtual int updateHeight(BinNodePosi(T) x); //更新节点x的高度
	void updateHeightAbove(BinNodePosi(T) x); //更新节点x及其祖先的高度
public:
	BinTree() : _size(0), _root(nullptr) { } //构造函数
	~BinTree() { if (0 < _size) remove(_root); } //析构函数
	int size() const { return _size; } //规模
	bool empty() const { return !_root; } //判空
	BinNodePosi(T) root() const { return _root; } //树根
	BinNodePosi(T) insertAsRoot(T const& e); //插入根节点
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e); //e作为x的左孩子（原无）插入
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e); //e作为x的右孩子（原无）插入
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>*&T); //T作为x左子树接入
	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>*&T); //T作为x右子树接入
	int remove(BinNodePosi(T) x); //删除以位置x处节点为根的子树，返回该子树原先的规模
	BinTree<T>*secede(BinNodePosi(T) x); //将子树x从当前树中摘除，并将其转换为一棵独立子树
	template < typename VST> //操作器
		void travLevel(VST & visit) { if (_root) _root->travLevel(visit); } //局次遍历
	template < typename VST> //操作器
		void travPre(VST & visit) { if (_root) _root->travPre(visit); } //先序遍历
	template < typename VST> //操作器
		void travIn(VST & visit) { if (_root) _root->travIn(visit); } //中序遍历
	template < typename VST> //操作器
		void travPost(VST & visit) { if (_root) _root->travPost(visit); } //后序遍历
		bool operator< (BinTree<T> const& t) //比较器（其余自行补充）
			{ return _root && t._root && lt(_root, t._root); }
		bool operator== (BinTree<T> const& t) //判等器
			{ return _root && t._root && (_root == t._root); }
}; //BinTree
//更新节点x的高度
template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) // 更新节点 x 高度
{
	return x->height = 1 + std::max(stature(x->lc), stature(x->rc));// 根据左右子树的高度，计算节点 x 的高度
}
//更新节点x及其祖先的高度
template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) // 更新节点 x 及其祖先的高度
{
	while (x) {
		updateHeight(x);
		x = x->parent;
	}
}
//插入根节点
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e)
{
	_size = 1;
	return _root = new BinNode<T>(e); // 将 e 作为根节点插入空的二叉树
}
//e作为x的左孩子（原无）插入
template <typename T> 
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e) 
{
    _size++; 
    x->insertAsLC(e); 
    updateHeightAbove(x); 
    return x->lc; 
}
//e作为x的右孩子（原无）插入
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)
{
	_size++;
	x->insertAsRC(e);
	updateHeightAbove(x);
	return x->rc;
} // e 插入为 x 的右孩子
//T作为x左子树接入
template <typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>*& S) { // x->lc == NULL
	if (x->lc = S->_root) x->lc->parent = x; // 接入
	_size += S->_size;
	updateHeightAbove(x); // 更新全树规模及 x 所有祖先的高度
	S->_root = NULL;
	S->_size = 0;
	S = NULL;
	return x; // 返回接入位置
}
//T作为x右子树接入
template <typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>*& S) { // x->rc == NULL
	if (x->rc = S->_root) x->rc->parent = x; // 接入
	_size += S->_size;
	updateHeightAbove(x); // 更新全树规模及 x 所有祖先的高度
	S->_root = NULL;
	S->_size = 0;
	S = NULL;
	return x; // 返回接入位置
}
//删除以位置x处节点为根的子树，返回该子树原先的规模
template <typename T>
int BinTree<T>::remove(BinNodePosi(T) x) {
	// assert: x为二叉树中的合法位置
	FromParentTo(*x) = NULL; // 切断来自父节点的指针
	updateHeightAbove(x->parent); // 更新祖先的高度
	int n = removeAt(x); _size -= n; return n; // 删除子树x，更新规模，返回删除的节点数
}
template <typename T>
static int removeAt(BinNodePosi(T) x) {
	// assert: x为二叉树中的合法位置
	if (!x) return 0; // 递归基：空节点
	int n = 1 + removeAt(x->lc) + removeAt(x->rc); // 递归删除左右子树
	release(x->data); release(x); return n; // 释放节点和数据，返回删除的节点数
}
//将子树x从当前树中摘除，并将其转换为一棵独立子树
template <typename T>
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) {
	// assert: x为二叉树中的合法位置
	FromParentTo(*x) = NULL; // 切断来自父节点的指针
	updateHeightAbove(x->parent); // 更新原树中所有祖先的高度
	BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = NULL; // 新树以x为根
	S->_size = x->size(); _size -= S->_size; return S; // 更新规模，返回分离出来的子树
}