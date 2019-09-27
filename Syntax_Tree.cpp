#include"Syntax_Tree.h"
#include<iomanip>
using namespace std;

/********************Global Function**********************/
character_type DecideType(int typenum)
{
	switch (typenum)
	{
	case 1:case 6:case 7:case 10:return unary_op; break;
	case 2:case 3:case 4:case 5:case 8:case 9:return binary_op; break;
	case 13:return AP; break;
	default:
		break;
	}
}

privilege DecidePrilevel(int typenum)
{
	switch (typenum)
	{
	case 1:case 6:case 7:case 10:return unary; break;
	case 8:case 9:return R_and_U; break;
	case 4:return And; break;
	case 5:return Or; break;
	case 2:return Impli; break;
	case 3:return Equ; break;
	case 15:return root; break;
	default:
		break;
	}
}
bool isoperand(int typenum)
{
	return (typenum == $ID) ? true : false;
}

void CopyTree(ST_Node *t1, ST_Node *&t2, ST_Node *papa)
{
	if (t1 != NULL)
	{
		t2 = new ST_Node;
		t2->character = t1->character;
		t2->ctype = t1->ctype;
		t2->formula = t1->formula;
		t2->left = t2->right = NULL;
		t2->parent = papa;
		CopyTree(t1->left, t2->left, t2);
		CopyTree(t1->right, t2->right, t2);
	}
}
int constate(CFTreeLeaf cfl1, CFTreeLeaf cfl2, CFTreeLeaf &cfl3)
{
	set<string>::iterator iter1;
	set<string>::iterator iter2;
	for (iter1 = cfl1.trscod.begin(); iter1 != cfl1.trscod.end(); iter1++)
	{
		if ((*iter1) == "false")
			return ERROR;
	}
	for (iter2 = cfl2.trscod.begin(); iter2 != cfl2.trscod.end(); iter2++)
	{
		if ((*iter2) == "false")
			return ERROR;
	}
	cfl3.trscod.insert(cfl1.trscod.begin(), cfl1.trscod.end());
	cfl3.trscod.insert(cfl2.trscod.begin(), cfl2.trscod.end());
	while (!cfl1.xstate.isEmpty())
	{
		STNode p = NULL;
		cfl1.xstate.pop(p);
		if(p->formula!="true")
			cfl3.xstate.insert(p);
	}
	while (!cfl2.xstate.isEmpty())
	{
		STNode q = NULL;
		cfl2.xstate.pop(q);
		if (!cfl3.xstate.isExist(q) && (q->formula != "true"))
			cfl3.xstate.insert(q);
	}
	if (cfl3.xstate.isEmpty())
	{
		STNode p = new ST_Node;
		p->cfnormal = new state_stack;
		p->character = "true";
		p->ctype = AP;
		p->formula = "true";
		p->left = p->parent = p->right = NULL;
		cfl3.xstate.insert(p);
	}
	return OK;
}
int checkfalse(CFTreeLeaf cfl)
{
	set<string>::iterator iter;
	for (iter = cfl.trscod.begin(); iter != cfl.trscod.end(); iter++)
	{
		if ((*iter) == "false")
			return ERROR;
	}
	return OK;
}
void printstate(state_stack ss)
{
	while (!ss.isEmpty())
	{
		CFTreeLeaf temp;
		ss.pop(temp);
		string beita = "";
		string xstate = "";
		set<string>::iterator iter;
		for (iter = temp.trscod.begin(); iter != temp.trscod.end(); iter++)
		{
			beita += (*iter) + "&&";
		}
		beita = beita.substr(0, beita.length() - 2);
		temp.xstate.print(xstate);
		cout << beita << "->" << xstate << endl;
	}
}
bool isinclufalse(CFTreeLeaf ctl)
{
	set<string>::iterator iter;
	for (iter = ctl.trscod.begin(); iter != ctl.trscod.end(); iter++)
	{
		if (*iter == "false")
			return true;
	}
	return false;
}
void constatestack(state_stack &s1, state_stack s2)
{
	state_stack temp;
	while (!s1.isEmpty())
	{
		CFTreeLeaf cfll;
		s1.pop(cfll);
		state_stack sstemp;
		sstemp = s2;
		while (!sstemp.isEmpty())
		{
			CFTreeLeaf cfltemp;
			CFTreeLeaf cflr;
			sstemp.pop(cflr);
			if (constate(cfll, cflr, cfltemp) == OK)
				temp.insert(cfltemp);
		}
	}
	s1 = temp;
}

void disAPdisj(AP_disj prop1, AP_disj prop2, AP_disj *prop3)
{
    while(prop1.size()!=0)
    {
        AP_stack temp_APS;
        prop1.pop(temp_APS);
        prop3->insert(temp_APS);
    }
    while(prop2.size()!=0)
    {
        AP_stack temp_APS;
        prop2.pop(temp_APS);
        prop3->insert(temp_APS);
    }
}

void conAPdisj(AP_disj prop1, AP_disj prop2, AP_disj *prop3)
{
    while(prop1.size()!=0)
    {
        AP_stack temp_APS1;
        prop1.pop(temp_APS1);
        while(prop2.size()!=0)
        {
            AP_stack temp_APS2,temp_APS3;
            prop2.pop(temp_APS2);
            conAPstack(temp_APS1,temp_APS2,&temp_APS3);
            prop3->insert(temp_APS3);
        }
    }

}

void conAPstack(AP_stack prop1, AP_stack prop2, AP_stack *prop3)
{
    set<string>::iterator iter1;
    for(iter1 = prop1.propositions.begin(); iter1 != prop1.propositions.end(); iter1++)
    {
        prop3->insert(*iter1);
    }

    set<string>::iterator iter2;
    for(iter2 = prop2.propositions.begin(); iter2 != prop2.propositions.end(); iter2++)
    {
        prop3->insert(*iter2);
    }
}


bool judgeC(string s) {
    int pos = s.find("<=");
    if (pos == string::npos)
    {
        return false;            //LTLC
    }
    else return true;          //LTLF
}

void getFV(string apstr, set<int> &vs)
{
    if(apstr[0] == '!') {
        apstr = apstr.substr(2,apstr.length()-2);  //去掉"!{"
    }
    else {
        apstr = apstr.substr(1,apstr.length()-1);  //去掉"{"
    }

    string subs;

    for(;;)            //取出每一个变迁
    {
        int pos = apstr.find_first_of(",");
        if(pos<0)
            break;
        subs = apstr.substr(0,pos);

        int idex = petri->getTPosition(subs);

        Transition trans = petri->transition[idex];
        //遍历他的所有前继库所
        vector<SArc>::iterator tpre;
        vector<SArc>::iterator ppre;
        vector<SArc>::iterator ppost;
        for(tpre=trans.producer.begin();tpre!=trans.producer.end();++tpre)
        {
            //得到一个前继库所
            Place pp = petri->place[(*tpre).idx];
            //遍历和pp所有相关联的变迁t，使得w(pp,t)!=w(t,pp)
            //先遍历前继；ppre代表pp的一个前继变迁t(t->pp)
            for(ppre=pp.producer.begin();ppre!=pp.producer.end();++ppre)
            {
                SArc ss = *ppre;
                vector<SArc>::iterator at = find(pp.consumer.begin(),pp.consumer.end(),ss);
                if(at == pp.consumer.end())  //说明没找到相等的
                {
                    vs.insert((*ppre).idx);
                }
            }
            //再遍历后继；
            for(ppost=pp.consumer.begin();ppost!=pp.consumer.end();++ppost)
            {
                SArc ss = *ppost;
                vector<SArc>::iterator at = find(pp.producer.begin(),pp.producer.end(),ss);
                if(at==pp.producer.end())
                {
                    vs.insert((*ppost).idx);
                }
            }
        }
        apstr = apstr.substr(pos + 1);
    }
}

