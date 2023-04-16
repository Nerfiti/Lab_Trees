#include <cassert>
#include <cstring>
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
    
    if (!target)
        return tree;

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
            ((target == target->parent->left) ? child->parent->left : child->parent->right) = child;
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
    tree->k = 1;

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

    if (tree->x <= key)
    {
        *left_ptr = tree;
        TreapSplit(tree->right, key, &tree->right, right_ptr);
        if (tree->right)
            tree->right->parent = tree;
        if (*right_ptr)
        {
            (*right_ptr)->parent = nullptr;
            tree->k -= (*right_ptr)->k;
        }
    }
    else
    {
        *right_ptr = tree;
        TreapSplit(tree->left, key, left_ptr, &tree->left);
        if (tree->left)
            tree->left->parent = tree;
        if (*left_ptr)
        {
            (*left_ptr)->parent = nullptr;
            tree->k -= (*left_ptr)->k;
        }
    }
}

Treap *TreapMerge(Treap *tree1, Treap *tree2)
{
    if (!tree1)
        return tree2;
    if (!tree2)
        return tree1;

    if (tree1->y < tree2->y)
    {
        tree1->k += tree2->k;
        tree1->right = TreapMerge(tree1->right, tree2);
        tree1->right->parent = tree1;
        return tree1;
    }
    else
    {
        tree2->k += tree1->k;
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

Treap *TreapInsert(Treap *tree, elem_t x, elem_t y)
{
    if (!tree)
        return TreapCtor(x, y);

    Treap *left  = nullptr,
          *right = nullptr;

    TreapSplit(tree, x, &left, &right);

    tree = TreapMerge(left, TreapCtor(x, y));
    tree = TreapMerge(tree, right);

    return tree;
}

Treap *TreapDelete(Treap *tree, elem_t x)
{
    if (!tree || !TreapExist(tree, x))
        return tree;

    Treap *temp   = nullptr,
          *left   = nullptr,
          *middle = nullptr,
          *right  = nullptr;

    TreapSplit(tree, x, &temp, &right);
    TreapSplit(temp, x-1, &left, &middle);

    if (middle)
    {
        Treap *Mleft = middle->left;
        Treap *Mright = middle->right;
        free(middle);
        middle = TreapMerge(Mleft, Mright);
    }
    return TreapMerge(left, TreapMerge(middle, right));
}

bool TreapExist(Treap *tree, elem_t x)
{
    if (!tree)
        return false;
    
    if (tree->x == x)
        return true;

    if (tree->x > x)
        return TreapExist(tree->left, x);
    
    return TreapExist(tree->right, x);
}

Treap *TreapNext(Treap *tree, elem_t x)
{
    if (!tree)
        return nullptr;
    
    if (x >= tree->x)
    {
        if (!tree->right)
            return nullptr;
        
        Treap *rightMin = TreapMinimum(tree->right);

        if (x >= rightMin->x)
            return TreapNext(tree->right, x);
        
        return rightMin;
    }

    if (!tree->left)
        return tree;
    
    elem_t leftMaxValue = TreapMaximum(tree->left)->x;

    if (x < leftMaxValue)
        return TreapNext(tree->left, x);
    
    return tree;

    return 0;
}

Treap *TreapPrev(Treap *tree, elem_t x)
{
    if (!tree)
        return nullptr;    

    if (x <= tree->x)
    {
        if (!tree->left)
            return nullptr;
        
        Treap *leftMax = TreapMaximum(tree->left);

        if (x <= leftMax->x)
            return TreapPrev(tree->left, x);
        
        return leftMax;
    }

    if (!tree->right)
        return tree;
    
    elem_t rightMinValue = TreapMinimum(tree->right)->x;

    if (x > rightMinValue)
        return TreapPrev(tree->right, x);
    
    return tree;

    return 0;
}

Treap *TreapKth(Treap *tree, size_t k)
{
    if (!tree)
        return nullptr;

    size_t leftK  = 0,
           rightK = 0;
    
    if (tree->left)
        leftK = tree->left->k;
    if (tree->right)
        rightK = tree->right->k;

    if (leftK + 1 == k)
        return tree;
    
    if (leftK + 1 < k)
        return TreapKth(tree->right, k - leftK - 1);
    
    return TreapKth(tree->left, k);
}

Treap *TreapMinimum(Treap *tree)
{
    if (!tree)
        return nullptr;

    while (tree->left)
        tree = tree->left;

    return tree;
}

Treap *TreapMaximum(Treap *tree)
{
    if (!tree)
        return nullptr;

    while (tree->right)
        tree = tree->right;

    return tree;
}

//--------------------------------------------------------------------------------

Splay *SplayCtr(elem_t val)
{
    Splay *tree = (Splay *)calloc(1, sizeof(Splay));

    tree->val = val;

    return tree;
}

void SplayZig(Splay *node)
{  
    assert(node && node->parent);

    if (node == node->parent->right)
    {
        Splay *ded              = node->parent->parent;
        Splay *parent           = node->parent;

        parent->right           = node->left;
        if (node->left)
            node->left->parent  = parent;

        parent->parent          = node;
        node->left              = parent;
        node->parent            = ded;
        if (ded)
        {
            if (ded->right == parent)
                ded->right = node;
            else
                ded->left = node;
        }
    }
    else
    {
        Splay *ded              = node->parent->parent;
        Splay *parent           = node->parent;

        parent->left            = node->right;
        if (node->right)
            node->right->parent = parent;

        parent->parent          = node;
        node->right             = parent;
        node->parent            = ded;
        if (ded)
        {
            if (ded->right == parent)
                ded->right = node;
            else
                ded->left = node;
        }
    }
}

void SplayZigZig(Splay *node)
{
    assert(node && node->parent && node->parent->parent);

    SplayZig(node->parent);
    SplayZig(node);
}

void SplayZigZag(Splay *node)
{
    assert(node && node->parent && node->parent->parent);

    SplayZig(node);
    SplayZig(node);
}

Splay *SplaySplay(Splay *node)
{
    if (!node)
        return node;

    while (node->parent)
    {
        if (!node->parent->parent)
            SplayZig(node);
        else if ( (node == node->parent->right && node->parent == node->parent->parent->right) ||
                  (node == node->parent->left  && node->parent == node->parent->parent->left )     )
        {
            SplayZigZig(node);
        }
        else
            SplayZigZag(node);
    }

    return node;
}

Splay *SplayMerge(Splay *tree1, Splay *tree2)
{
    if (!tree1)
        return tree2;
    if (!tree2)
        return tree1;

    Splay *max = tree1;

    while (max->right)
        max = max->right;

    SplaySplay(max);
    max->right = tree2;
    tree2->parent = max;

    return max;
}

void SplaySplit(Splay *tree, elem_t val, Splay **left_ptr, Splay **right_ptr)
{
    if (!tree)
    {
        *left_ptr  = nullptr;
        *right_ptr = nullptr;
    }

    Splay *target = SplayFind(tree, val);
    target = SplaySplay(target);

    if (target->val < val)
    {
        *left_ptr  = target;
        *right_ptr = target->right;
        
        if (target->right)
            target->right->parent = nullptr;
        
        target->right = nullptr;
    }
    else
    {
        *left_ptr  = target->left;
        *right_ptr = target;
        
        if (target->left)
            target->left->parent = nullptr;
        
        target->left = nullptr;
    }
}

Splay *SplayInsert(Splay *tree, elem_t val)
{
    if (!tree)
        return SplayCtr(val);

    Splay *left  = nullptr;
    Splay *right = nullptr;

    SplaySplit(tree, val, &left, &right);
    tree = SplayMerge(left, SplayCtr(val));
    tree = SplayMerge(tree, right);

    return tree;
}

Splay *SplayDelete(Splay *tree, elem_t val)
{
    Splay *target = SplayFind(tree, val);
    target = SplaySplay(target);

    if (val == target->val)
    {
        Splay *left   = target->left;
        Splay *right  = target->right;

        if (left)
            left->parent  = nullptr;
        if (right)
            right->parent = nullptr;
        free(target);
        return SplayMerge(left, right);
    }
    return target;
}

Splay *SplayFind(Splay *tree, elem_t val)
{
    if (!tree)
        return tree;

    while (val != tree->val)
    {
        if (val > tree->val)
        {
            if (!tree->right)
                return tree;
            
            tree = tree->right;
            continue;
        }

        if (!tree->left)
            return tree;

        tree = tree->left;
        continue;
    }

    return tree;
}

void SplayDtor(Splay **tree_ptr)
{
    if (!tree_ptr || !(*tree_ptr))
        return;

    SplayDtor(&(*tree_ptr)->left);
    SplayDtor(&(*tree_ptr)->right);
    free(*tree_ptr);
    *tree_ptr = nullptr;

    return;
}

//--------------------------------------------------------------------------------
