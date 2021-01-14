#include "BSTNode.hpp"
#include <iostream>
using namespace std;
/****************************************************************************************/
/*                            BinarySearchTree Prototype                                */
/****************************************************************************************/

template <class NODE, typename ITEM>
class BinarySearchTree
{
protected:
	NODE * root;
	int count;
	void traverseASC(NODE * n);
	void traverseDESC(NODE * n);
	NODE * insert(NODE * n, const ITEM & p_item);
    //void process(NODE *n,std::function <void(NODE *)> process_node);
public:
	BinarySearchTree();
	virtual NODE * insert(const ITEM & item);
	bool remove(const ITEM & item);
	void traverseDESC();
	void traverseASC();
	NODE * findMax(NODE * n = NULL);
	NODE * findMin(NODE * n = NULL);
	bool contains(const ITEM item);
	NODE  * find(const ITEM item);
	//void process(std::function <void(NODE *)> process_node);
	int getCount();
	NODE * getRoot();
	void levelOrder();
	virtual ~BinarySearchTree();
};

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                        BinarySearchTree Implementation                               */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


template <class NODE, typename ITEM> void BinarySearchTree<NODE, ITEM>::traverseASC(NODE * n)
{
	if (n == NULL) return;
	traverseASC(n->getLeft());
	cout << n->getItem() << endl;
	traverseASC(n->getRight());
}
template <class NODE, typename ITEM> void BinarySearchTree<NODE, ITEM>::traverseDESC(NODE * n)
{
	if (n == NULL) return;
	traverseDESC((NODE *)n->getRight());
	cout << n->getItem() << endl;
	traverseDESC((NODE *)n->getLeft());
}

template <class NODE, typename ITEM> NODE * BinarySearchTree<NODE, ITEM>::insert(NODE * n, const ITEM & p_item)
{
	if (n->getItem() > p_item) {
		if (n->getLeft() == NULL)  {
			NODE * new_node = new NODE(p_item);
			n->setLeft(new_node);
			new_node->setParent(n);
			return new_node;
		} return NULL;
	}
	else if (n->getItem() < p_item)
	{
		if (n->getRight() == NULL) {
			NODE * new_node = new NODE(p_item);
			n->setRight(new_node);
			new_node->setParent(n);
			return new_node;
		}
		else return NULL;
	}
	else return NULL;
}

template <class NODE, typename ITEM> BinarySearchTree<NODE, ITEM>::BinarySearchTree()
{
	root = NULL;
	count = 0;
}

template <class NODE, typename ITEM> NODE * BinarySearchTree<NODE, ITEM>::insert(const ITEM & item)
{
	NODE * new_node = NULL;
	if (root == NULL)
	{
		new_node = root = new NODE(item);
	}
	else
	{
		NODE * parent = root;
		NODE * next = (NODE *)parent->compareAndAdvance(item);
		while (next != parent)
		{
			if (next == NULL) { new_node = (NODE *)insert(parent, item); break; }
			else parent = next;
			next = (NODE *)parent->compareAndAdvance(item);
		}
	}
	if (new_node != NULL) count++;
	return new_node;
}

template <class NODE, typename ITEM> bool BinarySearchTree<NODE, ITEM>::remove(const ITEM & item)
{
	NODE * search_node = find(item);
	if (search_node != NULL)
	{
		search_node->setCascadeDelete(false);
		NODE * parent = (NODE *)search_node->getParent();
		bool left_child = false;
		if (parent != NULL && parent->getLeft() == search_node) left_child = true;
		if (parent == NULL) { // If root
			if (search_node->getRight() == NULL) root = search_node->getLeft();
			else if (search_node->getLeft() == NULL) root = search_node->getRight();
			else {
				NODE * min = findMin((NODE *)search_node->getRight());
				if (min == NULL) exit(1);
				if (min->getParent() != NULL) {
					if (min->getParent()->getLeft() == min)
						min->getParent()->setLeft(min->getRight());
					else if (min->getParent()->getRight() == min)
						min->getParent()->setRight(min->getRight());
					if (min->getRight() != NULL) min->getRight()->setParent(min->getParent());
				}
				min->setParent(parent);
				min->setLeft(search_node->getLeft());
				min->setRight(search_node->getRight());
				if (min->getLeft() != NULL) min->getLeft()->setParent(min);
				if (min->getRight() != NULL)  min->getRight()->setParent(min);
				root = min;
			}
			if (root != NULL) root->setParent(NULL);
		}
		else if (search_node->getLeft() != NULL && search_node->getRight() != NULL)  // Two Children
		{
			NODE * min = findMin((NODE *)search_node->getRight());
			if (search_node->getRight() == min)
			{
				min->setParent(search_node->getParent());
				if (left_child) search_node->getParent()->setLeft(min);
				else search_node->getParent()->setRight(min);
				min->setLeft(search_node->getLeft());
				if (min->getLeft() != NULL) min->getLeft()->setParent(min);
				if (min->getRight() != NULL)  min->getRight()->setParent(min);
			}
			else
			{
				if (min->getParent() != NULL) min->getParent()->setLeft(min->getRight());
				if (min->getRight() != NULL) min->getRight()->setParent(min->getParent());
				if (left_child) parent->setLeft(min);
				else parent->setRight(min);
				min->setParent(parent);
				min->setRight(search_node->getRight());
				min->setLeft(search_node->getLeft());
				if (min->getLeft() != NULL) min->getLeft()->setParent(min);
				if (min->getRight() != NULL)  min->getRight()->setParent(min);
			}
		}
		else if (search_node->getLeft() != NULL){          // One Left Children
			if (left_child) parent->setLeft(search_node->getLeft());
			else parent->setRight(search_node->getLeft());
			search_node->getLeft()->setParent(parent);
		}
		else if (search_node->getRight() != NULL){             // One Right Children
			if (left_child) parent->setLeft(search_node->getRight());
			else parent->setRight(search_node->getRight());
			search_node->getRight()->setParent(parent);
		}
		else {                   // Leaf Node
			if (left_child) parent->setLeft(NULL);
			else parent->setRight(NULL);
		}
		count--;
		search_node->setLeft(NULL);
		search_node->setRight(NULL);
		delete (search_node);
		return true;
	}
	return false;
}