void getCV(string apstr, set<int> &vs)
{
    //{token-count(p1,p3,...)<=token-count(p,p,...)}
    if(apstr[0] == '!')
    {
        apstr = apstr.substr(2,apstr.length()-3);   //去掉"!{"和"}"
    } else{
        apstr = apstr.substr(1,apstr.length()-2);   //去掉"{"和"}"
    }

    //token-count(p1,p3,...)<=token-count(p,p,...)
    int midpos = apstr.find_first_of("<=");
    string fsthalf = apstr.substr(0,midpos);
    string sechalf = apstr.substr(midpos+2);


    //先处理前半部分
    if(fsthalf[0]=='t')  //token-count的形式
    {
        fsthalf=fsthalf.substr(12,fsthalf.length()-13);  //去掉"token-count("和")"
        //p1,p3,...,
        int pos;
        string subs;

        //遍历每一个库所，找到影响该库所token取值的变迁
        for(;;)
        {
            pos = fsthalf.find_first_of(",");
            if(pos<0)
                break;

            subs = fsthalf.substr(0,pos);
            int idex = petri->getPPosition(subs);
            Place pls = petri->place[idex];  //得到一个库所

            vector<SArc>::iterator ppre;
            vector<SArc>::iterator ppost;
            //先遍历该库所的前继变迁
            for(ppre=pls.producer.begin();ppre!=pls.producer.end();++ppre)
            {
                SArc ss = *ppre;
                vector<SArc>::iterator at = find(pls.consumer.begin(),pls.consumer.end(),ss);
                if(at==pls.consumer.end())
                {
                    vs.insert((*ppre).idx);
                }
            }
            //再遍历该库所的后继变迁
            for(ppost=pls.consumer.begin();ppost!=pls.consumer.end();++ppost)
            {
                SArc ss = *ppost;
                vector<SArc>::iterator at = find(pls.producer.begin(),pls.producer.end(),ss);
                if(at==pls.producer.end())
                {
                    vs.insert((*ppost).idx);
                }
            }
            fsthalf = fsthalf.substr(pos+1);
        }
    }
    //再处理后半部分
    if(sechalf[0] == 't')
    {
        sechalf=sechalf.substr(12,sechalf.length()-13);  //去掉"token-count("和")"
        //p1,p3,...,
        int pos;
        string subs;

        //遍历每一个库所，找到影响该库所token取值的变迁
        for(;;)
        {
            pos = sechalf.find_first_of(",");
            if(pos<0)
                break;

            subs = sechalf.substr(0,pos);
            int idex = petri->getPPosition(subs);
            Place pls = petri->place[idex];  //得到一个库所

            vector<SArc>::iterator ppre;
            vector<SArc>::iterator ppost;
            //先遍历该库所的前继变迁,ppre=(tt->pls);
            for(ppre=pls.producer.begin();ppre!=pls.producer.end();++ppre)
            {
                SArc ss = *ppre;
                vector<SArc>::iterator at = find(pls.consumer.begin(),pls.consumer.end(),ss);
                if(at==pls.consumer.end())
                {
                    vs.insert((*ppre).idx);
                }
            }
            //再遍历该库所的后继变迁
            for(ppost=pls.consumer.begin();ppost!=pls.consumer.end();++ppost)
            {
                SArc ss = *ppost;
                vector<SArc>::iterator at = find(pls.producer.begin(),pls.producer.end(),ss);
                if(at==pls.producer.end())
                {
                    vs.insert((*ppost).idx);
                }
            }
            sechalf = sechalf.substr(pos+1);
        }
    }
}
/*******************************************************/

/*******************Syntan_Tree's**********************/
Syntax_Tree::Syntax_Tree()
{
	root = new ST_Node;
	root->left = root->right = root->parent = NULL;
	LSNode node;
	node.w.character = "#";
	node.w.typenum = $root;
	node.prilevel = privilege::root;
	Operator.push_ls(node);
}

void Syntax_Tree::reverse_polish(Lexer lex)
{
	//get a word from lexer
	word curw;
	int result;
	result = lex.GetWord(curw);

	while (result != $End)
	{
		if (result == ERROR)
			cout << "The LTL formula is incorrect! Please check!";

		//copy word to node and decide privilege
		LSNode node;
		node.w = curw;
		node.prilevel = DecidePrilevel(curw.typenum);

		if (isoperand(curw.typenum))             //��ԭ������
		{
			Operand.push_ls(node);
		}
		else if (curw.typenum == $Lpar)           //������
		{
			Operator.push_ls(node);
		}
		else if (curw.typenum == $Rpar)            //������
		{
			LSNode temp;
			while (!Operator.istoplpar())
			{
				if (Operator.pop_ls(temp) == false)
				{
					cout << "The LTL formula exists bracket mismatches! Please check!";
					exit(-1);
				}
				Operand.push_ls(temp);
			}
			Operator.pop_ls();        //����������
		}
		else
		{
			if (Operator.istoplpar())
				Operator.push_ls(node);
			else if (node.prilevel <= Operator.topprilevel())    //��ջ��Ԫ�����ȼ�Ҫ�ߣ�ֱ��ѹջ
			{
				Operator.push_ls(node);
			}
			else
			{
				while ((node.prilevel > Operator.topprilevel()) && (Operator.istoplpar()==false))
				{
					LSNode temp;
					Operator.pop_ls(temp);
					Operand.push_ls(temp);
				}
				Operator.push_ls(node);
			}
		}
		result = lex.GetWord(curw);
	}
	LSNode temp;
	while (Operator.pop_ls(temp))
		Operand.push_ls(temp);
	Operand.pop_ls();
}
void Syntax_Tree::print_reverse_polish()
{
	LSNode temp;
	while (Operand.pop_ls(temp))
	{
		cout << temp.w.character;
	}
	cout << endl;
 }

void Syntax_Tree::build_tree()
{
	ST_Node *last, *cur;
	//build first node
	LSNode temp;
	Operand.pop_ls(temp);
	cur = new ST_Node;
	cur->cfnormal = new state_stack;
	cur->character = temp.w.character;
	cur->ctype = DecideType(temp.w.typenum);
	cur->parent = root;
	root->left = last = cur;
	//first node was built

	if (cur->ctype == unary_op)
	{
		cur->left = new ST_Node;
		cur->left->cfnormal = new state_stack;
		cur->right = NULL;
		cur = cur->left;	
	}
	else if(cur->ctype==binary_op)
	{
		cur->right = new ST_Node;
		cur->right->cfnormal = new state_stack;
		cur->left = NULL;
		cur = cur->right;
	}
	else if (cur->ctype == AP)
	{
		cur->left = cur->right = NULL;
	}
	while (Operand.pop_ls(temp))
	{
		cur->character = temp.w.character;
		cur->ctype = DecideType(temp.w.typenum);
		cur->parent = last;
		last = cur;
		if (cur->ctype == unary_op)
		{
			cur->left = new ST_Node;
			cur->left->cfnormal = new state_stack;
			cur->right = NULL;
			cur = cur->left;
		}
		else if (cur->ctype == binary_op)
		{
			cur->right = new ST_Node;
			cur->right->cfnormal = new state_stack;
			cur->left = NULL;
			cur = cur->right;
		}
		else if (cur->ctype == AP)
		{
			cur->left = cur->right = NULL;
			if (Operand.isEmpty() == true)
				return;
			while (!(cur->ctype == binary_op && cur->left == NULL))
			{
				cur = cur->parent;
			}
			cur->left = new ST_Node;
			cur->left->cfnormal = new state_stack;
			last = cur;
			cur = cur->left;		
		}
	}
}

