#include <cassert>
#include <malloc.h>

#include "Trees.hpp"

//--------------------------------------------------------------------------------

Tree *NaiveTreeCtor(elem_t value, Tree *parent)
{
    Tree *tree = (Tree *)calloc(1, sizeof(Tree));

    tree->parent = parent;
    tree->value  = value;

    return tree;
}

void NaiveTreeInsert(Tree *tree, elem_t value)
{
    if (!tree)
        return;

    if (value >= tree->value)
    {
        if (!tree->right)
        {
            Tree *child = NaiveTreeCtor(value, tree);
            tree->right = child;
            return;
        }
        NaiveTreeInsert(tree->right, value);
        return; 
    }
        
    if (!tree->left)
    {
        Tree *child = NaiveTreeCtor(value, tree);
        tree->left = child;
        return;
    }
    NaiveTreeInsert(tree->left, value);
}

Tree *NaiveTreeErase(Tree *tree, elem_t value)
{
    if (!tree)
        return nullptr;
    
    Tree *target = Search(tree, value);

    if (!target->left && !target->right)
    {
        if (!target->parent)
        {
            free(tree);
            return nullptr;
        }
        ((target == target->parent->left) ? target->parent->left : target->parent->right) = nullptr;
        free(target);

        return tree;
    }

    if (!target->left || !target->right)
    {
        Tree *child = (target->left) ? target->left : target->right;
        child->parent = target->parent;

        if (target->parent)
        {
            ((child == child->parent->left) ? child->parent->left : child->parent->right) = child;
        }

        free(target);

        if (child->parent)
            return tree;
        return child;
    }

    Tree *next = NaiveGetNext(target);
    assert(next);
    
    next->left   = target->left;
    next->parent = target->parent;
    next->value  = target->value;

    if (next->left)
        next->left->parent = next;
    
    if (next->parent)
        ((target == target->parent->left) ? target->parent->left : target->parent->right) = next;

    free(target);

    if (next->parent)
        return tree;
    return next;
}

void NaiveTreeDtor(Tree **tree_ptr)
{
    if (!tree_ptr || !(*tree_ptr))
        return;

    NaiveTreeDtor(&(*tree_ptr)->left);
    NaiveTreeDtor(&(*tree_ptr)->right);
    free(*tree_ptr);
    *tree_ptr = nullptr;

    return;
}

Tree *NaiveGetNext(Tree *tree)
{
    if (tree->right)
        return GetMinimum(tree->right);

    Tree *next = tree->parent;
    while (next && tree == next->right)
    {
        tree = next;
        next = next->parent;
    }
    return next;
}

Tree *Search(Tree *tree, elem_t value)
{   
    if (!tree || tree->value == value)
        return tree;

    if (value > tree->value)
        return Search(tree->right, value);
    
    return Search(tree->left, value);
}

Tree *GetMinimum(Tree *tree)
{
    if (!tree)
        return nullptr;

    while (tree->left)
        tree = tree->left;

    return tree;
}

//--------------------------------------------------------------------------------

AVLTree *AVLTreeCtor(elem_t value)
{
    AVLTree *tree = (AVLTree *)calloc(1, sizeof(AVLTree));
    
    tree->value  = value;
    tree->height = 1;

    return tree;
}

AVLTree *AVLTreeInsert(AVLTree *tree, elem_t value)
{
    if (!tree)
        return AVLTreeCtor(value);

    if (value >= tree->value)
    {
        tree->right = AVLTreeInsert(tree->right, value);
    }
    else
    {
        tree->left = AVLTreeInsert(tree->left, value);
    }
    return AVLBalance(tree);
}

AVLTree *AVLTreeErase(AVLTree *tree, elem_t value)
{
    if (!tree)
        return nullptr;

    if (value < tree->value)
        tree->left = AVLTreeErase(tree->left, value);
    else if (value > tree->value)
        tree->right = AVLTreeErase(tree->right, value);
    else // if (value == tree->value)
    {
        AVLTree *left   = tree->left;
        AVLTree *right  = tree->right;
        free(tree);

        if (!right)
            return left;

        AVLTree *min = GetMinimum(right);
        min->right   = RemoveMin (right);
        min->left = left;

        return AVLBalance(min);
    }
    return AVLBalance(tree);
}

AVLTree *AVLRotateLeft(AVLTree *tree)
{
    if (!tree || !tree->right)
        return tree;

    AVLTree *right = tree->right;

    tree->right = right->left;

    right->left = tree;

    AVLFixHeight(tree);
    AVLFixHeight(right);

    return right;
}

AVLTree *AVLRotateRight(AVLTree *tree)
{
    if (!tree || !tree->left)
        return tree;

    AVLTree *left = tree->left;

    tree->left = left->right;

    left->right = tree;

    AVLFixHeight(tree);
    AVLFixHeight(left);

    return left;
}

