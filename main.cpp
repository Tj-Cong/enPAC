#include "product.h"
#include "xml2ltl.h"
#include "SBA.h"
#include <iostream>
#include <sys/time.h>
#include <sys/mman.h>
#include <unistd.h>

using namespace std;

NUM_t FIELDCOUNT;
NUM_t placecount;
NUM_t MARKLEN;
bool NUPN = false;
bool SAFE = false;

MemPool::CMemoryPool *g_ptrMemPool = NULL  ; //!< Global MemoryPool (Testing purpose)
void print_info()
{
    struct mallinfo mi = mallinfo();
    printf("count by mallinfo:\n");
    printf("\theap_malloc_total=%lu heap_free_total=%lu heap_in_use=%lu\n\tmmap_total=%lu mmap_count=%lu\n",
           mi.arena, mi.fordblks, mi.uordblks,
           mi.hblkhd, mi.hblks);
    printf("from malloc_stats:\n");
    malloc_stats();
}

double get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec / 1000000.0;
}
/**************************内存池***********************/
/******************
CreateGlobalMemPool
******************/
void CreateGlobalMemPool()
{
    std::cerr << "Creating MemoryPool...." ;
    g_ptrMemPool = new MemPool::CMemoryPool() ;
    std::cerr << "OK" << std::endl ;
}

/******************
DestroyGlobalMemPool
******************/
void DestroyGlobalMemPool()
{
    std::cerr << "Deleting MemPool...." ;
    if(g_ptrMemPool) ::delete g_ptrMemPool ;
    std::cerr << "OK" << std::endl ;
}
/*/////////////////////内存池/////////////////////////*/