void Syntax_Tree::print_syntax_tree(ST_Node *T, int n)
{
	int i;
	char ch = ' ';
	if (T != NULL)
	{
		for (i = 0; i < n; i++)
			cout << setw(3) << ch;
		cout << T->character << endl;
		print_syntax_tree(T->left, n + 1);
		print_syntax_tree(T->right, n + 1);
	}
}

void Syntax_Tree::print_state_stack(ST_Node *T, int n)
{
	int i;
	char ch = ' ';
	if (T != NULL)
	{
		for (i = 0; i < n; i++)
			cout << setw(3) << ch;
		cout << T->character << endl;
		if(T->isGetCF==true)
			printstate(*T->cfnormal);
		print_state_stack(T->left, n + 1);
		print_state_stack(T->right, n + 1);
	}
}

void Syntax_Tree::convert(ST_Node *T, formula_stack &Ustack)
{
	ST_Node *p;
	if (T->character == "!")
	{
		p = T->parent;
		if (p->left == T)              //T�������׽ڵ��������
		{
			p->left = T->left;
			T->left->parent = p;
			delete T;
			negconvert(p->left,Ustack);
		}
		else
		{
			p->right = T->left;
			T->left->parent = p;
			delete T;
			negconvert(p->right,Ustack);
		}
	}
	else if (T->character == "->")
	{
		T->character = "||";
		negconvert(T->left,Ustack);
		convert(T->right,Ustack);
		T->formula = "(" + T->left->formula + ")" + T->character + "(" + T->right->formula + ")";
	}
	else if (T->character == "<->")
	{
		ST_Node *copy_left, *copy_right, *p, *q;
		copy_left = copy_right = NULL;
		T->character = "&&";
		p = new ST_Node;
		p->cfnormal = new state_stack;
		p->character = "||";
		p->ctype = binary_op;
		p->parent = T;
		q = new ST_Node;
		q->cfnormal = new state_stack;
		q->character = "||";
		q->ctype = binary_op;
		q->parent = T;

		CopyTree(T->left, copy_left, q);
		CopyTree(T->right, copy_right, p);
		p->left = T->left;
		p->right = copy_right;
		q->left = copy_left;
		q->right = T->right;
		T->left->parent = p;
		T->right->parent = q;
		T->left = p;
		T->right = q;
		negconvert(p->left,Ustack);
		convert(p->right,Ustack);
		convert(q->left,Ustack);
		negconvert(q->right, Ustack);

		p->formula= "(" + p->left->formula + ")" + p->character + "(" + p->right->formula + ")";
		q->formula = "(" + q->left->formula + ")" + q->character + "(" + q->right->formula + ")";
		T->formula = "(" + T->left->formula + ")" + T->character + "(" + T->right->formula + ")";
	}
	else if (T->character == "&&" || T->character == "||" ||  T->character == "V")
	{
		convert(T->left, Ustack);
		convert(T->right, Ustack);
		T->formula = "(" + T->left->formula + ")" + T->character + "(" + T->right->formula + ")";
	}
	else if (T->character == "U")
	{
		Ustack.insert(T);
		convert(T->left, Ustack);
		convert(T->right, Ustack);
		T->formula = "(" + T->left->formula + ")" + T->character + "(" + T->right->formula + ")";
	}
	else if (T->character == "[]")
	{
		ST_Node *temp = new ST_Node;
		temp->cfnormal = new state_stack;
		temp->character = "false";
		temp->ctype = AP;
		temp->left = temp->right = NULL;
		temp->parent = T;
		T->right = T->left;
		T->left = temp;
		T->character = "V";
		T->ctype = binary_op;
		convert(T->right,Ustack);
		T->formula = "(false)" + T->character + "(" + T->right->formula + ")";
	}
	else if (T->character == "<>")
	{
		ST_Node *temp = new ST_Node;
		temp->cfnormal = new state_stack;
		temp->character = "true";
		temp->ctype = AP;
		temp->left = temp->right = NULL;
		temp->parent = T;
		T->right = T->left;
		T->left = temp;
		T->character = "U";
		T->ctype = binary_op;
		Ustack.insert(T);
		convert(T->right, Ustack);
		T->formula = "(true)" + T->character + "(" + T->right->formula + ")";
	}
	else if (T->character == "X")
	{
		convert(T->left, Ustack);
		T->formula = T->character + "(" + T->left->formula + ")";
	}
	else if (T->ctype == AP)
	{
		T->formula = T->character;
	}
}

void Syntax_Tree::negconvert(ST_Node *T, formula_stack&Ustack)
{
	ST_Node *p;
	if (T != NULL)
	{
		if (T->character == "!")
		{
			p = T->parent;
			if (p->left == T)
			{
				p->left = T->left;
				T->left->parent = p;
				delete T;
				convert(p->left, Ustack);
			}
			else
			{
				p->right = T->left;
				T->left->parent = p;
				delete T;
				convert(p->right, Ustack);
			}
		}
		else if (T->character == "->")
		{
			T->character = "&&";
			convert(T->left, Ustack);
			negconvert(T->right, Ustack);
			T->formula = "(" + T->left->formula + ")" + T->character + "(" + T->right->formula + ")";
		}
		else if (T->character == "<->")
		{
			ST_Node *m, *n;
			m = new ST_Node;
			m->cfnormal = new state_stack;
			n = new ST_Node;
			n->cfnormal = new state_stack;

			m->character = "&&";
			m->ctype = binary_op;
			m->parent = T;
			m->left = m->right = NULL;
			n->character = "&&";
			n->ctype = binary_op;
			n->parent = T;
			m->left = m->right = NULL;
			CopyTree(T->left, m->left, m);
			CopyTree(T->right, n->right, n);

			m->right = T->right;
			n->left = T->left;

			T->left->parent = n;
			T->right->parent = m;
			T->left = m;
			T->right = n;
			T->character = "||";
			convert(m->left, Ustack);
			negconvert(m->right, Ustack);
			negconvert(n->left, Ustack);
			convert(n->right, Ustack);
			m->formula = "(" + m->left->formula + ")" + m->character + "(" + m->right->formula + ")";
			n->formula = "(" + n->left->formula + ")" + n->character + "(" + n->right->formula + ")";
			T->formula = "(" + T->left->formula + ")" + T->character + "(" + T->right->formula + ")";
		}
		else if (T->character == "&&")
		{
			T->character = "||";
			negconvert(T->left, Ustack);
			negconvert(T->right, Ustack);
			T->formula = "(" + T->left->formula + ")" + T->character + "(" + T->right->formula + ")";
		}
		else if (T->character == "||")
		{
			T->character = "&&";
			negconvert(T->left, Ustack);
			negconvert(T->right, Ustack);
			T->formula = "(" + T->left->formula + ")" + T->character + "(" + T->right->formula + ")";
		}
		else if (T->character == "[]")
		{
			ST_Node *m;
			m = new ST_Node;
			m->cfnormal = new state_stack;
			m->character = "true";
			m->ctype = AP;
			m->formula = "true";
			m->parent = T;
			m->left = m->right = NULL;
			T->right = T->left;
			T->left = m;
			T->character = "U";
			Ustack.insert(T);
			negconvert(T->right, Ustack);
			T->formula = "(" + T->left->formula + ")" + T->character + "(" + T->right->formula + ")";
		}
		else if (T->character == "<>")
		{
			ST_Node *m;
			m = new ST_Node;
			m->cfnormal = new state_stack;
			m->character = "false";
			m->ctype = AP;
			m->formula = "false";
			m->parent = T;
			m->left = m->right = NULL;
			T->right = T->left;
			T->left = m;
			T->character = "V";
			negconvert(T->right, Ustack);

			T->formula = "(" + T->left->formula + ")" + T->character + "(" + T->right->formula + ")";
		}
		else if (T->character == "U")
		{
			T->character = "V";
			negconvert(T->left, Ustack);
			negconvert(T->right, Ustack);
			T->formula = "(" + T->left->formula + ")" + T->character + "(" + T->right->formula + ")";
		}
		else if (T->character == "V")
		{
			T->character = "U";
			Ustack.insert(T);
			negconvert(T->left, Ustack);
			negconvert(T->right, Ustack);
			T->formula = "(" + T->left->formula + ")" + T->character + "(" + T->right->formula + ")";
		}
		else if (T->character == "X")
		{
			negconvert(T->left, Ustack);
			T->formula = "X(" + T->left->formula + ")";
		}
		else if (T->ctype == AP)
		{
			T->character = "!" + T->character;
			T->formula = T->character;
		}
	}
}