AVLTree *AVLBalance(AVLTree *tree)
{
    if (!tree)
        return nullptr;

    AVLFixHeight(tree);
    int balanceFactor = AVLBalanceFactor(tree);

	if(balanceFactor == -2)
	{
		if(AVLBalanceFactor(tree->right) > 0)
			tree->right = AVLRotateRight(tree->right);
		return AVLRotateLeft(tree);
	}
	if(balanceFactor == 2)
	{
		if(AVLBalanceFactor(tree->left) < 0)
			tree->left = AVLRotateLeft(tree->left);
		return AVLRotateRight(tree);
	}
	return tree;
}

int AVLBalanceFactor(AVLTree *tree)
{
    return (AVLHeight(tree->left) - AVLHeight(tree->right));
}

size_t AVLHeight(AVLTree *tree)
{
    return (tree) ? tree->height : 0;
}

void AVLFixHeight(AVLTree *tree)
{
    size_t leftH  = AVLHeight(tree->left);
    size_t rightH = AVLHeight(tree->right);
    size_t maxH   = (leftH > rightH) ? leftH : rightH;

    tree->height = maxH + 1;
}

void AVLTreeDtor(AVLTree **tree_ptr)
{
    if (!tree_ptr || !(*tree_ptr))
        return;

    AVLTreeDtor(&(*tree_ptr)->left);
    AVLTreeDtor(&(*tree_ptr)->right);
    free(*tree_ptr);
    *tree_ptr = nullptr;

    return;
}

elem_t AVLGetNext(AVLTree *tree, elem_t value)
{
    if (!tree)
        return -1;
    
    if (value == tree->value)
        return tree->value;

    if (value > tree->value)
    {
        if (!tree->right)
            return -1;
        
        elem_t rightMinValue = GetMinimum(tree->right)->value;

        if (value >= rightMinValue)
            return AVLGetNext(tree->right, value);
        
        return rightMinValue;
    }

    if (!tree->left)
        return -1;
    
    elem_t leftMaxValue = GetMaximum(tree->left)->value;

    if (value <= leftMaxValue)
        return AVLGetNext(tree->left, value);
    
    return tree->value;

    return 0;
}

AVLTree *GetMinimum(AVLTree *tree)
{
    if (!tree)
        return nullptr;

    while (tree->left)
        tree = tree->left;

    return tree;
}

AVLTree *GetMaximum(AVLTree *tree)
{
    if (!tree)
        return nullptr;

    while (tree->right)
        tree = tree->right;

    return tree;
}

AVLTree *RemoveMin(AVLTree *tree)
{
    if (!tree->left)
        return tree->right;

    tree->left = RemoveMin(tree->left);

    return AVLBalance(tree);
}

//--------------------------------------------------------------------------------

Treap *TreapCtor(elem_t x, elem_t y)
{
    Treap *tree = (Treap *)calloc(1, sizeof(Treap));

    tree->x = x;
    tree->y = y;

    return tree;
}

void TreapSplit(Treap *tree, elem_t key, Treap **left_ptr, Treap **right_ptr)
{
    assert(left_ptr && right_ptr);

    if (!tree)
    {
        *left_ptr  = nullptr;
        *right_ptr = nullptr;
        return;
    }

    if (tree->x < key)
    {
        *left_ptr = tree;
        TreapSplit(tree->right, key, &tree->right, right_ptr);
        if (tree->right)
            tree->right->parent = tree;
        if (*right_ptr)
            (*right_ptr)->parent = nullptr;
    }
    else
    {
        *right_ptr = tree;
        TreapSplit(tree->left, key, left_ptr, &tree->left);
        if (tree->left)
            tree->left->parent = tree;
        if (*left_ptr)
            (*left_ptr)->parent = nullptr;
    }
}

Treap *TreapMerge(Treap *tree1, Treap *tree2)
{
    if (!tree1)
        return tree2;
    if (!tree2)
        return tree1;

    if (tree1->y > tree2->y)
    {
        tree1->right = TreapMerge(tree1->right, tree2);
        tree1->right->parent = tree1;
        return tree1;
    }
    else
    {
        tree2->left = TreapMerge(tree1, tree2->left);
        tree2->left->parent = tree2;
        return tree2;
    }
}

void TreapDtor(Treap **tree_ptr)
{
    if (!tree_ptr || !(*tree_ptr))
        return;

    TreapDtor(&(*tree_ptr)->left);
    TreapDtor(&(*tree_ptr)->right);
    free(*tree_ptr);
    *tree_ptr = nullptr;
}

void TreapPrintInfo(Treap *tree)
{
    if (!tree)
        return;

    int parent = 0;
    int left   = 0;
    int right  = 0;

    if (tree->parent)
        parent = tree->parent->x;
    if (tree->left)
        left = tree->left->x;
    if (tree->right)
        right = tree->right->x;

    TreapPrintInfo(tree->left);
    printf("%u %u %u\n", parent, left, right);
    TreapPrintInfo(tree->right);
}

//--------------------------------------------------------------------------------
