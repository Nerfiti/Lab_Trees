#include <random>
#include <time.h>

#include "Trees.hpp"
#include "tests.hpp"

#define TIME (float)(1000*(t2 - t1))/CLOCKS_PER_SEC
#define SUM(ARR) (float)(ARR[0] + ARR[1] + ARR[2] + ARR[3] + ARR[4])

void FreshArray(elem_t *arr, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        size_t index = rand()%size;
        
        elem_t temp = arr[i];
        arr[i] = arr[index];
        arr[index] = arr[i];
    }
}

void NaiveTest(FILE *result)
{
    const int size = 100000;
    const int orderSize = 10000;

    elem_t *randArr     = (elem_t *)calloc(size , sizeof(elem_t));
    elem_t *swapRandArr = (elem_t *)calloc(size , sizeof(elem_t));
    elem_t *ordArr      = (elem_t *)calloc(orderSize, sizeof(elem_t));
    elem_t *swapOrdArr  = (elem_t *)calloc(orderSize, sizeof(elem_t));

    for (int i = 0; i < size; ++i)
    {
        randArr[i] = rand();
        swapRandArr[i] = randArr[i];
    }
    for (int i = 0; i < orderSize; ++i)
    {
        ordArr[i] = i;
        swapOrdArr[i] = i;
    }

    int randInsertTimer[5] = {},
        randDeleteTimer[5] = {},
         ordInsertTimer[5] = {},
         ordDeleteTimer[5] = {};

    float   rITime = 0,
            rDTime = 0,
            oITime = 0,
            oDTime = 0;

    clock_t t1 = 0,
            t2 = 0;

    //--------------------------------------------------

    Tree *tree = nullptr;

    for (int k = 0; k < 5; ++k)
    {
        tree = NaiveTreeCtor(randArr[0]);

        t1 = clock();
        for (int i = 1; i < size; ++i)
            NaiveTreeInsert(tree, randArr[i]);
        t2 = clock();

        randInsertTimer[k] = TIME;

        FreshArray(swapRandArr, size);

        t1 = clock();
        for (int i = 0; i < size/2; ++i)
        {
            tree = NaiveTreeErase(tree, swapRandArr[i]);
        }
        t2 = clock();


        randDeleteTimer[k] = TIME;

        NaiveTreeDtor(&tree);
    }
    rITime = SUM(randInsertTimer)/5;
    rDTime = SUM(randDeleteTimer)/5;

    //--------------------------------------------------

    for (int k = 0; k < 5; ++k)
    {
        printf("A");

        tree = NaiveTreeCtor(ordArr[0]);

        t1 = clock();
        for (int i = 1; i < orderSize; ++i)
            NaiveTreeInsert(tree, ordArr[i]);
        t2 = clock();

        ordInsertTimer[k] = TIME;
        printf("B");
    
        FreshArray(swapOrdArr, orderSize);

        t1 = clock();
        for (int i = 0; i < orderSize/2; ++i)
            tree = NaiveTreeErase(tree, swapOrdArr[i]);
        t2 = clock();

        ordDeleteTimer[k] = TIME;

        printf("D");   

        NaiveTreeDtor(&tree);
    }
    oITime = SUM(ordInsertTimer)/5;
    oDTime = SUM(ordDeleteTimer)/5;
    
    fprintf(result, "%d random elements: \n"
                    "Insert - %g ms      \n"
                    "Delete - %g ms (n/2)\n"
                    "                    \n"
                    "%d ordered elements \n"
                    "Insert - %g ms      \n"
                    "Delete - %g ms (n/2)\n", size, rITime, rDTime, orderSize, oITime, oDTime);
}

