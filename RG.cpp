//
// Created by lewis on 19-7-21.
//
#include "RG.h"

/*********************Global Functions**********************/
void DecToBinary(index_t DecNum, unsigned short *Binarystr)
{
    int  i = 0;
    while(DecNum)
    {
        Binarystr[i++] = DecNum%2;
        DecNum = DecNum/2;
    }
}
void BinaryToDec(index_t &DecNum, unsigned short *Binarystr, NUM_t marklen)
{
    int i = marklen-1;
    DecNum = 0;
    for(i;i>=0;i--)
    {
        DecNum = DecNum*2 + Binarystr[i];
    }
}


/*************************************************************/
NUPN_RGNode::NUPN_RGNode(NUM_t marking_length) {
    marking = new bitset<1>[marking_length];
    next = NULL;
}

index_t NUPN_RGNode::Hash(NUM_t marking_length) {
    unsigned int seed = 3;
    unsigned int hash = 0;
    int i = 0;
    for(i; i<marking_length; i++){
        hash = hash*seed + (unsigned int)marking[i].to_ulong();
    }
    //int result = hash & 0x7fffffff;
    return hash;
}

NUPN_RGNode::~NUPN_RGNode() {
    delete [] marking;
}
/*****************************************************************/
/*RGNode::RGNode(int marking_length)
 * function: 构造函数，为marking数组申请空间，申请大小为哈希表大小，
 * 并全部初始化为0；
 * in: marking_length, 库所哈希表大小；
 * */
RGNode::RGNode(NUM_t marking_length) {
    marking = new Mark[marking_length];
    memset(marking, 0, sizeof(Mark)*marking_length);
    next = NULL;
}

/*int RGNode::tokensum(int marking_length)
 * function: 求当前marking每个库所的token和
 * in: marking_length, 状态数组长度
 * out: sum, token和
 * */
NUM_t RGNode::tokensum(NUM_t marking_length) {
    NUM_t sum = 0;
    for(int i=0; i<marking_length; i++){
        sum += marking[i];
    }
    return sum;
}

/*int RGNode::Hash(int length)
 * function: 求当前marking的哈希值；哈希函数类似于BKDR，种子的值取3；
 * in: length, 状态数组长度
 * out: hash value;
 * */
index_t RGNode::Hash(NUM_t length) {
    unsigned int seed = 3;
    unsigned int hash = 0;
    int i = 0;
    for(i; i<length; i++){
        hash = hash*seed + marking[i];
    }
    //int result = hash & 0x7fffffff;
    return hash;
}

/*RGNode::~RGNode()
 * function： 析构函数，释放空间
 * */
RGNode::~RGNode() {
    delete marking;
}

/****************************RG*****************************/
/*构造函数*/
RG::RG(Petri *pt) {
    ptnet = pt;
    rgnode = new RGNode* [RGTABLE_SIZE];
    for(int i=0; i<RGTABLE_SIZE; i++) {
        rgnode[i] = NULL;
    }
    initnode = NULL;
    hash_conflict_times = 0;
    nodecount = 0;
    RGNodelength = ptnet->placecount;
    outRG.open("ReachabilityGraph.txt",ios::out);
}

/*void RG::push(RGNode *mark)
 * function: 根据mark的哈希值将mark放进rgnode哈希表中
 * in: mark，待加入的状态节点
 * out:
 * */
void RG::addRGNode(RGNode *mark) {
    //计算哈希值
    index_t hashvalue = mark->Hash(RGNodelength);
    hashvalue = hashvalue % RGTABLE_SIZE;

    //计算哈希冲突次数
    if(rgnode[hashvalue]!=NULL)
        hash_conflict_times++;

    nodecount++;

    //加入rgnode哈希表中,头插法
    mark->next = rgnode[hashvalue];
    rgnode[hashvalue] = mark;

    //printRGNode(mark);
}

void RG::getFireableTranx(Mark *curmark, index_t **isFirable, unsigned short &firecount) {
    index_t *firearray = new index_t[ptnet->transitioncount];
    firecount = 0;

    //计算当前状态的可发生变迁；
    bool firable;
    unsigned int j = 0;
    int tlength = ptnet->transitioncount;

    //遍历每一个变迁
    for(j;j<tlength;j++)
    {
        //对于第j个变迁
        Transition *tranx = &ptnet->transition[j];
        firable = true;

        //遍历第j个变迁的所有前继库所
        vector<SArc>::iterator iterpre = tranx->producer.begin();
        vector<SArc>::iterator preend = tranx->producer.end();

        //遍历ptnet.transition[j]的所有前继库所，看其token值是否大于weight
        for(iterpre; iterpre!=preend; iterpre++)
        {
            int mark = curmark[iterpre->idx];
            if(mark<iterpre->weight)
            {
                firable = false;
                break;
            }
        }

        //判断是否能发生，若能发生，加入当前状态的可发生数组中
        if(firable)
            firearray[firecount++] = j;
    }

    if(firecount > 0) {
        *isFirable = new index_t[firecount];
        memcpy(*isFirable,firearray, sizeof(index_t)*firecount);
    }

    delete [] firearray;
}