void Syntax_Tree::simplify_LTL(ST_Node *T)
{
	if (T->ctype == AP)
	{
		return;
	}
	else if (T->ctype == binary_op)
	{
		if (T->character == "U")  
		{
			simplify_LTL(T->left);
			simplify_LTL(T->right);
			ST_Node *Tl = T->left;
			ST_Node *Tr = T->right;
			/*(Xa)U(Xb)=X(aUb)*/
			if (Tl->character == "X" && Tr->character == "X")    
			{
				ST_Node *fa = T->parent;
				T->left = Tl->left;
				Tl->left->parent = T;
				T->right = Tr->left;
				Tr->left->parent = T;
				delete Tr;
				if (fa->left == T)
				{
					fa->left = Tl;
				}
				else if (fa->right == T)
				{
					fa->right = Tl;
				}
				Tl->parent = fa;
				Tl->left = T;
				T->parent = Tl;
			}
			else if (Tr->character == "[]")
			{
				if (Tr->left->character == "<>")
					//b U GFa=GFa
				{
					ST_Node *fa = T->parent;
					if (fa->left == T)
					{
						fa->left = Tr;
						Tr->parent = fa;
						delete T;
					}
					else if (fa->right == T)
					{
						fa->right = Tr;
						Tr->parent = fa;
						delete T;
					}
				}
				/*else if (Tr->left->ctype == AP)
				{
					ST_Node *fa = T->parent;
					if (fa->left == T)
					{
						fa->left = Tr;
					}
					else if (fa->right == T)
					{
						fa->right = Tr;
					}
					Tr->parent = fa;
					delete T;
					delete Tl;
				}*/
			}
		}
		else if (T->character == "V")
		{
			simplify_LTL(T->left);
			simplify_LTL(T->right);
			ST_Node *Tl = T->left;
			ST_Node *Tr = T->right;
			if (Tr->character == "[]")
			{
				if (Tr->left->character == "<>")
					/*bR(GFa)=GFa*/
				{
					ST_Node *fa = T->parent;
					if (fa->left == T)
					{
						fa->left = Tr;
					}
					else if (fa->right == T)
					{
						fa->right = Tr;
					}
					Tr->parent = fa;
					delete T;
				}
			}
		}
		else if (T->character == "&&")
		{
			simplify_LTL(T->left);
			simplify_LTL(T->right);
			ST_Node *Tl = T->left;
			ST_Node *Tr = T->right;
			if (Tl->character == "X"&&Tr->character == "X")
			{
				ST_Node *fa = T->parent;
				T->left = Tl->left;
				T->right = Tr->left;
				Tl->left->parent = T;
				Tr->left->parent = T;
				if (fa->left == T)
				{
					fa->left = Tl;
				}
				else if (fa->right == Tl)
				{
					fa->right = Tl;
				}
				Tl->parent = fa;
				Tl->left = T;
				T->parent = Tl;
				delete Tr;
			}
			else if (Tl->character == "V"&&Tr->character == "V")
			{
				if (isTreeEqu(Tl->left, Tr->left))
				{
					ST_Node *fa = T->parent;
					T->left = Tl->right;
					T->right = Tr->right;
					Tl->right->parent = T;
					Tr->right->parent = T;
					if (fa->left == T)
					{
						fa->left = Tl;
					}
					else if (fa->right == T)
					{
						fa->right = Tl;
					}
					Tl->parent = fa;
					Tl->right = T;
					T->parent = Tl;
					delete Tr;
				}
			}
		}
		else if (T->character == "||")
		{
			simplify_LTL(T->left);
			simplify_LTL(T->right);
			ST_Node *Tl = T->left;
			ST_Node *Tr = T->right;
			if (Tl->character == "[]" && Tr->character == "[]")
			{
				ST_Node *ltemp = Tl->left;
				ST_Node *rtemp = Tr->left;
				if (ltemp->character == "<>"&&rtemp->character == "<>")
					/*(GFa)||(GFb)=GF(a||b)*/
				{
					ST_Node *fa = T->parent;
					T->left = ltemp->left;
					T->right = rtemp->left;
					ltemp->left->parent = T;
					rtemp->left->parent = T;
					if (fa->left == T)
					{
						fa->left = Tl;
					}
					else if (fa->right == T)
					{
						fa->right = Tl;
					}
					Tl->parent = fa;
					ltemp->left = T;
					T->parent = ltemp;
					delete Tr;
					delete rtemp;
				}
			}
			else if (Tl->character == "V"&&Tr->character == "V")
			{
				if (isTreeEqu(Tl->right, Tr->right))
				{
					ST_Node *fa = T->parent;
					T->left = Tl->left;
					Tl->left->parent = T;
					T->right = Tr->left;
					Tr->left->parent = T;
					if (fa->left == T)
					{
						fa->left = Tr;
					}
					else if (fa->right == T)
					{
						fa->right = Tr;
					}
					Tr->parent = fa;
					Tr->left = T;
					T->parent = Tr;
					delete Tl;
				}
			}

		}
		else if (T->character == "->")
		{
			simplify_LTL(T->left);
			simplify_LTL(T->right);
		}
		else if (T->character == "<->")
		{
			simplify_LTL(T->left);
			simplify_LTL(T->right);
		}
	}
	else if (T->ctype == unary_op)
	{
		if (T->character == "X")
		{
			simplify_LTL(T->left);
			ST_Node *temp = T->left;
			if (temp->character == "[]" && temp->left != NULL)   /*XGFa=GFa*/
			{
				if (temp->left->character == "<>")
				{
					ST_Node *fa = T->parent;
					if (fa->left == T)
					{
						fa->left = temp;
						temp->parent = fa;
						delete T;
					}
					else if (fa->right == T)
					{
						fa->right = temp;
						temp->parent = fa;
						delete T;
					}
				}
			}
			else if (temp->character == "&&" || temp->character == "||")         /*X(a && GF b)=(Xa)&&(GFb)*/
			{
				ST_Node *templ, *tempr;
				templ = temp->left;
				tempr = temp->right;
				if (tempr->character == "[]")				
                {
					if (tempr->left->character == "<>")  /*X(a && GF b)=(Xa)&&(GFb)*/
					{
						ST_Node *fa = T->parent;
						if (fa->left == T)
						{
							fa->left = temp;
							temp->parent = fa;
							T->left = templ;
							templ->parent = T;
							temp->left = T;
							T->parent = temp;
						}
						else if (fa->right == T)
						{
							fa->right = temp;
							temp->parent = fa;
							T->left = templ;
							templ->parent = T;
							temp->left = T;
							T->parent = temp;
						}
					}
				}
				else if (templ->character == "[]")
				{
					if (templ->left->character == "<>") /*X(GF b && a)=(Xa)&&(GFb)*/
					{
						ST_Node *fa = T->parent;
						if (fa->left == T)
						{
							fa->left = temp;
						}
						else if (fa->right == T)
						{
							fa->right = temp;
						}
						temp->parent = fa;
						T->left = tempr;
						tempr->parent = T;
						temp->right = T;
						T->parent = temp;
					}
				}
			}
		}
		else if (T->character == "<>")
		{
			/*FXa=XFa*/
			simplify_LTL(T->left);
			ST_Node *temp = T->left;
			if (temp->character == "X")
			{
				ST_Node *fa = T->parent;
				ST_Node *templ = temp->left;
				if (fa->left == T)
				{
					fa->left = temp;
				}
				else if (fa->right == T)
				{
					fa->right = temp;
				}
				temp->parent = fa;
				T->left = templ;
				templ->parent = T;
				temp->left = T;
				T->parent = temp;
			}
			else if (temp->character == "<>")
			{
				T->left = temp->left;
				temp->left->parent = T;
				delete temp;
			}
		}
		else if (T->character == "[]")
		{
			simplify_LTL(T->left);
			ST_Node *temp = T->left;
			if (temp->character == "[]")
			{
				T->left = temp->left;
				temp->left->parent = T;
				delete temp;
			}
		}
	}
}