void AVLTest(FILE *result)
{
    FILE *RIPlot = fopen("./plots/data/RIPlot", "w");
    FILE *RDPlot = fopen("./plots/data/RDPlot", "w");
    FILE *OIPlot = fopen("./plots/data/OIPlot", "w");
    FILE *ODPlot = fopen("./plots/data/ODPlot", "w");

    setvbuf(RIPlot, nullptr, _IONBF, 0);
    setvbuf(RDPlot, nullptr, _IONBF, 0);
    setvbuf(OIPlot, nullptr, _IONBF, 0);
    setvbuf(ODPlot, nullptr, _IONBF, 0);

    for (int size = 100000; size <= 1000000; size += 10000)
    {
        const int randSize  = size;
        const int orderSize = size;

        elem_t *randArr     = (elem_t *)calloc(size , sizeof(elem_t));
        elem_t *swapRandArr = (elem_t *)calloc(size , sizeof(elem_t));
        elem_t *ordArr      = (elem_t *)calloc(orderSize, sizeof(elem_t));
        elem_t *swapOrdArr  = (elem_t *)calloc(orderSize, sizeof(elem_t));

        for (int i = 0; i < randSize; ++i)
        {
            randArr[i] = rand();
            swapRandArr[i] = randArr[i];
        }
        for (int i = 0; i < orderSize; ++i)
        {
            ordArr[i] = i;
            swapOrdArr[i] = i;
        }

        int randInsertTimer[5] = {},
            randDeleteTimer[5] = {},
            ordInsertTimer[5] = {},
            ordDeleteTimer[5] = {};

        float   rITime = 0,
                rDTime = 0,
                oITime = 0,
                oDTime = 0;

        clock_t t1 = 0,
                t2 = 0;

        //--------------------------------------------------

        AVLTree *tree = nullptr;

        for (int k = 0; k < 5; ++k)
        {            
            tree = AVLTreeCtor(randArr[0]);

            t1 = clock();
            for (int i = 1; i < randSize; ++i)
                tree = AVLTreeInsert(tree, randArr[i]);
            t2 = clock();

            randInsertTimer[k] = TIME;

            FreshArray(swapRandArr, randSize);

            t1 = clock();
            for (int i = 0; i < randSize/2; ++i)
            {
                tree = AVLTreeErase(tree, swapRandArr[i]);
            }
            t2 = clock();

            randDeleteTimer[k] = TIME;

            AVLTreeDtor(&tree);
        }
        rITime = SUM(randInsertTimer)/5;
        rDTime = SUM(randDeleteTimer)/5;

        //--------------------------------------------------

        for (int k = 0; k < 5; ++k)
        {
            tree = AVLTreeCtor(ordArr[0]);

            t1 = clock();
            for (int i = 1; i < orderSize; ++i)
                tree = AVLTreeInsert(tree, ordArr[i]);
            t2 = clock();

            ordInsertTimer[k] = TIME;
        
            FreshArray(swapOrdArr, orderSize);

            t1 = clock();
            for (int i = 0; i < orderSize/2; ++i)
                tree = AVLTreeErase(tree, swapOrdArr[i]);
            t2 = clock();

            ordDeleteTimer[k] = TIME;   

            AVLTreeDtor(&tree);
        }
        oITime = SUM(ordInsertTimer)/5;
        oDTime = SUM(ordDeleteTimer)/5;
        
        fprintf(result, "%d random elements: \n"
                        "Insert - %g ms      \n"
                        "Delete - %g ms (n/2)\n"
                        "                    \n"
                        "%d ordered elements \n"
                        "Insert - %g ms      \n"
                        "Delete - %g ms (n/2)\n", randSize, rITime, rDTime, orderSize, oITime, oDTime);
        
        fprintf(RIPlot, "%d %d\n", randSize , (int)rITime);
        fprintf(RDPlot, "%d %d\n", randSize , (int)rDTime);
        fprintf(OIPlot, "%d %d\n", orderSize, (int)oITime);
        fprintf(ODPlot, "%d %d\n", orderSize, (int)oDTime);
    }

    fclose(RIPlot);
    fclose(RDPlot);
    fclose(OIPlot);
    fclose(ODPlot);
}

void TreapTest(FILE *result)
{
    FILE *IPlot = fopen("./plots/data/IPlot", "w");
    FILE *DPlot = fopen("./plots/data/DPlot", "w");

    setvbuf(IPlot, nullptr, _IONBF, 0);
    setvbuf(DPlot, nullptr, _IONBF, 0);

    for (int size = 100000; size <= 1000000; size += 10000)
    {
        elem_t *randArr     = (elem_t *)calloc(size , sizeof(elem_t));
        elem_t *swapRandArr = (elem_t *)calloc(size , sizeof(elem_t));

        for (int i = 0; i < size; ++i)
        {
            randArr[i] = rand();
            swapRandArr[i] = randArr[i];
        }

        int InsertTimer[5] = {},
            DeleteTimer[5] = {};

        float ITime = 0,
              DTime = 0;

        clock_t t1 = 0,
                t2 = 0;

        //--------------------------------------------------

        Treap *tree = nullptr;

        for (int k = 0; k < 5; ++k)
        {
            printf("A\n");

            t1 = clock();
            for (int i = 0; i < size; ++i)
                tree = TreapInsert(tree, randArr[i], rand());
            t2 = clock();

            printf("B\n");

            InsertTimer[k] = TIME;

            FreshArray(swapRandArr, size);

            t1 = clock();
            for (int i = 0; i < size/2; ++i)
            {
                tree = TreapDelete(tree, swapRandArr[i]);
            }
            t2 = clock();

            printf("C\n");

            DeleteTimer[k] = TIME;

            TreapDtor(&tree);
        }
        ITime = SUM(InsertTimer)/5;
        DTime = SUM(DeleteTimer)/5;

        //--------------------------------------------------
        
        fprintf(result, "%d elements:        \n"
                        "Insert - %g ms      \n"
                        "Delete - %g ms (n/2)\n\n", size, ITime, DTime);
        
        fprintf(IPlot, "%d %d\n", size , (int)ITime);
        fprintf(DPlot, "%d %d\n", size , (int)DTime);
    }

    fclose(IPlot);
    fclose(DPlot);
}

