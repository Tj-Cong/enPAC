#pragma once
#include"Syntax_Tree.h"

typedef struct vex
{
	int num;                //顶点编号
	bool explored = false;  //该节点是否已拓展过
	formula_stack form;     //该节点所代表的子公式集合
	set<int> nextvexset ;   //his successors
}TGBA_vex;

typedef struct arc
{
	int num;
	int pioneer;
	int succeed;
	set<string> transcondi;
	set<int> label;
}TGBA_arc;

/*********************Global_Function***********************/
bool isSame(TGBA_vex s1, TGBA_vex s2);
void shrink(TGBA_vex &ss);
bool isAincludeB(set<string> A, set<string> B);
bool isAincludeB(set<int> A, set<int> B);
bool isAequB(set<int> A, set<int> B);
bool isAequB(set<string> A, set<string> B);
bool isSatisfyUright(STNode p, set<string> transcond);
/************************************************************/

class TGBA
{
private:
	vector<TGBA_vex> states;
	vector<TGBA_arc> transition;
	int statecounter;         //状态个数
	int transcounter;         //变迁个数
public:
	TGBA();
	void AssignID(TGBA_vex &ss);
	//creat-serial functions:
	void CreatTGBA(formula_stack Ustack, ST_Node *root);
	void CreatState(CFTreeLeaf ctl, TGBA_vex &ss, set<string> &transcond);
	void CreatTransition(vector<TGBA_vex>::iterator &front, vector<TGBA_vex>::iterator rear, set<string> transcond, formula_stack Ustack);
	void AddState(TGBA_vex ss, vector<TGBA_vex>::iterator &addposi);
	bool isStateEqu(TGBA_vex A, TGBA_vex B);
	//find-serial functions:
	bool isFindUnexpState(vector<TGBA_vex>::iterator &iter);
	void findstatesbynum(int num, vector<TGBA_vex>::iterator &frontposi);
	void findtransitionbyps(int pioneer, int succeed, vector<TGBA_arc>::iterator &transloc);
	bool findtransitionbysuc(int succeed, vector<TGBA_arc>::iterator &transloc);
	//fetch-serial functions:
	void fetchalltransbypri(int pri, vector<TGBA_arc> &transet);
	//simplify-related functions:
	void SimplifyStates();
	void deltransbypri(int pri);                //删除pri发出的所有变迁
	void modifytransbysuc(int oldsuc, int newsuc);     //将指向oldsuc的迁移重新指向newsuc，并修改发出该迁移的状态的nextvexset
};