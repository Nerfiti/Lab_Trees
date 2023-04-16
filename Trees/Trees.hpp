#include <cstdio>

typedef int elem_t;

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

    size_t k = 0; 
};

Treap   *TreapCtor  (elem_t x, elem_t y);
void     TreapSplit (Treap *tree, elem_t key, Treap **left_ptr, Treap **right_ptr);
Treap   *TreapMerge (Treap *tree1, Treap *tree2);
void     TreapDtor  (Treap **tree_ptr);

Treap   *TreapInsert(Treap *tree, elem_t x, elem_t y);
Treap   *TreapDelete(Treap *tree, elem_t x);
bool     TreapExist (Treap *tree, elem_t x);
Treap   *TreapNext  (Treap *tree, elem_t x);
Treap   *TreapPrev  (Treap *tree, elem_t x);
Treap   *TreapKth   (Treap *tree, size_t k);

Treap   *TreapMinimum(Treap *tree);
Treap   *TreapMaximum(Treap *tree);

//--------------------------------------------------------------------------------

const unsigned int MAX_STR_LEN = 500;

struct Splay
{
    elem_t val;

    Splay *parent;
    Splay *left;
    Splay *right;
};

Splay *SplayCtr     (elem_t val);
void   SplayZig     (Splay *node);
void   SplayZigZig  (Splay *node);
void   SplayZigZag  (Splay *node);
Splay *SplaySplay   (Splay *node);
Splay *SplayMerge   (Splay *tree1, Splay *tree2);
void   SplaySplit   (Splay *tree, elem_t val, Splay **left_ptr, Splay **right_ptr);
Splay *SplayInsert  (Splay *tree, elem_t val);
Splay *SplayDelete  (Splay *tree, elem_t val);
Splay *SplayFind    (Splay *tree, elem_t val);
void   SplayDtor    (Splay **tree_ptr);
//--------------------------------------------------------------------------------
