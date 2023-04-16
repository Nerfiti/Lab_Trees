#include <cassert>
#include <cstdio>
#include <cstring>
#include <random>

#include "tests.hpp"
#include "Trees.hpp"

int main()
{
    setvbuf(stdout, nullptr, _IONBF, 0);
    FILE *result = fopen("./plots/data/Treap_Test.res", "w");

    TreapTest(result);    

    fclose(result);
    return 0; 
}