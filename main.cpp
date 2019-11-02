#include "product.h"
#include "xml2ltl.h"
#include "SBA.h"
#include "Petri_Net.h"
#include <iostream>
#include <sys/time.h>
#include <sys/mman.h>
#include <unistd.h>
#include <exception>
using namespace std;

size_t  heap_malloc_total, heap_free_total,mmap_total, mmap_count;
void print_info() {
    struct mallinfo mi = mallinfo();
    printf("count by itself:\n");
    printf("\033[31m\theap_malloc_total=%lu heap_free_total=%lu heap_in_use=%lu\n\tmmap_total=%lu mmap_count=%lu\n",
           heap_malloc_total*1024, heap_free_total*1024, heap_malloc_total*1024-heap_free_total*1024,
           mmap_total*1024, mmap_count);
    printf("count by mallinfo:\n");
    printf("\theap_malloc_total=%lu heap_free_total=%lu heap_in_use=%lu\n\tmmap_total=%lu mmap_count=%lu\n\033[0m",
           mi.arena, mi.fordblks, mi.uordblks,
           mi.hblkhd, mi.hblks);
//    malloc_stats();
}

NUM_t FIELDCOUNT;
NUM_t placecount;
NUM_t MARKLEN;
bool NUPN = false;
bool SAFE = false;
bool STUBBORN = false;
bool ready2exit = false;

//以MB为单位
short int total_mem;
short int total_swap;
pid_t mypid;

Petri *petri = NULL;

double get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec / 1000000.0;
}

void getinvisible(int transcount,const set<int> &vis, vector<int> &invis)
{
    set<int>::iterator it;
    for(int i=0;i<transcount;++i)
    {
        it = find(vis.begin(),vis.end(),i);
        if(it == vis.end())
        {
            //没有找到
            invis.push_back(i);
        }
    }
}

