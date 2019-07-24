#pragma once
#include"TGBA.h"
#include<fstream>
#include<iomanip>
#include<cstring>
#define MAX_VERTEX_NUM 1000     //最大顶点（状态）数
#define Nofind -1

class SBA;
/************************Global_Functions****************************/
bool isExist(set<int> a, int val);
void fullfill_info(set<string> transcondi, string &info);
/*********************************************************************/
typedef struct VertexType
{
	int name=-1;                  //该顶点的名字
	int num=-1;                    //该顶点的编号0~r，r表示U子式的个数
} VertexType;                //用string类型来表示顶点（状态）
typedef struct ArcNode
{
	int adjvex;
	ArcNode *nextarc = NULL;                 //下一个状态
	string info;                             //存放变迁
	set<string> transi_condi;
	int sstateloc = -1;
}ArcNode;
typedef struct VNode
{
	VertexType data;		 //顶点（状态）
	bool isexplored = false;        //该节点是否拓展过
	bool isAccept;           //该状态是否为可接受状态
	int outdegree;           //该节点的变迁数量
	set<int> incoming;      //该状态的前集
	ArcNode *firstarc = NULL;     //下一个状态
}VNode;

class TBA
{
private:
	int vex_num;       //顶点个数
	int arc_num;       //变迁个数
	VNode vertics[MAX_VERTEX_NUM];    //邻接表
	int visit[MAX_VERTEX_NUM];               //访问数组
public:
	TBA();
	void CreatTBA(TGBA tgba, formula_stack Ustack);
	void CreatTBAsub(TGBA tgba, formula_stack Ustack);
	void AddArc(int vexloc, ArcNode *&arcloc);     //增加一个变迁
	bool fetchvirgin(int &adj);         //从顶点列表中取出一个还未拓展的节点
	int LocateVex(VertexType v);    //定位顶点编号
	void PrintBuchi(string filename);
	friend class SBA;
};
