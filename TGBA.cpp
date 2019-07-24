#include"TGBA.h"

/*********************Global_Function********************/
bool isSame(TGBA_vex s1, TGBA_vex s2)
{
	shrink(s1);
	shrink(s2);
	if (s1.form.size() != s2.form.size())
		return false;
	while (!s1.form.isEmpty())
	{
		STNode p = NULL;
		s1.form.pop(p);
		if (!s2.form.isExist(p))
		{
			return false;
		}
	}
	return true;
}

/* void shrink(TGBA_vex &ss)
 * function: for a TGBA state, there is a possibility where A=B&&C
 *           (B, C are included in state, but A is not, A is B and C's parent)
 *           so we should substitute A for B and C
 * in: TGBA_vex &ss, a TGBA state
 * out: TGBA_vex &ss, when out,it is more simple
 * */
void shrink(TGBA_vex &ss)
{
	bool simplest = false;
	while (!simplest)
	{
		simplest = true;
		vector<STNode>::iterator iter1;
		vector<STNode>::iterator iterfind;
		vector<STNode> temp_parent;
		for (iter1=ss.form.loc.begin();iter1!=ss.form.loc.end();iter1++)
		{
			STNode q = *iter1;
			STNode p = (*iter1)->parent;
			if (p!=NULL && p->character == "&&")
			{
				if (p->left == q)
				{
					iterfind = find(ss.form.loc.begin(), ss.form.loc.end(), p->right);
					if (iterfind != ss.form.loc.end())
					{
						simplest = false;
						temp_parent.push_back((*iter1)->parent);
						ss.form.loc.erase(iterfind);
						iterfind = find(ss.form.loc.begin(), ss.form.loc.end(), q);
						ss.form.loc.erase(iterfind);
						break;
					}
				}
				else     //p->right=q
				{
					iterfind = find(ss.form.loc.begin(), ss.form.loc.end(), p->left);
					if (iterfind != ss.form.loc.end())
					{
						simplest = false;
						temp_parent.push_back((*iter1)->parent);
						ss.form.loc.erase(iterfind);
						iterfind = find(ss.form.loc.begin(), ss.form.loc.end(), q);
						ss.form.loc.erase(iterfind);
						break;
					}
				}
			}
		}
		vector<STNode>::iterator iter_temp;
		for (iter_temp = temp_parent.begin(); iter_temp != temp_parent.end(); iter_temp++)
		{
			ss.form.insert(*iter_temp);
		}
		temp_parent.clear();
	}
}
bool isAincludeB(set<string> A, set<string> B)
{
	if (A.size() < B.size())
		return false;
	set<string>::iterator iterB;
	for (iterB = B.begin(); iterB != B.end(); iterB++)
	{
		set<string>::iterator iterA;
		iterA = find(A.begin(), A.end(), *iterB);
		if (iterA == A.end())
			return false;
	}
	return true;
}
bool isAincludeB(set<int> A, set<int> B)
{
	if (A.size() < B.size())
		return false;
	set<int>::iterator iterB;
	for (iterB = B.begin(); iterB != B.end(); iterB++)
	{
		set<int>::iterator iterA;
		iterA = find(A.begin(), A.end(), *iterB);
		if (iterA == A.end())   //找不到
			return false;
	}
	return true;
}
bool isAequB(set<int> A, set<int> B)
{
	if (A.size() != B.size())
		return false;
	set<int>::iterator iterA;
	set<int>::iterator iterB;
	for (iterA = A.begin(), iterB = B.begin(); iterA != A.end(); iterA++, iterB++)
	{
		if ((*iterA) != (*iterB))
			return false;
	}
	return true;
}
bool isAequB(set<string> A, set<string> B)
{
	if (A.size() != B.size())
		return false;
	set<string>::iterator iterA;
	set<string>::iterator iterB;
	for (iterA = A.begin(), iterB = B.begin(); iterA != A.end(); iterA++, iterB++)
	{
		if ((*iterA) != (*iterB))
			return false;
	}
	return true;
}
bool isSatisfyUright(STNode p, set<string> transcond)
{
    transcond.insert("true");
    bool satisfy = false;
    vector<AP_stack>::iterator iter;
    //(a1 && b1 && c1)||(a2 && b2 && c2)||...
    for(iter = p->right->props->prop_disj.begin(); iter != p->right->props->prop_disj.end(); iter++)
    {
        //a1 && b1 && c1
        AP_stack temp_APS = *iter;
        if(isAincludeB(transcond, temp_APS.propositions))
        {
            satisfy = true;
            break;
        }
        else
            continue;
    }
    return satisfy;
}
/*********************************************************/
TGBA::TGBA()
{
	statecounter = 0;
	transcounter = 0;
}
void TGBA::AssignID(TGBA_vex &ss)
{
	ss.num = statecounter++;
}

