//
// Created by lewis on 19-7-21.
//

#ifndef ENPAC_2020_3_0_RG_H
#define ENPAC_2020_3_0_RG_H

#include "Petri_Net.h"
#include <bitset>
#include <malloc.h>
#include "BasicIncludes.h"
#include "CMemoryPool.h"
#include <google/malloc_extension.h>

using namespace std;

#define RGTABLE_SIZE 100000


extern MemPool::CMemoryPool *g_ptrMemPool;
extern NUM_t FIELDCOUNT;   //占用bitfield个数，仅仅用于NUPN和SAFE网
extern NUM_t MARKLEN;      //Petri网
extern NUM_t placecount;   //Petri网库所个数
extern bool NUPN;          //当前Petri网是否有NUPN信息
extern bool SAFE;          //当前Petri网是否为安全网

void setGlobalValue(Petri *ptnet);

typedef struct Bitfielduint
{
    unsigned int a0:1;
    unsigned int a1:1;
    unsigned int a2:1;
    unsigned int a3:1;
    unsigned int a4:1;
    unsigned int a5:1;
    unsigned int a6:1;
    unsigned int a7:1;
    unsigned int a8:1;
    unsigned int a9:1;
    unsigned int a10:1;
    unsigned int a11:1;
    unsigned int a12:1;
    unsigned int a13:1;
    unsigned int a14:1;
    unsigned int a15:1;
    unsigned int a16:1;
    unsigned int a17:1;
    unsigned int a18:1;
    unsigned int a19:1;
    unsigned int a20:1;
    unsigned int a21:1;
    unsigned int a22:1;
    unsigned int a23:1;
    unsigned int a24:1;
    unsigned int a25:1;
    unsigned int a26:1;
    unsigned int a27:1;
    unsigned int a28:1;
    unsigned int a29:1;
    unsigned int a30:1;
    unsigned int a31:1;

    //将相应位置上设置为1；
    void set(int index);
    void reset(int index);   //将相应位置上设置为0；
    bool test0(int index);
    bool test1(int index);
} myuint;


/*********************Global Functions**********************/
void DecToBinary(index_t DecNum, unsigned short *Binarystr);
void BinaryToDec(index_t &DecNum, unsigned short *Binarystr, NUM_t marklen);

typedef unsigned short Mark;
typedef unsigned long ID;

class RGNode
{
public:
    Mark *marking;
    RGNode *next;
public:
    RGNode();
    //NUM_t tokensum();
    index_t Hash();
    ~RGNode();

//    void *operator new(std::size_t ObjectSize)
//    {
//        return g_ptrMemPool->GetMemory(ObjectSize) ;
//    }
//
//    void operator delete(void *ptrObject, std::size_t ObjectSize)
//    {
//        g_ptrMemPool->FreeMemory(ptrObject, ObjectSize) ;
//    }
};

class BitRGNode
{
public:
    myuint *marking;
    BitRGNode *next;
public:
    BitRGNode();
    //NUM_t tokensum();
    index_t Hash();
    ~BitRGNode();

//    void *operator new(std::size_t ObjectSize)
//    {
//        return g_ptrMemPool->GetMemory(ObjectSize) ;
//    }
//
//    void operator delete(void *ptrObject, std::size_t ObjectSize)
//    {
//        g_ptrMemPool->FreeMemory(ptrObject, ObjectSize) ;
//    }
};


class RG
{
public:
    RGNode **rgnode;
    Petri *ptnet;
    RGNode *initnode;
    NUM_t RGNodelength;
    unsigned long nodecount;
    int hash_conflict_times;
    ofstream outRG;
public:
    RG(Petri *pt);
    void addRGNode(RGNode *mark);
    void enCoder(unsigned short *equmark,RGNode *curnode);
    void deCoder(unsigned short *equmark,RGNode *curnode);
    RGNode *RGinitialnode();
    RGNode *RGcreatenode(RGNode *curnode, int tranxnum, bool &exist);
    void getFireableTranx(RGNode *curnode, index_t **isFirable, unsigned short &firecount);
    void Generate(RGNode *node);
    void printRGNode(RGNode *node);
    ~RG();

//        void *operator new(std::size_t ObjectSize)
//    {
//        return g_ptrMemPool->GetMemory(ObjectSize) ;
//    }
//
//    void operator delete(void *ptrObject, std::size_t ObjectSize)
//    {
//        g_ptrMemPool->FreeMemory(ptrObject, ObjectSize) ;
//    }
};

class BitRG
{
public:
    BitRGNode **rgnode;
    Petri *ptnet;
    BitRGNode *initnode;
    NUM_t RGNodelength;
    unsigned long nodecount;
    int hash_conflict_times;
    ofstream outRG;
public:
    BitRG(Petri *pt);
    void addRGNode(BitRGNode *mark);
    void enCoder(unsigned short *equmark,BitRGNode *curnode);
    void deCoder(unsigned short *equmark,BitRGNode *curnode);
    BitRGNode *RGinitialnode();
    BitRGNode *RGcreatenode(BitRGNode *curnode, int tranxnum, bool &exist);
    void getFireableTranx(BitRGNode *curnode, index_t **isFirable, unsigned short &firecount);
    void Generate(BitRGNode *node);
    void printRGNode(BitRGNode *node);
    ~BitRG();

//        void *operator new(std::size_t ObjectSize)
//    {
//        return g_ptrMemPool->GetMemory(ObjectSize) ;
//    }
//
//    void operator delete(void *ptrObject, std::size_t ObjectSize)
//    {
//        g_ptrMemPool->FreeMemory(ptrObject, ObjectSize) ;
//    }
};
#endif //ENPAC_2020_3_0_RG_H
