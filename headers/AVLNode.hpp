#include "BinarySearchTree.hpp"
/****************************************************************************************/
/*                                 AVLNode Prototype                                    */
/****************************************************************************************/

template <typename ITEM>
class AVLNode : public BSTNode <ITEM>
{
private:
	int height;
public:
	AVLNode();
	AVLNode(ITEM p_item);
	void setHeight(int p_height);
	int getHeight();
	AVLNode<ITEM> * getLeft();
	AVLNode<ITEM> * getRight();
	AVLNode<ITEM> * getParent();
	~AVLNode();
};



/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                               AVLNode Implementation                                 */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

template <typename ITEM> AVLNode<ITEM>::AVLNode() :BSTNode<ITEM>(){ height = 1; }
template <typename ITEM> AVLNode<ITEM>::AVLNode(ITEM p_item) : BSTNode<ITEM>(p_item){ height = 1; }
template <typename ITEM> void AVLNode<ITEM>::setHeight(int p_height) { height = p_height; }
template <typename ITEM> int AVLNode<ITEM>::getHeight() { return height; }
template <typename ITEM> AVLNode<ITEM> * AVLNode<ITEM>::getLeft() { return (AVLNode<ITEM> *) this->left; }
template <typename ITEM> AVLNode<ITEM> * AVLNode<ITEM>::getRight() { return (AVLNode<ITEM> *) this->right; }
template <typename ITEM> AVLNode<ITEM> * AVLNode<ITEM>::getParent() { return (AVLNode<ITEM> *) this->parent; }
template <typename ITEM> AVLNode<ITEM>::~AVLNode() {}
