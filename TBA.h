#pragma once
#include"TGBA.h"
#include<fstream>
#include<iomanip>
#include<cstring>
#define MAX_VERTEX_NUM 90000     //��󶥵㣨״̬����
#define Nofind -1

class SBA;
/************************Global_Functions****************************/
bool isExist(set<int> a, int val);
void fullfill_info(set<string> transcondi, string &info);
/*********************************************************************/
typedef struct VertexType
{
	int name=-1;                  //�ö��������
	int num=-1;                    //�ö���ı��0~r��r��ʾU��ʽ�ĸ���
} VertexType;                //��string��������ʾ���㣨״̬��
typedef struct ArcNode
{
	int adjvex;
	ArcNode *nextarc = NULL;                 //��һ��״̬
	string info;                             //��ű�Ǩ
	set<string> transi_condi;
	int sstateloc = -1;
}ArcNode;
typedef struct VNode
{
	VertexType data;		 //���㣨״̬��
	bool isexplored = false;        //�ýڵ��Ƿ���չ��
	bool isAccept;           //��״̬�Ƿ�Ϊ�ɽ���״̬
	int outdegree;           //�ýڵ�ı�Ǩ����
	set<int> incoming;      //��״̬��ǰ��
	ArcNode *firstarc = NULL;     //��һ��״̬
}VNode;

class TBA
{
private:
	int vex_num;       //�������
	int arc_num;       //��Ǩ����
	VNode vertics[MAX_VERTEX_NUM];    //�ڽӱ�
	int visit[MAX_VERTEX_NUM];               //��������
public:
	TBA();
	void CreatTBA(TGBA tgba, formula_stack Ustack);
	void CreatTBAsub(TGBA tgba, formula_stack Ustack);
	void AddArc(int vexloc, ArcNode *&arcloc);     //����һ����Ǩ
	bool fetchvirgin(int &adj);         //�Ӷ����б���ȡ��һ����δ��չ�Ľڵ�
	int LocateVex(VertexType v);    //��λ������
	void PrintBuchi(string filename);
	friend class SBA;
};