int main() {

    mypid = getpid();
    //检测本机环境下的内存和swap分区
    FILE *pf;
    char sh_mem[20];
    pf = popen("sh detect_total_mem.sh","r");
    fread(sh_mem, sizeof(sh_mem),1,pf);
    total_mem = atoi(sh_mem);
    pclose(pf);

    //检测swap分区大小
    pf = popen("sh detect_swap.sh","r");
    char sh_swap[20];
    fread(sh_swap, sizeof(sh_swap),1,pf);
    total_swap = atoi(sh_swap);
    pclose(pf);
    cout<<"total mem:"<<total_mem<<"\tswap:"<<total_swap<<endl;

//    string category = argv[1];
//    if(category!="LTLFireability" && category!="LTLCardinality")
//    {
//        cout<<"DO_NOT_COMPETE"<<endl;
//        exit(0);
//    }

    cout << "=================================================" << endl;
    cout << "=====This is our tool-enPAC for the MCC'2019=====" << endl;
    cout << "=================================================" << endl;

    int formula_num = 32;
    int totalruntime = 3600;
    int timeleft;

    string strcol;
    ifstream readcol("iscolored", ios::in);
    if (!readcol) {
        cout << "error!";
        getchar();
        exit(-1);
    }
    getline(readcol, strcol);
    if (strcol == "TRUE") {
        cout << "DO_NOT_COMPETE" << endl;
        return 0;
    }

    //parse xml files
    char Ffile[50] = "LTLFireability.xml";
    char Cfile[50] = "LTLCardinality.xml";
    convertf(Ffile);
    convertc(Cfile);

    double starttime, endtime;
    starttime = get_time();
    Petri *ptnet = new Petri;
    char filename[]="model.pnml";
    ptnet->getSize(filename);
    if(ptnet->NUPN) {
        ptnet->readNUPN(filename);
    }
    else {
        ptnet->readPNML(filename);
    }

    ptnet->judgeSAFE();
//    ptnet->getwrup();       //计算wrupset
//    ptnet->getaccd();       //计算non-accord with变迁

#ifdef DEBUG
    ptnet->checkarc();
    ptnet->printGraph();
    ptnet->printPlace();
    ptnet->printTransition();
    ptnet->printWrup();
    ptnet->printAccord();
#endif

    setGlobalValue(ptnet);
    petri = ptnet;

    BitRG *bitgraph;
    RG *graph;
    string S, propertyid; //propertyid stores names of LTL formulae
    char *form = new char[20000];     //store LTL formulae

    ofstream outresult("boolresult.txt", ios::out);  //outresult export results to boolresult.txt

    ifstream read("LTLCardinality.txt", ios::in);
    if (!read) {
        cout << "error!";
        getchar();
        exit(-1);
    }

//    if(category == "LTLCardinality") {
        while (getline(read, propertyid, ':')) {
//            STUBBORN = false;

            timeleft = totalruntime / formula_num;
            int timetemp = timeleft;

            getline(read, S);

            int len = S.length();
            if (len >= 20000) {
                outresult << '?';
                cout << "FORMULA " + propertyid + " " + "CANNOT_COMPUTE" << endl;
                continue;
            }

            strcpy(form, S.c_str());

            //lexer
            Lexer *lex = new Lexer(form, S.length());
            //syntax analysis
            Syntax_Tree *ST;
            ST = new Syntax_Tree;
            formula_stack Ustack;
            ST->reverse_polish(*lex);
            ST->build_tree();

//            ST->getSingleVTS(ST->root->left);

            ST->simplify_LTL(ST->root->left);

            ST->negconvert(ST->root->left, Ustack);
            ST->computeCurAP(ST->root->left);
            delete lex;
//            double factor = (double)ST->visibles.size()/(double)ptnet->transitioncount;
//            if(factor>0.618)
//                STUBBORN = false;

            if (NUPN || SAFE) {
                bitgraph = new BitRG(ptnet);
//                if(STUBBORN) {
//                    bitgraph->visibleset = ST->visibles;
//                    getinvisible(ptnet->transitioncount,ST->visibles,bitgraph->invisibleset);
//                }
            } else {
                graph = new RG(ptnet);
//                if(STUBBORN) {
//                    graph->visibleset = ST->visibles;
//                    getinvisible(ptnet->transitioncount,ST->visibles,graph->invisibleset);
//                }
            }

            TGBA *Tgba;
            Tgba = new TGBA;
            Tgba->CreatTGBA(Ustack, ST->root->left);
            delete ST;

            TBA *tba;
            tba = new TBA;
            tba->CreatTBA(*Tgba, Ustack);
            delete Tgba;
            string filename = propertyid + ".txt";

            SBA *sba;
            sba = new SBA;
            sba->CreatSBA(*tba);
            sba->Simplify();
            sba->Compress();
            sba->PrintSBA();
            delete tba;
            ready2exit = false;
            //cout << "begin:ON-THE-FLY" << endl;
            if (NUPN || SAFE) {
                Product_Automata<BitRGNode, BitRG> *product;
                product = new Product_Automata<BitRGNode, BitRG>(ptnet, bitgraph, sba);
                product->ModelChecker(propertyid);
                cout<<" "<<bitgraph->nodecount<<endl;
                int ret = product->getresult();
                outresult << (ret == -1 ? '?' : (ret == 0 ? 'F' : 'T'));
                //cout<<"CONFLICT_TIMES:"<<product->getConflictTimes()<<endl;
                delete product;

            } else {
                Product_Automata<RGNode, RG> *product;
                product = new Product_Automata<RGNode, RG>(ptnet, graph, sba);
                product->ModelChecker(propertyid);
                cout<<" "<<graph->nodecount<<endl;
                int ret = product->getresult();
                outresult << (ret == -1 ? '?' : (ret == 0 ? 'F' : 'T'));
                //cout<<"CONFLICT_TIMES:"<<product->getConflictTimes()<<endl;
                delete product;
            }
            delete sba;

            formula_num--;
            totalruntime = totalruntime - (timetemp - timeleft);


            if (NUPN || SAFE) {
                delete bitgraph;
            } else {
                delete graph;
            }

            MallocExtension::instance()->ReleaseFreeMemory();

        }
    //}
    outresult<<endl;
    //if(category == "LTLFireability") {
        ifstream readF("LTLFireability.txt", ios::in);
        if (!readF) {
            cout << "error!";
            getchar();
            exit(-1);
        }

        while (getline(readF, propertyid, ':')) {

//            STUBBORN = false;

            timeleft = totalruntime / formula_num;
            int timetemp;

            //cout << propertyid << ':';
            getline(readF, S);

            int len = S.length();
            if (len >= 20000) {
                outresult << '?';
                cout << "FORMULA " + propertyid + " " + "CANNOT_COMPUTE" << endl;
                continue;
            }

            strcpy(form, S.c_str());
            //cout << form << endl;
            //cout << endl;
            //lexer
//        starttime = get_time();

            Lexer *lex = new Lexer(form, S.length());
            //syntax analysis
            Syntax_Tree *ST;
            ST = new Syntax_Tree;
            formula_stack Ustack;
            ST->reverse_polish(*lex);
            ST->build_tree();

//            ST->getSingleVTS(ST->root->left);

            ST->simplify_LTL(ST->root->left);

            ST->negconvert(ST->root->left, Ustack);
            ST->computeCurAP(ST->root->left);

//            double factor = (double)ST->visibles.size()/(double)ptnet->transitioncount;
//            if(factor>0.618)
//                STUBBORN = false;

            if (NUPN || SAFE) {
                bitgraph = new BitRG(ptnet);
//                if(STUBBORN) {
//                    bitgraph->visibleset = ST->visibles;
//                    getinvisible(ptnet->transitioncount,ST->visibles,bitgraph->invisibleset);
//                }
            } else {
                graph = new RG(ptnet);
//                if(STUBBORN) {
//                    graph->visibleset = ST->visibles;
//                    getinvisible(ptnet->transitioncount,ST->visibles,graph->invisibleset);
//                }
            }

            TGBA *Tgba;
            Tgba = new TGBA;
            Tgba->CreatTGBA(Ustack, ST->root->left);
            //Tgba->SimplifyStates();
            delete ST;
            //cout << endl;
            //¹¹ÔìTBA
            TBA *tba;
            tba = new TBA;
            tba->CreatTBA(*Tgba, Ustack);
            delete Tgba;
            string filename = propertyid + ".txt";
            //tba->PrintBuchi(filename);
            /*cout << "Please check the file" + filename + ". In this file you can see the Buchi automata related to the LTL formula!";
            cout << endl;*/
            //¹¹ÔìSBA
            SBA *sba;
            sba = new SBA;
            sba->CreatSBA(*tba);
            sba->Simplify();
            sba->Compress();
            delete tba;
            //cout << "begin:ON-THE-FLY" << endl;
            ready2exit = false;
            if (NUPN || SAFE) {
                Product_Automata<BitRGNode, BitRG> *product;
                product = new Product_Automata<BitRGNode, BitRG>(ptnet, bitgraph, sba);
                product->ModelChecker(propertyid);
                cout<<" "<<bitgraph->nodecount<<endl;
                int ret = product->getresult();
                outresult << (ret == -1 ? '?' : (ret == 0 ? 'F' : 'T'));
                //cout<<"CONFLICT_TIMES:"<<product->getConflictTimes()<<endl;
                delete product;
            } else {
                Product_Automata<RGNode, RG> *product;
                product = new Product_Automata<RGNode, RG>(ptnet, graph, sba);
                product->ModelChecker(propertyid);
                cout<<" "<<graph->nodecount<<endl;
                int ret = product->getresult();

                outresult << (ret == -1 ? '?' : (ret == 0 ? 'F' : 'T'));
                //cout<<"CONFLICT_TIMES:"<<product->getConflictTimes()<<endl;
                delete product;
            }
            delete sba;
            formula_num--;
            totalruntime = totalruntime - (timetemp - timeleft);

            if (NUPN || SAFE) {
                delete bitgraph;
            } else {
                delete graph;
            }

            MallocExtension::instance()->ReleaseFreeMemory();
        }
    //}

    endtime = get_time();
    cout<<"RUNTIME:"<<endtime-starttime<<endl;
    delete ptnet;
    return 0;
}

