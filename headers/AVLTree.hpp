#include "AVLNode.hpp"
/****************************************************************************************/
/*                                 AVLTree Prototype                                    */
/****************************************************************************************/
template <typename ITEM>
class AVLTree : public BinarySearchTree<AVLNode<ITEM>, ITEM>
{
private:
	AVLNode <ITEM> * updateHeightAndEvaluateBalance(AVLNode <ITEM> * n);
	void rotateWithLeftChild(AVLNode <ITEM> * p);
	void rotateWithRightChild(AVLNode <ITEM> * p);
	void doubleRotateWithLeftChild(AVLNode <ITEM> * p);
	void doubleRotateWithRightChild(AVLNode <ITEM> * p);
	int min;
	int max;
public:
	AVLTree();
	AVLNode <ITEM> * insert(const ITEM & item);
	bool remove(const ITEM & item);
	void traverse(AVLNode <ITEM> * n = NULL, int depth = 0);
	void printStats();
	~AVLTree();
};

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                               AVLTree Implementation                                 */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

template <typename ITEM>  AVLNode <ITEM> * AVLTree<ITEM>::updateHeightAndEvaluateBalance(AVLNode <ITEM> * n)
{
	if (n == NULL) return NULL;
	n->setHeight(MAX(HEIGHT(n->getLeft()), HEIGHT(n->getRight())) + 1);
	if (!(HEIGHT(n->getLeft()) - HEIGHT(n->getRight()) > 1 || HEIGHT(n->getRight()) - HEIGHT(n->getLeft()) > 1))
		return updateHeightAndEvaluateBalance(n->getParent());
	else return n;
}



template <typename ITEM> void AVLTree<ITEM>::rotateWithLeftChild(AVLNode <ITEM> * p)
{
	//This function need to be written by you.
	if(p->getParent() != NULL){
		if(p->getRight() == NULL){
			p->getParent()->setRight(p->getLeft());
			p->getLeft()->setParent(p->getParent()); 
			p->getLeft()->setRight(p);
			p->setLeft(NULL);
			p->setParent(p->getLeft());  
			p->setRight(NULL);
			p->setHeight(p->getHeight()-1);
		}else{
			p->setRight(p->getLeft()->getRight());
			p->getLeft()->setRight(NULL);  
			p->getLeft()->getLeft()->setLeft(NULL); 
			p->getLeft()->getLeft()->setRight(NULL); 
			p->getRight()->setRight(NULL);
			p->getRight()->setLeft(NULL);
			p->setHeight(p->getHeight()-1);
		}
	}
}

template <typename ITEM> void AVLTree<ITEM>::rotateWithRightChild(AVLNode <ITEM> * p)
{
	//This function need to be written by you.
	if(p->getParent() != NULL){
		if(p->getLeft() == NULL){
			p->getParent()->setRight(p->getRight());
			p->getRight()->setParent(p->getParent()); 
			p->getRight()->setLeft(p);
			p->setParent(p->getRight());
			p->setRight(NULL);  
			p->setLeft(NULL);  
			p->setHeight(p->getHeight()-1);
		}else{
			p->setLeft(p->getRight()->getLeft());
			p->getRight()->setLeft(NULL);  
			p->getRight()->getRight()->setRight(NULL); 
			p->getRight()->getRight()->setLeft(NULL); 
			p->getLeft()->setLeft(NULL);
			p->getLeft()->setRight(NULL);
			p->setHeight(p->getHeight()-1);
		}
	}
	
}

template <typename ITEM> void AVLTree<ITEM>::doubleRotateWithLeftChild(AVLNode <ITEM> * p)
{
	//This function need to be written by you.
	rotateWithRightChild(p->getLeft());
	rotateWithLeftChild(p);
}
template <typename ITEM> void AVLTree<ITEM>::doubleRotateWithRightChild(AVLNode <ITEM> * p)
{
	//This function need to be written by you.
	rotateWithLeftChild(p->getRight());
	rotateWithRightChild(p);
}

template <typename ITEM> AVLTree<ITEM>::AVLTree() :BinarySearchTree<AVLNode<ITEM>, ITEM>() { min = 1000000; max = 0; }
template <typename ITEM> AVLNode <ITEM> * AVLTree<ITEM>::insert(const ITEM & item) {
	AVLNode <ITEM> * avlNode = BinarySearchTree<AVLNode<ITEM>, ITEM>::insert(item);

	while (avlNode != NULL)
	{
		AVLNode <ITEM> * balance_node = updateHeightAndEvaluateBalance(avlNode);
		if (balance_node != NULL)
		{
			if (balance_node->getLeft() != NULL)
			{
				if (HEIGHT(balance_node->getLeft()->getLeft()) >= HEIGHT(balance_node->getLeft()->getRight()))
					rotateWithLeftChild(balance_node); // Insertion into the left sub tree of the left child
				else doubleRotateWithLeftChild(balance_node);
			}
			if (balance_node->getRight() != NULL)
			{
				if (HEIGHT(balance_node->getRight()->getRight()) >= HEIGHT(balance_node->getRight()->getLeft()))
					rotateWithRightChild(balance_node);  // Insertion into the right sub tree of the right child
				else doubleRotateWithRightChild(balance_node);
			}
		}
		avlNode = balance_node;
	}
	return avlNode;
}
template <typename ITEM> void AVLTree<ITEM>::traverse(AVLNode <ITEM> * n, int depth)
{
	if (n == NULL) n = this->root;
	depth++;
	if (ISLEAF(n))
	{
		if (max < depth) max = depth;
		if (min > depth) min = depth;
		return;
	}
	if (n->getLeft() != NULL) traverse(n->getLeft(), depth);
	if (n->getRight() != NULL) traverse(n->getRight(), depth);
}



template <typename ITEM> void AVLTree<ITEM>::printStats()
{
	cout << "Min:" << min << endl;
	cout << "Max:" << max << endl;

}



template <typename ITEM> bool AVLTree<ITEM>::remove(const ITEM & item) {
	return BinarySearchTree<AVLNode<ITEM>, ITEM>::remove(item);
}

template <typename ITEM> AVLTree<ITEM>::~AVLTree(){}