/* void TGBA::AddState(TGBA_vex ss, vector<TGBA_vex>::iterator &addposi)
 * function: put a TGBA state into TGBA states vector,
 * 		     and return his position in this vector;
 * in: TGBA_vex ss, a state which should be put in
 * out: vector<TGBA_vex>::iterator &addposi, a pointer
 * 		indicates the new added states's position in the vector
 * */
void TGBA::AddState(TGBA_vex ss, vector<TGBA_vex>::iterator &addposi)
{
	//查重
	bool isExist = false;
	vector<TGBA_vex>::iterator iters;
	for (iters = states.begin(); iters != states.end(); iters++)
	{
		if (isSame(*iters, ss))
		{
			addposi = iters;
			isExist = true;
			break;
		}
	}
	//没有重复节点
	if (!isExist)
	{
		this->states.push_back(ss);
		addposi = states.end() - 1;
	}
}
bool TGBA::isFindUnexpState(vector<TGBA_vex>::iterator &iter)
{
	for (iter = states.begin(); iter != states.end(); iter++)
	{
		if (iter->explored == false)
			return true;            //找到一个未拓展的节点
	}
	return false;
}
bool TGBA::isStateEqu(TGBA_vex A, TGBA_vex B)
{
	//A，B两个状态等价
	//1.AB的后继一样
	//2.到每一个后继的迁移条件和标签是一样的``
	if (!isAequB(A.nextvexset, B.nextvexset))   //判断后继是否一样
		return false;
	set<int>::iterator iterint;
	for (iterint = A.nextvexset.begin(); iterint != A.nextvexset.end(); iterint++)   //检查到每一个后继的迁移条件和标签
	{
		int next = *iterint;
		vector<TGBA_arc>::iterator Aloc;
		vector<TGBA_arc>::iterator Bloc;
		findtransitionbyps(A.num, next, Aloc);
		findtransitionbyps(B.num, next, Bloc);
		if (!(isAequB(Aloc->label, Bloc->label) && isAequB(Aloc->transcondi, Bloc->transcondi)))
			return false;
	}
	return true;
}
void TGBA::findstatesbynum(int num, vector<TGBA_vex>::iterator &frontposi)
{
	for (frontposi = states.begin(); frontposi != states.end(); frontposi++)
	{
		if (frontposi->num == num)
			return;
	}
}
void TGBA::findtransitionbyps(int pioneer, int succeed, vector<TGBA_arc>::iterator &transloc)
{
	for (transloc = transition.begin(); transloc != transition.end(); transloc++)
	{
		if (transloc->pioneer == pioneer && transloc->succeed == succeed)
			return;
	}
}
bool TGBA::findtransitionbysuc(int succeed, vector<TGBA_arc>::iterator &transloc)
{
	for (transloc = transition.begin(); transloc != transition.end(); transloc++)
	{
		if (transloc->succeed == succeed)
			return true;
	}
	return false;
}
void TGBA::fetchalltransbypri(int pri, vector<TGBA_arc> &transet)
{
	vector<TGBA_arc>::iterator itertrans;
	for (itertrans = transition.begin(); itertrans != transition.end(); itertrans++)
	{
		if (itertrans->pioneer == pri)
		{
			transet.push_back(*itertrans);
		}
	}
}
/*void TGBA::CreatState(CFTreeLeaf ctl, TGBA_vex &ss, set<string> &transcond)
 * function: create a TGBA state(TGBA_vex &ss),
 *           according to a basic structure(CFTreeLeaf ctl)
 * in:CFTreeLeaf ctl, a basic structure of CF Normal form which contains transition conditions and next state
 * out:TGBA_vex &ss, a state(vertex) of TGBA;
 * 	   set<string> &transcond, transition conditions
 */
