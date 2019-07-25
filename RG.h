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
using namespace std;

#define RGTABLE_SIZE 100000


extern MemPool::CMemoryPool *g_ptrMemPool;

typedef struct Bitfield
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
} myuint;


/*********************Global Functions**********************/
void DecToBinary(index_t DecNum, unsigned short *Binarystr);
void BinaryToDec(index_t &DecNum, unsigned short *Binarystr, NUM_t marklen);

class RG;
class NUPN_RG;
class RGNode;

typedef unsigned short Mark;
typedef unsigned long ID;

class NUPN_RGNode
{
public:
    bitset<1> *marking;
    NUPN_RGNode *next;
public:
    NUPN_RGNode(NUM_t marking_length);
    index_t Hash(NUM_t marking_length);
    ~NUPN_RGNode();
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

class RGNode
{
public:
    Mark *marking;
    RGNode *next;
public:
    RGNode(NUM_t placecount);
    NUM_t tokensum(NUM_t placecount);
    index_t Hash(NUM_t placecount);
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
    void enCoder(unsigned short *equmark,RGNode *curnode){};
    void deCoder(unsigned short *equmark,RGNode *curnode){};
    RGNode *RGinitialnode();
    RGNode *RGcreatenode(RGNode *curnode, int tranxnum, bool &exist);
    void getFireableTranx(Mark *curmark, index_t **isFirable, unsigned short &firecount);
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

class NUPN_RG
{
public:
    NUPN_RGNode **rgnode;
    Petri *ptnet;
    NUPN_RGNode *initnode;
    NUM_t RGNodelength;
    unsigned long nodecount;
    int hash_conflict_times;
public:
    NUPN_RG(Petri *pt);
    void enCoder(unsigned short *equmark,NUPN_RGNode *curnode);
    void deCoder(unsigned short *equmark,NUPN_RGNode *curnode);
    void getFireableTranx(unsigned short *equmark,index_t **isFirable, unsigned short &firecount);
    void addRGNode(NUPN_RGNode *mark);
    NUPN_RGNode *RGinitialnode();
    NUPN_RGNode *RGcreatenode(NUPN_RGNode *curnode,int tranxum, bool &exist);
    void Generate(NUPN_RGNode *node);
    ~NUPN_RG();
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
#endif //ENPAC_2020_3_0_RG_H