int main1()
{
    double starttime, endtime;
    starttime = get_time();

    Petri *ptnet = new Petri;
    char filename[]="model.pnml";
    ptnet->judgeSAFE();
    ptnet->getSize(filename);
    if(ptnet->NUPN)
    {
        ptnet->readNUPN(filename);
    }
    else{
        ptnet->readPNML(filename);
    }

    setGlobalValue(ptnet);

//    NUPN_RG *graph = new NUPN_RG(ptnet);
//    NUPN_RGNode *initnode=graph->RGinitialnode();
//    graph->Generate(initnode);
//    cout<<"STATESPACE:"<<graph->nodecount<<endl;
//    endtime = get_time();
//    cout<<"RUNTIME:"<<endtime-starttime<<endl;
//    cout<<endl;
//    print_info();
//    delete ptnet;
//    delete graph;

    if(NUPN || SAFE)
    {
        BitRG *graph = new BitRG(ptnet);
        BitRGNode *initnode = graph->RGinitialnode();
        graph->Generate(initnode);
        cout<<"STATESPACE: "<<graph->nodecount<<endl;
        endtime = get_time();
        cout<<"RUNTIME:"<<endtime-starttime<<endl;
        cout<<endl;
        cout<<endl;
        malloc_stats();
        delete graph;
    }
    else {
        RG *graph = new RG(ptnet);
        RGNode *initnode = graph->RGinitialnode();
        graph->Generate(initnode);
        cout<<"STATESPACE: "<<graph->nodecount<<endl;
        endtime = get_time();
        cout<<"RUNTIME:"<<endtime-starttime<<endl;
        cout<<endl;
        cout<<endl;
        malloc_stats();
        delete graph;
    }

    delete ptnet;
    malloc_stats();
    //DestroyGlobalMemPool();
    return 0;
}

