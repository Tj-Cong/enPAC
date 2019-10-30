#pragma once
#include"TBA.h"

typedef int SVertexType ;
typedef struct SArcNode
{
	int adjvex;
	SArcNode *nextarc = NULL;                 //下一个状态
}SArcNode;
typedef struct SVNode
{
	SVertexType data=-1;		 //顶点（状态）
	bool isAccept;           //该状态是否为可接受状态
	bool isInitial;              //该状态是否为
	set<int> incoming;      //该状态的前集
	set<int> outset;           //该状态的后集
	//该状态的label集合，以如下方式给出：{} && {} && {}，其中每一个{}就是一个原子命题
	//如果是LTLFirability公式，大括号里是如下形式：{t1,t2,...}，其中t1,t2表示变迁的名字
	//如果是LTLCardinality公式，大括号里面是如下形式{token-count(p1,p3,...)<=token-count(p,p,...)}，其中pn表示库所的名字
	//注意！：label集合中的原子命题可能会出现其否定形式，即!{}的形式，例如 !{}&&!{}&&{}
	//注意！：LTLCardinality公式中token-count()可能会是一个常数的形式，例如 {2<=token-count(p,p,...)}或{token-count(p1,p3,...)<=2}
	//建议的处理方式：首先对label这个string，以“&&”符号为切割符，分出一个一个 {} 或 !{} ,然后再对{}解析
	//判断{}里是否出现“<=”符号，如果有则表示是LTLCardinality公式，否则则表示是LTLFirability公式
	//如果是LTLCardinality公式，则以“<=”为切割符号，分出前后两部分，如果是常数就无需处理，如果是token-count()形式，则以“,”为切割符号得到一个一个p
	//如果是LTLFirability公式，则以“,”为切割符号，分出一个一个t
	string label;
	set<string> fullfilledAP;
	SArcNode *firstarc = NULL;     //下一个状态
}SVNode;

class SBA
{
public:
	int svex_num;
	SVNode svertics[MAX_VERTEX_NUM];
public:
	SBA();
	void CreatSBA(TBA &tba);
	void Simplify();
	void Compress();
    void PrintSBA();
private:
	void AddArc(int pri, int suc);
	void Delallarc(int vexloc);
	void Delarc(int vexloc, int adj);
	void findnextempty(int start, int &next);
	void findnextoccupied(int start, int &next, int &vexn);
};
