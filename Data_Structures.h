#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<set>
#include<algorithm>
#include"Lexer.h"
using namespace std;
#define max_set_num 1000               //Old, New, Next中集合元素最大个数
#define max_incoming 1000
#define max_nodes_num 1000

class state_stack;
class AP_disj;
/**********************语法树用到的数据结构***************************/
enum character_type {
	unary_op,                   //单目运算符
	binary_op,                  //双目运算符
	AP,                             //原子命题，包括其否定形式
};
enum privilege { unary=1, R_and_U=2, And=3, Or=4, Impli=5, Equ=6, root=7 };
typedef struct Syntax_Tree_Node
{
	string character;         //该节点代表的主逻辑运算符或原子命题
	string formula;            //该节点代表的公式
	character_type ctype;         //节点类型
	state_stack *cfnormal;
	AP_disj *props;
	bool isGetCF=false;
	Syntax_Tree_Node *left;
	Syntax_Tree_Node *right;
	Syntax_Tree_Node *parent;
}ST_Node, *STNode;

/*********************逆波兰式用到的堆栈结构************************/
typedef struct stack_unit
{
	word w;
	privilege prilevel;
	stack_unit *next;
}LSNode;

class list_stack           //栈数据结构
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
//CF语法树的叶子节点， 每一个叶子节点代表CF范式的基本结构
//基本结构有两部分组成，一个是转移条件，另一个是下一个状态
//下一个状态是由LTL公式的子式子组成，即由语法树中的节点组成的集合
typedef struct CFTLeaf        //CF语法树叶子节点
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

typedef struct CF_Tree_Node  //CF语法树节点
{
	string character;
	state_stack ss;          //ss中存储的该节点的所有基本结构，即他的CF范式
	CF_Tree_Node *nleft;
	CF_Tree_Node *nright;
	CFTreeLeaf *lleft;
	CFTreeLeaf *lright;
	ST_Node *relevant = NULL;
}CFTreeNode, *PCFTreeNode;

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