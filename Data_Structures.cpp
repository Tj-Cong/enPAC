#include "Data_Structures.h"
#define Error NULL

/***********************Global variables*********************/
LTLNode_t fireability[16];
LTLNode_t cardinality[16];

Syntax_Tree_Node::~Syntax_Tree_Node() {

    if(!cfnormal)
        delete cfnormal;
    if(!props)
        delete props;
}
/***********************list_stark***************************/
list_stack::list_stack()
{
	top = NULL;
}

void list_stack::push_ls(LSNode node)
{
	LSNode *p = new LSNode;
	p->w = node.w;
	p->prilevel = node.prilevel;
	p->next = top;
	top = p;
}

bool list_stack::pop_ls(LSNode &node)
{
	bool iscorrect = true;
	if (top != NULL)
	{
		node.w = top->w;
		node.prilevel = top->prilevel;
		LSNode *p = top;
		top = p->next;
		delete p;
	}
	else             
		iscorrect = false;
	return iscorrect;
}
void list_stack::pop_ls()
{
	LSNode *p = top;
	top = p->next;
	delete p;
}
bool list_stack::topelem(LSNode &node)
{
	bool iscorrect = true;
	if (top != NULL)
	{
		node.w = top->w;
		node.prilevel = top->prilevel;
	}
	else              
		iscorrect = false;
	return iscorrect;
}
bool list_stack::istoplpar()
{
	return (top->w.typenum == $Lpar) ? true : false;
}
int list_stack::topprilevel()
{
	return top->prilevel;
}
bool list_stack::isEmpty()
{
	return (top == NULL) ? true : false;
}
list_stack::~list_stack() {
    LSNode *p,*q;
    p = top;
    while (p){
        q=p->next;
        delete p;
        p=q;
    }
}

/**********************formula_stack************************/
void formula_stack::insert(STNode T)
{
	loc.push_back(T);
}
void formula_stack::pop(STNode &T)
{
	vector<STNode>::reverse_iterator iter = this->loc.rbegin();
	T = *iter;
	loc.pop_back();
}
bool formula_stack::isEmpty()
{
	return this->loc.empty();
}
bool formula_stack::isExist(STNode T)
{
	vector<STNode>::iterator iter;
	for (iter = loc.begin(); iter != loc.end(); iter++)
	{
		if ((*iter)->formula == T->formula)
			return true;
	}
	return false;
}
bool formula_stack::decideLabel(STNode label)
{
	vector<STNode>::iterator iter;
	for (iter = loc.begin(); iter != loc.end(); iter++)
	{
		if (isHaveLabel(*iter, label))
			return true;
	}
	return false;
}
bool formula_stack::isHaveLabel(STNode T, STNode label)
{
	if (T->character == "&&")
	{
		if (isHaveLabel(T->left, label))
			return true;
		if (isHaveLabel(T->right, label))
			return true;
	}
	else if(T->formula==label->formula)
	{
		return true;
	}
	return false;
}
void formula_stack::print(string &str)
{
	vector<STNode>::iterator iter;
	for (iter = loc.begin(); iter != loc.end(); iter++)
	{
		str += "X(" + (*iter)->formula + ")&&";
	}
	str = str.substr(0, str.length() - 2);
}
int formula_stack::size()const
{
	return this->loc.size();
}

/***************************state_stack*****************************/
void state_stack::insert(CFTreeLeaf ctl)
{
	this->states.push_back(ctl);
}
void state_stack::pop(CFTreeLeaf &ctl)
{
	vector<CFTreeLeaf>::reverse_iterator riter = states.rbegin();
	ctl = (*riter);
	states.pop_back();
}
bool state_stack::isEmpty()
{
	return (states.empty());
}
int state_stack::size()
{
	return this->states.size();
}
/*bool state_stack::isExist(CFTreeLeaf ctl)
{
	vector<CFTreeLeaf>::iterator iter;
	for (iter = states.begin(); iter != states.end(); iter++)
	{
		
	}
}*/
void state_stack::clear()
{
	this->states.clear();
}

/**************************AP_stack***************************/
void AP_stack::insert(string str) {
    propositions.insert(str);
}

int AP_stack::size() {
    return propositions.size();
}

void AP_stack::pop(string str) {
    propositions.erase(str);
}

void AP_stack::clear() {
    propositions.clear();
}

bool AP_stack::isEmpty() {
    return (propositions.empty());
}

/*****************************AP_disj*****************************/
AP_disj::AP_disj() {

}
AP_disj::AP_disj(AP_disj *APD) {
    vector<AP_stack>::reverse_iterator riter;
    for(riter = APD->prop_disj.rbegin(); riter != APD->prop_disj.rend(); riter++)
    {
        this->insert(*riter);
    }
}
int AP_disj::size() {
    return prop_disj.size();
}

bool AP_disj::isEmpty() {
    return (prop_disj.empty());
}
void AP_disj::clear() {
    prop_disj.clear();
}
void AP_disj::insert(AP_stack APS) {
    prop_disj.push_back(APS);
}
void AP_disj::pop(AP_stack &APS) {
    vector<AP_stack>::reverse_iterator riter = prop_disj.rbegin();
    APS = (*riter);
    prop_disj.pop_back();
}