void SplayTest(FILE *result)
{
    FILE *RIPlot = fopen("./plots/data/RIPlot", "w");
    FILE *RDPlot = fopen("./plots/data/RDPlot", "w");
    FILE *OIPlot = fopen("./plots/data/OIPlot", "w");
    FILE *ODPlot = fopen("./plots/data/ODPlot", "w");

    setvbuf(RIPlot, nullptr, _IONBF, 0);
    setvbuf(RDPlot, nullptr, _IONBF, 0);
    setvbuf(OIPlot, nullptr, _IONBF, 0);
    setvbuf(ODPlot, nullptr, _IONBF, 0);
    setvbuf(result, nullptr, _IONBF, 0);

    for (int size = 100000; size <= 1000000; size += 10000)
    {
        const int randSize  = size;
        const int orderSize = size;

        elem_t *randArr     = (elem_t *)calloc(size , sizeof(elem_t));
        elem_t *swapRandArr = (elem_t *)calloc(size , sizeof(elem_t));
        elem_t *ordArr      = (elem_t *)calloc(orderSize, sizeof(elem_t));
        elem_t *swapOrdArr  = (elem_t *)calloc(orderSize, sizeof(elem_t));

        for (int i = 0; i < randSize; ++i)
        {
            randArr[i] = rand();
            swapRandArr[i] = randArr[i];
        }
        for (int i = 0; i < orderSize; ++i)
        {
            ordArr[i] = i;
            swapOrdArr[i] = i;
        }

        int randInsertTimer[5] = {},
            randDeleteTimer[5] = {},
            ordInsertTimer[5] = {},
            ordDeleteTimer[5] = {};

        float   rITime = 0,
                rDTime = 0,
                oITime = 0,
                oDTime = 0;

        clock_t t1 = 0,
                t2 = 0;

        //--------------------------------------------------

        Splay *tree = nullptr;

        for (int k = 0; k < 5; ++k)
        {            
            t1 = clock();
            for (int i = 0; i < randSize; ++i)
                tree = SplayInsert(tree, randArr[i]);
            t2 = clock();

            randInsertTimer[k] = TIME;

            FreshArray(swapRandArr, randSize);

            t1 = clock();
            for (int i = 0; i < randSize/2; ++i)
            {
                tree = SplayDelete(tree, swapRandArr[i]);
            }
            t2 = clock();

            randDeleteTimer[k] = TIME;

            SplayDtor(&tree);
        }
        rITime = SUM(randInsertTimer)/5;
        rDTime = SUM(randDeleteTimer)/5;

        //--------------------------------------------------

        for (int k = 0; k < 5; ++k)
        {
            t1 = clock();
            for (int i = 0; i < orderSize; ++i)
                tree = SplayInsert(tree, ordArr[i]);
            t2 = clock();

            ordInsertTimer[k] = TIME;
        
            FreshArray(swapOrdArr, orderSize);

            t1 = clock();
            for (int i = 0; i < orderSize/2; ++i)
                tree = SplayDelete(tree, swapOrdArr[i]);
            t2 = clock();

            ordDeleteTimer[k] = TIME;   

            SplayDtor(&tree);
        }
        oITime = SUM(ordInsertTimer)/5;
        oDTime = SUM(ordDeleteTimer)/5;
        
        fprintf(result, "%d random elements: \n"
                        "Insert - %g ms      \n"
                        "Delete - %g ms (n/2)\n"
                        "                    \n"
                        "%d ordered elements \n"
                        "Insert - %g ms      \n"
                        "Delete - %g ms (n/2)\n", randSize, rITime, rDTime, orderSize, oITime, oDTime);
        
        fprintf(RIPlot, "%d %d\n", randSize , (int)rITime);
        fprintf(RDPlot, "%d %d\n", randSize , (int)rDTime);
        fprintf(OIPlot, "%d %d\n", orderSize, (int)oITime);
        fprintf(ODPlot, "%d %d\n", orderSize, (int)oDTime);
    }

    fclose(RIPlot);
    fclose(RDPlot);
    fclose(OIPlot);
    fclose(ODPlot);
}