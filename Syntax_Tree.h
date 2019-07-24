#pragma once
/*语法分析器*/
#include"Data_Structures.h"
using namespace std;

/********************Global Function**********************/
bool isoperand(int typenum);
privilege DecidePrilevel(int typenum);
character_type DecideType(int typenum);
void CopyTree(ST_Node *t1, ST_Node *&t2, ST_Node *papa);
int constate(CFTreeLeaf cfl1, CFTreeLeaf cfl2, CFTreeLeaf &cfl3);
int checkfalse(CFTreeLeaf cfl);
void printstate(state_stack ss);
bool isinclufalse(CFTreeLeaf ctl);
void constatestack(state_stack &s1, state_stack s2);
void disAPdisj(AP_disj prop1, AP_disj prop2, AP_disj *prop3);
void conAPdisj(AP_disj prop1, AP_disj prop2, AP_disj *prop3);
void conAPstack(AP_stack prop1, AP_stack prop2, AP_stack *prop3);
/***************Syntan_Tree's definition** ****************/

class Syntax_Tree
{
public:
	ST_Node *root;
	list_stack Operator;            //操作符栈
	list_stack Operand;            //操作数栈
public:
	Syntax_Tree();
	void reverse_polish(Lexer lex);
	void build_tree();             //建立整棵语法树
	void convert(ST_Node *T,formula_stack &Ustack);
	void negconvert(ST_Node *T, formula_stack &Ustack);
	void print_reverse_polish();
	void print_syntax_tree(ST_Node *T, int n);
	void print_state_stack(ST_Node *T, int n);
	void simplify_LTL(ST_Node *T);                                        //化简LTL公式
	bool isTreeEqu(ST_Node *T1, ST_Node *T2);
	void computeCurAP(ST_Node *T);
};


class CF_Tree
{
public:
	CFTreeNode *root;
public:
	CF_Tree();
	void CFBuilder(ST_Node *T);               //build a CF normal tree form syntax tree
	void CFBuilderSub(CFTreeNode *&ctn, CFTreeLeaf *&ctl, ST_Node *stn);
	void PrintCFTree(CFTreeNode *ct, CFTreeLeaf *ctl, int n);
	void Disjunction(CFTreeNode *&ctn);  //convert to disjunction normal form
	void DisjuncTreeBuilder(CFTreeNode *&ctn, state_stack ss);
	void DelCFTree(CFTreeNode *ctn);
};