void TGBA::CreatState(CFTreeLeaf ctl, TGBA_vex &ss, set<string> &transcond)
{
	transcond = ctl.trscod;
	ss.explored = false;
	AssignID(ss);
	ss.form = ctl.xstate;
}
void TGBA::CreatTransition(vector<TGBA_vex>::iterator &front, vector<TGBA_vex>::iterator rear, set<string> transcond, formula_stack Ustack)
{
	TGBA_arc tt;
	tt.num = transcounter++;
	tt.pioneer = front->num;
	tt.succeed = rear->num;
	tt.transcondi = transcond;
	int i = 0;
	for (i; i < Ustack.size(); i++)           //添加标签
	{
		bool ishave = false;
		STNode p = Ustack.loc[i];
		ishave = rear->form.decideLabel(p);
		if (!ishave)
			tt.label.insert(i);
        else if(isSatisfyUright(p,transcond)){
            tt.label.insert(i);
        }
	}

	bool isttunnecessary = false;       //tt是多余的吗
	vector<TGBA_arc>::iterator itertran;
	for (itertran = transition.begin(); itertran != transition.end(); )
	{
		if ((*itertran).pioneer == tt.pioneer && (*itertran).succeed == tt.succeed)   //前驱后继都一样
		{
			if (isAincludeB((*itertran).transcondi, tt.transcondi) && isAincludeB(tt.label,(*itertran).label))
			{
				isttunnecessary = false;
				itertran = transition.erase(itertran);
				break;
			}
			else if (isAincludeB(tt.transcondi, (*itertran).transcondi) && isAincludeB((*itertran).label,tt.label))
			{
				isttunnecessary = true;
				break;
			}
			else
				itertran++;
		}
		else
			itertran++;
	}
	if (isttunnecessary == false)
	{
		this->transition.push_back(tt);
		front->nextvexset.insert(rear->num);     //给前驱添加后继
	}
}
void TGBA::CreatTGBA(formula_stack Ustack, ST_Node *root)
{
	TGBA_vex init;
	AssignID(init);
	init.form.insert(root);
	this->states.push_back(init);
	vector<TGBA_vex>::iterator iter;
	while (isFindUnexpState(iter))                     //从顶点列表中找到一个未拓展的节点开始拓展
	{
		int formnum = iter->form.size();            //该状态有多少个子式组成
		state_stack *nextsates;
		nextsates = new state_stack[formnum];       //每个子式所代表的CF范式
		for (int i = 0; i < formnum; i++)                    //求每个子式的CF范式
		{
			if (iter->form.loc[i]->isGetCF == false)      //当前子式还未得到CF范式
			{
				CF_Tree CFT;
				CFT.CFBuilder(iter->form.loc[i]);
				//CFT.PrintCFTree(CFT.root->nleft, CFT.root->lleft, 0);
				//cout << endl;
				if (CFT.root->nleft != NULL)
				{
					CFT.Disjunction(CFT.root->nleft);
					nextsates[i] = CFT.root->nleft->ss;
				}
				else
				{
					nextsates[i].insert(*CFT.root->lleft);
				}
			}
			else                                                           //当前子式已得到CF范式
			{
				nextsates[i] = *(iter->form.loc[i]->cfnormal);
			}
		}//已得到每个子式的CF范式
		for (int j = 1; j < formnum; j++)                     //求所有子式范式的交
		{
			constatestack(nextsates[0], nextsates[j]);
		}//当前节点的CF范式（转移条件->下一个状态）

		iter->explored = true;

		int xstatenum = nextsates[0].size();    //下一个状态的个数
		TGBA_vex *xstates;
		xstates = new TGBA_vex[xstatenum];
		int pioneernum = iter->num;
		for (int k = 0; k < xstatenum; k++)
		{
			vector<TGBA_vex>::iterator newaddposi;        //新加入的状态位置，用于添加变迁
			set<string> transcond;
			CreatState(nextsates[0].states[k], xstates[k], transcond);
			AddState(xstates[k], newaddposi);
			vector<TGBA_vex>::iterator frontposi;
			findstatesbynum(pioneernum, frontposi);
			CreatTransition(frontposi, newaddposi, transcond, Ustack);
		}
	}
}
void TGBA::SimplifyStates()
{
	vector<TGBA_vex>::iterator iteri;
	for (iteri = states.begin(); iteri != states.end()-1; iteri++)
	{
		if (iteri == states.end())
			break;
		vector<TGBA_vex>::iterator iterj = iteri + 1;
		for (iterj; iterj != states.end();)
		{
			if (isStateEqu(*iteri, *iterj))
			{
				//如果两个状态i，j等价，删除其中一个状态j和j所发出的所有迁移，
				//并把指向j的所有迁移，指向其等价状态
				//1.删除状态j所发出的所有迁移
				deltransbypri(iterj->num);
				//2.把指向j的所有迁移，指向其等价状态
				modifytransbysuc(iterj->num, iteri->num);
				//3.删除状态j	
				iterj = states.erase(iterj);
			}
			else
				iterj++;
		}
	}
}
void TGBA::deltransbypri(int pri)                //删除pri发出的所有变迁
{
	vector<TGBA_arc>::iterator itertrans;
	for (itertrans = transition.begin(); itertrans != transition.end();)
	{
		if (itertrans->pioneer == pri)
		{
			itertrans = transition.erase(itertrans);
		}
		else
			itertrans++;
	}
}
void TGBA::modifytransbysuc(int oldsuc, int newsuc)
{
	vector<TGBA_arc>::iterator itertrans;
	while(findtransitionbysuc(oldsuc, itertrans))
	{
		//检查重新指向newsuc是否会造成变迁冗余
		vector<TGBA_arc>::iterator oldtrans;
		findtransitionbyps(itertrans->pioneer, newsuc, oldtrans);
		if (oldtrans == transition.end())  //表示没有迁移（oldsuc->pri, newsuc）
		{
			itertrans->succeed = newsuc;
			vector<TGBA_vex>::iterator pri;
			findstatesbynum(itertrans->pioneer, pri);
			pri->nextvexset.erase(oldsuc);
			pri->nextvexset.insert(newsuc);
		}
		else     //表示已有迁移oldtrans（oldsuc->pri, newsuc），需要判断冗余
		{
			if (isAincludeB(oldtrans->transcondi, itertrans->transcondi) && isAincludeB(itertrans->label, oldtrans->label))     //旧迁移是冗余的
			{
				itertrans->succeed = newsuc;
				vector<TGBA_vex>::iterator pri;
				findstatesbynum(itertrans->pioneer, pri);
				pri->nextvexset.erase(oldsuc);
				transition.erase(oldtrans);
			}
			else if (isAincludeB(itertrans->transcondi, oldtrans->transcondi) && isAincludeB(oldtrans->label, itertrans->label))
			{
				vector<TGBA_vex>::iterator pri;
				findstatesbynum(itertrans->pioneer, pri);
				pri->nextvexset.erase(oldsuc);
				transition.erase(itertrans);
			}
			else
			{
				itertrans->succeed = newsuc;
				vector<TGBA_vex>::iterator pri;
				findstatesbynum(itertrans->pioneer, pri);
				pri->nextvexset.erase(oldsuc);
			}
		}
	}
}
