#include <cstdio>

typedef unsigned int elem_t;

struct Tree
{
    elem_t value    = 0;

    Tree  *parent   = nullptr;
    Tree  *left     = nullptr;
    Tree  *right    = nullptr;
};

Tree *NaiveTreeCtor     (elem_t value, Tree *parent = nullptr);
void  NaiveTreeInsert   (Tree *tree, elem_t value);
Tree *NaiveTreeErase    (Tree *tree, elem_t value);
void  NaiveTreeDtor     (Tree **tree_ptr);
Tree *NaiveGetNext      (Tree *tree);

Tree  *Search           (Tree *tree, elem_t value);
Tree  *GetMinimum       (Tree *tree);

//--------------------------------------------------------------------------------

struct AVLTree
{
    elem_t value    = 0;
    size_t height   = 0;

    AVLTree  *left     = nullptr;
    AVLTree  *right    = nullptr;
};

AVLTree  *AVLTreeCtor       (elem_t value);
AVLTree  *AVLTreeInsert     (AVLTree *tree, elem_t value);
AVLTree  *AVLTreeErase      (AVLTree *tree, elem_t value);
AVLTree  *AVLRotateLeft     (AVLTree *tree);
AVLTree  *AVLRotateRight    (AVLTree *tree);
AVLTree  *AVLBalance        (AVLTree *tree);
int       AVLBalanceFactor  (AVLTree *tree);
size_t    AVLHeight         (AVLTree *tree);
void      AVLFixHeight      (AVLTree *tree);
elem_t    AVLGetNext        (AVLTree *tree, elem_t value);
AVLTree  *GetMinimum        (AVLTree *tree);
AVLTree  *GetMaximum        (AVLTree *tree);
AVLTree  *RemoveMin         (AVLTree *tree);
void      AVLTreeDtor       (AVLTree **tree_ptr);

//--------------------------------------------------------------------------------

struct Treap
{
    elem_t x = 0;
    elem_t y = 0;

    Treap *parent   = nullptr;
    Treap *left     = nullptr;
    Treap *right    = nullptr;
};

Treap   *TreapCtor  (elem_t x, elem_t y);
void     TreapSplit (Treap *tree, elem_t key, Treap **left_ptr, Treap **right_ptr);
Treap   *TreapMerge (Treap *tree1, Treap *tree2);
void     TreapDtor  (Treap **tree_ptr);

void     TreapPrintInfo(Treap *tree);


//--------------------------------------------------------------------------------