bool Syntax_Tree::isTreeEqu(ST_Node *T1, ST_Node *T2)
{
	if ((T1 == NULL) ^ (T2 == NULL))
		return false;
	if (T1 == NULL && T2 == NULL)
		return true;
	if (T1->character != T2->character)
		return false;
	else {
		bool LEqu = isTreeEqu(T1->left, T2->left);
		bool REqu = isTreeEqu(T1->right, T2->right);
		if (LEqu == false || REqu == false)
			return false;
		else
			return true;
	}
}

void Syntax_Tree::computeCurAP(ST_Node *T) {
    if(T->character == "U")   //until
    {
        computeCurAP(T->left);
        computeCurAP(T->right);
        T->props = new AP_disj(T->right->props);
    }
    else if(T->character == "V")   //release
    {
        computeCurAP(T->left);
        computeCurAP(T->right);
        T->props = new AP_disj(T->right->props);
    }
    else if(T->character == "X")   //next
    {
        T->props = new AP_disj;
        computeCurAP(T->left);
        return;
    }
    else if(T->character == "&&")   //conjunction
    {
        computeCurAP(T->left);
        computeCurAP(T->right);
        T->props = new AP_disj;
        conAPdisj(*(T->left->props),*(T->right->props),T->props);
    }
    else if(T->character == "||")    //disjunction
    {
        computeCurAP(T->left);
        computeCurAP(T->right);
        T->props = new AP_disj;
        disAPdisj(*(T->left->props),*(T->right->props),T->props);
    }
    else if(T->ctype == AP)
    {
        T->props = new AP_disj;
        AP_stack temp_APS;
        if(T->character!="false")
        {
            temp_APS.insert(T->character);
            T->props->insert(temp_APS);
        }
    }
}

void Syntax_Tree::DelSynTree(ST_Node *T) {
    if(T == NULL)
        return;
    if(T->left != NULL)
        DelSynTree(T->left);
    if(T->right!=NULL)
        DelSynTree(T->right);
    delete T;
}

Syntax_Tree::~Syntax_Tree() {
    DelSynTree(root);
}

