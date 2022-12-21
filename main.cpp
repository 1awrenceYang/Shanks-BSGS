#include"BSGS.h"
#include<random>
int main()
{
    miracl* mips = Init();
    big a, b, p, order, x, y, z;
    point G;
    a = mirvar(0);
    b = mirvar(0);
    p = mirvar(0);
    x = mirvar(0);
    y = mirvar(0);
    order = mirvar(0);
    G = epoint_init();
    InitSystem(a, b, p, order, G);
    big k = mirvar(0x18E8EB24);
    point kG = epoint_init();
    ecurve_mult(k, G, kG);
    //PrintPoint(kG);
    big Result = mirvar(0);
    BSGS(G, kG, Result);
    

    //This part of code is for Binary Search Tree's benchmark
    //Node* root = (Node*)malloc(sizeof(Node));
    //char TestData[8] = { 0 };
    //root->LeftChild = NULL;
    //root->RightChild = NULL;
    ////root->MyValueNode = NULL;
    //root->Value = 0;
    //int RightCounter = 0;
    //int WrongCounter = 0;
    //int ValueRightCounter = 0;
    //int ValueWrongCounter = 0;
    //unsigned int TestValue[46341] = { 0 };
    //for (int i = 0; i < 46341; i++)
    //{
    //    TestValue[i] = rand() % 0xffffffff;
    //    for (int j = 0; j < 8; j++)
    //    {
    //        TestData[j] = rand() & 0xff;
    //    }
    //    
    //    InsertDataIntoSearchTree(TestData, TestValue[i], root, 8);
    //    unsigned int Out;
    //    bool Change = rand() % 2;
    //    if (Change)
    //    {
    //        int index = rand() % 8;
    //        TestData[index] = TestData[index] + 1;
    //        bool Result = SearchData(TestData, root, 8, &Out);
    //        if (Result == 0)
    //            RightCounter++;
    //        else
    //            WrongCounter++;
    //    }
    //    else
    //    {
    //        bool Result = SearchData(TestData, root, 8, &Out);
    //        if (Result == 1)
    //            RightCounter++;
    //        else
    //            WrongCounter++;
    //        if (Out == TestValue[i])
    //            ValueRightCounter++;
    //        else
    //            ValueWrongCounter++;

    //    }

    //}
    //printf("Right:%d\n", RightCounter);

    //printf("Wrong:%d\n", WrongCounter);
    //
    //printf("Right Value:%d\n", ValueRightCounter);

    //printf("Wrong Value:%d\n", ValueWrongCounter);

}