/*void RG::RGinitialnode()
 * function:计算初始状态；分为三步：1.计算（marking），2.计算初始状态的可发生变迁，
 * 3.将当前状态加入到rgnode哈希表中
 * in:
 * out:
 * */
RGNode *RG::RGinitialnode() {
    RGNode *rg = new RGNode(RGNodelength);
    int i = 0;

    //计算状态
    for(i;i<RGNodelength;i++)
    {
        rg->marking[i] = ptnet->place[i].initialMarking;
    }


    //将当前状态加入到rgnode哈希表中
    initnode = rg;
    addRGNode(rg);
    return rg;
}

/*RGNode *RG::RGcreatenode(RGNode *curnode, int tranxnum, bool &exist)
 * function: 根据当前节点rg, 和可发生变迁，创建新的状态节点；
 * 分为5步：1.计算状态值，2.判断是否无界，3.判断是否已存在该节点，4.计算可发生变迁，5.加入rgnode哈希表中
 * in:curnode,当前节点； tranxnum,transition表中第tranxnum个变迁
 * out:新加入节点的地址
 * */
RGNode *RG::RGcreatenode(RGNode *curnode, int tranxnum, bool &exist) {

    RGNode *newnode = new RGNode(RGNodelength);
    memcpy(newnode->marking, curnode->marking, sizeof(Mark)*RGNodelength);
    //1.计算状态值
    Transition *firingTanx = &ptnet->transition[tranxnum];

    //1.1 计算前继节点的token值；前继库所的token值=当前前继节点的token值-weight
    vector<SArc>::iterator iterpre = firingTanx->producer.begin();
    vector<SArc>::iterator preend = firingTanx->producer.end();

    for(iterpre; iterpre!=preend; iterpre++){
        newnode->marking[iterpre->idx] = newnode->marking[iterpre->idx] - iterpre->weight;
    }

    //1.2 计算后继结点的token值；后继结点的token值=当前后继结点的token值+weight
    vector<SArc>::iterator iterpost = firingTanx->consumer.begin();
    vector<SArc>::iterator postend = firingTanx->consumer.end();
    for(iterpost; iterpost!=postend; iterpost++){
        newnode->marking[iterpost->idx] = newnode->marking[iterpost->idx] + iterpost->weight;
    }
    //2.判断无界

    //3.判断是否已存在该节点
    index_t hashvalue = newnode->Hash(RGNodelength);
    hashvalue = hashvalue % RGTABLE_SIZE;
    bool repeated;
    RGNode *p = rgnode[hashvalue];

    //3.1遍历相同哈希值的状态节点，依次比较每一位
    while(p!=NULL)
    {
        repeated = true;
        //比较每一位
        for(int i=0; i<RGNodelength; i++){
            if(newnode->marking[i]!=p->marking[i]){
                repeated = false;
                break;
            }
        }
        if(repeated){
            exist = true;
            delete newnode;
            return p;
        }
        p=p->next;
    }

    //没有重复

    //5.加入rgnode哈希表
    addRGNode(newnode);
    return newnode;
}

void RG::Generate(RGNode *node) {
    int i=0;

    index_t *isFirable;
    unsigned short firecount = 0;

    getFireableTranx(node->marking,&isFirable,firecount);

    for(i;i<firecount;i++){
        bool exist = false;
        RGNode *nextnode = RGcreatenode(node, isFirable[i], exist);
        if(!exist)
            Generate(nextnode);
    }

    if(firecount > 0)
        delete [] isFirable;
}

void RG::printRGNode(RGNode *node) {
    outRG<<"(";
    for(int i=0; i<RGNodelength; i++)
    {
        if(ptnet->place[i].id!="")
            outRG<<node->marking[i]<<",";
    }
    outRG<<")[>";

    index_t *isFirable;
    unsigned short firecount = 0;
    getFireableTranx(node->marking,&isFirable,firecount);
    int fireT;
    for(fireT=0; fireT<firecount; fireT++)
    {
        int tidx = isFirable[fireT];
        outRG<<ptnet->transition[tidx].id<<" ";
    }
    outRG<<endl;
    if(firecount > 0)
        delete [] isFirable;
}

