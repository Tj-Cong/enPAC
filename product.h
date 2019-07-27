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
#include <unistd.h>
#include <signal.h>
#include "RG.h"
#include "SBA.h"

#define TIME_LEFT 112
#define max_to_string 30
#define max_array_num 10000
#define hash_table_num 100000
#define each_ltl_time 120
using namespace std;

extern bool timeflag;    //超时标志
void  sig_handler(int num);

extern NUM_t FIELDCOUNT;
extern NUM_t MARKLEN;
extern NUM_t placecount;
extern bool NUPN;
extern bool SAFE;

/*******************************************/

template <class rgnode>
class Product  //交自动机的一个状态
{
public:
    int id;               //交状态的大小标号
    rgnode *RGname_ptr;   //可达图状态所在位置，可以根据该指针索引到可达图状态
    int BAname_id;        //buchi自动机在自动机邻接表中的位置
    Product *hashnext;
public:
    Product();
    Product(Product *n);
};

template <class rgnode>
class hashtable
{
public:
    Product<rgnode> **table;
    NUM_t nodecount;
    NUM_t hash_conflict_times;
public:
    hashtable();
    void insert(Product<rgnode> *n);
    index_t hashfunction(Product<rgnode> *q);
    Product<rgnode> *search(Product<rgnode> *n);
    void pop(Product<rgnode> *n);
    ~hashtable();

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

template <class rgnode, class rg_T>
class Product_Automata
{
private:
    vector<Product<rgnode>> initial_status;
    hashtable<rgnode> h;
    hashtable<rgnode> dfs_stack;
    Petri *ptnet;
    rg_T *rg;
    SBA *ba;
    bool result;
    int ret;
    vector<int> negpath;
public:
    Product_Automata(Petri *pt, rg_T* r, SBA *sba);
    void getProduct();         //合成交自动机
    void addinitial_status(rgnode *initnode);  //生成交自动机的初始状态
    void ModelChecker(string propertyid, int &timeleft);  //最外层的函数
    void simplified_dfs(Product<rgnode> *q, int recurdepth, int CBANid);          //CBANid: current biggest accepted node id
    bool isLabel(rgnode *state, int sj);  //判断能否合成交状态
    bool judgeF(string s);         //判断该公式是否为F类型的公式
    NUM_t sumtoken(string s, rgnode *state);   //计算s中所有库所的token和
    bool handleLTLF(string s, rgnode *state);
    bool handleLTLC(string s, rgnode *state);
    void handleLTLCstep(short int &front_sum, short int &latter_sum, string s, rgnode *state);
    int getresult();
    NUM_t getConflictTimes();
    int getNodecount();
    void printNegapth(ofstream &outpath);
    ~Product_Automata();

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

template <class rgnode>
Product<rgnode>::Product() {
    RGname_ptr = NULL;
    hashnext = NULL;
}

template <class rgnode>
Product<rgnode>::Product(Product *n) {
    id = n->id;               //交状态的大小标号
    RGname_ptr = n->RGname_ptr;   //可达图状态所在位置，可以根据该指针索引到可达图状态
    BAname_id = n->BAname_id;   //buchi自动机在自动机邻接表中的位置
    hashnext = NULL;
}

/****************************HashTable**************************/
template <class rgnode>
hashtable<rgnode>::hashtable() {
    table = new Product<rgnode>* [hash_table_num];
    int i=0;
    for(i;i<hash_table_num;i++)
    {
        table[i] = NULL;
    }
    nodecount = 0;
    hash_conflict_times = 0;
}

template <class rgnode>
hashtable<rgnode>::~hashtable() {
    int i=0;
    for(i;i<hash_table_num;i++)
    {
        if(table[i]!=NULL){
            Product<rgnode> *p = table[i];
            Product<rgnode> *q;
            while(p!=NULL){
                q = p->hashnext;
                delete p;
                p = q;
            }
        }
    }
    delete [] table;
    MallocExtension::instance()->ReleaseFreeMemory();
}

template <class rgnode>
void hashtable<rgnode>::insert(Product<rgnode> *q)
{
    int idex = hashfunction(q);
    if(table[idex]!=NULL)
        hash_conflict_times++;
    Product<rgnode> *qs = new Product<rgnode>(q);
    qs->hashnext = table[idex];
    table[idex] = qs;
    nodecount++;

}

template <class rgnode>
index_t hashtable<rgnode>::hashfunction(Product<rgnode> *q)
{
    index_t RGhashvalue;
    RGhashvalue = q->RGname_ptr->Hash();
    RGhashvalue = RGhashvalue % RGTABLE_SIZE;

    index_t Prohashvalue = RGhashvalue + q->BAname_id;
    Prohashvalue = Prohashvalue % hash_table_num;
    return Prohashvalue;
}

template <class rgnode>
Product<rgnode> *hashtable<rgnode>::search(Product<rgnode> *n) {
    int idex = hashfunction(n);
    Product<rgnode> *p = table[idex];
    while (p != NULL) {
        if (p->BAname_id == n->BAname_id && p->RGname_ptr == n->RGname_ptr)
            return p;
        p = p->hashnext;
    }
    return NULL;
}

template <class rgnode>
void hashtable<rgnode>::pop(Product<rgnode> *n) {
    int idex = hashfunction(n);
    Product<rgnode> *p = table[idex];
    Product<rgnode> *q;
    //判断头结点是不是
    if(p->RGname_ptr == n->RGname_ptr && p->BAname_id == n->BAname_id)
    {
        q=p->hashnext;
        table[idex] = q;
        delete p;
        return;
    }
    //头结点不是
    q = p;
    p = p->hashnext;
    while(p!=NULL)
    {
        if(p->RGname_ptr == n->RGname_ptr && p->BAname_id == n->BAname_id){
            q->hashnext = p->hashnext;
            delete p;
            return;
        }
        q = p;
        p=p->hashnext;
    }
    cout<<"Couldn't delete from hashtable!"<<endl;
}
/************************Product_automata************************/
template <class rgnode, class rg_T>
Product_Automata<rgnode,rg_T>::Product_Automata(Petri *pt, rg_T* r, SBA *sba) {
    ptnet = pt;
    rg = r;
    ba = sba;
    result = true;
}

/*bool Product_Automata::judgeF(string s)
 * function:判断一个公式是否属于F类型的公式
 * in: s,输入的公式；
 * out: true(F类型公式) or false(C类型公式)
 * */
template <class rgnode, class rg_T>
bool Product_Automata<rgnode,rg_T>::judgeF(string s) {
    int pos = s.find("<=");
    if (pos == string::npos)
    {
        return true;            //LTLFireability category
    }
    else return false;          //LTLCardinality category
}

/*void Product_Automata::ModelChecker(string propertyid, int &timeleft)
 * function: 最外层的函数，供main函数调用；
 * in: propertyid,公式字符串; timeleft,该公式剩余时间
 * out: void
 * */
template <class rgnode, class rg_T>
void Product_Automata<rgnode,rg_T>::ModelChecker(string propertyid, int &timeleft) {
    //预设超时机制
    signal(SIGALRM, sig_handler);
    alarm(each_ltl_time);
    timeflag = true;

    //核心部分
    result = true;
    getProduct();     //合成交自动机并进行搜索

    //打印结果
    string re;
    if(timeflag)
    {
        if(result)
        {
            re="TRUE";

            cout << "FORMULA " + propertyid + " " + re +
                    " TECHNIQUES SEQUENTIAL EXPLICIT TOPOLOGICAL ABSTRACTIONS LTLNFBA" << endl;
            ret = 1;
        }
        else
        {
            re="FALSE";
            cout << "FORMULA " + propertyid + " " + re +
                    " TECHNIQUES SEQUENTIAL EXPLICIT TOPOLOGICAL ABSTRACTIONS LTLNFBA" << endl;
            ret = 0;
        }
    }
    else
    {
        cout<<"FORMULA "+propertyid+" "+"CANNOT_COMPUTE"<<endl;
        ret = -1;
    }
    alarm(0);
    //timeleft = alarm(0);//Cancel alarm
    return;
}

/*void Product_Automata::getProduct()
 * function: 合成交自动机并进行搜索
 * */
template <class rgnode, class rg_T>
void Product_Automata<rgnode,rg_T>::getProduct() {
    //如果还未得到rg的初始状态，那么就生成他的初始状态
    if(rg->initnode == NULL){
        rg->RGinitialnode();
    }

    //生成交自动机的初始状态
    addinitial_status(rg->initnode);

    //从初始状态开始搜索
    int i = 0;
    int end = initial_status.size();
    for(i;i<end;i++)
    {
        Product<rgnode> *init = new Product<rgnode>;
        init->id= 0;
        init->RGname_ptr = initial_status[i].RGname_ptr;
        init->BAname_id = initial_status[i].BAname_id;
        init->hashnext = NULL;
//        dfs1(init);
        simplified_dfs(init,0,-1);
        delete init;
        if(!timeflag)  //如果已经出结果或超时，则退出
            break;
    }
}

template <class rgnode, class rg_T>
void Product_Automata<rgnode,rg_T>::simplified_dfs(Product<rgnode> *q, int recurdepth, int CBANid) {
    if(!result || !timeflag){
        return ;
    }

    int cban;
    if((ba->svertics[q->BAname_id].isAccept == true) && (q->id > CBANid))
        cban = q->id;
    else
        cban = CBANid;

    h.insert(q);
    dfs_stack.insert(q);
    SArcNode *pba = ba->svertics[q->BAname_id].firstarc;

    //遍历BA的后继结点
    while(pba != NULL) {
        if (!result || !timeflag)
        {
            return;
        }
        index_t *isFirable;
        unsigned short firecount;
        rg->getFireableTranx(q->RGname_ptr,&isFirable,firecount);

        int rg_i = 0;
        if(firecount == 0)
        {
            if (!result || !timeflag)
            {
                return;
            }
            rgnode *rgseed = q->RGname_ptr;
            if(isLabel(rgseed, pba->adjvex))
            {
                Product<rgnode> *qs = new Product<rgnode>;
                qs->id = recurdepth + 1;
                qs->BAname_id = pba->adjvex;
                qs->RGname_ptr = rgseed;

                Product<rgnode> *existpos = dfs_stack.search(qs);
                if(existpos != NULL)
                {
                    if(existpos->id <= cban)
                    {
                        result = false;
                        delete qs;
                        return;
                    }
                }
                if(h.search(qs) == NULL)
                {
                    simplified_dfs(qs,recurdepth+1,cban);
                }
                delete qs;
            }
        }
        for(rg_i; rg_i<firecount; rg_i++)
        {
            if (!result || !timeflag)
            {
                return;
            }
            bool exist;
            rgnode *rgseed = rg->RGcreatenode(q->RGname_ptr,isFirable[rg_i],exist);
            if(isLabel(rgseed, pba->adjvex))
            {
                Product<rgnode> *qs = new Product<rgnode>;
                qs->id = recurdepth + 1;
                qs->BAname_id = pba->adjvex;
                qs->RGname_ptr = rgseed;

//                if(rgseed->fireptr == 0 && ba->svertics[qs->BAname_id].isAccept == true)
//                {
//                    cout<<"no successors!"<<endl;
//                    result = false;
//                    return;
//                }

                Product<rgnode> *existpos = dfs_stack.search(qs);
                if(existpos != NULL)
                {
                    if(existpos->id <= cban)
                    {
                        result = false;
                        delete qs;
                        return;
                    }
                }
                if(h.search(qs) == NULL)
                {
                    simplified_dfs(qs,recurdepth+1,cban);
                }
                delete qs;
            }
        }
        if(firecount>0)
            delete [] isFirable;
        pba = pba->nextarc;
    }
    dfs_stack.pop(q);
    MallocExtension::instance()->ReleaseFreeMemory();
}

/*void Product_Automata::addinitial_status(RGNode *initnode)
 * function: 生成交自动机的初始状态，并加入到initial_status数组中
 * in: initnode,可达图的初始节点
 * out: void
 * */
template <class rgnode, class rg_T>
void Product_Automata<rgnode,rg_T>::addinitial_status(rgnode *initnode) {
    //遍历buchi自动机的所有状态，找到所有初始状态
    for(int i=0; i<ba->svex_num; i++)
    {
        if(ba->svertics[i].isInitial)
        {
            //如果是初始状态，就和可达图的初始状态生成交自动机的初始状态
            if(isLabel(initnode, i))
            {
                //生成交自动机初始状态
                Product<rgnode> N;
                N.BAname_id = i;
                N.RGname_ptr = initnode;
                //加入初始状态数组中
                initial_status.push_back(N);
            }
        }
    }
}

/*bool Product_Automata::isLabel(RGNode *state, int sj)
 * function: 判断可达图的一个状态和buchi自动机的一个状态能否合成交状态
 * in: state,可达图状态指针，指向要合成的可达图状态
 * sj,buchi自动机状态在邻接表中的序号
 * out: true(可以合成交状态) or false(不能合成状态)
 * */
template <class rgnode, class rg_T>
bool Product_Automata<rgnode,rg_T>::isLabel(rgnode *state, int sj) {

    //取出自动机状态中的label
    string str = ba->svertics[sj].label;
    if(str == "true")
        return true;

    bool mark = false;
    while (1)
    {
        int pos = str.find_first_of("&&");

        if (pos == string::npos)     //最后一个原子命题
        {
            //cout<<str;
            if (judgeF(str))
            {
                /*a && b && c:
                 * true: 都为true
                 * false： 只要有一个为false
                 * */
                mark = handleLTLF(str, state);
                if(mark == false)
                    return false;
            }
            else {  //C类型公式
                //cout<<"c����";
                mark = handleLTLC(str, state);
                if(mark == false)
                    return false;
            }
            break;
        }

        string subprop = str.substr(0, pos);    //取出一个原子命题
        //cout<<subprop<<" ";

        if (judgeF(subprop))   //当前公式为F类型公式
        {
            /*a && b && c:
            * true: 都为true
            * false： 只要有一个为false
            * */
            mark = handleLTLF(subprop, state);
            if (mark == false)
            {
                //当前原子命题不成立
                return false;
            }
        }
        else                   //当前公式为C类型公式
        {
            /*a && b && c:
            * true: 都为true
            * false： 只要有一个为false
            * */
            mark = handleLTLC(subprop, state);
            if (mark == false)
            {
                //有一个原子命题不成立
                return false;
            }
        }
        //cout<<endl;
        str = str.substr(pos + 2, str.length() - pos -2);
    }
    return true;
}

/*bool Product_Automata::handleLTLF(string s, RGNode *state)
 * function:判断F类型中的一个原子命题在状态state下是否成立
 * in: s是公式的一小部分，一个原子命题； state，状态
 * out: true(成立) or false(不成立)
 * */
template <class rgnode, class rg_T>
bool Product_Automata<rgnode,rg_T>::handleLTLF(string s, rgnode *state) {

    index_t *isFirable;
    unsigned short firecount = 0;
    rg->getFireableTranx(state,&isFirable,firecount);


    if(s[0] == '!') //前面带有'!'的is-fireable{}
    {
        /*!{t1 || t2 || t3}：
         * true：t1不可发生 并且 t2不可发生 并且 t3不可发生
         * false： 只要有一个能发生
         * */
        s = s.substr(2,s.length()-2); //去掉“!{”
        while(1)
        {
            int pos = s.find_first_of(",");
            if (pos < 0)
                break;
            string subs = s.substr(0, pos);            //取出一个变迁

            int idex = ptnet->getTPosition(subs);      //得到变迁在petri网中的index
            if(idex == INDEX_ERROR){
                cerr<<"Can not find transition:"<<subs<<endl;
                exit(0);
            }

            //遍历当前节点的可放生变迁

            int j = 0;
            for(j;j<firecount;j++)
            {
                if(idex == isFirable[j]){
                    //取出的变迁在当前状态下可发生, 满足false条件
                    if(firecount>0)
                        delete [] isFirable;
                    return false;
                }
            }

            s = s.substr(pos + 1, s.length() - pos);
        }
        if(firecount>0)
            delete [] isFirable;
        return true;

    } else{         //单纯的is-fireable{}原子命题
        /*{t1 || t2 || t3}:
	     * true: 只要有一个能发生
	     * false: 都不能发生
	     * */
        s = s.substr(1, s.length() - 1);//去掉‘{’

        while(1)
        {
            int pos = s.find_first_of(",");
            if (pos < 0)
                break;
            string subs = s.substr(0, pos);  //取出一个变迁

            int idex = ptnet->getTPosition(subs);  //变迁的序列号
            //遍历当前状态的可发生变迁
            int j=0;
            for(j;j<firecount;j++)
            {
                if(idex == isFirable[j]){
                    if(firecount>0)
                        delete [] isFirable;
                    //取出的状态可发生
                    return true;
                }
            }
            //走到这里表示取出来的变迁不可发生
            s = s.substr(pos + 1, s.length() - pos);
        }
        if(firecount>0)
            //走到这里表示所有的变迁都不能发生
            delete [] isFirable;
        return false;
    }
}

/*bool Product_Automata::handleLTLC(string s, RGNode *state)
 * function: 判断C类型公式中的一个原子命题s在状态state下是否成立
 * in: s,原子命题； state,状态
 * out: true(成立) or false(不成立)
 * */
template <class rgnode,class rg_T>
bool Product_Automata<rgnode,rg_T>::handleLTLC(string s, rgnode *state) {
    short int front_sum, latter_sum;     //前半部分和 后半部分和
    if(s[0] == '!')
    {
        /*!(front <= latter)
	     * true:front > latter
	     * false:front <= latter
	     * */
        s = s.substr(2, s.length() - 2);   //去除"!{"
        handleLTLCstep(front_sum, latter_sum, s, state);
        if(front_sum <= latter_sum)
            return false;
        else
            return true;
    }
    else {
        /*(front <= latter)
         * true:front <= latter
         * false:front > latter
         * */
        s = s.substr(1, s.length() - 1);   //去除"{"
        handleLTLCstep(front_sum, latter_sum, s, state);
        if(front_sum <= latter_sum)
            return true;
        else
            return false;
    }
}

/*void handleLTLCstep(int &front_sum, int &latter_sum, string s, RGNode *state)
 * function:计算在状态state下，C公式"<="前面库所的token和front_sum和后面库所的token和latter_sum
 * in: s,公式； state,状态
 * out: front_sum前半部分的和, latter_sum后半部分的和
 * */
template <class rgnode,class rg_T>
void Product_Automata<rgnode,rg_T>::handleLTLCstep(short int &front_sum, short int &latter_sum, string s, rgnode *state) {
    if(s[0] == 't')   //前半部分是token-count的形式
    {
        int pos = s.find_first_of("<=");           //定位到<=前
        string s_token = s.substr(12, pos - 13);   //去除"token-count(" ")"  ֻ只剩p1,p2,
        front_sum = sumtoken(s_token, state);      //计算token和

        //计算后半部分的和
        s = s.substr(pos + 2, s.length() - pos - 2); //取得后半部分，若是常数 则是常数+}

        if(s[0] == 't')   //后半部分是token-count
        {
            string s_token = s.substr(12, s.length() - 14);
            latter_sum = sumtoken(s_token, state);
        }
        else{             //后半部分是常数
            s = s.substr(0, s.length() - 1);        //去除后面的}
            latter_sum = atoi(s.c_str());
        }
    }
    else{             //前半部分是常数，那后半部分肯定是token-count
        //处理前半部分  现有的串 是从常数开始的
        int pos = s.find_first_of("<=");//定位到<=,取其前半部分
        string num = s.substr(0, pos);//取出常数
        front_sum = atoi(num.c_str());
        //	 cout<< " "<<front_sum<<" ";

        //处理后半部分
        s = s.substr(pos + 14, s.length() - pos - 16);
        //	 cout<<" "<<s<<" ";
        latter_sum = sumtoken(s, state);
    }
}

/*int Product_Automata::sumtoken(string s, RGNode *state)
 * function: 计算在状态state下，某些库所的token和
 * in: s:p1,p2,p3,p4,这样的库所名字字符串；state，状态
 * out: 库所的token和
 * */
template <class rgnode,class rg_T>
NUM_t Product_Automata<rgnode,rg_T>::sumtoken(string s, rgnode *state) {

    Mark *marking = new Mark[ptnet->placecount];
    NUM_t sum = 0;

    if(ptnet->NUPN) {
        rg->deCoder(marking,state);
        while(1)
        {
            int pos = s.find_first_of(",");
            if (pos == string::npos)
                break;
            string subs = s.substr(0, pos);        //取得一个p1
            index_t idex = ptnet->getPPosition(subs);  //得到该库所的索引号

            if(idex ==INDEX_ERROR) {
                cerr<<"Can not find place:"<<subs<<endl;
                exit(0);
            }

            sum += marking[idex];

            //将前面的用过的P1去除 从p2开始作为新的s串
            s = s.substr(pos + 1, s.length() - pos);
        }
    }
    else if(ptnet->SAFE){
        myuint *bitmark = new myuint[FIELDCOUNT];
        memcpy(bitmark,state->marking, sizeof(myuint)*FIELDCOUNT);

        while(1)
        {
            int pos = s.find_first_of(",");
            if (pos == string::npos)
                break;
            string subs = s.substr(0, pos);        //取得一个p1
            index_t idex = ptnet->getPPosition(subs);  //得到该库所的索引号

            if(idex ==INDEX_ERROR) {
                cerr<<"Can not find place:"<<subs<<endl;
                exit(0);
            }

            int unit = idex / (sizeof(myuint)*8);
            int offset = idex % (sizeof(myuint)*8);
            sum += (bitmark[unit].test1(offset))?1:0;

            //将前面的用过的P1去除 从p2开始作为新的s串
            s = s.substr(pos + 1, s.length() - pos);
        }
        delete [] bitmark;
    }
    else {
        memcpy(marking,state->marking, sizeof(Mark)*(ptnet->placecount));
        while(1)
        {
            int pos = s.find_first_of(",");
            if (pos == string::npos)
                break;
            string subs = s.substr(0, pos);        //取得一个p1
            index_t idex = ptnet->getPPosition(subs);  //得到该库所的索引号

            if(idex ==INDEX_ERROR) {
                cerr<<"Can not find place:"<<subs<<endl;
                exit(0);
            }

            sum += marking[idex];

            //将前面的用过的P1去除 从p2开始作为新的s串
            s = s.substr(pos + 1, s.length() - pos);
        }
    }

    delete [] marking;
    return sum;
}

template <class rgnode,class rg_T>
int Product_Automata<rgnode,rg_T>::getresult() {
    return ret;
}

template <class rgnode,class rg_T>
NUM_t Product_Automata<rgnode,rg_T>::getConflictTimes() {
    return h.hash_conflict_times;
}


template <class rgnode,class rg_T>
int Product_Automata<rgnode,rg_T>::getNodecount() {
    return (h.nodecount+initial_status.size());
}

template <class rgnode,class rg_T>
void Product_Automata<rgnode,rg_T>::printNegapth(ofstream &outpath) {
    vector<int>::reverse_iterator iter;
    for(iter=negpath.rbegin(); iter!=negpath.rend(); iter++){
        outpath<<(*iter)<<endl;
    }
}

template <class rgnode,class rg_T>
Product_Automata<rgnode,rg_T>::~Product_Automata() {

}
#endif //ENPAC_2020_2_0_PRODUCT_H