template <class NODE, typename ITEM> NODE * BinarySearchTree<NODE, ITEM>::findMax(NODE  * n)
{
	if (root == NULL) return NULL;
	if (n == NULL) return findMax(root);
	else if (n->getRight() == NULL) return n;
	else return findMax((NODE *)n->getRight());
}
template <class NODE, typename ITEM> NODE * BinarySearchTree<NODE, ITEM>::findMin(NODE * n)
{
	if (root == NULL) return NULL;
	if (n == NULL) return findMin(root);
	else if (n->getLeft() == NULL) return n;
	else return findMin((NODE *)n->getLeft());
}

template <class NODE, typename ITEM> void BinarySearchTree<NODE, ITEM>::traverseASC()
{
	traverseASC(root);
}
template <class NODE, typename ITEM> void BinarySearchTree<NODE, ITEM>::traverseDESC()
{
	traverseDESC(root);
}
template <class NODE, typename ITEM> bool BinarySearchTree<NODE, ITEM>::contains(const ITEM item)
{
	if (find(item) == NULL) return false;
	else return true;
}

template <class NODE, typename ITEM> NODE * BinarySearchTree<NODE, ITEM>::find(const ITEM item)
{
	//This function need to be written by you.
	if ( root != NULL) {
		NODE * parent = root;
		NODE * next = (NODE *) parent->compareAndAdvance(item);
		while ( next != parent) {
			if ( next == NULL ) break;
			else parent = next;
			next = (NODE *) parent->compareAndAdvance(item);
		}
		if ( next == parent ) return next;
	}
	return NULL;
}

template <class NODE, typename ITEM> void BinarySearchTree<NODE, ITEM>::process(NODE * n,std::function <void (NODE *)> process_node, bool flag)
{
		if (flag){

		if (n == NULL) return;

		process(n->getLeft(),[](NODE* n)->void {

			cout<<n->getItem().filename << "--" << n->getItem().blocksize << "--"<< n-> getItem().blocks << "--" << n-> getItem().bytes<< "--" << n->getItem().shreds << "--" << n->getItem().hashed_name << endl;

		 },flag);
		
		process_node(n);

		process(n->getRight(),[](NODE* n)->void {

			cout<<n->getItem().filename << "--" << n->getItem().blocksize << "--"<< n-> getItem().blocks << "--" << n-> getItem().bytes<< "--" << n->getItem().shreds << "--" << n->getItem().hashed_name << endl;

		 },flag);
			

		} else {

		if (n == NULL) return;

			process(n->getRight(),[](NODE* n)->void {

			cout<<n->getItem().filename << "--" << n->getItem().blocksize << "--"<< n-> getItem().blocks << "--" << n-> getItem().bytes<< "--" << n->getItem().shreds << "--" << n->getItem().hashed_name << endl;

		 },flag);
		
		process_node(n);

		process(n->getLeft(),[](NODE* n)->void {

			cout<<n->getItem().filename << "--" << n->getItem().blocksize << "--"<< n-> getItem().blocks << "--" << n-> getItem().bytes<< "--" << n->getItem().shreds << "--" << n->getItem().hashed_name << endl;

		 },flag);
}

template <class NODE, typename ITEM> void BinarySearchTree<NODE, ITEM>::process(std::function <void (NODE *)> process_node)
{
	//NODE * n = getRoot();
	process(root, process_node, flag);
}

template <class NODE, typename ITEM> int BinarySearchTree<NODE, ITEM>::getCount()
{
	return count;
}

template <class NODE, typename ITEM> NODE * BinarySearchTree<NODE, ITEM>::getRoot()
{
	return root;
}

template <class NODE, typename ITEM> void BinarySearchTree<NODE, ITEM>::levelOrder()
{

}

template <class NODE, typename ITEM> BinarySearchTree<NODE, ITEM>::~BinarySearchTree()
{
	if (root != NULL) delete (root);
}
