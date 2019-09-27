//
// Created by lewis on 19-7-21.
//

#ifndef ENPAC_2020_3_0_RG_H
#define ENPAC_2020_3_0_RG_H

#include "Petri_Net.h"
#include "Data_Structures.h"
#include <bitset>
#include <malloc.h>
#include <gperftools/malloc_extension.h>

using namespace std;

#define RGTABLE_SIZE 100000


extern NUM_t FIELDCOUNT;   //占用bitfield个数，仅仅用于NUPN和SAFE网
extern NUM_t MARKLEN;      //Petri网
extern NUM_t placecount;   //Petri网库所个数
extern bool NUPN;          //当前Petri网是否有NUPN信息
extern bool SAFE;          //当前Petri网是否为安全网
extern Petri *petri;
extern bool STUBBORN;

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
    void set(int index);     //设置index位上为1
    void reset(int index);   //将index位置上设置为0；
    bool test0(int index);   //判断index位上是否为0；
    bool test1(int index);   //判断index位上是否为1；
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
    bool isFirable(const Transition &t) const;
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

//用于SAFE和NUPN
class BitRGNode
{
public:
    myuint *marking;
    BitRGNode *next;
public:
    BitRGNode();
    //NUM_t tokensum();
    index_t Hash();
    bool isFirable(const Transition &t) const;
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
    RGNode **rgnode;        //状态哈希表
    Petri *ptnet;           //Petri网
    RGNode *initnode;       //初始状态节点
    NUM_t RGNodelength;     //marking长度
    unsigned long nodecount;//状态个数
    int hash_conflict_times;//哈希冲突次数
    ofstream outRG;
    set<int> visibleset;
    vector<int> invisibleset;

private:
    //stubborn set 有关数据结构
    RGNode *cur;            //当前处理节点
    index_t *fire;
    bool **TGraph;          //有向图邻接表
    int *DFN;               //每个节点的访问数组
    int *Low;               //该节点所在的强连通分量的最小根节点
    int *visited;
    int *isEnVis;
    int *isEnInVis;
    int *myroot;
    bool *successor;        /*数组，长度为Petri网中变迁的个数
                            （也是TGraph中定点的个数），
                             表示当前是否得到该节点的后继*/
    bool *hasenvis;         //每一个变迁他所在的强连通分量是否包含使能的可视变迁
    vector<int> *stgcmponts;             //每一个节点的strong components
    int timepoint;                       //tarjan算法用到的时间戳
    CStack<int> tarstack;                //tarjan算法用到的栈
    set<int> reachset;                //algorithm2中第二步的Tu
public:
    RG(Petri *pt);

    /*初始化TGraph，全部设置为false，重置successor数组，全部设置为false*/
    void initTGraph();

    /*在进行Tarjan算法之前进行的预备工作：
     * 重置DFN数组，Low数组，visited数组，C集合，timepoint，tarstack*/
    void initTarjan();

    /*得到在TGraph中变迁transition[idx]的后继结点
     * 对于使能变迁，他的后继为和它non-accordwith的变迁
     * 对于非使能变迁来说，他的后继为他的wrup*/
    void parturient(int tidx);

    /*得到以source集合中变迁为起始节点在TGraph中所能
     * 到达的节点的集合，放到reachset中
     * 注意：在每次调用这个函数前必须初始化visited数组
     * (内部递归调用不用)*/
    void getReachable(const set<int> &source);
    /*一个递归函数，寻找以变迁transition[root]为根节点的强连通分量*/
    void Tarjan(int tidx);

    void getEn_visible();

    void isFirable();
    void addRGNode(RGNode *mark);
    void enCoder(unsigned short *equmark,RGNode *curnode);
    void deCoder(unsigned short *equmark,RGNode *curnode);
    void genStbnSet(RGNode *curnode,vector<int> &stbset,bool &red);
    void re_expand(RGNode *curnode,const vector<int> &oldstbset,vector<int> &newstbset);
    RGNode *RGinitialnode();
    RGNode *RGcreatenode(RGNode *curnode, int tranxnum, bool &exist);
    void getFireableTranx(RGNode *curnode, index_t **isFirable, unsigned short &firecount);
    void Generate(RGNode *node);
    void printRGNode(RGNode *node);
    ~RG();
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
    set<int> visibleset;
    vector<int> invisibleset;

private:
    //stubborn set 有关数据结构
    BitRGNode *cur;            //当前处理节点
    index_t *fire;
    bool **TGraph;          //有向图邻接表
    int *DFN;               //每个节点的访问数组
    int *Low;               //该节点所在的强连通分量的最小根节点
    int *visited;
    int *isEnVis;
    int *isEnInVis;
    int *myroot;
    bool *successor;        /*数组，长度为Petri网中变迁的个数
                            （也是TGraph中定点的个数），
                             表示当前是否得到该节点的后继*/
    bool *hasenvis;         //每一个变迁他所在的强连通分量是否包含使能的可视变迁
    vector<int> *stgcmponts;             //每一个节点的strong components
    int timepoint;                       //tarjan算法用到的时间戳
    CStack<int> tarstack;                //tarjan算法用到的栈
    set<int> reachset;                //algorithm2中第二步的Tu
public:
    BitRG(Petri *pt);
    /*初始化TGraph，全部设置为false，重置successor数组，全部设置为false*/
    void initTGraph();

    /*在进行Tarjan算法之前进行的预备工作：
     * 重置DFN数组，Low数组，visited数组，C集合，timepoint，tarstack*/
    void initTarjan();

    /*得到在TGraph中变迁transition[idx]的后继结点
     * 对于使能变迁，他的后继为和它non-accordwith的变迁
     * 对于非使能变迁来说，他的后继为他的wrup*/
    void parturient(int tidx);

    /*得到以source集合中变迁为起始节点在TGraph中所能
     * 到达的节点的集合，放到reachset中
     * 注意：在每次调用这个函数前必须初始化visited数组
     * (内部递归调用不用)*/
    void getReachable(const set<int> &source);
    /*一个递归函数，寻找以变迁transition[root]为根节点的强连通分量*/
    void Tarjan(int tidx);

    void getEn_visible();
    void isFirable();
    void addRGNode(BitRGNode *mark);
    void enCoder(unsigned short *equmark,BitRGNode *curnode);
    void deCoder(unsigned short *equmark,BitRGNode *curnode);
    void genStbnSet(BitRGNode *curnode,vector<int> &stbset,bool &red);
    void re_expand(BitRGNode *curnode,const vector<int> &oldstbset,vector<int> &newstbset);
    BitRGNode *RGinitialnode();
    BitRGNode *RGcreatenode(BitRGNode *curnode, int tranxnum, bool &exist);
    void getFireableTranx(BitRGNode *curnode, index_t **isFirable, unsigned short &firecount);
    void Generate(BitRGNode *node);
    void printRGNode(BitRGNode *node);
    ~BitRG();
};
#endif //ENPAC_2020_3_0_RG_H