/*析构函数，释放空间*/
RG::~RG() {
    int i=0;
    for(i;i<RGTABLE_SIZE;i++)
    {
        if(rgnode[i]!=NULL)
        {
            RGNode *p = rgnode[i];
            RGNode *q;
            while(p){
                q=p->next;
                delete p;
                p=q;
            }
        }
    }
    delete rgnode;
}

/*******************************************************************/
NUPN_RG::NUPN_RG(Petri *pt) {
    ptnet = pt;
    rgnode = new NUPN_RGNode*[RGTABLE_SIZE];
    index_t i=0;
    for(i;i<RGTABLE_SIZE;i++)
    {
        rgnode[i] = NULL;
    }
    initnode = 0;

    RGNodelength = 0;
    //计算RGNodelength
    for(i=0;i<pt->unitcount;i++)
    {
        RGNodelength += pt->unittable[i].mark_length;
    }
    nodecount = 0;
    hash_conflict_times = 0;
}

void NUPN_RG::addRGNode(NUPN_RGNode *mark) {
    //计算哈希值
    index_t hashvalue = mark->Hash(RGNodelength);
    hashvalue = hashvalue % RGTABLE_SIZE;

    //计算哈希冲突次数
    if(rgnode[hashvalue]!=NULL)
        hash_conflict_times++;

    nodecount++;

    //加入rgnode哈希表中,头插法
    mark->next = rgnode[hashvalue];
    rgnode[hashvalue] = mark;

    //printRGNode(mark);
}

NUPN_RGNode* NUPN_RG::RGinitialnode() {

    NUPN_RGNode *rg = new NUPN_RGNode(RGNodelength);

    unsigned short *mark = new unsigned short[ptnet->placecount];
    for(index_t i = 0; i<ptnet->placecount; i++)
    {
        mark[i] = ptnet->place[i].initialMarking;
    }

    enCoder(mark,rg);

    //计算可发生变迁
    //getFireableTranx(mark,&(rg->isFirable),rg->fireptr);

    initnode = rg;
    addRGNode(rg);
    delete [] mark;
    return rg;
}

NUPN_RGNode* NUPN_RG::RGcreatenode(NUPN_RGNode *curnode, int tranxum, bool &exist) {
    NUPN_RGNode *newnode = new NUPN_RGNode(RGNodelength);
    Mark *curmark = new Mark[ptnet->placecount];
    Mark *newmark = new Mark[ptnet->placecount];

    deCoder(curmark,curnode);
    memcpy(newmark,curmark, sizeof(Mark)*ptnet->placecount);

    Transition *tranx = &ptnet->transition[tranxum];
    //计算前继库所
    vector<SArc>::iterator preStart = tranx->producer.begin();
    vector<SArc>::iterator preEnd = tranx->producer.end();
    for(preStart;preStart!=preEnd;preStart++)
    {
        newmark[preStart->idx] = 0;
    }

    //计算后继库所
    vector<SArc>::iterator postStart = tranx->consumer.begin();
    vector<SArc>::iterator postEnd = tranx->consumer.end();
    for(postStart;postStart!=postEnd;postStart++)
    {
        newmark[postStart->idx] = 1;
    }

    enCoder(newmark,newnode);

    //判断是否已经存在该节点
    index_t hashvalue = newnode->Hash(RGNodelength);
    hashvalue = hashvalue % RGTABLE_SIZE;
    bool repeated;
    NUPN_RGNode *p = rgnode[hashvalue];
    while(p!=NULL)
    {
        repeated = true;
        index_t i=0;
        for(i;i<RGNodelength;i++)
        {
            if(p->marking[i][0] != newnode->marking[i][0]){
                repeated = false;
                break;
            }
        }

        if(repeated)
        {
            exist = true;
            delete newnode;
            return p;
        }
        p=p->next;
    }

    //没有重复
    addRGNode(newnode);

    delete [] curmark;
    delete [] newmark;

    return newnode;
}

void NUPN_RG::Generate(NUPN_RGNode *node) {
    int i=0;

    index_t *isFirable;
    unsigned short firecount = 0;

    unsigned short *equmark = new unsigned short[ptnet->placecount];
    deCoder(equmark,node);

    getFireableTranx(equmark,&isFirable,firecount);

    delete [] equmark;

    for(i;i<firecount;i++){
        bool exist = false;
        NUPN_RGNode *nextnode = RGcreatenode(node, isFirable[i], exist);
        if(!exist)
            Generate(nextnode);
    }
    if(firecount > 0)
        delete [] isFirable;
}


/*void NUPN_RG::enCoder(unsigned short *equmark, NUPN_RGNode *curnode)
 * in: equmark是一个状态序列，长度为placecount；curnode是当前状态
 * function:将equmark转化为压缩的比特序列；首先要遍历每一个单元，然后找到该单元中
 * 有token的库所，然后将该库所在该单元的偏移量转化为比特序列
 * */
