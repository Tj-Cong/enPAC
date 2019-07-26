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
extern NUM_t FIELDCOUNT;   //占用bitfield个数，仅仅用于NUPN和SAFE网
extern NUM_t MARKLEN;      //Petri网
extern NUM_t placecount;   //Petri网库所个数
extern bool NUPN;          //当前Petri网是否有NUPN信息
extern bool SAFE;          //当前Petri网是否为安全网

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
} myuint;


/*********************Global Functions**********************/
void DecToBinary(index_t DecNum, unsigned short *Binarystr);
void BinaryToDec(index_t &DecNum, unsigned short *Binarystr, NUM_t marklen);

template <class t_mark> class RG;
class NUPN_RG;
template <class t_mark> class RGNode;

typedef unsigned short Mark;
typedef unsigned long ID;

class NUPN_RGNode
{
public:
    myuint *marking;
    NUPN_RGNode *next;
public:
    NUPN_RGNode();
    index_t Hash();
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

template <class t_mark>
class RGNode
{
public:
    t_mark *marking;
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

template <class t_mark>
class RG
{
public:
    RGNode<t_mark> **rgnode;
    Petri *ptnet;
    RGNode<t_mark> *initnode;
    NUM_t RGNodelength;
    unsigned long nodecount;
    int hash_conflict_times;
    ofstream outRG;
public:
    RG(Petri *pt);
    void addRGNode(RGNode<t_mark> *mark);
    void enCoder(unsigned short *equmark,RGNode<t_mark> *curnode){};
    void deCoder(unsigned short *equmark,RGNode<t_mark> *curnode){};
    RGNode<t_mark> *RGinitialnode();
    RGNode<t_mark> *RGcreatenode(RGNode<t_mark> *curnode, int tranxnum, bool &exist);
    void getFireableTranx(Mark *curmark, index_t **isFirable, unsigned short &firecount);
    void Generate(RGNode<t_mark> *node);
    void printRGNode(RGNode<t_mark> *node);
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



/*****************************************************************/
/*RGNode::RGNode(int marking_length)
 * function: 构造函数，为marking数组申请空间，申请大小为哈希表大小，
 * 并全部初始化为0；
 * in: marking_length, 库所哈希表大小；
 * */
template <class t_mark>
RGNode<t_mark>::RGNode() {
    if(NUPN || SAFE) {
        marking = new myuint[FIELDCOUNT];
        memset(marking, 0, sizeof(myuint)*FIELDCOUNT);
    } else {
        marking = new Mark[MARKLEN];
        memset(marking,0, sizeof(Mark)*MARKLEN);
    }
    next = NULL;
}

/*int RGNode::tokensum(int marking_length)
 * function: 求当前marking每个库所的token和
 * in: marking_length, 状态数组长度
 * out: sum, token和
 * */
//template <class t_mark>
//NUM_t RGNode<t_mark>::tokensum() {
//    NUM_t sum = 0;
//    for(int i=0; i<marking_length; i++){
//        sum += marking[i];
//    }
//    return sum;
//}

/*int RGNode::Hash(int length)
 * function: 求当前marking的哈希值；哈希函数类似于BKDR，种子的值取3；
 * in: length, 状态数组长度
 * out: hash value;
 * */
template <class t_mark>
index_t RGNode<t_mark>::Hash() {
    unsigned int seed = 3;
    unsigned int hash = 0;
    int i = 0;
    if(NUPN || SAFE) {
        for(i; i<FIELDCOUNT; i++){
            hash = hash*seed + (unsigned int)marking[i];
        }
    }
    else{
        for(i; i<MARKLEN; i++){
            hash = hash*seed + marking[i];
        }
    }

    //int result = hash & 0x7fffffff;
    return hash;
}

/*RGNode::~RGNode()
 * function： 析构函数，释放空间
 * */
template <class t_mark>
RGNode<t_mark>::~RGNode() {
    delete marking;
}




#endif //ENPAC_2020_3_0_RG_H
