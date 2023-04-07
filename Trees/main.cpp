#include <cassert>
#include <cstdio>

#include "Trees.hpp"

void execute()
{
    unsigned int n = 0;
    int trash = scanf("%u\n", &n);
    assert(trash);

    Treap *tree = nullptr;
    for (int i = 0; i < n; ++i)
    {
        elem_t x = 0;
        elem_t y = 0;

        trash = scanf("%u %u", &x, &y);
        assert(trash);

        tree = TreapMerge(tree, TreapCtor(x, y));        
    }

    printf("YES\n");
    TreapPrintInfo(tree);
}

int main()
{
    execute();
    return 0; 
}