void Syntax_Tree::directemporal(ST_Node *T,bool &exchanged) {

     exchanged = false;

     if(T->ctype == AP) {
         return;
     }
     else if(T->ctype == binary_op) {

         directemporal(T->left,exchanged);
         directemporal(T->right,exchanged);

         if(T->character == "U"){
             ST_Node *Tl = T->left;
             ST_Node *Tr = T->right;
             if(Tl->character == "&&") {
                T->character = "&&";
                STNode copy_right;
                T->left->character = "U";

                STNode rightU = new ST_Node;
                rightU->character = "U";
                rightU->ctype=binary_op;

                rightU->left = T->left->right;
                T->left->right->parent = rightU;
                CopyTree(T->right,copy_right,T->left);
                T->left->right = copy_right;
                rightU->right = T->right;
                T->right->parent = rightU;
                T->right = rightU;
                rightU->parent = T;

                exchanged = true;
             }
             else if(Tr->character == "||") {
                 T->character = "||";
                 T->right->character="U";
                 STNode leftU = new ST_Node;

                 leftU->character="U";
                 leftU->ctype=binary_op;
                 leftU->right=T->right->left;
                 T->right->left->parent=leftU;
                 STNode copyleft;
                 CopyTree(T->left,copyleft,T->right);
                 T->right->left=copyleft;
                 leftU->left=T->left;
                 T->left->parent = leftU;
                 T->left=leftU;
                 leftU->parent = T;

                 exchanged = true;
             }
         }

//         directemporal(T->left,exchanged);
//         directemporal(T->right,exchanged);
     }
     else if(T->ctype == unary_op) {

         directemporal(T->left,exchanged);

         if(T->character=="[]"){
             if(T->left->character=="<>"){
                 STNode tl = T->left;
                 if(tl->left->character=="||") {      //GF(a||b)=GFa||GFb
                     STNode vee = new ST_Node;
                     vee->character="||";
                     vee->ctype=binary_op;
                     STNode G=new ST_Node;
                     G->character="[]";
                     G->ctype=unary_op;
                     STNode F=new ST_Node;
                     F->character="<>";
                     F->ctype=unary_op;
                     if(T->parent->left == T) {
                         T->parent->left=vee;
                     }
                     else if(T->parent->right == T) {
                         T->parent->right = vee;
                     }
                     vee->parent = T->parent;
                     T->parent=vee;
                     vee->left=T;

                     STNode tll=tl->left;

                     vee->right = G;
                     G->parent = vee;
                     G->left = F;
                     F->parent=G;
                     F->left = tll->right;
                     tll->right->parent = F;
                     tl->left=tll->left;
                     tll->left->parent = tl;
                     delete tll;
//                     directemporal(vee->left);
//                     directemporal(vee->right);
                     exchanged = true;
                 }
             }
             else
             {
                 if(T->left->character == "&&")   //G(a&&b)=Ga&&Gb
                 {
                     STNode tl = T->left;
                     STNode wedge = new ST_Node;
                     wedge->character="&&";
                     wedge->ctype=binary_op;

                     STNode G = new ST_Node;
                     G->character="[]";
                     G->ctype=unary_op;

                     if(T->parent->left == T) {
                         T->parent->left = wedge;
                     }
                     else if(T->parent->right == T) {
                         T->parent->right = wedge;
                     }
                     wedge->parent = T->parent;
                     wedge->left=T;
                     T->parent = wedge;

                     wedge->right = G;
                     G->parent=wedge;
                     G->left = tl->right;
                     tl->right->parent = G;
                     T->left=tl->left;
                     tl->left->parent=T;
                     delete tl;
//                     directemporal(wedge->left);
//                     directemporal(wedge->right);
                     exchanged = true;
                 }
             }
         }
         else if(T->character=="<>") {
             if(T->left->character=="[]") {
                 STNode tl = T->left;
                 STNode tll = T->left->left;
                 if(tll->character=="&&") {  //FG(a&&b)=FGa&&FGb
                     STNode wedge = new ST_Node;
                     wedge->character = "&&";
                     wedge->ctype = binary_op;

                     if(T->parent->left == T){
                         T->parent->left = wedge;
                     }
                     else if(T->parent->right ==T)
                     {
                         T->parent->right =wedge;
                     }
                     wedge->parent = T->parent;
                     wedge->left=T;
                     T->parent = wedge;

                     STNode G =new ST_Node;
                     G->character="[]";
                     G->ctype=unary_op;
                     STNode F = new ST_Node;
                     F->ctype = unary_op;
                     F->character = "<>";

                     wedge->right=F;
                     F->parent=wedge;
                     F->left=G;
                     G->parent=F;
                     G->left=tll->right;
                     tll->right->parent=G;
                     tl->left = tll->left;
                     tll->parent = tl;
                     delete tll;
//                     directemporal(wedge->left);
//                     directemporal(wedge->right);
                     exchanged = true;
                 }
             }
             else
             {
                 STNode tl = T->left;
                 if(tl->character == "||")   //F(a||b)=Fa||Fb
                 {
                     STNode vee = new ST_Node;
                     vee->character = "||";
                     vee->ctype = binary_op;

                     if(T->parent->left == T)
                     {
                         T->parent->left = vee;
                     }
                     else if(T->parent->right == T)
                     {
                         T->parent->right = vee;
                     }
                     vee->parent = T->parent;
                     vee->left = T;
                     T->parent = vee;

                     STNode F = new ST_Node;
                     F->character = "<>";
                     F->ctype = unary_op;
                     vee->right = F;
                     F->parent = vee;
                     F->left = tl->right;
                     tl->right->parent = F;
                     T->left = tl->left;
                     tl->left->parent = T;
                     delete tl;
//                     directemporal(vee->left);
//                     directemporal(vee->right);
                     exchanged = true;
                 }

             }
         }
//         else
//            directemporal(T->left);
     }
}

void Syntax_Tree::getAPVisible(ST_Node *node) {
    if(node->ctype!=AP)
        return;
    string apstr = node->character;

    if(judgeC(apstr))   //C类型公式
    {
        getCV(apstr,visibles);
    } else{
        getFV(apstr,visibles);
    }
}

//void Syntax_Tree::getVisibleTrans(ST_Node *node) {
//    if(node->character == "&&" || node->character == "||")
//    {
//        getVisibleTrans(node->left);
//        getVisibleTrans(node->right);
//        node->VTS = new DoubleSet;
//        vector<set<int>>::iterator ll;
//        for(ll=node->left->VTS->myelems.begin();ll!=node->left->VTS->myelems.end();++ll)
//        {
//            node->VTS->myelems.push_back(*ll);
//        }
//        vector<set<int>>::iterator rr;
//        for(rr=node->right->VTS->myelems.begin();rr!=node->right->VTS->myelems.end();++rr)
//        {
//            node->VTS->myelems.push_back(*rr);
//        }
//    }
//    else if(node->ctype == binary_op)
//    {
//        getVisibleTrans(node->left);
//        getVisibleTrans(node->right);
//        node->VTS = new DoubleSet;
//        if(node->left->VTS->myelems.size()!=1
//            || node->right->VTS->myelems.size()!=1)
//        {
//            cerr<<"Error! It Is Not a Directly Temporary LTL Formula!"<<endl;
//            exit(-1);
//        }
//        else
//        {
//            node->VTS->myelems = node->left->VTS->myelems;
//            vector<set<int>>::iterator it = node->right->VTS->myelems.begin();
//            node->VTS->myelems[0].insert((*it).begin(),(*it).end());
//        }
//    }
//    else if(node->ctype == unary_op)
//    {
//        getVisibleTrans(node->left);
//        node->VTS = new DoubleSet;
//        if(node->left->VTS->myelems.size()!=1)
//        {
//            cerr<<"Error! It Is Not a Directly Temporary LTL Formula!"<<endl;
//            exit(-1);
//        }
//        else
//            node->VTS->myelems = node->left->VTS->myelems;
//    }
//    else if(node->ctype == AP)
//    {
//        getAPVisible(node);
//    }
//}

