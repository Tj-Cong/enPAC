#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<set>
#include<algorithm>
#include"Lexer.h"
#include <gperftools/tcmalloc.h>

using namespace std;
#define max_set_num 1000               //Old, New, Next�м���Ԫ��������
#define max_incoming 1000
#define max_nodes_num 1000

class state_stack;
class AP_disj;
/**********************�﷨���õ������ݽṹ***************************/
enum character_type {
	unary_op,                   //��Ŀ�����
	binary_op,                  //˫Ŀ�����
	AP,                             //ԭ�����⣬���������ʽ
};
enum privilege { unary=1, R_and_U=2, And=3, Or=4, Impli=5, Equ=6, root=7 };

//struct DoubleSet
//{
//    vector<set<int>> myelems;
//};

typedef struct Syntax_Tree_Node
{
	string character;         //�ýڵ��������߼��������ԭ������
	string formula;            //�ýڵ����Ĺ�ʽ
	character_type ctype;         //�ڵ�����
	state_stack *cfnormal = NULL;
	AP_disj *props = NULL;
    //DoubleSet *VTS = NULL;
	bool isGetCF=false;
	Syntax_Tree_Node *left = NULL;
	Syntax_Tree_Node *right = NULL;
	Syntax_Tree_Node *parent = NULL;
	~Syntax_Tree_Node();
}ST_Node, *STNode;

/*********************�沨��ʽ�õ��Ķ�ջ�ṹ************************/
typedef struct stack_unit
{
	word w;
	privilege prilevel;
	stack_unit *next = NULL;
}LSNode;

class list_stack           //ջ���ݽṹ
{
private:
	LSNode *top;
public:
	list_stack();
	void push_ls(LSNode node);
	bool pop_ls(LSNode &node);
	void pop_ls();
	bool topelem(LSNode &node);
	bool istoplpar();
	bool isEmpty();
	int topprilevel();
	~list_stack();
};
class formula_stack
{
public:
	vector<STNode> loc;
public:
	void insert(STNode T);
	void pop(STNode &T);
	bool isEmpty();
	bool isExist(STNode T);
	bool decideLabel(STNode label);
	bool isHaveLabel(STNode T, STNode label);
	void print(string &str);
	int size()const;
};

/********************CF_Tree Date_Structures**********************/
//CF�﷨����Ҷ�ӽڵ㣬 ÿһ��Ҷ�ӽڵ����CF��ʽ�Ļ����ṹ
//�����ṹ����������ɣ�һ����ת����������һ������һ��״̬
//��һ��״̬����LTL��ʽ����ʽ����ɣ������﷨���еĽڵ���ɵļ���
typedef struct CFTLeaf        //CF�﷨��Ҷ�ӽڵ�
{
	set<string> trscod;         //transition condition beita
	formula_stack xstate;       //next state
	ST_Node *relevant = NULL;
}CFTreeLeaf, *PCFTreeLeaf;

//state_stack is used for storing CFTreeLeaf
//which represents a basic structure of CF Normal form
class state_stack
{
public:
	vector<CFTreeLeaf> states;
public:
	void insert(CFTreeLeaf cfl);
	void pop(CFTreeLeaf &cfl);
	bool isEmpty();
	void clear();
	int size();
	//bool isExist(CFTreeLeaf cfl);
};

class AP_stack
{
public:
    set<string> propositions;
public:
    void insert(string str);
    void pop(string str);
    bool isEmpty();
    void clear();
    int size();
};

class AP_disj
{
public:
    vector<AP_stack> prop_disj;
public:
    AP_disj();
    AP_disj(AP_disj *APD);
    void insert(AP_stack APS);
    void pop(AP_stack &APS);
    bool isEmpty();
    void clear();
    int size();
};

typedef struct CF_Tree_Node  //CF�﷨���ڵ�
{
	string character;
	state_stack ss;          //ss�д洢�ĸýڵ�����л����ṹ��������CF��ʽ
	CF_Tree_Node *nleft;
	CF_Tree_Node *nright;
	CFTreeLeaf *lleft;
	CFTreeLeaf *lright;
	ST_Node *relevant = NULL;
}CFTreeNode, *PCFTreeNode;


template <class T>
class CStack
{
private:
    vector<T> myDate;
public:
    T top() const;
    void push(const T &item);
    T pop();
    int size() const;
    bool empty() const;
    bool isinstack(const T& item) const;
    void clear();
};
/*****************Data Structures for print out*******************/
typedef struct LTLNode
{
	string propertyid;
	string formula_str;
	int result = -1;
}LTLNode_t;

/*********************Global variables**************************/
extern LTLNode_t fireability[16];
extern LTLNode_t cardinality[16];

template <class T>
T CStack<T>::top() const {
    return myDate.back();
}

template <class T>
void CStack<T>::push(const T &item) {
    myDate.push_back(item);
}

template <class T>
T CStack<T>::pop() {
    T item = myDate.back();
    myDate.pop_back();
    return item;
}

template <class T>
int CStack<T>::size() const {
    return myDate.size();
}

template <class T>
bool CStack<T>::empty() const {
    return myDate.empty();
}

template <class T>
void CStack<T>::clear() {
    myDate.clear();
}

template<class T>
bool CStack<T>::isinstack(const T &item) const {
    for(int i=0;i<myDate.size();++i)
    {
        if(myDate[i] == item)
            return true;
    }
    return false;
}
