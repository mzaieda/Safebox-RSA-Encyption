#include <iostream>
using namespace std;


#define MAX(X,Y) ((X>Y) ? X : Y)
#define HEIGHT(n) (n == NULL? 0 : n->getHeight())
#define ISLEAF(n) (n->getLeft() == NULL && n->getRight() == NULL? true :false)

/****************************************************************************************/
/*                                 BSTNode Prototype                                    */
/****************************************************************************************/

template <typename ITEM>
class BSTNode
{
protected:
	ITEM item;
	BSTNode <ITEM> * left;
	BSTNode <ITEM> * right;
	BSTNode <ITEM> * parent;
	bool cascade_delete;
public:
	BSTNode();
	BSTNode(ITEM p_item);
	void setLeft(BSTNode <ITEM> * l);
	void setRight(BSTNode <ITEM> * r);
	void setParent(BSTNode <ITEM> * p);
	BSTNode<ITEM> * getLeft();
	BSTNode<ITEM> * getRight();
	BSTNode<ITEM> * getParent();
	BSTNode<ITEM> * compareAndAdvance(const ITEM & p_item);
	ITEM & getItem();
	void setCascadeDelete(bool p_cascade_delete);
	bool getCascadeDelete();
	virtual ~BSTNode();
};


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                 BSTNode Implementation                               */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



template <typename ITEM> BSTNode<ITEM>::BSTNode() { left = right = parent = NULL; cascade_delete = true; }
template <typename ITEM> BSTNode<ITEM>::BSTNode(ITEM p_item) { left = right = parent = NULL; item = p_item; }
template <typename ITEM> void BSTNode<ITEM>::setLeft(BSTNode <ITEM> * l) { left = l; }
template <typename ITEM> void BSTNode<ITEM>::setRight(BSTNode <ITEM> * r) { right = r; }
template <typename ITEM> void BSTNode<ITEM>::setParent(BSTNode <ITEM> * p) { parent = p; }
template <typename ITEM> BSTNode<ITEM> * BSTNode<ITEM>::getLeft() { return left; }
template <typename ITEM> BSTNode<ITEM> * BSTNode<ITEM>::getRight() { return right; }
template <typename ITEM> BSTNode<ITEM> * BSTNode<ITEM>::getParent() { return parent; }
template <typename ITEM> BSTNode<ITEM> * BSTNode<ITEM>::compareAndAdvance(const ITEM & p_item)
{
	if (item > p_item)  return left;
	else if (item < p_item)  return right;
	else return this;
}
template <typename ITEM> ITEM & BSTNode<ITEM>::getItem(){ return item; }
template <typename ITEM> void BSTNode<ITEM>::setCascadeDelete(bool p_cascade_delete) { cascade_delete = p_cascade_delete; }
template <typename ITEM> bool BSTNode<ITEM>::getCascadeDelete(){ return cascade_delete; }

template <typename ITEM> BSTNode<ITEM>::~BSTNode()
{
	if (!cascade_delete) return;
	if (left != NULL) delete(left);
	if (right != NULL) delete(right);
}