void Syntax_Tree::getSingleVTS(ST_Node *node) {
    if(node->ctype == binary_op)
    {
        getSingleVTS(node->left);
        getSingleVTS(node->right);
    }
    else if(node->ctype == unary_op)
    {
        getSingleVTS(node->left);
    }
    else if(node->ctype == AP)
    {
        getAPVisible(node);
    }
}
/****************************CF_Tree******************************/
CF_Tree::CF_Tree()
{
	root = new CFTreeNode;
	root->lleft = NULL;
	root->lright = NULL;
	root->nleft = NULL;
	root->nright = NULL;
}
void CF_Tree::CFBuilder(ST_Node *T)
{
	/*if (T->ctype == AP)
	{
		PCFTreeLeaf ctl = new CFTreeLeaf;
		ctl->trscod.insert(T->character);
		if (!ctl->xstate.isExist(T))
			ctl->xstate.insert(T);
		root->lleft = ctl;
	}
	else
	{*/
		CFBuilderSub(root->nleft, root->lleft, T);
	//}
}
void CF_Tree::CFBuilderSub(CFTreeNode *&ctn, CFTreeLeaf *&ctl, ST_Node *stn)
{
	if (stn->ctype == AP)
	{
		ctl = new CFTreeLeaf;
		ctl->relevant = stn;
		if (stn->character != "true")
			ctl->trscod.insert(stn->character);
		STNode p = new ST_Node;
		p->cfnormal = new state_stack;
		p->character = "true";
		p->ctype = AP;
		p->formula = "true";
		p->left = p->parent = p->right = NULL;
		if (!ctl->xstate.isExist(p))
			ctl->xstate.insert(p);
	}
	else if (stn->character == "&&")
	{
		ctn = new CFTreeNode;
		ctn->character = "&&";
		ctn->lleft = ctn->lright = NULL;
		ctn->nleft = ctn->nright = NULL;
		ctn->relevant = stn;
		CFBuilderSub(ctn->nleft, ctn->lleft, stn->left);
		CFBuilderSub(ctn->nright, ctn->lright, stn->right);
	}
	else if (stn->character == "||")
	{
		ctn = new CFTreeNode;
		ctn->character = "||";
		ctn->lleft = ctn->lright = NULL;
		ctn->nleft = ctn->nright = NULL;
		ctn->relevant = stn;
		CFBuilderSub(ctn->nleft, ctn->lleft, stn->left);
		CFBuilderSub(ctn->nright, ctn->lright, stn->right);
	}
	else if (stn->character == "U")
	{
		ctn = new CFTreeNode;
		ctn->character = "||";
		ctn->lleft = ctn->lright = NULL;
		ctn->nleft = ctn->nright = NULL;
		ctn->relevant = stn;
		CFBuilderSub(ctn->nleft, ctn->lleft, stn->right);
		PCFTreeNode ptn = new CFTreeNode;
		ptn->character = "&&";
		ptn->lleft = ptn->lright = NULL;
		ptn->nleft = ptn->nright = NULL;
		ctn->nright = ptn;
		CFBuilderSub(ptn->nleft, ptn->lleft, stn->left);
		PCFTreeLeaf ptl = new CFTreeLeaf;
		//ptl->trscod.insert("true");
		ptl->xstate.insert(stn);
		ptn->lright = ptl;
	}
	else if (stn->character == "V")
	{
		ctn = new CFTreeNode;
		ctn->character = "||";
		ctn->lleft = ctn->lright = NULL;
		ctn->nleft = ctn->nright = NULL;
		ctn->relevant = stn;
		STNode p = new ST_Node;
		p->cfnormal = new state_stack;
		p->character = "&&";
		p->ctype = binary_op;
		p->formula = "(" + stn->left->formula + ")&&(" + stn->right->formula + ")";
		p->left = stn->left;
		p->right = stn->right;
		p->parent = NULL;
		CFBuilderSub(ctn->nleft, ctn->lleft, p);
		PCFTreeNode ptn = new CFTreeNode;
		ptn->character = "&&";
		ptn->lleft = ptn->lright = NULL;
		ptn->nleft = ptn->nright = NULL;
		ctn->nright = ptn;
		CFBuilderSub(ptn->nleft, ptn->lleft, stn->right);
		PCFTreeLeaf ptl = new CFTreeLeaf;
		//ptl->trscod.insert("true");
		ptl->xstate.insert(stn);
		ptn->lright = ptl;
	}
	else if (stn->character == "X")
	{
		ctl = new CFTreeLeaf;
		ctl->relevant = stn;
		//ctl->trscod.insert("true");
		if (!ctl->xstate.isExist(stn->left))
			ctl->xstate.insert(stn->left);
	}
}
void CF_Tree::PrintCFTree(CFTreeNode *ctn, CFTreeLeaf *ctl, int n)
{
	int i;
	char ch = ' ';
	if (ctn != NULL)
	{
		for (i = 0; i < n; i++)
			cout << setw(3) << ch;
		cout << ctn->character << endl;
		PrintCFTree(ctn->nleft, ctn->lleft, n + 1);
		PrintCFTree(ctn->nright, ctn->lright, n + 1);
	}
	else if (ctl != NULL)
	{
		for (i = 0; i < n; i++)
			cout << setw(3) << ch;
		string beita="";
		string xstate="";
		set<string>::iterator iter;
		for (iter = ctl->trscod.begin(); iter != ctl->trscod.end(); iter++)
		{
			beita += (*iter) + "&&";
		}
		beita = beita.substr(0, beita.length() - 2);
		ctl->xstate.print(xstate);
		cout << beita << "->" << xstate << endl;
	}
}
void CF_Tree::Disjunction(CFTreeNode *&ctn)
{
	//��֦
	/*if (ctn->character=="&&" && ctn->lleft != NULL)
	{
		if (isinclufalse(*ctn->lleft))
		{
			ctn->ss.clear();
			if(ctn->nright!=NULL)
				DelCFTree(ctn->nright);
			delete ctn->lleft;
			ctn->lleft = ctn->lright = NULL;
			ctn->nleft = ctn->nright = NULL;
			return;
		}
	}
	if (ctn->character == "&&" && ctn->lright != NULL)
	{
		if (isinclufalse(*ctn->lright))
		{
			ctn->ss.clear();
			if (ctn->nleft != NULL)       //�ڴ����
				DelCFTree(ctn->nleft);
			delete ctn->lright;
			ctn->lleft = ctn->lright = NULL;
			ctn->nleft = ctn->nright = NULL;
			return;
		}
	}*/
	//�������
	if (ctn->nleft != NULL)
		Disjunction(ctn->nleft);
	if (ctn->nright != NULL)
		Disjunction(ctn->nright);
	if (ctn->character == "&&")
	{
		if (ctn->lleft != NULL && ctn->lright != NULL)
		{
			CFTreeLeaf ctltemp;
			if(constate(*ctn->lleft, *ctn->lright, ctltemp)==OK)
				ctn->ss.insert(ctltemp);

			if (ctn->lleft->relevant != NULL)
			{
				if (ctn->lleft->relevant->isGetCF == false)
				{
					ctn->lleft->relevant->cfnormal->insert(*(ctn->lleft));
					ctn->lleft->relevant->isGetCF = true;
				}
			}
			if (ctn->lright->relevant != NULL)
			{
				if (ctn->lright->relevant->isGetCF == false)
				{
					ctn->lright->relevant->cfnormal->insert(*(ctn->lright));
					ctn->lright->relevant->isGetCF = true;
				}
			}
			if (ctn->relevant != NULL)
			{
				if (ctn->relevant->isGetCF == false)
				{
					*ctn->relevant->cfnormal = ctn->ss;
					ctn->relevant->isGetCF = true;
				}
			}

			delete ctn->lleft;
			delete ctn->lright;
			ctn->lleft = ctn->lright = NULL;			
		}
		else if (ctn->lleft != NULL && ctn->nright != NULL)
		{
			if (isinclufalse(*ctn->lleft))
				ctn->ss.clear();
			else
			{
				while (!ctn->nright->ss.isEmpty())
				{
					CFTreeLeaf cfltemp;
					CFTreeLeaf cflr;
					ctn->nright->ss.pop(cflr);
					if (constate(*ctn->lleft, cflr, cfltemp) == OK)
						ctn->ss.insert(cfltemp);
				}
			}
			/***************************************/
			if (ctn->lleft->relevant != NULL)
			{
				if (ctn->lleft->relevant->isGetCF == false)
				{
					ctn->lleft->relevant->cfnormal->insert(*(ctn->lleft));
					ctn->lleft->relevant->isGetCF = true;
				}
			}
			if (ctn->relevant != NULL)
			{
				if (ctn->relevant->isGetCF == false)
				{
					*ctn->relevant->cfnormal = ctn->ss;
					ctn->relevant->isGetCF = true;
				}
			}
			/****************************************/
			delete ctn->lleft;
			ctn->lleft = NULL;
			delete ctn->nright;
			ctn->nright = NULL;
		}
		else if (ctn->nleft != NULL && ctn->lright != NULL)
		{
			if (isinclufalse(*ctn->lright))
				ctn->ss.clear();
			else
			{
				while (!ctn->nleft->ss.isEmpty())
				{
					CFTreeLeaf cfltemp;
					CFTreeLeaf cfll;
					ctn->nleft->ss.pop(cfll);
					if (constate(cfll, *ctn->lright, cfltemp) == OK)
						ctn->ss.insert(cfltemp);
				}
			}
			/****************************************/
			if (ctn->lright->relevant != NULL)
			{
				if (ctn->lright->relevant->isGetCF == false)
				{
					ctn->lright->relevant->cfnormal->insert(*ctn->lright);
					ctn->lright->relevant->isGetCF = true;
				}
			}
			if (ctn->relevant != NULL)
			{
				if (ctn->relevant->isGetCF == false)
				{
					*ctn->relevant->cfnormal = ctn->ss;
					ctn->relevant->isGetCF = true;
				}
			}
			/****************************************/
			delete ctn->nleft;
			delete ctn->lright;
			ctn->nleft = NULL;
			ctn->lright = NULL;
		}
		else if (ctn->nleft != NULL && ctn->nright != NULL)
		{
			
			while (!ctn->nleft->ss.isEmpty())
			{
				CFTreeLeaf cfll;
				ctn->nleft->ss.pop(cfll);
				state_stack sstemp;
				sstemp = ctn->nright->ss;
				while (!sstemp.isEmpty())
				{
					CFTreeLeaf cfltemp;
					CFTreeLeaf cflr;
					sstemp.pop(cflr);
					if(constate(cfll, cflr, cfltemp)==OK)
						ctn->ss.insert(cfltemp);
				}
			}
			/****************************************/
			if (ctn->relevant != NULL)
			{
				if (ctn->relevant->isGetCF == false)
				{
					*ctn->relevant->cfnormal = ctn->ss;
					ctn->relevant->isGetCF = true;
				}
			}
			/****************************************/
			delete ctn->nleft;
			delete ctn->nright;
			ctn->nleft = ctn->nright = NULL;
		}
	}
	else if (ctn->character == "||")
	{
		if (ctn->lleft != NULL && ctn->lright != NULL)
		{
			if(checkfalse(*ctn->lleft))
				ctn->ss.insert(*ctn->lleft);
			if(checkfalse(*ctn->lright))
				ctn->ss.insert(*ctn->lright);
			/****************************************/
			if (ctn->lleft->relevant != NULL)
			{
				if (ctn->lleft->relevant->isGetCF == false)
				{
					ctn->lleft->relevant->cfnormal->insert(*ctn->lleft);
					ctn->lleft->relevant->isGetCF = true;
				}
			}
			if (ctn->lright->relevant != NULL)
			{
				if (ctn->lright->relevant->isGetCF == false)
				{
					ctn->lright->relevant->cfnormal->insert(*ctn->lright);
					ctn->lright->relevant->isGetCF = true;
				}
			}
			if (ctn->relevant != NULL)
			{
				if (ctn->relevant->isGetCF == false)
				{
					*ctn->relevant->cfnormal = ctn->ss;
					ctn->relevant->isGetCF = true;
				}
			}
			/****************************************/
			delete ctn->lleft;
			delete ctn->lright;
			ctn->lleft = ctn->lright = NULL;
		}
		else if (ctn->lleft != NULL && ctn->nright != NULL)
		{
			if(checkfalse(*ctn->lleft))
				ctn->ss.insert(*ctn->lleft);
			while (!ctn->nright->ss.isEmpty())
			{
				CFTreeLeaf temp;
				ctn->nright->ss.pop(temp);
				if(checkfalse(temp))
					ctn->ss.insert(temp);
			}
			/****************************************/
			if (ctn->lleft->relevant != NULL)
			{
				if (ctn->lleft->relevant->isGetCF == false)
				{
					ctn->lleft->relevant->cfnormal->insert(*ctn->lleft);
					ctn->lleft->relevant->isGetCF = true;
				}
			}
			if (ctn->relevant != NULL)
			{
				if (ctn->relevant->isGetCF == false)
				{
					*ctn->relevant->cfnormal = ctn->ss;
					ctn->relevant->isGetCF = true;
				}
			}
			/****************************************/
			delete ctn->lleft;
			ctn->lleft = NULL;
			delete ctn->nright;
			ctn->nright = NULL;
		}
		else if (ctn->nleft != NULL && ctn->lright != NULL)
		{
			while (!ctn->nleft->ss.isEmpty())
			{
				CFTreeLeaf temp;
				ctn->nleft->ss.pop(temp);
				if(checkfalse(temp))
					ctn->ss.insert(temp);
			}
			if(checkfalse(*ctn->lright))
				ctn->ss.insert(*ctn->lright);
			/****************************************/
			if (ctn->lright->relevant != NULL)
			{
				if (ctn->lright->relevant->isGetCF == false)
				{
					ctn->lright->relevant->cfnormal->insert(*ctn->lright);
					ctn->lright->relevant->isGetCF = true;
				}
			}
			if (ctn->relevant != NULL)
			{
				if (ctn->relevant->isGetCF == false)
				{
					*ctn->relevant->cfnormal = ctn->ss;
					ctn->relevant->isGetCF = true;
				}
			}
			/****************************************/
			delete ctn->nleft;
			ctn->nleft = NULL;
			delete ctn->lright;
			ctn->lright = NULL;
		}
		else if (ctn->nleft != NULL && ctn->nright != NULL)
		{
			CFTreeLeaf temp;
			while (!ctn->nleft->ss.isEmpty())
			{
				ctn->nleft->ss.pop(temp);
				if (checkfalse(temp)==OK)
					ctn->ss.insert(temp);
			}
			while (!ctn->nright->ss.isEmpty())
			{
				ctn->nright->ss.pop(temp);
				if (checkfalse(temp) == OK)
					ctn->ss.insert(temp);
			}
			/****************************************/
			if (ctn->relevant != NULL)
			{
				if (ctn->relevant->isGetCF == false)
				{
					*ctn->relevant->cfnormal = ctn->ss;
					ctn->relevant->isGetCF = true;
				}
			}
			/****************************************/
			delete ctn->nleft;
			ctn->nleft = NULL;
			delete ctn->nright;
			ctn->nright = NULL;
		}
	}
}

void CF_Tree::DisjuncTreeBuilder(CFTreeNode *&ctn, state_stack ss)
{
	CFTreeNode *p = new CFTreeNode;
	p->character = "||";
	p->lright = p->lleft = NULL;
	p->nleft = p->nright = NULL;
	CFTreeLeaf *ctl = new CFTreeLeaf;
	ss.pop(*ctl);
	p->lleft = ctl;
	ctn = p;
	CFTreeNode *q = p;
	while (!ss.isEmpty())
	{
		p = new CFTreeNode;
		p->character = "||";
		p->lright = p->lleft = NULL;
		p->nleft = p->nright = NULL;
		q->nright = p;
		CFTreeLeaf *ctl = new CFTreeLeaf;
		ss.pop(*ctl);
		p->lleft = ctl;
		q = p;
	}
}
void CF_Tree::DelCFTree(CFTreeNode *ctn)
{
	if (ctn->nleft != NULL)
		DelCFTree(ctn->nleft);
	delete ctn->lleft;
	if (ctn->nright != NULL)
		DelCFTree(ctn->nright);
	delete ctn->lright;
	delete ctn;
}

CF_Tree::~CF_Tree() {
    DelCFTree(root);
}
