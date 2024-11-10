#include <iostream>
#include <stack>
#include "Queue.h"
#define BinNodePosi(T) BinNode<T>* //节点位置
#define stature(p) ((p) ? (p)->height : -1) //节点高度（不“空树高度为-1”的约定相统一）
typedef enum { RB_RED, RB_BLACK } RBColor; //节点颜色

template <typename T> struct BinNode { //二叉树节点模板类
// 成员（为简化描述起见统一开放，读者可根据需要进一步封装）
	T data; //数值
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc; //父节点及左、右孩子
	int height; //高度（通用）
	int npl; //Null Path Length（左式堆，也可直接用height代替）
	RBColor color; //颜色（红黑树）
// 构造函数
	BinNode() :
		parent(nullptr), lc(nullptr), rc(nullptr), height(0), npl(1), color(RB_RED) { }
	BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lc = nullptr, BinNodePosi(T) rc = nullptr,
			int h = 0, int l = 1, RBColor c = RB_RED) :
		data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) { }
// 操作接口
	int size(); //统计当前节点后代总数，亦即以其为根的子树的规模
	BinNodePosi(T) insertAsLC(T const&); //作为当前节点的左孩子插入新节点
	BinNodePosi(T) insertAsRC(T const&); //作为当前节点的右孩子插入新节点
	BinNodePosi(T) succ(); //取当前节点的直接后继
	template <typename VST> void travLevel(VST&); //子树层次遍历
	template <typename VST> void travPre(BinNodePosi(T) x, VST&); //子树先序遍历
	template <typename VST> void travIn(BinNodePosi(T) x, VST&); //子树中序遍历
	template <typename VST> void travPost(BinNodePosi(T) x, VST&); //子树后序遍历
// 比较器、判等器（各列其一，其余自行补充）
	bool operator< (BinNode const& bn) { return data < bn.data; } //小于
	bool operator== (BinNode const& bn) { return data == bn.data; } //等于
};

#define IsRoot(x) (!(x.parent))// 判断是否为根节点  
#define IsLChild(x) (!IsRoot(x) && (&(x) == x.parent->lc))// 判断是否为左孩子    
#define IsRChild(x) (!IsRoot(x) && (&(x) == x.parent->rc))// 判断是否为右孩子    
#define HasParent(x) (!IsRoot(x))// 判断是否有父节点  
#define HasLChild(x) (x.lc != nullptr)// 判断是否有左孩子   
#define HasRChild(x) (x.rc != nullptr)// 判断是否有右孩子    
#define HasChild(x) (HasLChild(x) || HasRChild(x))// 判断是否有孩子    
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))// 判断是否有两个孩子   
#define IsLeaf(x) (!HasChild(x))// 判断是否为叶子节点    
#define sibling(p) (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->lc)// 获取兄弟节点   
#define uncle(x) (IsLChild(*(x->parent)) ? (x)->parent->parent->rc : (x)->parent->parent->lc)// 获取叔叔节点    
#define FromParentTo(x) ( IsRoot(x) ? _root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) )
//统计当前节点后代总数，亦即以其为根的子树的规模
template <typename T> int BinNode<T>::size() {
		int count = 1;// 当前节点本身算作1个节点
		if (lc != nullptr) count += lc->size(); //递归计入左子树规模
		if (rc != nullptr) count += rc->size(); //递归计入右子树规模
		return count;
}
//将e作为当前节点的左孩子插入二叉树
template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC(T const& e)
{
	lc = new BinNode<T>(e, this);
	return lc;
} 
//将e作为当前节点的右孩子插入二叉树
template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC(T const& e)
{
	rc = new BinNode<T>(e, this);
	return rc;
} 
//取当前节点的直接后继
template <typename T>
BinNodePosi(T) BinNode<T>::succ() {
	BinNodePosi(T) s = this; // 临时变量，指向当前节点
	if (rc) { // 若有右孩子
		s = rc; // 右子树中
		while (HasLChild(*s)) s = s->lc; // 找到右子树中最靠左的节点
	}
	else { // 否则，直接后继是最低的祖先节点
		while (IsRChild(*s)) s = s->parent; // 逆向沿右向分支上移
		s = s->parent; // 最后上移到父节点
	}
	return s;
}
//子树层次遍历
template <typename T> template <typename VST> 
void BinNode<T>::travLevel(VST& visit) {
	Queue<BinNodePosi(T)> Q; // 辅助队列，用于存储当前层的节点
	Q.enqueue(this); // 根节点入队

	while (!Q.empty()) { // 在队列非空之前，不断遍历
		BinNodePosi(T) x = Q.dequeue(); // 取出队首节点
		visit(x->data); // 访问节点的数据
		if (HasLChild(*x)) Q.enqueue(x->lc); // 如果有左孩子，将左孩子入队
		if (HasRChild(*x)) Q.enqueue(x->rc); // 如果有右孩子，将右孩子入队
	}
}
//子树先序遍历
template <typename T, typename VST>
void travPre(BinNodePosi(T) x, VST& visit) { // 二叉树先序遍历算法（迭代版#1）
	if (!x)
		return;
	travPre_R(x->lc, visit);
	visit(x->data);
	travPre_R(x->rc, visit);
}
//子树中序遍历
template <typename T, typename VST> // 元素类型、操作器
void travIn(BinNodePosi(T) x, VST& visit) { // 二叉树中序遍历算法（迭代版#3，无需辅助栈）
	bool backtrack = false; // 前一步是否刚从右子树回溯，省去栈，仅用 O(1) 辅助空间
	while (true) {
		// 1. 若当前节点不是刚从右子树回溯且有左子树
		if (!backtrack && HasLChild(*x)) {
			x = x->lc; // 深入左子树
		}
		else {
			// 2. 访问当前节点
			visit(x->data);
			// 3. 若右子树非空，进入右子树继续遍历
			if (HasRChild(*x)) {
				x = x->rc;
				backtrack = false; // 关闭回溯标志
			}
			else {
				// 4. 否则，右子树为空，进行回溯
				x = x->succ(); // 使用 `succ()` 查找直接后继
				if (x == nullptr) break; // 若没有直接后继，遍历结束
				backtrack = true; // 设置回溯标志
			}
		}
	}
}
//子树后序遍历
template <typename T, typename VST>
void travPost(BinNodePosi(T) x, VST& visit) {
	if (!x)
		return;
	travPost_R(x->lc, visit);
	travPost_R(x->rc, visit);
	visit(x->data);
}