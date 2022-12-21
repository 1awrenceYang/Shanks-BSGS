#pragma once
#ifndef  _BSGS_H
#define _BSGS_H
extern "C"
{
#include"miracl.h"
#include<stdlib.h>
}
#include<iostream>
#include"CurveParameters.h"
#include<unordered_map>
#include<utility>
#include<bitset>
typedef epoint* point;
#define RightJustify 1
typedef struct ValueNode
{
    int Value;
}Vnode;
typedef struct MyNode
{
    MyNode* LeftChild = NULL;
    MyNode* RightChild = NULL;
    int Value;
}Node;

miracl* Init()
{
    miracl* mip = mirsys(36, MAXBASE);
    time_t seed;
    time(&seed);
    irand((unsigned long long)seed);
    mip->IOBASE = 16;
    return mip;
}
void ChangeCurve(big a,big b,big p)//Change elliptic curve for current miracl instance
{
    ecurve_init(a, b, p, MR_PROJECTIVE);
    return;
}
void PrintPoint(point a)//print the poinnt on the current curve
{
    big x = mirvar(0);
    big y = mirvar(0);
    epoint_get(a, x, y);
    printf("X:");
    cotnum(x, stdout);
    printf("Y:");
    cotnum(y, stdout);
    mirkill(x);
    mirkill(y);
    return;
}
void LoadCurveParam(big a, big b, big p,big order)//Load elliptic curve parameters, and set them on the current active curve
{
    big TempA = mirvar(0);
    big TempB = mirvar(0);
    big TempP = mirvar(0);
    big TempOrder = mirvar(0);
    big TempX = mirvar(0);
    big TempY = mirvar(0);
    bytes_to_big(4, Param_a, TempA);
    bytes_to_big(4, Param_b, TempB);
    bytes_to_big(4, ParamPrime, TempP);
    bytes_to_big(4, Param_x, TempX);
    bytes_to_big(4, Param_y, TempY);
    bytes_to_big(4, Param_order, TempOrder);
    point G = epoint_init();
    copy(TempA, a);
    copy(TempB, b);
    copy(TempP, p);
    copy(TempOrder, order);
    mirkill(TempA);
    mirkill(TempB);
    mirkill(TempP);
    mirkill(TempOrder);
    epoint_free(G);
}
void SetGenerator(point G)
{
    big X = mirvar(0);
    big Y = mirvar(0);
    bytes_to_big(4, Param_x, X);
    bytes_to_big(4, Param_y, Y);
    epoint_set(X, Y, 1, G);
    mirkill(X);
    mirkill(Y);
}
void InitSystem(big outA,big outB,big outP,big outOrder,point outG)
{
    big a, b, p, order;
    a = mirvar(0);
    b = mirvar(0);
    p = mirvar(0);
    order = mirvar(0);
    point G = epoint_init();
    LoadCurveParam(a, b, p, order);
    ChangeCurve(a, b, p);
    SetGenerator(G);
    copy(a, outA);
    copy(b, outB);
    copy(p, outP);
    copy(order, outOrder);
    epoint_copy(G, outG);
    mirkill(a);
    mirkill(b);
    mirkill(p);
    mirkill(order);
    epoint_free(G);
}
void GetXY(point A, big outX, big outY)
{
    big X = mirvar(0);
    big Y = mirvar(0);
    epoint_get(A, X, Y);
    copy(X, outX);
    copy(Y, outY);
    mirkill(X);
    mirkill(Y);
}
void HashMapInsert(std::unordered_map<unsigned long long, unsigned int>MyMap, point MyPoint,big Value)
{
    big X = mirvar(0);
    big Y = mirvar(0);
    epoint_get(MyPoint, X, Y);
    char* Xchar, * Ychar, * Allchar, *ValueChar;
    Xchar = (char*)malloc(4 * sizeof(char));
    Ychar = (char*)malloc(4 * sizeof(char));
    Allchar = (char*)malloc(8 * sizeof(char));
    ValueChar = (char*)malloc(4 * sizeof(char));
    big_to_bytes(4, X, (char*)Xchar, RightJustify);
    big_to_bytes(4, Y, (char*)Ychar, RightJustify);
    big_to_bytes(4, Value, ValueChar, RightJustify);
    for (int i = 0; i < 4; i++)
    {
        Allchar[i] = Xchar[i];
        Allchar[i + 4] = Ychar[i];
    }
    unsigned long long index = Allchar[7] + (Allchar[6] << 8) + (Allchar[5] << 16) + \
        (Allchar[4] << 24) + (Allchar[3] << 32) + (Allchar[2] << 40) + (Allchar[1] << 48) + \
        (Allchar[0] << 56);
    for (int i = 0; i < 4; i++)
        printf("%02X", (uint8_t)ValueChar[i]);
    unsigned int MapValue = ValueChar[3] + ValueChar[2] << 8 + ValueChar[1] << 16 + ValueChar[0] << 24;
    MapValue = 11111;
    printf("%lld\n", index);
    mirkill(X);
    mirkill(Y);
    free(Xchar);
    free(Ychar);
}

