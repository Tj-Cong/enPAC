//
// Created by hecong on 19-5-6.
//

#ifndef ENPAC_2020_2_0_PRODUCT_H
#define ENPAC_2020_2_0_PRODUCT_H

#include <cstdio>
#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <string>
#include <cstdlib>
#include "RG.h"
#include "SBA.h"

#define TIME_LEFT 112
#define max_to_string 30
#define max_array_num 10000
#define hash_table_num 100000
#define each_ltl_time 120
using namespace std;

class Product_Automata;
void  sig_handler(int num);
NUM_t placecount;

typedef class Product  //交自动机的一个状态
{
public:
    int id;               //交状态的大小标号
    RGNode *RGname_ptr;   //可达图状态所在位置，可以根据该指针索引到可达图状态
    int BAname_id;   //buchi自动机在自动机邻接表中的位置
    Product *hashnext;
public:
    Product();
    Product(Product *n);
} *P_Product;

class hashtable
{
private:
    Product **table;
    NUM_t nodecount;
public:
    hashtable();
    void insert(Product *n);
    index_t hashfunction(Product *q);
    P_Product search(Product *n);
    void pop(Product *n);
    ~hashtable();
    friend Product_Automata;
};

class Product_Automata
{
private:
    vector<Product> initial_status;
    hashtable h;
    hashtable dfs_stack;
    Petri *ptnet;
    RG *rg;
    SBA *ba;
    bool result;
    int ret;
    vector<int> negpath;
public:
    Product_Automata(Petri *pt, RG* r, SBA *sba);
    void getProduct();         //合成交自动机
    void addinitial_status(RGNode *initnode);  //生成交自动机的初始状态
    void ModelChecker(string propertyid, int &timeleft);  //最外层的函数
    void simplified_dfs(Product *q, int recurdepth, int CBANid);          //CBANid: current biggest accepted node id
    bool isLabel(RGNode *state, int sj);  //判断能否合成交状态
    bool judgeF(string s);         //判断该公式是否为F类型的公式
    short int sumtoken(string s, RGNode *state);   //计算s中所有库所的token和
    bool handleLTLF(string s, RGNode *state);
    bool handleLTLC(string s, RGNode *state);
    void handleLTLCstep(short int &front_sum, short int &latter_sum, string s, RGNode *state);
    int getresult();
    int getNodecount();
    void printNegapth(ofstream &outpath);
    ~Product_Automata();
};
#endif //ENPAC_2020_2_0_PRODUCT_H
