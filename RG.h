//
// Created by lewis on 19-7-21.
//

#ifndef ENPAC_2020_3_0_RG_H
#define ENPAC_2020_3_0_RG_H

#include "Petri_Net.h"
#include <bitset>
#include <malloc.h>
using namespace std;

#define RGTABLE_SIZE 100000000

/*********************Global Functions**********************/
void DecToBinary(index_t DecNum, unsigned short *Binarystr);
void BinaryToDec(index_t &DecNum, unsigned short *Binarystr, NUM_t marklen);

class RG;
class NUPN_RG;
class RGNode;
class Product_Automata;

typedef unsigned short Mark;
typedef unsigned long ID;

class NUPN_RGNode
{
private:
    bitset<1> *marking;
    NUPN_RGNode *next;
public:
    NUPN_RGNode(NUM_t marking_length);
    index_t Hash(NUM_t marking_length);
    ~NUPN_RGNode();
    friend NUPN_RG;
    friend Product_Automata;
};

class RGNode
{
private:
    Mark *marking;
    RGNode *next;
public:
    RGNode(NUM_t placecount);
    NUM_t tokensum(NUM_t placecount);
    index_t Hash(NUM_t placecount);
    ~RGNode();
    friend RG;
    friend Product_Automata;
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
    RGNode *RGinitialnode();
    RGNode *RGcreatenode(RGNode *curnode, int tranxnum, bool &exist);
    void getFireableTranx(Mark *curmark, index_t **isFirable, unsigned short &firecount);
    void Generate(RGNode *node);
    void printRGNode(RGNode *node);
    ~RG();
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
};
#endif //ENPAC_2020_3_0_RG_H