int main0()
{
    //parse xml files
    char Ffile[50] = "LTLFireability.xml";
    char Cfile[50] = "LTLCardinality.xml";
    convertf(Ffile);
    convertc(Cfile);
    int timeleft = 60;

    Petri *ptnet = new Petri;
    ptnet->judgeSAFE();
    char filename[]="model.pnml";
    ptnet->getSize(filename);
    if(ptnet->NUPN)
    {
        ptnet->readNUPN(filename);
    }
    else{
        ptnet->readPNML(filename);
    }

    ptnet->checkarc();
    ptnet->getwrup();
    ptnet->getaccd();

#ifdef DEBUG
    cout<<"this is debug"<<endl;
    ptnet->printGraph();
    ptnet->printPlace();
    ptnet->printTransition();
    ptnet->printWrup();
    ptnet->printAccord();
#endif
    setGlobalValue(ptnet);
    petri = ptnet;

    ifstream infile("a.txt",ios::in);
    if(!infile) {
        cerr<<"Cannot Open File!"<<endl;
        exit(-1);
    }
    char *form = new char[20000];

    string S,propertyid;
    getline(infile, propertyid, ':');
    getline(infile, S);

    int len = S.length();
    if (len >= 20000) {
        cerr << "Form is too long to compute!"<<endl;
    }

    strcpy(form, S.c_str());

    Lexer *lex = new Lexer(form, S.length());

    Syntax_Tree *ST;
    ST = new Syntax_Tree;
    formula_stack Ustack;
    ST->reverse_polish(*lex);
    ST->build_tree();
    ST->print_syntax_tree(ST->root->left,1);
    ST->getSingleVTS(ST->root->left);

    double factor = (double)ST->visibles.size()/(double)ptnet->transitioncount;
    if(factor>0.618)
        STUBBORN = false;

    BitRG *bitgraph;
    RG *graph;
    if (NUPN || SAFE) {
        bitgraph = new BitRG(ptnet);
        bitgraph->visibleset = ST->visibles;
        getinvisible(ptnet->transitioncount,ST->visibles,bitgraph->invisibleset);
    } else {
        graph = new RG(ptnet);
        graph->visibleset = ST->visibles;
        getinvisible(ptnet->transitioncount,ST->visibles,graph->invisibleset);
    }

    ST->simplify_LTL(ST->root->left);
    ST->negconvert(ST->root->left, Ustack);
    ST->computeCurAP(ST->root->left);
    delete lex;
    TGBA *Tgba;
    Tgba = new TGBA;
    Tgba->CreatTGBA(Ustack, ST->root->left);
    delete ST;
    TBA *tba;
    tba = new TBA;
    tba->CreatTBA(*Tgba, Ustack);
    tba->PrintBuchi("BA.txt");
    delete Tgba;

    SBA *sba;
    sba = new SBA;
    sba->CreatSBA(*tba);
    sba->Simplify();
    sba->Compress();
    delete tba;
    //cout << "begin:ON-THE-FLY" << endl;
    if (NUPN || SAFE) {
        Product_Automata<BitRGNode, BitRG> *product;
        product = new Product_Automata<BitRGNode, BitRG>(ptnet, bitgraph, sba);
        product->ModelChecker(propertyid);
        int ret = product->getresult();

        delete product;
    } else {
        Product_Automata<RGNode, RG> *product;
        product = new Product_Automata<RGNode, RG>(ptnet, graph, sba);
        product->ModelChecker(propertyid);
        int ret = product->getresult();

        delete product;
    }
    return 0;
}