#include"TBA.h"

/****************************Global_Functions*****************************/
bool isExist(set<int> a, int val)
{
	set<int>::iterator i;
	for (i = a.begin(); i != a.end(); i++)
	{
		if ((*i) == val)
			return true;
	}
	return false;
}
void fullfill_info(set<string> transcondi, string &info)
{
	if (transcondi.size() == 0)
	{
		info = "true";
	}
	else
	{
		set<string>::iterator iter;
		for (iter = transcondi.begin(); iter != transcondi.end(); iter++)
		{
			info += (*iter) + "&&";
		}
		info = info.substr(0, info.length() - 2);
	}
}
/**************************************************************************/
TBA::TBA()
{
	vex_num = 0;
	arc_num = 0;
	memset(visit, 0, sizeof(int)*MAX_VERTEX_NUM);
}
bool TBA::fetchvirgin(int &adj)         //从顶点列表中取出一个还未拓展的节点
{
	int i = 0;
	for (i; i < vex_num; i++)
	{
		if (vertics[i].isexplored == false)
		{
			adj = i;
			return true;
		}
	}
	return false;
}
int TBA::LocateVex(VertexType v)    //定位顶点编号
{
	for (int i = 0; i < vex_num; i++)
	{
		if (vertics[i].data.name == v.name && vertics[i].data.num == v.num)
		{
			return i;
		}
	}
	return Nofind;
}
void TBA::CreatTBA(TGBA tgba, formula_stack Ustack)
{
	//加入初始化节点
	vertics[vex_num].data.name = 0;
	vertics[vex_num].data.num = 0;
	vertics[vex_num].firstarc = NULL;
	vertics[vex_num].isAccept = (Ustack.size() == 0) ? true : false;
	vertics[vex_num].isexplored = false;
	vertics[vex_num].outdegree = 0;
	vex_num++;
	//建造TBA
	CreatTBAsub(tgba, Ustack);
}
void TBA::CreatTBAsub(TGBA tgba, formula_stack Ustack)
{
	int virginloc;                       //将要拓展的节点的位置
	while (fetchvirgin(virginloc))
	{
		VNode *virgin = &vertics[virginloc];
		//1.找到该状态出发的所有变迁
		vector<TGBA_arc> virginarc;
		tgba.fetchalltransbypri(virgin->data.name, virginarc);
		//2.对每一个变迁计算匹配数字
		int base;
		if (virgin->data.num == Ustack.size())    //如果当前状态的匹配数字是r
		{
			//从0开始计算匹配值
			base = 0;
		}
		else                                                        //如果当前状态的匹配数字非r
		{
			//从当前状态匹配的数字开始计算匹配值
			base = virgin->data.num;
		}
		while (!virginarc.empty())
		{
			int increasebase = base;
			TGBA_arc curarc = virginarc.back();
			virginarc.pop_back();
			//计算匹配数字
			for (int i = increasebase + 1; i <= Ustack.size(); i++)
			{
				if (isExist(curarc.label, i - 1))
					increasebase = i;
				else
					break;
			}//数字匹配完成
			VertexType newvexdata;
			newvexdata.name = curarc.succeed;
			newvexdata.num = increasebase;
			int newvexloc=LocateVex(newvexdata);
			if (newvexloc == Nofind)    //不存在这样的节点，添加新节点
			{
				//新建一个节点，加入到vertics数组中
				vertics[vex_num].data = newvexdata;
				vertics[vex_num].firstarc = NULL;
				vertics[vex_num].isAccept = (vertics[vex_num].data.num == Ustack.size()) ? true : false;
				vertics[vex_num].isexplored = false;
				vertics[vex_num].outdegree = 0;
				//新建一条迁移
				ArcNode *p = new ArcNode;
				p->adjvex = vex_num;
				p->nextarc = NULL;
				p->transi_condi = curarc.transcondi;
				fullfill_info(p->transi_condi, p->info);
				AddArc(virginloc, p);
				//顶点个数+1
				vex_num++;
			}
			else              //已经存在该节点
			{
				ArcNode *p = new ArcNode;
				p->adjvex = newvexloc;
				p->nextarc = NULL;
				p->transi_condi = curarc.transcondi;
				fullfill_info(p->transi_condi, p->info);
				AddArc(virginloc, p);
			}
		}
		virgin->isexplored = true;
	}
}
void TBA::AddArc(int vexloc, ArcNode *&arcloc)     //增加一个变迁
{
	//1.判断是否已经存在迁移vexloc->arcloc.adjvex
	ArcNode *p = vertics[vexloc].firstarc;
	while (p != NULL)
	{
		if (p->adjvex == arcloc->adjvex)
			break;
		else
			p = p->nextarc;
	}
	if (p == NULL)    //不存在
	{
		//之间添加迁移
		arcloc->nextarc = vertics[vexloc].firstarc;
		vertics[vexloc].firstarc = arcloc;
		vertics[vexloc].outdegree++;
		int adj = arcloc->adjvex;
		vertics[adj].incoming.insert(vexloc);
		return;
	}
	else                   //存在
	{
		//进行冗余对比
		if (isAincludeB(p->transi_condi, arcloc->transi_condi))  //p是冗余的
		{
			//将p删除，将arcloc添加进来
			p->info = arcloc->info;
			p->transi_condi = arcloc->transi_condi;
		}
		else if (isAincludeB(arcloc->transi_condi, p->transi_condi))   //新加的是冗余的
		{
			//之间删除arcloc，不添加进来
			delete arcloc;
		}
		else    //没有冗余关系
		{
			//将arcloc添加进来
			arcloc->nextarc = vertics[vexloc].firstarc;
			vertics[vexloc].firstarc = arcloc;
			vertics[vexloc].outdegree++;
			vertics[arcloc->adjvex].incoming.insert(vexloc);
		}
	}
}
void TBA::PrintBuchi(string filename)
{
	const char *file = filename.c_str();
	ofstream outfile(file, ios::out);
	for (int i = 0; i < vex_num; i++)
	{
		outfile << "S" << i << ":";
		outfile << (vertics[i].isAccept == true ? "Accepted" : "") << endl;
		ArcNode *p = vertics[i].firstarc;
		while (p != NULL)
		{
			outfile << setw(3) << " " << "if (" << p->info << ")" << endl;
			outfile << setw(3) << " " << "goto S" << p->adjvex << endl;
			outfile << setw(3) << " " << "---------------------------------" << endl;;
			p = p->nextarc;
		}
		outfile << endl;
	}
}