int main() {

//    CreateGlobalMemPool() ;
//    string category = argv[1];
//    if(category!="LTLFireability" && category!="LTLCardinality")
//    {
//        cout<<"DO_NOT_COMPUTE"<<endl;
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

    setGlobalValue(ptnet);
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

    //if(category == "LTLCardinality") {
        while (getline(read, propertyid, ':')) {

            if (NUPN || SAFE) {
                bitgraph = new BitRG(ptnet);
            } else {
                graph = new RG(ptnet);
            }

            timeleft = totalruntime / formula_num;
            int timetemp = timeleft;

            cout << propertyid << ':';
            getline(read, S);
            strcpy(form, S.c_str());
            //cout << form << endl;
            cout << endl;
            int len = S.length();
            if (len > 10000) {
                outresult << '?';
                cout << "FORMULA " + propertyid + " " + "CANNOT_COMPUTE" << endl;
                continue;
            }
//        starttime = get_time();

            //lexer
            Lexer *lex = new Lexer(form, S.length());
            //syntax analysis
            Syntax_Tree *ST;
            ST = new Syntax_Tree;
            formula_stack Ustack;
            ST->reverse_polish(*lex);
            ST->build_tree();
            /*cout << "The syntax tree of unsimplified formula£º" << endl;
            ST.print_syntax_tree(ST.root, 0);*/
            //LTL formula rewrite
            ST->simplify_LTL(ST->root->left);
            /*cout << endl;
            cout << "The syntax tree of simplified formula£º" << endl;
            ST.print_syntax_tree(ST.root, 0);*/
            //syntax tree convert
            ST->negconvert(ST->root->left, Ustack);
            ST->computeCurAP(ST->root->left);
            delete lex;
            /*cout << endl;
            cout << "The converted formula£º" << endl;
            cout << ST.root->left->formula << endl;
            cout << endl;*/
            //Êä³öU×ÓÊ½
            /*cout << "The subformulas of LTL whose main operator is \'U\'£º" << endl;
            vector<STNode>::iterator Uiter;
            for (Uiter = Ustack.loc.begin(); Uiter != Ustack.loc.end(); Uiter++)
            {
                cout << (*Uiter)->formula << endl;
            }
            cout << endl;*/
            //¹¹ÔìTGBA
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
            //tba.PrintBuchi(filename);
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
            if (NUPN || SAFE) {
                Product_Automata<BitRGNode, BitRG> *product;
                product = new Product_Automata<BitRGNode, BitRG>(ptnet, bitgraph, sba);
                product->ModelChecker(propertyid, timeleft);
                int ret = product->getresult();

                outresult << (ret == -1 ? '?' : (ret == 0 ? 'F' : 'T'));
                //cout<<"CONFLICT_TIMES:"<<product->getConflictTimes()<<endl;
                delete product;
            } else {
                Product_Automata<RGNode, RG> *product;
                product = new Product_Automata<RGNode, RG>(ptnet, graph, sba);
                product->ModelChecker(propertyid, timeleft);
                int ret = product->getresult();

                outresult << (ret == -1 ? '?' : (ret == 0 ? 'F' : 'T'));
                //cout<<"CONFLICT_TIMES:"<<product->getConflictTimes()<<endl;
                delete product;
            }


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

    //if(category == "LTLFireability") {
        ifstream readF("LTLFireability.txt", ios::in);
        if (!readF) {
            cout << "error!";
            getchar();
            exit(-1);
        }

        //cout<<"timeleft:"<<timeleft*16<<endl;
        outresult << endl;
        while (getline(readF, propertyid, ':')) {

            if (NUPN || SAFE) {
                bitgraph = new BitRG(ptnet);
            } else {
                graph = new RG(ptnet);
            }

            timeleft = totalruntime / formula_num;
            int timetemp;

            cout << propertyid << ':';
            getline(readF, S);
            strcpy(form, S.c_str());
            //cout << form << endl;
            cout << endl;
            int len = S.length();
            if (len > 10000) {
                outresult << '?';
                cout << "FORMULA " + propertyid + " " + "CANNOT_COMPUTE" << endl;
                continue;
            }
            //lexer
//        starttime = get_time();

            Lexer *lex = new Lexer(form, S.length());
            //syntax analysis
            Syntax_Tree *ST;
            ST = new Syntax_Tree;
            formula_stack Ustack;
            ST->reverse_polish(*lex);
            ST->build_tree();
            /*cout << "The syntax tree of unsimplified formula£º" << endl;
            ST.print_syntax_tree(ST.root, 0);*/
            //LTL formula rewrite
            ST->simplify_LTL(ST->root->left);
            /*cout << endl;
            cout << "The syntax tree of simplified formula£º" << endl;
            ST.print_syntax_tree(ST.root, 0);*/
            //syntax tree convert
            ST->negconvert(ST->root->left, Ustack);
            ST->computeCurAP(ST->root->left);
            /*cout << endl;
            cout << "The converted formula£º" << endl;
            cout << ST.root->left->formula << endl;
            cout << endl;*/
            //Êä³öU×ÓÊ½
            /*cout << "The subformulas of LTL whose main operator is \'U\'£º" << endl;
            vector<STNode>::iterator Uiter;
            for (Uiter = Ustack.loc.begin(); Uiter != Ustack.loc.end(); Uiter++)
            {
                cout << (*Uiter)->formula << endl;
            }
            cout << endl;*/
            //¹¹ÔìTGBA
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

            if (NUPN || SAFE) {
                Product_Automata<BitRGNode, BitRG> *product = new Product_Automata<BitRGNode, BitRG>(ptnet, bitgraph,
                                                                                                     sba);
                product->ModelChecker(propertyid, timeleft);
                int ret = product->getresult();

                outresult << (ret == -1 ? '?' : (ret == 0 ? 'F' : 'T'));
                //cout<<"CONFLICT_TIMES:"<<product->getConflictTimes()<<endl;
                delete product;
            } else {
                Product_Automata<RGNode, RG> *product = new Product_Automata<RGNode, RG>(ptnet, graph, sba);
                product->ModelChecker(propertyid, timeleft);
                int ret = product->getresult();

                outresult << (ret == -1 ? '?' : (ret == 0 ? 'F' : 'T'));
                //cout<<"CONFLICT_TIMES:"<<product->getConflictTimes()<<endl;
                delete product;
            }

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

int main0()
{
    //CreateGlobalMemPool();
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
        print_info();
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
        print_info();
        delete graph;
    }

    delete ptnet;
    print_info();
    //DestroyGlobalMemPool();
    return 0;
}