void InsertDataIntoSearchTree(char* Data, unsigned int Value, Node* root,unsigned int ByteNumber)
{
    Node* Ptr = root;
    char Byte = 0x00;
    char Bit = 0x00;
    for (int i = 0; i < ByteNumber; i++)
    {
        Byte = Data[i];
        for (int j = 7; j >=0; j--)
        {
            Bit = (Byte >> j) & 0x01;
            if (Bit == 0)
            {
                if (Ptr->LeftChild != NULL)
                {
                    Ptr = Ptr->LeftChild;
                    continue;
                }
                Node* LeftChild = (Node*)malloc(sizeof(Node));
                Ptr->LeftChild = LeftChild;
                Ptr = Ptr->LeftChild;
                Ptr->LeftChild = NULL;
                Ptr->RightChild = NULL;
                //Ptr->MyValueNode = NULL;
                Ptr->Value = Value;
            }
            else if (Bit == 1)
            {
                if (Ptr->RightChild != NULL)
                {
                    Ptr = Ptr->RightChild;
                    continue;
                }
                Node* RightChild = (Node*)malloc(sizeof(Node));
                Ptr->RightChild = RightChild;
                Ptr = Ptr->RightChild;
                Ptr->LeftChild = NULL;
                Ptr->RightChild = NULL;
                //Ptr->MyValueNode = NULL;
                Ptr->Value = Value;
            }
            if (i == 7 && j == 0)
            {
                Ptr->Value = Value;
            }
            
        }
    }
}
bool SearchData(char* Data, Node* root,unsigned int ByteNumber,unsigned int* outValue)
{
    Node* Ptr = root;
    char Byte = 0x00;
    char Bit = 0x00;
    //*outValue = 01;
    for (int i = 0; i < ByteNumber; i++)
    {
        Byte = Data[i];
        for (int j = 7; j >= 0; j--)
        {
            Bit = (Byte >> j);
            Bit = Bit & 0x01;
            if (Bit == 0)
            {
                if (Ptr->LeftChild == NULL)
                    return false;
                else
                {
                    if (i == 7 && j == 0)
                        *outValue = Ptr->Value;
                    Ptr = Ptr->LeftChild;
                }
                
            }
            if (Bit == 1)
            {
                if (Ptr->RightChild == NULL)
                    return false;
                else
                {
                    if (i == 7 && j == 0)
                        *outValue = Ptr->Value;
                    Ptr = Ptr->RightChild;
                }
                
            }
        }
    }
    return true;
}
void InitRootNode(Node* root)
{
    root->LeftChild = NULL;
    root->RightChild = NULL;
    
}
void BSGS(point G, point kG, big outK)
{
    Node* root = (Node*)malloc(sizeof(Node));
    InitRootNode(root);
    big SqrtPBig = mirvar(46341);
    int SqrtPInt = 46341;
    big X = mirvar(0); big Y = mirvar(0);
    point iG = epoint_init();
    big BigI = mirvar(0);
    point SqrtPG = epoint_init();
    point Left = epoint_init();
    char* Xchar, * Ychar, * Allchar;
    Xchar = (char*)malloc(4 * sizeof(char));
    Ychar = (char*)malloc(4 * sizeof(char));
    Allchar = (char*)malloc(8 * sizeof(char));
    ecurve_mult(SqrtPBig, G, SqrtPG);
    unsigned int Collision = 0;
    for (int i = 0; i < SqrtPInt; i++)
    {
        BigI = mirvar(i);
        ecurve_mult(BigI, G, iG);
        ecurve_add(kG, iG);
        epoint_get(iG, X, Y);
        big_to_bytes(4, X, Xchar, RightJustify);
        big_to_bytes(4, Y, Ychar, RightJustify);
        for (int i = 0; i < 4; i++)
        {
            Allchar[i] = Xchar[i];
            Allchar[i + 4] = Xchar[i];
        }
        //HashMapInsert(HashMap, iG);
        InsertDataIntoSearchTree(Allchar, i, root, 8);
        //HashMap[iG] = BigI;
    }
    for (int i = 0; i < SqrtPInt; i++)
    {
        BigI = mirvar(i);
        ecurve_mult(BigI, SqrtPG, Left);
        epoint_get(Left, X, Y);
        big_to_bytes(4, X, Xchar, RightJustify);
        big_to_bytes(4, Y, Ychar, RightJustify);

        for (int i = 0; i < 4; i++)
        {
            Allchar[i] = Xchar[i];
            Allchar[i + 4] = Xchar[i];
        }
        if (SearchData(Allchar,root,8, &Collision))
        {
            //big B = mirvar(0);
            printf("There is a collision\n");
            printf("My value %d,vaule:%d\n", i, Collision);
            printf("The Secret Key is:%d\n", i * SqrtPInt - Collision);
            /*big Temp = mirvar(0);
            multiply(BigI, SqrtPBig, Temp);
            subtract(Temp, B, Temp);
            copy(Temp, outK);*/
            return;
        }
    }
}


















#endif // ! _BSGS_H

