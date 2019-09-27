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

void setGlobalValue(Petri *ptnet)
{
    NUPN = ptnet->NUPN;
    SAFE = ptnet->SAFE;
    placecount = ptnet->placecount;
    if(NUPN)
    {
        MARKLEN = 0;
        for(int i = 0;i<ptnet->unitcount;i++)
        {
            MARKLEN += ptnet->unittable[i].mark_length;
        }
    }
    else if(SAFE){
        MARKLEN = ptnet->placecount;
    }
    else{
        MARKLEN = ptnet->placecount;
    }
    FIELDCOUNT = ceil(double(MARKLEN)/(sizeof(myuint)*8));

}

void Bitfielduint::set(int index) {
    switch (index)
    {
        case 0:{
            a0 = 1;
            break;
        }
        case 1:{
            a1 =1;
            break;
        }
        case 2:{
            a2 = 1;
            break;
        }
        case 3:{
            a3 = 1;
            break;
        }
        case 4:{
            a4 = 1;
            break;
        }
        case 5:{
            a5 = 1;
            break;
        }
        case 6:{
            a6 = 1;
            break;
        }
        case 7:{
            a7 = 1;
            break;
        }
        case 8:{
            a8 = 1;
            break;
        }
        case 9:{
            a9 = 1;
            break;
        }
        case 10:{
            a10 = 1;
            break;
        }
        case 11:{
            a11 = 1;
            break;
        }
        case 12:{
            a12 = 1;
            break;
        }
        case 13:{
            a13 = 1;
            break;
        }
        case 14:{
            a14 = 1;
            break;
        }
        case 15:{
            a15 = 1;
            break;
        }
        case 16:{
            a16 = 1;
            break;
        }
        case 17:{
            a17 = 1;
            break;
        }
        case 18:{
            a18 = 1;
            break;
        }
        case 19:{
            a19 = 1;
            break;
        }
        case 20:{
            a20 = 1;
            break;
        }
        case 21:{
            a21 = 1;
            break;
        }
        case 22:{
            a22 = 1;
            break;
        }
        case 23:{
            a23 = 1;
            break;
        }
        case 24:{
            a24 = 1;
            break;
        }
        case 25:{
            a25 = 1;
            break;
        }
        case 26:{
            a26 = 1;
            break;
        }
        case 27:{
            a27 = 1;
            break;
        }
        case 28:{
            a28 = 1;
            break;
        }
        case 29:{
            a29 = 1;
            break;
        }
        case 30:{
            a30 = 1;
            break;
        }
        case 31:{
            a31 = 1;
            break;
        }
        default:{
            cerr<<"Bitfield wrong index"<<endl;
            exit(0);
        }
    }
}

void Bitfielduint::reset(int index) {
    switch (index)
    {
        case 0:{
            a0 = 0;
            break;
        }
        case 1:{
            a1 =0;
            break;
        }
        case 2:{
            a2 = 0;
            break;
        }
        case 3:{
            a3 = 0;
            break;
        }
        case 4:{
            a4 = 0;
            break;
        }
        case 5:{
            a5 = 0;
            break;
        }
        case 6:{
            a6 = 0;
            break;
        }
        case 7:{
            a7 = 0;
            break;
        }
        case 8:{
            a8 = 0;
            break;
        }
        case 9:{
            a9 = 0;
            break;
        }
        case 10:{
            a10 = 0;
            break;
        }
        case 11:{
            a11 = 0;
            break;
        }
        case 12:{
            a12 = 0;
            break;
        }
        case 13:{
            a13 = 0;
            break;
        }
        case 14:{
            a14 = 0;
            break;
        }
        case 15:{
            a15 = 0;
            break;
        }
        case 16:{
            a16 = 0;
            break;
        }
        case 17:{
            a17 = 0;
            break;
        }
        case 18:{
            a18 = 0;
            break;
        }
        case 19:{
            a19 = 0;
            break;
        }
        case 20:{
            a20 = 0;
            break;
        }
        case 21:{
            a21 = 0;
            break;
        }
        case 22:{
            a22 = 0;
            break;
        }
        case 23:{
            a23 = 0;
            break;
        }
        case 24:{
            a24 = 0;
            break;
        }
        case 25:{
            a25 = 0;
            break;
        }
        case 26:{
            a26 = 0;
            break;
        }
        case 27:{
            a27 = 0;
            break;
        }
        case 28:{
            a28 = 0;
            break;
        }
        case 29:{
            a29 = 0;
            break;
        }
        case 30:{
            a30 = 0;
            break;
        }
        case 31:{
            a31 = 0;
            break;
        }
        default:{
            cerr<<"Bitfield wrong index"<<endl;
            exit(0);
        }
    }
}