void NUPN_RG::enCoder(unsigned short *equmark, NUPN_RGNode *curnode) {
    unsigned int i = 0;
    //遍历每一个unit
    for(i;i < ptnet->unitcount;i++)
    {
        //i表示第i个unit
        //遍历该unit中的每一个place
        index_t sp = ptnet->unittable[i].startpos;  //该unit的起始地址
        index_t end = sp + ptnet->unittable[i].size;  //该unit的终止地址
        NUM_t mark_len = ptnet->unittable[i].mark_length;  //该unit的marking length

        bool somebodyhastoken = false;
        for(sp;sp<end;sp++)
        {
            //找到该单元中有token的库所
            if(equmark[sp])
            {
                somebodyhastoken = true;
                unsigned short *Binarystr = new unsigned short[mark_len];
                memset(Binarystr,0,sizeof(short)*mark_len);
                DecToBinary(ptnet->place[sp].myoffset+1,Binarystr);
                //设置marking
                index_t msp = ptnet->unittable[i].mark_sp;
                //遍历每一位bit
                index_t mi = 0;
                for(mi,msp ; mi<mark_len; mi++,msp++)
                {
                    if(Binarystr[mi] == 1)
                    {
                        curnode->marking[msp].set();
                    } else{
                        curnode->marking[msp].reset();
                    }
                }
                delete [] Binarystr;
                break;
            }
        }

        if(!somebodyhastoken){
            index_t msp = ptnet->unittable[i].mark_sp;
            index_t mend = msp+ptnet->unittable[i].mark_length;

            for(msp;msp<mend;msp++)
            {
                curnode->marking[msp].reset();
            }
        }
    }
}

/*void NUPN_RG::deCoder(unsigned short *equmark, NUPN_RGNode *curnode)
 * in:equmark为从比特数组转化到的状态位序列；curnode为当前状态
 * function：将curnode中的marking比特数组转化为位序列
 * */
void NUPN_RG::deCoder(unsigned short *equmark, NUPN_RGNode *curnode) {
    memset(equmark,0, sizeof(short)*ptnet->placecount);

    //遍历每一个unit
    unsigned int i = 0;
    for(i;i<ptnet->unitcount;i++)
    {
        //get this unit's start position and end position
        index_t msp = ptnet->unittable[i].mark_sp;
        NUM_t marklen = ptnet->unittable[i].mark_length;
        index_t mend = msp + marklen;
        int mi = 0;
        unsigned short *Binarystr = new unsigned short[marklen];
        //set Binarystr array
        for(mi,msp;msp<mend;mi++,msp++)
        {
            if(curnode->marking[msp].any())  //1
            {
                Binarystr[mi] = 1;
            }
            else{   //0
                Binarystr[mi] = 0;
            }

        }
        index_t offset;
        BinaryToDec(offset,Binarystr,marklen);

        delete [] Binarystr;
        malloc_trim(0);

        if(offset > 0)
        {
            index_t pos = ptnet->unittable[i].startpos + offset-1;
            equmark[pos] = 1;
        }
    }
}

void NUPN_RG::getFireableTranx(unsigned short *equmark, index_t **isFirable, unsigned short &firecount) {

    index_t tp = 0;
    index_t tranxnum = ptnet->transitioncount;
    index_t *firearray = new index_t[tranxnum];
    firecount = 0;

    //遍历每一个变迁
    for(tp;tp<tranxnum;tp++)
    {
        //对于第tp个变迁
        bool fireable = true;
        //检查第tp个变迁的每一个前继库所
        vector<SArc>::iterator tcpre = ptnet->transition[tp].producer.begin();
        vector<SArc>::iterator tcend = ptnet->transition[tp].producer.end();

        for(tcpre;tcpre!=tcend;tcpre++)
        {
            if(equmark[tcpre->idx] < tcpre->weight){
                fireable = false;
                break;
            }
        }

        if(fireable){
            firearray[firecount++] = tp;
        }
    }

    if(firecount>0)
    {
        *isFirable = new index_t[firecount];
        memcpy(*isFirable,firearray, sizeof(index_t)*firecount);
    }

    delete [] firearray;
}

NUPN_RG::~NUPN_RG() {
    index_t i = 0;
    for(i;i<RGTABLE_SIZE;i++)
    {
        if(rgnode[i]!=NULL)
        {
            NUPN_RGNode *p = rgnode[i];
            NUPN_RGNode *q;
            while(p){
                q=p->next;
                delete p;
                p=q;
            }
        }
    }
    delete [] rgnode;
}