bool Bitfielduint::test0(int index) {
    switch (index)
    {
        case 0:{
            if(a0 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 1:{
            if(a1 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 2:{
            if(a2 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 3:{
            if(a3 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 4:{
            if(a4 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 5:{
            if(a5 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 6:{
            if(a6 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 7:{
            if(a7 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 8:{
            if(a8 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 9:{
            if(a9 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 10:{
            if(a10 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 11:{
            if(a11 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 12:{
            if(a12 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 13:{
            if(a13 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 14:{
            if(a14 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 15:{
            if(a15 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 16:{
            if(a16 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 17:{
            if(a17 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 18:{
            if(a18 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 19:{
            if(a19 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 20:{
            if(a20 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 21:{
            if(a21 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 22:{
            if(a22 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 23:{
            if(a23 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 24:{
            if(a24 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 25:{
            if(a25 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 26:{
            if(a26 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 27:{
            if(a27 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 28:{
            if(a28 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 29:{
            if(a29 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 30:{
            if(a30 == 0){
                return true;
            } else{
                return false;
            }
        }
        case 31:{
            if(a31 == 0){
                return true;
            } else{
                return false;
            }
        }
        default:{
            cerr<<"Bitfield wrong index"<<endl;
            exit(0);
        }
    }
}

bool Bitfielduint::test1(int index) {
    switch (index)
    {
        case 0:{
            if(a0 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 1:{
            if(a1 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 2:{
            if(a2 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 3:{
            if(a3 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 4:{
            if(a4 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 5:{
            if(a5 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 6:{
            if(a6 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 7:{
            if(a7 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 8:{
            if(a8 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 9:{
            if(a9 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 10:{
            if(a10 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 11:{
            if(a11 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 12:{
            if(a12 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 13:{
            if(a13 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 14:{
            if(a14 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 15:{
            if(a15 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 16:{
            if(a16 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 17:{
            if(a17 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 18:{
            if(a18 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 19:{
            if(a19 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 20:{
            if(a20 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 21:{
            if(a21 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 22:{
            if(a22 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 23:{
            if(a23 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 24:{
            if(a24 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 25:{
            if(a25 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 26:{
            if(a26 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 27:{
            if(a27 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 28:{
            if(a28 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 29:{
            if(a29 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 30:{
            if(a30 == 1){
                return true;
            } else{
                return false;
            }
        }
        case 31:{
            if(a31 == 1){
                return true;
            } else{
                return false;
            }
        }
        default:{
            cerr<<"Bitfield wrong index"<<endl;
            exit(0);
        }
    }
}

/*****************************************************************/
/*RGNode::RGNode(int marking_length)
 * function: 构造函数，为marking数组申请空间，申请大小为哈希表大小，
 * 并全部初始化为0；
 * in: marking_length, 库所哈希表大小；
 * */
RGNode::RGNode() {
    marking = new Mark[MARKLEN];
    memset(marking,0, sizeof(Mark)*MARKLEN);
    next = NULL;
}

/*int RGNode::tokensum(int marking_length)
 * function: 求当前marking每个库所的token和
 * in: marking_length, 状态数组长度
 * out: sum, token和
 * */
//template <class t_mark>
//NUM_t RGNode<t_mark>::tokensum() {
//    NUM_t sum = 0;
//    for(int i=0; i<marking_length; i++){
//        sum += marking[i];
//    }
//    return sum;
//}

/*int RGNode::Hash(int length)
 * function: 求当前marking的哈希值；哈希函数类似于BKDR，种子的值取3；
 * in: length, 状态数组长度
 * out: hash value;
 * */
index_t RGNode::Hash() {
    unsigned int seed = 3;
    unsigned int hash = 0;
    int i = 0;

    for(i; i<MARKLEN; i++){
        hash = hash*seed + marking[i];
    }
    //int result = hash & 0x7fffffff;
    return hash;
}

bool RGNode::isFirable(const Transition &t) const {
    bool isfirable = true;
    vector<SArc>::const_iterator tpre = t.producer.begin();
    for(tpre;tpre!=t.producer.end();++tpre)
    {
        if(marking[tpre->idx] < tpre->weight) {
            isfirable = false;
            break;
        }
    }
    return isfirable;
}

/*RGNode::~RGNode()
 * function： 析构函数，释放空间
 * */
RGNode::~RGNode() {
    delete marking;
    MallocExtension::instance()->ReleaseFreeMemory();

}

/*****************************************************************/
/*RGNode::RGNode(int marking_length)
 * function: 构造函数，为marking数组申请空间，申请大小为哈希表大小，
 * 并全部初始化为0；
 * in: marking_length, 库所哈希表大小；
 * */
BitRGNode::BitRGNode() {
    marking = new myuint[FIELDCOUNT];
    memset(marking, 0, sizeof(myuint)*FIELDCOUNT);
    next = NULL;
}

/*int RGNode::tokensum(int marking_length)
 * function: 求当前marking每个库所的token和
 * in: marking_length, 状态数组长度
 * out: sum, token和
 * */
//template <class t_mark>
//NUM_t RGNode<t_mark>::tokensum() {
//    NUM_t sum = 0;
//    for(int i=0; i<marking_length; i++){
//        sum += marking[i];
//    }
//    return sum;
//}

/*int RGNode::Hash(int length)
 * function: 求当前marking的哈希值；哈希函数类似于BKDR，种子的值取3；
 * in: length, 状态数组长度
 * out: hash value;
 * */
index_t BitRGNode::Hash() {
    unsigned int seed = 3;
    unsigned int hash = 0;
    int i = 0;
    unsigned int equvalue;
    for(i; i<FIELDCOUNT; i++) {
        memcpy(&equvalue,&marking[i],sizeof(unsigned int));
        hash = hash * seed + equvalue;
    }
    //int result = hash & 0x7fffffff;
    return hash;
}

bool BitRGNode::isFirable(const Transition &t) const {

    if(SAFE)
    {
        bool isfirable = true;
        vector<SArc>::const_iterator tpre = t.producer.begin();
        for(tpre;tpre!=t.producer.end();++tpre)
        {
            int unit = tpre->idx / (sizeof(myuint)*8);
            int offset = tpre->idx % (sizeof(myuint)*8);
            if(this->marking[unit].test0(offset)) {
                isfirable = false;
                break;
            }
        }
        return isfirable;
    }

    Mark *equmark = new Mark[petri->placecount];
    memset(equmark,0, sizeof(short)*petri->placecount);

    //遍历每一个unit
    for(int i = 0;i<petri->unitcount;i++)
    {
        index_t msp = petri->unittable[i].mark_sp;
        NUM_t marklen = petri->unittable[i].mark_length;
        index_t mend = msp + marklen;
        int mi = 0;
        unsigned short *Binarystr = new unsigned short[marklen];
        int unit,offset;
        for(mi,msp;msp<mend;mi++,msp++)
        {
            unit = msp / (sizeof(myuint)*8);
            offset = msp % (sizeof(myuint)*8);
            if(this->marking[unit].test1(offset))  //1
            {
                Binarystr[mi] = 1;
            }
            else{   //0
                Binarystr[mi] = 0;
            }

        }

        index_t place_offset;
        BinaryToDec(place_offset,Binarystr,marklen);

        delete [] Binarystr;

        if(place_offset > 0)
        {
            index_t pos = petri->unittable[i].startpos + place_offset-1;
            equmark[pos] = 1;
        }
    }

    bool isfirable = true;
    vector<SArc>::const_iterator tpre = t.producer.begin();
    for(tpre;tpre!=t.producer.end();++tpre)
    {
        if(equmark[tpre->idx] < tpre->weight) {
            isfirable = false;
            break;
        }
    }
    delete [] equmark;
    return isfirable;
}

/*RGNode::~RGNode()
 * function： 析构函数，释放空间
 * */
BitRGNode::~BitRGNode() {
    delete marking;
    MallocExtension::instance()->ReleaseFreeMemory();
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

    int transcount = ptnet->transitioncount;
    //如果要用到顽固集，申请邻接表和tarjan要用到的DFN和Low数组
    if(STUBBORN)
    {
        TGraph = new bool*[transcount];
        successor = new bool[transcount];
        hasenvis = new bool[transcount];
        for(int j=0;j<transcount;++j)
        {
            TGraph[j] = new bool[transcount];
        }

        DFN = new int[transcount];
        Low = new int[transcount];
        visited = new int[transcount];
        stgcmponts = new vector<int>[transcount];
        isEnVis = new int[transcount];
        isEnInVis = new int[transcount];
        myroot = new int[transcount];
        fire = new index_t[transcount];
    }
    else
        TGraph = NULL;
    outRG.open("ReachabilityGraph.txt",ios::out);
}

//初始化邻接矩阵，全部初始化为false
void RG::initTGraph() {
    reachset.clear();
    memset(isEnVis,0, sizeof(int)*ptnet->transitioncount);
    memset(isEnInVis,0, sizeof(int)*ptnet->transitioncount);
    memset(myroot,-1, sizeof(int)*ptnet->transitioncount);
    memset(fire,0, sizeof(index_t)*ptnet->transitioncount);
    for(int j=0;j<ptnet->transitioncount;++j)
    {
        successor[j] = false;
        hasenvis[j] = false;
        stgcmponts[j].clear();
        for(int k=0;k<ptnet->transitioncount;++k)
        {
            TGraph[j][k] = false;
        }
    }
}

/*在进行Tarjan算法之前进行的预备工作：
* 重置DFN数组，Low数组，visited数组，C集合，timepoint，tarstack*/
void RG::initTarjan() {
    timepoint = 0;
    tarstack.clear();
    memset(DFN,0, sizeof(int)*ptnet->transitioncount);
    memset(Low,0, sizeof(int)*ptnet->transitioncount);
    memset(visited,0, sizeof(int)*ptnet->transitioncount);
}

/*在状态curnode下得到变迁transition[tidx]的后继结点，
 * 在TGraph中把相应的值设为true,并把successor[tidx]设为true
 * 对于使能变迁，他的后继为和它non-accordwith的变迁
 * 对于非使能变迁来说，他的后继为他的wrup
 * */
void RG::parturient(int tidx) {
    Transition trans = ptnet->transition[tidx];
    if(fire[tidx])  //当前状态变迁可发生
    {
        set<int>::iterator it;
        for(it=trans.nonaccordwith.begin();it!=trans.nonaccordwith.end();++it)
        {
            TGraph[tidx][*it] = true;
        }
    } else{                        //当前状态变迁不可发生
        set<int>::iterator it;
        for(it=trans.wrup.begin();it!=trans.wrup.end();++it)
        {
            TGraph[tidx][*it] = true;
        }
    }
    successor[tidx] = true;
}

void RG::Tarjan(int tidx) {
    DFN[tidx] = Low[tidx] = ++timepoint;
    tarstack.push(tidx);
    visited[tidx] = 1;    //表示tidx已经入栈

    if(!successor[tidx])
        parturient(tidx);

    int i = 0;
    for(i;i<ptnet->transitioncount;++i)
    {
        if(TGraph[tidx][i])   //如果transition[i]是transition[tidx]的后继
        {
            if(!DFN[i])  //transition[i]还未被访问过
            {
                Tarjan(i);
                Low[i] = Low[tidx]<Low[i]?Low[tidx]:Low[i];
            }
            else if(visited[i])  //如果u还在栈内
            {
                Low[i] = Low[tidx]<Low[i]?Low[tidx]:Low[i];
            }
        }
    }

    if(DFN[tidx] == Low[tidx])         //这是强连通分量的根节点
    {
        int popitem;
        do {
            popitem = tarstack.pop();   //从栈中弹出一个节点
            visited[popitem] = 0;       //表示popitem已经出栈
            if(isEnVis[popitem])
            {
                hasenvis[tidx] = true;
            }
            stgcmponts[tidx].push_back(popitem);
            myroot[popitem] = tidx;
        } while(popitem != tidx);
    }

}

/*得到以source集合中变迁为起始节点在TGraph中所能
* 到达的节点的集合，放到reachset中
* 注意：在每次调用这个函数前必须初始化visited数组
* (内部递归调用不用)*/
void RG::getReachable(const set<int> &source) {
    if(source.size() == 0)
        return;
    set<int> nextsource;
    set<int>::const_iterator it=source.begin();
    for(it;it!=source.end();++it)
    {
        if(visited[*it]==false)
        {
            if(!successor[*it])
                parturient(*it);
            for(int i=0;i<ptnet->transitioncount;i++)
            {
                if(TGraph[*it][i])
                {
                    //如果i是*it的后继
                    TGraph[*it][i] = false;
                    reachset.insert(i);
                    nextsource.insert(i);
                }
            }
        }
    }
    getReachable(nextsource);
}

/*void RG::push(RGNode *mark)
 * function: 根据mark的哈希值将mark放进rgnode哈希表中
 * in: mark，待加入的状态节点
 * out:
 * */
void RG::addRGNode(RGNode *mark) {
    //计算哈希值
    index_t hashvalue = mark->Hash();
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

void RG::getFireableTranx(RGNode *curnode, index_t **isFirable, unsigned short &firecount) {
    index_t *firearray = new index_t[ptnet->transitioncount];
    firecount = 0;

    //计算当前状态的可发生变迁；
    bool firable;
    NUM_t tlength = ptnet->transitioncount;
    Transition *tranx;

    //遍历每一个变迁
    for(unsigned int j = 0;j<tlength;j++)
    {
        //对于第j个变迁
        tranx = &ptnet->transition[j];
        firable = true;

        //遍历第j个变迁的所有前继库所
        vector<SArc>::iterator iterpre = tranx->producer.begin();
        vector<SArc>::iterator preend = tranx->producer.end();

        //遍历ptnet.transition[j]的所有前继库所，看其token值是否大于weight
        for(iterpre; iterpre!=preend; iterpre++)
        {
            if(curnode->marking[iterpre->idx] < iterpre->weight)
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
    MallocExtension::instance()->ReleaseFreeMemory();
}

//计算初始状态节点
RGNode *RG::RGinitialnode() {
    RGNode *rg = new RGNode;

    //计算状态
    for(int i = 0;i<RGNodelength;i++)
    {
        rg->marking[i] = ptnet->place[i].initialMarking;
    }

    //将当前状态加入到rgnode哈希表中
    initnode = rg;
    addRGNode(rg);
    return rg;
}

//当前状态cuenode发生transition[tranxnum]变前后所得到的状态，并把该状态返回
RGNode *RG::RGcreatenode(RGNode *curnode, int tranxnum, bool &exist) {

    RGNode *newnode = new RGNode;

    Transition *firingTanx = &ptnet->transition[tranxnum];
    vector<SArc>::iterator iterpre = firingTanx->producer.begin();
    vector<SArc>::iterator preend = firingTanx->producer.end();

    vector<SArc>::iterator iterpost = firingTanx->consumer.begin();
    vector<SArc>::iterator postend = firingTanx->consumer.end();

        memcpy(newnode->marking, curnode->marking, sizeof(Mark)*RGNodelength);

        //1.1 计算前继节点的token值；前继库所的token值=当前前继节点的token值-weight
        for(iterpre; iterpre!=preend; iterpre++){
            newnode->marking[iterpre->idx] = newnode->marking[iterpre->idx] - iterpre->weight;
        }

        //1.2 计算后继结点的token值；后继结点的token值=当前后继结点的token值+weight
        for(iterpost; iterpost!=postend; iterpost++){
            newnode->marking[iterpost->idx] = newnode->marking[iterpost->idx] + iterpost->weight;
        }


        //3.判断是否已存在该节点
        index_t hashvalue = newnode->Hash();
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
                //MallocExtension::instance()->ReleaseFreeMemory();
                return p;
            }
            p=p->next;
        }

        //5.加入rgnode哈希表
        addRGNode(newnode);
    return newnode;
}

//一次性生成全部状态空间
void RG::Generate(RGNode *node) {
    int i=0;

    index_t *isFirable;
    unsigned short firecount = 0;

    getFireableTranx(node,&isFirable,firecount);

    for(i;i<firecount;i++){
        bool exist = false;
        RGNode *nextnode = RGcreatenode(node, isFirable[i], exist);
        if(!exist)
            Generate(nextnode);
    }

    if(firecount > 0)
        delete [] isFirable;
    MallocExtension::instance()->ReleaseFreeMemory();
}

void RG::printRGNode(RGNode *node) {
    outRG<<"(";

    for(int i=0; i<RGNodelength; i++)
    {
        outRG<<node->marking[i]<<",";
    }
    outRG<<")[>";

    index_t *isFirable;
    unsigned short firecount = 0;
    getFireableTranx(node,&isFirable,firecount);
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

    if(STUBBORN)
    {
        for(int i=0;i<ptnet->transitioncount;i++)
        {
            delete [] TGraph[i];
        }
        delete [] DFN;
        delete [] Low;
        delete [] visited;
        delete [] successor;
        delete [] hasenvis;
        delete [] stgcmponts;
        delete [] myroot;
        delete [] isEnInVis;
        delete [] isEnVis;
        delete [] fire;
    }
    MallocExtension::instance()->ReleaseFreeMemory();
}

void RG::enCoder(unsigned short *equmark, RGNode *curnode) {

}

void RG::deCoder(unsigned short *equmark, RGNode *curnode) {

}

void RG::isFirable() {
    //计算当前状态的可发生变迁；
    bool firable;
    NUM_t tlength = ptnet->transitioncount;
    Transition *tranx;

    //遍历每一个变迁
    for(unsigned int j = 0;j<tlength;j++)
    {
        //对于第j个变迁
        tranx = &ptnet->transition[j];
        firable = true;

        //遍历第j个变迁的所有前继库所
        vector<SArc>::iterator iterpre = tranx->producer.begin();
        vector<SArc>::iterator preend = tranx->producer.end();

        //遍历ptnet.transition[j]的所有前继库所，看其token值是否大于weight
        for(iterpre; iterpre!=preend; iterpre++)
        {
            if(cur->marking[iterpre->idx] < iterpre->weight)
            {
                firable = false;
                break;
            }
        }

        //判断是否能发生，若能发生，加入当前状态的可发生数组中
        if(firable)
            fire[j] = 1;
        else
            fire[j] = 0;
    }
}

void RG::genStbnSet(RGNode *curnode,vector<int> &stbset,bool &red) {
    cur = curnode;
    initTGraph();
    getEn_visible();
    isFirable();

    //进行第一步
    for(int i=0;i<ptnet->transitioncount;++i)
    {
        if(isEnInVis[i]) //如果是使能的非可见变迁
        {
            if(myroot[i]!=-1)   //该节点已经在一个强连通分量中
            {
                if(!hasenvis[myroot[i]])
                {
                    vector<int>::iterator it;
                    for(it=stgcmponts[myroot[i]].begin();it!=stgcmponts[myroot[i]].end();++it)
                    {
                        if(fire[*it])
                            stbset.push_back(*it);
                    }
                    red = true;
                    //stbset = stgcmponts[myroot[i]];
                    return;
                }
            }
            else
            {
                initTarjan();
                Tarjan(i);
                if(!hasenvis[i])
                {
                    vector<int>::iterator it;
                    for(it=stgcmponts[i].begin();it!=stgcmponts[i].end();++it)
                    {
                        if(fire[*it])
                            stbset.push_back(*it);
                    }
                    red = true;
                    //stbset = stgcmponts[i];
                    return;
                }
            }
        }
    }

    //进行第二步
    memset(visited,0,sizeof(int)*ptnet->transitioncount);
    getReachable(visibleset);

    //得到Tu后检查是否包含一个使能的非可见变迁
    set<int>::iterator rit;
    bool contain = false;
    for(rit=reachset.begin();rit!=reachset.end();++rit)
    {
        if(fire[*rit])
            stbset.push_back(*rit);
        if(isEnInVis[*rit])
        {
            contain = true;
        }
    }
    if(contain)
    {
        red = false;
        return;
    }


    bool noeninvis = true;
    int j=0;
    for(j=0;j<ptnet->transitioncount;++j)
    {
        if(isEnInVis[j])
        {
            noeninvis = false;
            break;
        }
    }
    if(noeninvis)
    {
        red = false;
        return;
    }

    //进行第三步
    initTarjan();
    int eeii = j;
    Tarjan(eeii);

    vector<int>::iterator ii;
    for(ii=stgcmponts[eeii].begin();ii!=stgcmponts[eeii].end();++ii)
    {
        if(fire[*ii])
            stbset.push_back(*ii);
    }
    red = false;
    return;
}

void RG::getEn_visible() {

    vector<int>::iterator it;
    for(it=invisibleset.begin();it!=invisibleset.end();++it)
    {
        //遍历所有的非可视变迁
        //检查其是否是使能的
        if(fire[*it])
        {
            //使能
            isEnInVis[*it] = 1;
        }
    }

    set<int>::iterator vis;
    for(vis=visibleset.begin();vis!=visibleset.end();++vis)
    {
        if(fire[*vis])
            isEnVis[*vis] = 1;
    }
}

void RG::re_expand(RGNode *curnode, const vector<int> &oldstbset, vector<int> &newstbset) {
    cur = curnode;
    initTGraph();
    getEn_visible();
    isFirable();

    //进行第二步
    memset(visited,0,sizeof(int)*ptnet->transitioncount);
    getReachable(visibleset);

    set<int>::iterator rit;
    for(rit=reachset.begin();rit!=reachset.end();++rit)
    {
        if(fire[*rit])
        {
            vector<int>::const_iterator oid = find(oldstbset.begin(),oldstbset.end(),*rit);
            if(oid == oldstbset.end())
                newstbset.push_back(*rit);
        }
    }
}
/****************************BitRG*****************************/
/*构造函数*/
BitRG::BitRG(Petri *pt) {
    ptnet = pt;
    rgnode = new BitRGNode* [RGTABLE_SIZE];
    for(int i=0; i<RGTABLE_SIZE; i++) {
        rgnode[i] = NULL;
    }
    initnode = NULL;
    hash_conflict_times = 0;
    nodecount = 0;
    if(NUPN){
        RGNodelength = 0;
        for(int i=0;i < pt->unitcount;i++)
        {
            RGNodelength += pt->unittable[i].mark_length;
        }
    } else{
        RGNodelength = pt->placecount;
    }

    int transcount = ptnet->transitioncount;
    if(STUBBORN)
    {
        TGraph = new bool*[transcount];
        successor = new bool[transcount];
        hasenvis = new bool[transcount];
        for(int i=0;i<transcount;++i)
        {
            TGraph[i] = new bool[transcount];
        }

        DFN = new int[transcount];
        Low = new int[transcount];
        visited = new int[transcount];
        stgcmponts = new vector<int>[transcount];
        isEnVis = new int[transcount];
        isEnInVis = new int[transcount];
        myroot = new int[transcount];
        fire = new index_t[transcount];
    }
    else
        TGraph = NULL;
    outRG.open("ReachabilityGraph.txt",ios::out);
}

//每个getstbnset时进行的初始化
void BitRG::initTGraph() {
    reachset.clear();
    memset(isEnVis,0, sizeof(int)*ptnet->transitioncount);
    memset(isEnInVis,0, sizeof(int)*ptnet->transitioncount);
    memset(myroot,-1, sizeof(int)*ptnet->transitioncount);
    memset(fire,0, sizeof(index_t)*ptnet->transitioncount);
    for(int i=0;i<ptnet->transitioncount;++i)
    {
        successor[i] = false;
        hasenvis[i] = false;
        stgcmponts[i].clear();
        for(int j=0;j<ptnet->transitioncount;++j)
        {
            TGraph[i][j] = false;
        }
    }
}

void BitRG::initTarjan() {
    timepoint = 0;
    tarstack.clear();
    memset(DFN,0, sizeof(int)*ptnet->transitioncount);
    memset(Low,0, sizeof(int)*ptnet->transitioncount);
    memset(visited,0, sizeof(int)*ptnet->transitioncount);
}

/*在状态curnode下得到变迁transition[tidx]的后继结点，
 * 在TGraph中把相应的值设为true,并把successor[tidx]设为true
 * 对于使能变迁，他的后继为和它non-accordwith的变迁
 * 对于非使能变迁来说，他的后继为他的wrup
 * */
void BitRG::parturient(int tidx) {
    Transition trans = ptnet->transition[tidx];
    if(fire[tidx])
    {
        set<int>::iterator it;
        for(it=trans.nonaccordwith.begin();it!=trans.nonaccordwith.end();++it)
        {
            TGraph[tidx][*it] = true;
        }
    }
    else
    {
        set<int>::iterator it;
        for(it=trans.wrup.begin();it!=trans.wrup.end();++it)
        {
            TGraph[tidx][*it] = true;
        }
    }
    successor[tidx] = true;
}

/*得到以source集合中变迁为起始节点在TGraph中所能
* 到达的节点的集合，放到reachset中
* 注意：在每次调用这个函数前必须初始化visited数组
* (内部递归调用不用)*/
void BitRG::getReachable(const set<int> &source) {
    if(source.size() == 0)
        return;
    set<int> nextsource;
    set<int>::const_iterator it=source.begin();
    for(it;it!=source.end();++it)
    {
        if(visited[*it]==false)
        {
            if(!successor[*it])
                parturient(*it);
            for(int i=0;i<ptnet->transitioncount;i++)
            {
                if(TGraph[*it][i])
                {
                    //如果i是*it的后继
                    TGraph[*it][i] = false;
                    reachset.insert(i);
                    nextsource.insert(i);
                }
            }
        }
    }
    getReachable(nextsource);
}

/*一个递归函数，寻找以变迁transition[root]为根节点的强连通分量*/
void BitRG::Tarjan(int tidx) {
    DFN[tidx] = Low[tidx] = ++timepoint;
    tarstack.push(tidx);
    visited[tidx] = 1;

    if(!successor[tidx])
        parturient(tidx);

    int i=0;
    for(i;i<ptnet->transitioncount;++i)
    {
        if(TGraph[tidx][i])   //如果transition[i]是transition[tidx]的后继
        {
            if(!DFN[i])  //transition[i]还未被访问过
            {
                Tarjan(i);
                Low[i] = Low[tidx]<Low[i]?Low[tidx]:Low[i];
            }
            else if(visited[i])
            {
                Low[i] = Low[tidx]<Low[i]?Low[tidx]:Low[i];
            }
        }
    }

    if(DFN[tidx] == Low[tidx])
    {
        int popitem;
        do {
            popitem = tarstack.pop();
            visited[popitem] = 0;
            myroot[popitem] = tidx;

            if(isEnVis[popitem])
                hasenvis[tidx] = true;
            stgcmponts[tidx].push_back(popitem);
        }while (popitem!=tidx);
    }
}

void BitRG::getEn_visible() {
    vector<int>::iterator it;
    for(it=invisibleset.begin();it!=invisibleset.end();++it)
    {
        //遍历所有的非可视变迁
        //检查其是否是使能的
        if(fire[*it])
        {
            //使能
            isEnInVis[*it] = 1;
        }
    }

    set<int>::iterator vis;
    for(vis=visibleset.begin();vis!=visibleset.end();++vis)
    {
        if(fire[*vis])
            isEnVis[*vis] = 1;
    }
}

//判断变迁tidx在当前状态下是否可发生
void BitRG::isFirable() {
    unsigned short *mark = NULL;
    if(NUPN){
        mark = new unsigned short[placecount];
        deCoder(mark,cur);
    }

    //计算当前状态的可发生变迁；
    bool firable;
    NUM_t tlength = ptnet->transitioncount;
    Transition *tranx;

    //遍历每一个变迁
    for(unsigned int j = 0;j<tlength;j++)
    {
        //对于第j个变迁
        tranx = &ptnet->transition[j];
        firable = true;

        //遍历第j个变迁的所有前继库所
        vector<SArc>::iterator iterpre = tranx->producer.begin();
        vector<SArc>::iterator preend = tranx->producer.end();

        //遍历ptnet.transition[j]的所有前继库所，看其token值是否大于weight
        for(iterpre; iterpre!=preend; iterpre++)
        {
            if(NUPN){
                if(mark[iterpre->idx] == 0){
                    firable = false;
                    break;
                }
            }
            else if(SAFE){
                int unit = iterpre->idx / (sizeof(myuint)*8);
                int offset = iterpre->idx % (sizeof(myuint)*8);
                if(cur->marking[unit].test0(offset)){
                    firable = false;
                    break;
                }
            }
        }
        if(firable)
            fire[j] = 1;
        else
            fire[j] = 0;
    }
    if(NUPN) {
        delete [] mark;
    }
}

void BitRG::genStbnSet(BitRGNode *curnode, vector<int> &stbset,bool &red) {
    cur = curnode;
    initTGraph();
    isFirable();
    getEn_visible();

    //进行第一步
    for(int i=0;i<ptnet->transitioncount;++i)
    {
        if(isEnInVis[i]) //如果是使能的非可见变迁
        {
            if(myroot[i]!=-1)   //该节点已经在一个强连通分量中
            {
                if(!hasenvis[myroot[i]])
                {
                    vector<int>::iterator it;
                    for(it=stgcmponts[myroot[i]].begin();it!=stgcmponts[myroot[i]].end();++it)
                    {
                        if(fire[*it])
                            stbset.push_back(*it);
                    }
                    //stbset = stgcmponts[myroot[i]];
                    return;
                }
            }
            else
            {
                initTarjan();
                Tarjan(i);
                if(!hasenvis[i])
                {
                    vector<int>::iterator it;
                    for(it=stgcmponts[i].begin();it!=stgcmponts[i].end();++it)
                    {
                        if(fire[*it])
                            stbset.push_back(*it);
                    }
                    //stbset = stgcmponts[i];
                    return;
                }
            }
        }
    }

    //进行第二步
    memset(visited,0,sizeof(int)*ptnet->transitioncount);
    getReachable(visibleset);

    //得到Tu后检查是否包含一个使能的非可见变迁
    set<int>::iterator rit;
    bool contain = false;
    for(rit=reachset.begin();rit!=reachset.end();++rit)
    {
        if(fire[*rit])
            stbset.push_back(*rit);
        if(isEnInVis[*rit])
        {
            contain = true;
        }
    }
    if(contain)
        return;

    bool noeninvis = true;
    int j=0;
    for(j=0;j<ptnet->transitioncount;++j)
    {
        if(isEnInVis[j])
        {
            noeninvis = false;
            break;
        }
    }
    if(noeninvis)
        return;

    //进行第三步
    initTarjan();
    int eeii = j;
    Tarjan(eeii);

    vector<int>::iterator ii;
    for(ii=stgcmponts[eeii].begin();ii!=stgcmponts[eeii].end();++ii)
    {
        if(fire[*ii])
            stbset.push_back(*ii);
    }
    return;
}
/*void RG::push(RGNode *mark)
 * function: 根据mark的哈希值将mark放进rgnode哈希表中
 * in: mark，待加入的状态节点
 * out:
 * */
void BitRG::addRGNode(BitRGNode *mark) {
    //计算哈希值
    index_t hashvalue = mark->Hash();
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

void BitRG::getFireableTranx(BitRGNode *curnode, index_t **isFirable, unsigned short &firecount) {

    unsigned short *mark = NULL;
    if(NUPN){
        mark = new unsigned short[placecount];
        deCoder(mark,curnode);
    }

    index_t *firearray = new index_t[ptnet->transitioncount];
    firecount = 0;

    //计算当前状态的可发生变迁；
    bool firable;
    NUM_t tlength = ptnet->transitioncount;
    Transition *tranx;

    //遍历每一个变迁
    for(unsigned int j = 0;j<tlength;j++)
    {
        //对于第j个变迁
        tranx = &ptnet->transition[j];
        firable = true;

        //遍历第j个变迁的所有前继库所
        vector<SArc>::iterator iterpre = tranx->producer.begin();
        vector<SArc>::iterator preend = tranx->producer.end();

        //遍历ptnet.transition[j]的所有前继库所，看其token值是否大于weight
        for(iterpre; iterpre!=preend; iterpre++)
        {
            if(NUPN){
                if(mark[iterpre->idx] == 0){
                    firable = false;
                    break;
                }
            }
            else if(SAFE){
                int unit = iterpre->idx / (sizeof(myuint)*8);
                int offset = iterpre->idx % (sizeof(myuint)*8);
                if(curnode->marking[unit].test0(offset)){
                    firable = false;
                    break;
                }
            }
        }

        //判断是否能发生，若能发生，加入当前状态的可发生数组中
        if(firable)
            firearray[firecount++] = j;
    }

    if(mark!=NULL)
        delete mark;

    if(firecount > 0) {
        *isFirable = new index_t[firecount];
        memcpy(*isFirable,firearray, sizeof(index_t)*firecount);
    }

    delete [] firearray;
    MallocExtension::instance()->ReleaseFreeMemory();
}

/*void RG::RGinitialnode()
 * function:计算初始状态；分为三步：1.计算（marking），2.计算初始状态的可发生变迁，
 * 3.将当前状态加入到rgnode哈希表中
 * in:
 * out:
 * */

BitRGNode *BitRG::RGinitialnode() {
    BitRGNode *rg = new BitRGNode;

    //计算状态
    if(NUPN) {
        unsigned short *mark = new unsigned short[placecount];
        for(index_t i = 0; i<placecount; i++)
        {
            mark[i] = ptnet->place[i].initialMarking;
        }

        enCoder(mark,rg);
        delete mark;
    }else if(SAFE) {
        for(int i = 0;i<placecount;i++)
        {
            int unit = i/(sizeof(myuint)*8);
            int offset = i%(sizeof(myuint)*8);
            if(ptnet->place[i].initialMarking == 1){

                rg->marking[unit].set(offset);
            }
            else{
                rg->marking[unit].reset(offset);
            }
        }
    }

    //将当前状态加入到rgnode哈希表中
    initnode = rg;
    addRGNode(rg);
    MallocExtension::instance()->ReleaseFreeMemory();
    return rg;
}

/*RGNode *RG::RGcreatenode(RGNode *curnode, int tranxnum, bool &exist)
 * function: 根据当前节点rg, 和可发生变迁，创建新的状态节点；
 * 分为5步：1.计算状态值，2.判断是否无界，3.判断是否已存在该节点，4.计算可发生变迁，5.加入rgnode哈希表中
 * in:curnode,当前节点； tranxnum,transition表中第tranxnum个变迁
 * out:新加入节点的地址
 * */

BitRGNode *BitRG::RGcreatenode(BitRGNode *curnode, int tranxnum, bool &exist) {

    BitRGNode *newnode = new BitRGNode;

    Transition *firingTanx = &ptnet->transition[tranxnum];
    vector<SArc>::iterator iterpre = firingTanx->producer.begin();
    vector<SArc>::iterator preend = firingTanx->producer.end();

    vector<SArc>::iterator iterpost = firingTanx->consumer.begin();
    vector<SArc>::iterator postend = firingTanx->consumer.end();

    if(NUPN)
    {
        unsigned short *curmark = new unsigned short[ptnet->placecount];
        unsigned short *newmark = new unsigned short[ptnet->placecount];

        deCoder(curmark,curnode);
        memcpy(newmark,curmark, sizeof(unsigned short)*ptnet->placecount);

        for(iterpre;iterpre!=preend;iterpre++)
        {
            newmark[iterpre->idx] = 0;
        }

        for(iterpost;iterpost!=postend;iterpost++)
        {
            newmark[iterpost->idx] = 1;
        }

        enCoder(newmark,newnode);

        //判断是否已经存在该节点
        index_t hashvalue = newnode->Hash();
        hashvalue = hashvalue % RGTABLE_SIZE;
        bool repeated;
        BitRGNode *p = rgnode[hashvalue];
        while(p!=NULL)
        {
            repeated = true;
            index_t i=0;
            for(i; i < FIELDCOUNT; i++)
            {
                unsigned int equp;
                unsigned int equnewnode;
                memcpy(&equp,&p->marking[i],sizeof(unsigned int));
                memcpy(&equnewnode,&newnode->marking[i],sizeof(unsigned int));
                if(equp != equnewnode){
                    repeated = false;
                    break;
                }
            }

            if(repeated)
            {
                exist = true;
                delete [] curmark;
                delete [] newmark;
                delete newnode;
                //MallocExtension::instance()->ReleaseFreeMemory();
                return p;
            }
            p=p->next;
        }

        //没有重复
        addRGNode(newnode);

        delete [] curmark;
        delete [] newmark;
    }
    else if(SAFE)
    {
        memcpy(newnode->marking,curnode->marking, sizeof(myuint)*FIELDCOUNT);

        int unit;
        int offset;
        for(iterpre;iterpre!=preend;iterpre++)
        {
            unit = iterpre->idx / (sizeof(myuint)*8);
            offset = iterpre->idx % (sizeof(myuint)*8);
            newnode->marking[unit].reset(offset);
        }

        for(iterpost;iterpost!=postend;iterpost++)
        {
            unit = iterpost->idx / (sizeof(myuint)*8);
            offset = iterpost->idx % (sizeof(myuint)*8);
            newnode->marking[unit].set(offset);
        }

        index_t hashvalue = newnode->Hash();
        hashvalue = hashvalue % RGTABLE_SIZE;
        bool repeated;
        BitRGNode *p = rgnode[hashvalue];
        while(p!=NULL)
        {
            repeated = true;
            index_t i=0;
            for(i; i < FIELDCOUNT; i++)
            {
                unsigned int equp;
                unsigned int equnewnode;
                memcpy(&equp,&p->marking[i],sizeof(unsigned int));
                memcpy(&equnewnode,&newnode->marking[i],sizeof(unsigned int));
                if(equp != equnewnode){
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
    }
    MallocExtension::instance()->ReleaseFreeMemory();
    return newnode;
}


void BitRG::Generate(BitRGNode *node) {
    int i=0;

    index_t *isFirable;
    unsigned short firecount = 0;

    getFireableTranx(node,&isFirable,firecount);

    for(i;i<firecount;i++){
        bool exist = false;
        BitRGNode *nextnode = RGcreatenode(node, isFirable[i], exist);
        if(!exist)
            Generate(nextnode);
    }

    if(firecount > 0)
        delete [] isFirable;
    MallocExtension::instance()->ReleaseFreeMemory();
}

void BitRG::printRGNode(BitRGNode *node) {
    outRG<<"(";
    if(NUPN){

    }
    else if(SAFE)
    {
        int unit;
        int offset;
        for(int i=0; i<RGNodelength; i++)
        {
            unit = i/(sizeof(myuint)*8);
            offset = i%(sizeof(myuint)*8);
            outRG<<(node->marking[unit].test1(offset))<<",";
        }
        outRG<<")[>";
    }

    index_t *isFirable;
    unsigned short firecount = 0;
    getFireableTranx(node,&isFirable,firecount);
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
BitRG::~BitRG() {
    int i=0;
    for(i;i<RGTABLE_SIZE;i++)
    {
        if(rgnode[i]!=NULL)
        {
            BitRGNode *p = rgnode[i];
            BitRGNode *q;
            while(p){
                q=p->next;
                delete p;
                p=q;
            }
        }
    }
    delete rgnode;

    if(STUBBORN)
    {
        delete [] DFN;
        delete [] Low;
        delete [] visited;
        delete [] successor;
        delete [] hasenvis;
        delete [] stgcmponts;
        delete [] myroot;
        delete [] isEnInVis;
        delete [] isEnVis;
        delete [] fire;
        for(int i=0;i<ptnet->transitioncount;++i)
            delete [] TGraph[i];
    }
    MallocExtension::instance()->ReleaseFreeMemory();
}

//
void BitRG::enCoder(unsigned short *equmark, BitRGNode *curnode) {
    if(NUPN)
    {
        //遍历每一个unit
        for(int i=0;i < ptnet->unitcount;i++)
        {
            //对于unit[i]来说
            //遍历该unit中的每一个库所
            index_t sp = ptnet->unittable[i].startpos;  //该unit的起始地址
            index_t end = sp + ptnet->unittable[i].size;  //该unit的终止地址
            NUM_t unitmarklen = ptnet->unittable[i].mark_length;
            bool somebodyhastoken = false;

            //遍历unit[i]中的每一个库所
            for(sp;sp<end;sp++)
            {
                if(equmark[sp])
                {
                    somebodyhastoken = true;
                    unsigned short *Binarystr = new unsigned short[unitmarklen];
                    memset(Binarystr,0,sizeof(short)*unitmarklen);
                    DecToBinary(ptnet->place[sp].myoffset+1,Binarystr);
                    //设置marking
                    index_t msp = ptnet->unittable[i].mark_sp;
                    //遍历每一位bit
                    index_t mi = 0;
                    int unit;
                    int offset;
                    for(mi,msp ; mi<unitmarklen; mi++,msp++)
                    {
                        unit = msp / (sizeof(myuint)*8);
                        offset = msp % (sizeof(myuint)*8);
                        if(Binarystr[mi] == 1)
                        {
                            curnode->marking[unit].set(offset);
                        } else{
                            curnode->marking[unit].reset(offset);
                        }
                    }
                    delete [] Binarystr;
                    break;
                }
            }

            if(!somebodyhastoken) {
                index_t msp = ptnet->unittable[i].mark_sp;
                index_t mend = msp+ptnet->unittable[i].mark_length;

                int unit;
                int offset;

                for(msp;msp<mend;msp++)
                {
                    unit = msp / (sizeof(myuint)*8);
                    offset = msp % (sizeof(myuint)*8);
                    curnode->marking[unit].reset(offset);
                }
            }
        }
    }
    MallocExtension::instance()->ReleaseFreeMemory();
}


void BitRG::deCoder(unsigned short *equmark, BitRGNode *curnode) {
    if(!NUPN)
        return;
    memset(equmark,0, sizeof(short)*ptnet->placecount);

    //遍历每一个unit
    for(int i = 0;i<ptnet->unitcount;i++)
    {
        index_t msp = ptnet->unittable[i].mark_sp;
        NUM_t marklen = ptnet->unittable[i].mark_length;
        index_t mend = msp + marklen;
        int mi = 0;
        unsigned short *Binarystr = new unsigned short[marklen];
        int unit,offset;
        for(mi,msp;msp<mend;mi++,msp++)
        {
            unit = msp / (sizeof(myuint)*8);
            offset = msp % (sizeof(myuint)*8);
            if(curnode->marking[unit].test1(offset))  //1
            {
                Binarystr[mi] = 1;
            }
            else{   //0
                Binarystr[mi] = 0;
            }

        }

        index_t place_offset;
        BinaryToDec(place_offset,Binarystr,marklen);

        delete [] Binarystr;

        if(place_offset > 0)
        {
            index_t pos = ptnet->unittable[i].startpos + place_offset-1;
            equmark[pos] = 1;
        }
    }
    MallocExtension::instance()->ReleaseFreeMemory();
}

void BitRG::re_expand(BitRGNode *curnode, const vector<int> &oldstbset, vector<int> &newstbset) {
    cur = curnode;
    initTGraph();
    isFirable();
    getEn_visible();
    //进行第二步
    memset(visited,0,sizeof(int)*ptnet->transitioncount);
    getReachable(visibleset);

    set<int>::iterator rit;
    for(rit=reachset.begin();rit!=reachset.end();++rit)
    {
        if(fire[*rit])
        {
            vector<int>::const_iterator oid = find(oldstbset.begin(),oldstbset.end(),*rit);
            if(oid == oldstbset.end())
                newstbset.push_back(*rit);
        }
    }
}
